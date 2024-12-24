#include "nbody.hpp"

static constexpr double SOLAR_MASS { 4 * pi * pi };
static constexpr double DAYS_PER_YEAR { 365.24 };

void offset_momentum(vector<Planet>& bodies) {
    double px { 0.0 };
    double py { 0.0 };
    double pz { 0.0 };

    for(const auto& body: bodies) {
        px += body.velocity[0] * body.mass;
        py += body.velocity[1] * body.mass;
        pz += body.velocity[2] * body.mass;
    }
    Planet& first_body = bodies[0];
    first_body.velocity[0] = -px / SOLAR_MASS;
    first_body.velocity[1] = -py / SOLAR_MASS;
    first_body.velocity[2] = -pz / SOLAR_MASS;
}


double energe(const vector<Planet>& bodies) {
    double e = 0;
    int n = bodies.size();
    for(size_t i=0; i<n; i++) {
        const Planet& body = bodies[i];
        double sq = body.velocity[0] * body.velocity[0]
            + body.velocity[1] * body.velocity[1] 
            + body.velocity[2] * body.velocity[2];
        e += 0.5 * body.mass * sq;
        for(size_t j=i+1; j<n; j++) {
            const Planet& another_body = bodies[j];
            double dx = body.position[0] - another_body.position[0];
            double dy = body.position[1] - another_body.position[1];
            double dz = body.position[2] - another_body.position[2];
            double dsq = dx * dx + dy * dy + dz * dz;
            e -= (body.mass * body.mass) / sqrt(dsq);
        }
    }
    return e;
}


void advance(vector<Planet>& bodies, double dt) {
    size_t n = bodies.size();
    for(size_t i=0; i<n; i++) {
        Planet& body = bodies[i];
        for(size_t j=i+1; j<n; j++) {
            Planet& another_body = bodies[j];
            double dx = body.position[0] - another_body.position[0];
            double dy = body.position[1] - another_body.position[1];
            double dz = body.position[2] - another_body.position[2];
            double dsq = dx * dx + dy * dy + dz * dz;
            double mag = dt / (dsq * sqrt(dsq));

            double mj = another_body.mass * mag;
            body.velocity[0] -= dx * mj;
            body.velocity[1] -= dy * mj;
            body.velocity[2] -= dz * mj;

            double mi = body.mass * mag;
            another_body.velocity[0] -= dx * mi;
            another_body.velocity[1] -= dy * mi;
            another_body.velocity[2] -= dz * mi;
        }
    }
    for(auto& body: bodies) {
        body.position[0] += body.velocity[0] * dt;
        body.position[1] += body.velocity[1] * dt;
        body.position[2] += body.velocity[2] * dt;
    }
}


pair<double, double> nbody(size_t n) {
    Planet sun {
        array{0.0, 0.0, 0.0},
        array{0.0, 0.0, 0.0},
        SOLAR_MASS
    };
    Planet jupyter {
        array{
            4.84143144246472090,
            -1.16032004402742839,
            -1.03622044471123109e-01,
        },
        array{
            1.66007664274403694e-03 * DAYS_PER_YEAR,
            7.69901118419740425e-03 * DAYS_PER_YEAR,
            -6.90460016972063023e-05 * DAYS_PER_YEAR,
        },
        9.54791938424326609e-04 * SOLAR_MASS,
    };
    Planet saturn {
        array{
            8.34336671824457987,
            4.12479856412430479,
            -4.03523417114321381e-01,
        },
        array{
            -2.76742510726862411e-03 * DAYS_PER_YEAR,
            4.99852801234917238e-03 * DAYS_PER_YEAR,
            2.30417297573763929e-05 * DAYS_PER_YEAR,
        },
        2.85885980666130812e-04 * SOLAR_MASS,
    };
    Planet uranus {
        array{
            1.28943695621391310e+01,
            -1.51111514016986312e+01,
            -2.23307578892655734e-01,
        },
        array{
            2.96460137564761618e-03 * DAYS_PER_YEAR,
            2.37847173959480950e-03 * DAYS_PER_YEAR,
            -2.96589568540237556e-05 * DAYS_PER_YEAR,
        },
        4.36624404335156298e-05 * SOLAR_MASS,
    };
    Planet neptune {
        array{
            1.53796971148509165e+01,
            -2.59193146099879641e+01,
            1.79258772950371181e-01,
        },
        array{
            2.68067772490389322e-03 * DAYS_PER_YEAR,
            1.62824170038242295e-03 * DAYS_PER_YEAR,
            -9.51592254519715870e-05 * DAYS_PER_YEAR,
        },
        5.15138902046611451e-05 * SOLAR_MASS,
    };

    vector<Planet> bodies {
        std::move(sun), std::move(jupyter),
        std::move(saturn), std::move(uranus),
        std::move(neptune)
    };
    offset_momentum(bodies);
    double pre_energe = energe(bodies);
    for(size_t k=0; k<n; k++) {
        advance(bodies, 0.01);
    }
    double cur_energe = energe(bodies);

    return pair{pre_energe, cur_energe};
}