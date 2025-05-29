// 使用aarch64架构的SIMD指令集
use std::arch::aarch64::{
    // ARM-specific 128-bit wide vector of two packed f64
    float64x2_t,
    // Duplicate vector element to vector or scalar
    vdupq_n_f64,
    // Floating-point fused Multiply-Add to accumulator(vector)
    vfmaq_f64,
    // Floating-point fused Multiply-Subtract to accumulator(vector)
    vfmsq_f64,
    // Load multiple single-element structures to one, two, three, or four registers.
    vld1q_f64,
    // Multiply
    vmulq_f64,
    // Reciprocal estimate
    vrecpeq_f64,
    // Square root reciprocal estimate
    vrsqrteq_f64,
    // Store vector to memory
    vst1q_f64,
};
use std::f64::consts::PI;
use std::mem::{MaybeUninit, transmute};

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct Body {
    position: [f64; 3],
    velocity: [f64; 3],
    mass: f64,
}

pub const SOLAR_MASS: f64 = 4.0 * PI * PI;
pub const DAYS_PER_YEAR: f64 = 365.24;
pub const BODIES_COUNT: usize = 5;

pub const INTERACTIONS_COUNT: usize = (BODIES_COUNT * (BODIES_COUNT - 1)) / 2;
pub const ROUNDED_INTERACTIONS_COUNT: usize = (INTERACTIONS_COUNT + 1) & !1;

pub static mut SOLAR_BODIES: [Body; BODIES_COUNT] = [
    Body {
        position: [0.0, 0.0, 0.0],
        velocity: [0.0, 0.0, 0.0],
        mass: SOLAR_MASS,
    },
    Body {
        position: [
            4.841_431_442_464_721,
            -1.160_320_044_027_428_4e0,
            -1.036_220_444_711_231_1e-1,
        ],
        velocity: [
            1.660_076_642_744_037e-3 * DAYS_PER_YEAR,
            7.699_011_184_197_404e-3 * DAYS_PER_YEAR,
            -6.904_600_169_720_63e-5 * DAYS_PER_YEAR,
        ],
        mass: 9.547_919_384_243_266e-4 * SOLAR_MASS,
    },
    Body {
        position: [
            8.343_366_718_244_58,
            4.124_798_564_124_305,
            -4.035_234_171_143_214e-1,
        ],
        velocity: [
            -2.767_425_107_268_624e-3 * DAYS_PER_YEAR,
            4.998_528_012_349_172e-3 * DAYS_PER_YEAR,
            2.304_172_975_737_639_3e-5 * DAYS_PER_YEAR,
        ],
        mass: 2.858_859_806_661_308e-4 * SOLAR_MASS,
    },
    Body {
        position: [
            1.289_436_956_213_913_1e1,
            -1.511_115_140_169_863_1e1,
            -2.233_075_788_926_557_3e-1,
        ],
        velocity: [
            2.964_601_375_647_616e-3 * DAYS_PER_YEAR,
            2.378_471_739_594_809_5e-3 * DAYS_PER_YEAR,
            -2.965_895_685_402_375_6e-5 * DAYS_PER_YEAR,
        ],
        mass: 4.366_244_043_351_563e-5 * SOLAR_MASS,
    },
    Body {
        position: [
            1.537_969_711_485_091_7e1,
            -2.591_931_460_998_796_4e1,
            1.792_587_729_503_711_8e-1,
        ],
        velocity: [
            2.680_677_724_903_893_2e-3 * DAYS_PER_YEAR,
            1.628_241_700_382_423e-3 * DAYS_PER_YEAR,
            -9.515_922_545_197_159e-5 * DAYS_PER_YEAR,
        ],
        mass: 5.151_389_020_466_114_5e-5 * SOLAR_MASS,
    },
];

#[repr(align(16))]
#[derive(Debug, Clone, Copy)]
pub struct Align16(pub [f64; ROUNDED_INTERACTIONS_COUNT]);

pub static mut POSITION_DELTAS: [Align16; 3] = [Align16([0.0; ROUNDED_INTERACTIONS_COUNT]); 3];

pub static mut MAGNITUDES: Align16 = Align16([0.0; ROUNDED_INTERACTIONS_COUNT]);

unsafe fn advance(bodies: *mut Body) {
    let mut k = 0;
    for i in 0..BODIES_COUNT - 1 {
        for j in i + 1..BODIES_COUNT {
            (0..3).for_each(|m| unsafe {
                POSITION_DELTAS[m].0[k] =
                    (*bodies.add(i)).position[m] - (*bodies.add(j)).position[m];
            });
            k += 1;
        }
    }

    for i in 0..ROUNDED_INTERACTIONS_COUNT / 2 {
        let mut position_delta = [MaybeUninit::<float64x2_t>::uninit(); 3];
        position_delta[0] = unsafe { MaybeUninit::new(vld1q_f64(&POSITION_DELTAS[0].0[i * 2])) };
        position_delta[1] = unsafe { MaybeUninit::new(vld1q_f64(&POSITION_DELTAS[1].0[i * 2])) };
        position_delta[2] = unsafe { MaybeUninit::new(vld1q_f64(&POSITION_DELTAS[2].0[i * 2])) };

        let position_delta: [float64x2_t; 3] = unsafe { transmute(position_delta) };

        let mut distance_squared = unsafe { vmulq_f64(position_delta[0], position_delta[0]) };
        distance_squared =
            unsafe { vfmaq_f64(distance_squared, position_delta[1], position_delta[1]) };
        distance_squared =
            unsafe { vfmaq_f64(distance_squared, position_delta[2], position_delta[2]) };

        let distance_reciprocal = unsafe { vrsqrteq_f64(distance_squared) };

        // 使用牛顿-拉夫森法进行平方根倒数的精确计算
        // 步骤1: 使用 x_{n+1} = x_n * (1.5 - 0.5 * a * x_n^2) 公式进行优化
        let half_dist_sq = unsafe { vmulq_f64(vdupq_n_f64(0.5), distance_squared) };
        let mut term = unsafe {
            vfmsq_f64(
                vdupq_n_f64(1.5),
                half_dist_sq,
                vmulq_f64(distance_reciprocal, distance_reciprocal),
            )
        };
        let mut distance_reciprocal = unsafe { vmulq_f64(distance_reciprocal, term) };

        // 步骤2: 再次应用相同的计算以提高精度
        term = unsafe {
            vfmsq_f64(
                vdupq_n_f64(1.5),
                half_dist_sq,
                vmulq_f64(distance_reciprocal, distance_reciprocal),
            )
        };
        distance_reciprocal = unsafe { vmulq_f64(distance_reciprocal, term) };

        // 计算距离平方的倒数
        let r_dist_sq = unsafe { vrecpeq_f64(distance_squared) };
        // 使用牛顿-拉夫森法优化倒数: x_{n+1} = x_n * (2 - a*x_n)
        let r_dist_sq = unsafe {
            vmulq_f64(
                r_dist_sq,
                vfmsq_f64(vdupq_n_f64(2.0), distance_squared, r_dist_sq),
            )
        };
        let r_dist_sq = unsafe {
            vmulq_f64(
                r_dist_sq,
                vfmsq_f64(vdupq_n_f64(2.0), distance_squared, r_dist_sq),
            )
        };

        // 计算力的大小: mag = 0.01 * r_dist_sq * distance_reciprocal
        let mag = unsafe { vmulq_f64(vdupq_n_f64(0.01), r_dist_sq) };
        let mag = unsafe { vmulq_f64(mag, distance_reciprocal) };

        // 存储计算出的力大小
        unsafe { vst1q_f64(&mut MAGNITUDES.0[i * 2], mag) };

        // 使用计算出的力大小更新所有天体的速度
        let mut k = 0;
        for i in 0..BODIES_COUNT - 1 {
            for j in i + 1..BODIES_COUNT {
                // 预先计算质量与力大小的乘积，因为它可以被重复使用
                let i_mass_magnitude = unsafe { (*bodies.add(i)).mass * MAGNITUDES.0[k] };
                let j_mass_magnitude = unsafe { (*bodies.add(j)).mass * MAGNITUDES.0[k] };

                (0..3).for_each(|m| unsafe {
                    (*bodies.add(i)).velocity[m] -= POSITION_DELTAS[m].0[k] * j_mass_magnitude;
                    (*bodies.add(j)).velocity[m] += POSITION_DELTAS[m].0[k] * i_mass_magnitude;
                });
                k += 1;
            }
        }

        // 使用更新后的速度更新所有天体的位置
        for i in 0..BODIES_COUNT {
            for m in 0..3 {
                unsafe {
                    (*bodies.add(i)).position[m] += 0.01 * (*bodies.add(i)).velocity[m];
                }
            }
        }
    }
}

unsafe fn offset_momentum(bodies: *mut Body) {
    for i in 0..BODIES_COUNT {
        for m in 0..3 {
            unsafe {
                (*bodies.add(0)).velocity[m] -=
                    (*bodies.add(i)).velocity[m] * (*bodies.add(i)).mass / SOLAR_MASS;
            }
        }
    }
}

unsafe fn output_energy(bodies: *mut Body) {
    let mut energy = 0.0;
    for i in 0..BODIES_COUNT {
        let mut k = 0;
        for j in 0..BODIES_COUNT {
            if i != j {
                let mut mag = 0.0;
                (0..3).for_each(|m| unsafe {
                    mag += POSITION_DELTAS[m].0[k] * (*bodies.add(j)).velocity[m];
                });
                unsafe {
                    energy += (*bodies.add(i)).mass * (*bodies.add(j)).mass / mag.abs();
                }
                k += 1;
            }
        }
    }
    println!("Energy: {}", energy);
}

pub fn solve(n: i32) {
    unsafe {
        let raw_mut_bodies = &raw mut SOLAR_BODIES as *mut Body;
        offset_momentum(raw_mut_bodies);
        output_energy(raw_mut_bodies);
        for _ in 0..n {
            advance(raw_mut_bodies);
        }
        output_energy(raw_mut_bodies);
    }
}
