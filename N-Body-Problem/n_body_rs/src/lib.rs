use std::f64::consts::PI;

static SOLAR_MASS: f64 = 4. * PI * PI;
static DAYS_PER_YEAR: f64 = 365.24;

struct Planet {
    position: [f64; 3],
    velocity: [f64; 3],
    mass: f64,
}

fn offset_momentum(bodies: &mut [Planet]) {
    let mut px = 0.0;
    let mut py = 0.0;
    let mut pz = 0.0;
    for body in bodies.iter() {
        px += body.velocity[0] * body.mass;
        py += body.velocity[1] * body.mass;
        pz += body.velocity[2] * body.mass;
    }
    let first_body = &mut bodies[0];
    first_body.velocity[0] = -px / SOLAR_MASS;
    first_body.velocity[1] = -py / SOLAR_MASS;
    first_body.velocity[2] = -pz / SOLAR_MASS;
}

fn energe(bodies: &[Planet]) -> f64 {
    let mut e = 0.0;
    let nums = bodies.len();
    for i in 0..nums {
        let body = &bodies[i];
        let sq = body.velocity[0] * body.velocity[0]
            + body.velocity[1] * body.velocity[1]
            + body.velocity[2] * body.velocity[2];
        e += 0.5 * body.mass * sq;
        for another_body in bodies.iter().skip(i + 1) {
            let dx = body.position[0] - another_body.position[0];
            let dy = body.position[1] - another_body.position[1];
            let dz = body.position[2] - another_body.position[2];
            let dsq = dx * dx + dy * dy + dz * dz;
            e -= (body.mass * body.mass) / dsq.sqrt();
        }
    }
    e
}

fn advance(bodies: &mut [Planet], dt: f64) {
    let nums = bodies.len();
    for i in 0..nums {
        let (left_bodies, right_bodies) = bodies.split_at_mut(i + 1);
        let body = &mut left_bodies[i];

        for another_body in right_bodies.iter_mut() {
            let dx = body.position[0] - another_body.position[0];
            let dy = body.position[1] - another_body.position[1];
            let dz = body.position[2] - another_body.position[2];
            let dsq = dx * dx + dy * dy + dz * dz;
            let mag = dt / (dsq * dsq.sqrt());

            let mj = another_body.mass * mag;
            body.velocity[0] -= dx * mj;
            body.velocity[1] -= dy * mj;
            body.velocity[2] -= dz * mj;

            let mi = body.mass * mag;
            another_body.velocity[0] -= dx * mi;
            another_body.velocity[1] -= dy * mi;
            another_body.velocity[2] -= dz * mi;
        }
    }
    for body in bodies.iter_mut() {
        body.position[0] += body.velocity[0] * dt;
        body.position[1] += body.velocity[1] * dt;
        body.position[2] += body.velocity[2] * dt;
    }
}

pub fn nbody(n: usize) -> (f64, f64) {
    let sun = Planet {
        position: [0.0, 0.0, 0.0],
        velocity: [0.0, 0.0, 0.0],
        mass: SOLAR_MASS,
    };
    let jupyter = Planet {
        position: [
            4.841_431_442_464_720_90,
            -1.160_320_044_027_428_39,
            -1.036_220_444_711_231_09e-01,
        ],
        velocity: [
            1.66007664274403694e-03 * DAYS_PER_YEAR,
            7.69901118419740425e-03 * DAYS_PER_YEAR,
            -6.90460016972063023e-05 * DAYS_PER_YEAR,
        ],
        mass: 9.54791938424326609e-04 * SOLAR_MASS,
    };
    let saturn = Planet {
        position: [
            8.343_366_718_244_579_87,
            4.124_798_564_124_304_79,
            -4.03523417114321381e-01,
        ],
        velocity: [
            -2.767_425_107_268_624_11e-03 * DAYS_PER_YEAR,
            4.998_528_012_349_172_38e-03 * DAYS_PER_YEAR,
            2.304_172_975_737_639_29e-05 * DAYS_PER_YEAR,
        ],
        mass: 2.858_859_806_661_308_12e-04 * SOLAR_MASS,
    };
    let uranus = Planet {
        position: [
            1.289_436_956_213_913_10e+01,
            -1.511_115_140_169_863_12e+01,
            -2.233_075_788_926_557_34e-01,
        ],
        velocity: [
            2.964_601_375_647_616_18e-03 * DAYS_PER_YEAR,
            2.378_471_739_594_809_50e-03 * DAYS_PER_YEAR,
            -2.965_895_685_402_375_56e-05 * DAYS_PER_YEAR,
        ],
        mass: 4.366_244_043_351_562_98e-05 * SOLAR_MASS,
    };
    let neptune = Planet {
        position: [
            1.537_969_711_485_091_65e+01,
            -2.591_931_460_998_796_41e+01,
            1.792_587_729_503_711_81e-01,
        ],
        velocity: [
            2.680_677_724_903_893_22e-03 * DAYS_PER_YEAR,
            1.628_241_700_382_422_95e-03 * DAYS_PER_YEAR,
            -9.515_922_545_197_158_70e-05 * DAYS_PER_YEAR,
        ],
        mass: 5.151_389_020_466_114_51e-05 * SOLAR_MASS,
    };

    let mut bodies = [sun, jupyter, saturn, uranus, neptune];
    offset_momentum(&mut bodies);
    let prev_energe = energe(&bodies);
    for _ in 0..n {
        advance(&mut bodies, 0.01);
    }
    let cur_energe = energe(&bodies);

    (prev_energe, cur_energe)
}
