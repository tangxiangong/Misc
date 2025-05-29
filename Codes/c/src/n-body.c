// The Computer Language Benchmarks Game
// https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
//
// Contributed by Mark C. Lewis.
// Modified slightly by Chad Whipkey.
// Converted from Java to C++ and added SSE support by Branimir Maksimovic.
// Converted from C++ to C by Alexey Medvedchikov.
// Modified by Jeremy Zerfas.
// Adapted for ARM NEON by AI Assistant.

#include <stdint.h>
#include <stdalign.h>
#ifdef __ARM_NEON // Use NEON intrinsics for ARM
#include <arm_neon.h>
#else // Fallback or specific x86 code removed for clarity
// If needed, x86 intrinsics could be placed here with #elif defined(__x86_64__) etc.
// #include <immintrin.h> // Removed as we target ARM
#warning "Target architecture not explicitly supported for vectorization, performance may vary."
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // For atoi

// intptr_t should be the native integer type on most sane systems.
typedef intptr_t intnative_t;

typedef struct{
    double position[3], velocity[3], mass;
} body;

#define SOLAR_MASS (4*M_PI*M_PI)
#define DAYS_PER_YEAR 365.24
#define BODIES_COUNT 5

// Figure out how many total different interactions there are between each
// body and every other body. Some of the calculations for these
// interactions will be calculated two at a time by using vector
// instructions and because of that it will also be useful to have a
// ROUNDED_INTERACTIONS_COUNT that is equal to the next highest even number
// which is equal to or greater than INTERACTIONS_COUNT.
#define INTERACTIONS_COUNT (BODIES_COUNT*(BODIES_COUNT-1)/2)
// Ensure count is even for vector processing pairs
#define ROUNDED_INTERACTIONS_COUNT ((INTERACTIONS_COUNT + 1) & ~1)

static body solar_Bodies[]={
    {    // Sun
        .mass=SOLAR_MASS
    },
    {    // Jupiter
        {
             4.84143144246472090e+00,
            -1.16032004402742839e+00,
            -1.03622044471123109e-01
        },
        {
             1.66007664274403694e-03 * DAYS_PER_YEAR,
             7.69901118419740425e-03 * DAYS_PER_YEAR,
            -6.90460016972063023e-05 * DAYS_PER_YEAR
        },
        9.54791938424326609e-04 * SOLAR_MASS
    },
    {    // Saturn
        {
             8.34336671824457987e+00,
             4.12479856412430479e+00,
            -4.03523417114321381e-01
        },
        {
            -2.76742510726862411e-03 * DAYS_PER_YEAR,
             4.99852801234917238e-03 * DAYS_PER_YEAR,
             2.30417297573763929e-05 * DAYS_PER_YEAR
        },
        2.85885980666130812e-04 * SOLAR_MASS
    },
    {    // Uranus
        {
             1.28943695621391310e+01,
            -1.51111514016986312e+01,
            -2.23307578892655734e-01
        },
        {
             2.96460137564761618e-03 * DAYS_PER_YEAR,
             2.37847173959480950e-03 * DAYS_PER_YEAR,
            -2.96589568540237556e-05 * DAYS_PER_YEAR
        },
        4.36624404335156298e-05 * SOLAR_MASS
    },
    {    // Neptune
        {
             1.53796971148509165e+01,
            -2.59193146099879641e+01,
             1.79258772950371181e-01
        },
        {
             2.68067772490389322e-03 * DAYS_PER_YEAR,
             1.62824170038242295e-03 * DAYS_PER_YEAR,
            -9.51592254519715870e-05 * DAYS_PER_YEAR
        },
        5.15138902046611451e-05 * SOLAR_MASS
    }
};

// Moved to file scope to allow alignas with static duration
#ifdef __ARM_NEON
// Align to 16 bytes for float64x2_t
// Separate declarations for alignas compatibility
// Place alignas before the type
static _Alignas(16) double position_Deltas[3][ROUNDED_INTERACTIONS_COUNT];
static _Alignas(16) double magnitudes[ROUNDED_INTERACTIONS_COUNT];
#else // Non-vectorized or other architecture version
static double position_Deltas[3][INTERACTIONS_COUNT];
static double magnitudes[INTERACTIONS_COUNT];
#endif

// Advance all the bodies in the system by one timestep. Calculate the
// interactions between all the bodies, update each body's velocity based on
// those interactions, and update each body's position by the distance it
// travels in a timestep at it's updated velocity.
static void advance(body bodies[]){

    // Calculate the position_Deltas between the bodies for each interaction.
    for(intnative_t i=0, k=0; i<BODIES_COUNT-1; ++i)
        for(intnative_t j=i+1; j<BODIES_COUNT; ++j, ++k)
            for(intnative_t m=0; m<3; ++m)
                position_Deltas[m][k]=
                  bodies[i].position[m]-bodies[j].position[m];

    // Calculate the magnitudes of force between the bodies for each
    // interaction. This loop processes two interactions at a time which is why
    // ROUNDED_INTERACTIONS_COUNT/2 iterations are done.
    #ifdef __ARM_NEON
    for(intnative_t i=0; i<ROUNDED_INTERACTIONS_COUNT/2; ++i){

        // Load position_Deltas of two bodies into position_Delta vectors.
        float64x2_t position_Delta[3];
        position_Delta[0] = vld1q_f64(&position_Deltas[0][i*2]);
        position_Delta[1] = vld1q_f64(&position_Deltas[1][i*2]);
        position_Delta[2] = vld1q_f64(&position_Deltas[2][i*2]);

        // distance_Squared = dx*dx + dy*dy + dz*dz;
        float64x2_t distance_Squared = vmulq_f64(position_Delta[0], position_Delta[0]);
        distance_Squared = vfmaq_f64(distance_Squared, position_Delta[1], position_Delta[1]); // Fused multiply-accumulate
        distance_Squared = vfmaq_f64(distance_Squared, position_Delta[2], position_Delta[2]);


        // Calculate reciprocal square root using NEON estimate and Newton-Raphson steps.
        // Initial estimate: x0 = vrsqrteq_f64(distance_Squared)
        float64x2_t distance_Reciprocal = vrsqrteq_f64(distance_Squared);

        // Refinement using Newton-Raphson: x_{n+1} = x_n * (1.5 - 0.5 * a * x_n^2)
        // Step 1:
        float64x2_t half_dist_sq = vmulq_f64(vdupq_n_f64(0.5), distance_Squared);
        float64x2_t term = vfmsq_f64(vdupq_n_f64(1.5), half_dist_sq, vmulq_f64(distance_Reciprocal, distance_Reciprocal)); // 1.5 - 0.5*d2*x0*x0
        distance_Reciprocal = vmulq_f64(distance_Reciprocal, term);

        // Step 2: (Same calculation with updated distance_Reciprocal)
        term = vfmsq_f64(vdupq_n_f64(1.5), half_dist_sq, vmulq_f64(distance_Reciprocal, distance_Reciprocal)); // 1.5 - 0.5*d2*x1*x1
        distance_Reciprocal = vmulq_f64(distance_Reciprocal, term);


        // Calculate magnitude: magnitude = dt / (distance * distance * distance)
        // Which is equivalent to: magnitude = (dt / distance_Squared) * (1 / distance)
        // Or using the reciprocal: magnitude = (dt / distance_Squared) * distance_Reciprocal
        // dt is 0.01 in this simulation's advance step.
        // magnitude = (0.01 / distance_Squared) * distance_Reciprocal
        // Avoid division: Calculate reciprocal of distance_Squared if needed, or use the original approach
        // original: 0.01/distance_Squared*distance_Reciprocal;
        // NEON: compute 1.0/distance_Squared first? vrecpeq gives estimate.
        // Let's try to compute reciprocal precisely if needed, or stick to original formula structure.
        // Using precise reciprocal estimate and refinement:
        // float64x2_t dist_sq_reciprocal = vrecpeq_f64(distance_Squared);
        // dist_sq_reciprocal = vmulq_f64(vrecpsq_f64(distance_Squared, dist_sq_reciprocal), dist_sq_reciprocal); // Refine step 1
        // dist_sq_reciprocal = vmulq_f64(vrecpsq_f64(distance_Squared, dist_sq_reciprocal), dist_sq_reciprocal); // Refine step 2
        // float64x2_t mag = vmulq_f64(vdupq_n_f64(0.01), dist_sq_reciprocal);
        // mag = vmulq_f64(mag, distance_Reciprocal);

        // Stick closer to the original calculation structure for potentially better pipelining/accuracy trade-offs
        // explored in the original SSE version: 0.01 / distance_Squared * distance_Reciprocal
        // We still need a division or reciprocal of distance_Squared.
        // Let's use the refined distance_Reciprocal:
        // magnitude = (0.01 * distance_Reciprocal) / distance_Squared - this avoids cube
        float64x2_t mag_numerator = vmulq_f64(vdupq_n_f64(0.01), distance_Reciprocal);
        // A division operation might be necessary here if precise reciprocal isn't available/efficient
        // float64x2_t mag = vdivq_f64(mag_numerator, distance_Squared); // Requires division support

        // Replicating the original formula which avoids division by cube but uses division by square:
        // magn = 0.01 / distance_Squared * distance_Reciprocal;
        // We need a division by distance_Squared. Most ARM NEON implementations might not have a direct f64 division.
        // Let's approximate 1.0 / distance_Squared using reciprocal estimate + refinement.
        float64x2_t r_dist_sq = vrecpeq_f64(distance_Squared);
        // Newton-Raphson for reciprocal: x_{n+1} = x_n * (2 - a*x_n)
        r_dist_sq = vmulq_f64(r_dist_sq, vfmsq_f64(vdupq_n_f64(2.0), distance_Squared, r_dist_sq)); // Step 1
        r_dist_sq = vmulq_f64(r_dist_sq, vfmsq_f64(vdupq_n_f64(2.0), distance_Squared, r_dist_sq)); // Step 2

        float64x2_t mag = vmulq_f64(vdupq_n_f64(0.01), r_dist_sq);
        mag = vmulq_f64(mag, distance_Reciprocal);

        // Store the calculated magnitudes.
        vst1q_f64(&magnitudes[i*2], mag);
    }
    #else // Scalar version if NEON is not available
        // Calculate magnitudes without vectorization
        for(intnative_t i=0; i<INTERACTIONS_COUNT; ++i){
            const double dx = position_Deltas[0][i];
            const double dy = position_Deltas[1][i];
            const double dz = position_Deltas[2][i];
            const double distance_Squared = dx*dx + dy*dy + dz*dz;
            const double distance = sqrt(distance_Squared);
            magnitudes[i] = 0.01 / (distance_Squared * distance);
        }
    #endif

    // Use the calculated magnitudes of force to update the velocities for all
    // of the bodies.
    #ifdef __ARM_NEON
    // Process remaining interactions if INTERACTIONS_COUNT was odd.
    // The loop above handles pairs up to ROUNDED_INTERACTIONS_COUNT.
    // If ROUNDED_INTERACTIONS_COUNT > INTERACTIONS_COUNT, the last slot is unused.
    intnative_t k=0;
    for(intnative_t i=0; i<BODIES_COUNT-1; ++i)
        for(intnative_t j=i+1; j<BODIES_COUNT; ++j, ++k){
            // Precompute the products of the mass and magnitude since it can be
            // reused a couple times.
            const double
              i_mass_magnitude=bodies[i].mass*magnitudes[k],
              j_mass_magnitude=bodies[j].mass*magnitudes[k];
            for(intnative_t m=0; m<3; ++m){
                bodies[i].velocity[m]-=position_Deltas[m][k]*j_mass_magnitude;
                bodies[j].velocity[m]+=position_Deltas[m][k]*i_mass_magnitude;
            }
        }
    #else // Scalar version
        intnative_t k=0;
        for(intnative_t i=0; i<BODIES_COUNT-1; ++i)
            for(intnative_t j=i+1; j<BODIES_COUNT; ++j, ++k){
                const double
                  i_mass_magnitude=bodies[i].mass*magnitudes[k],
                  j_mass_magnitude=bodies[j].mass*magnitudes[k];
                for(intnative_t m=0; m<3; ++m){
                    bodies[i].velocity[m]-=position_Deltas[m][k]*j_mass_magnitude;
                    bodies[j].velocity[m]+=position_Deltas[m][k]*i_mass_magnitude;
                }
            }
    #endif

    // Use the updated velocities to update the positions for all of the bodies.
    for(intnative_t i=0; i<BODIES_COUNT; ++i)
        for(intnative_t m=0; m<3; ++m)
            bodies[i].position[m]+=0.01*bodies[i].velocity[m];
}


// Calculate the momentum of each body and conserve momentum of the system by
// adding to the Sun's velocity the appropriate opposite velocity needed in
// order to offset that body's momentum.
static void offset_Momentum(body bodies[]){
    for(intnative_t i=0; i<BODIES_COUNT; ++i)
        for(intnative_t m=0; m<3; ++m)
            bodies[0].velocity[m]-=
              bodies[i].velocity[m]*bodies[i].mass/SOLAR_MASS;
}


// Output the total energy of the system.
static void output_Energy(body bodies[]){
    double energy=0;
    for(intnative_t i=0; i<BODIES_COUNT; ++i){

        // Add the kinetic energy for each body.
        energy+=0.5*bodies[i].mass*(
          bodies[i].velocity[0]*bodies[i].velocity[0]+
          bodies[i].velocity[1]*bodies[i].velocity[1]+
          bodies[i].velocity[2]*bodies[i].velocity[2]);

        // Add the potential energy between this body and every other body.
        for(intnative_t j=i+1; j<BODIES_COUNT; ++j){
            double position_Delta[3];
            for(intnative_t m=0; m<3; ++m)
                position_Delta[m]=bodies[i].position[m]-bodies[j].position[m];

            energy-=bodies[i].mass*bodies[j].mass/sqrt(
              position_Delta[0]*position_Delta[0]+
              position_Delta[1]*position_Delta[1]+
              position_Delta[2]*position_Delta[2]);
        }
    }

    // Output the total energy of the system.
    printf("%.9f\n", energy);
}


int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_steps>\n", argv[0]);
        return 1;
    }
    intnative_t n = atoi(argv[1]);
    if (n < 0) {
         fprintf(stderr, "Number of steps must be non-negative.\n");
         return 1;
    }

    offset_Momentum(solar_Bodies);
    output_Energy(solar_Bodies);
    for(; n--; advance(solar_Bodies)); // Use prefix decrement
    output_Energy(solar_Bodies);

    return 0; // Explicit return 0 for success
}