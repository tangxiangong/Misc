module n_body_jl

using StaticArrays
using LinearAlgebra

export nbody

const SOLAR_MASS::Float64 = 4.0 * pi * pi
const DAYS_PER_YEAR::Float64 = 365.24

mutable struct Planet
    position::MVector{3, Float64}
    velocity::MVector{3, Float64}
    mass::Float64
end

function offset_momentum!(bodies)
    p = MVector(0.0, 0.0, 0.0)
    @inbounds for body in bodies 
        @. p += body.position * body.mass
    end
    first_body = bodies[1]
    @. first_body.velocity = -p / SOLAR_MASS
end

function energe(bodies) 
    e = 0.0
    d = MVector(0.0, 0.0, 0.0)
    @inbounds @fastmath for i in firstindex(bodies):lastindex(bodies)
        body = bodies[i]
        sq = dot(body.velocity, body.velocity)
        e += 0.5 * body.mass * sq
        @inbounds for j = (firstindex(bodies)+i):lastindex(bodies) 
            @. d = body.position - bodies[j].position
            dsq = dot(d, d)
            e -= body.mass ^ 2 / sqrt(dsq)
        end
    end
    e
end

function advance!(bodies, dt) 
    d = MVector(0.0, 0.0, 0.0)
    @inbounds @fastmath for i = firstindex(bodies):lastindex(bodies)
        body = bodies[i]
        @inbounds for j = (firstindex(bodies)+i):lastindex(bodies)
            @. d = body.position - bodies[j].position
            dsq = dot(d, d)
            mag = dt / (dsq * sqrt(dsq))

            mj = bodies[j].mass * mag
            @. body.velocity -= d * mj

            mi = body.mass * mag
            @. bodies[j].velocity -= d * mi
        end
    end
    @inbounds for i = firstindex(bodies):lastindex(bodies) 
        @. bodies[i].position += bodies[i].velocity * dt
    end
end

function nbody(n::Int)
    sun = Planet(
        MVector(0.0, 0.0, 0.0),
        MVector(0.0, 0.0, 0.0),
        SOLAR_MASS
    )
    jupyter = Planet(
        MVector(
            4.841_431_442_464_720_90,
            -1.160_320_044_027_428_39,
            -1.036_220_444_711_231_09e-01
        ), 
        MVector(
            1.66007664274403694e-03 * DAYS_PER_YEAR,
            7.69901118419740425e-03 * DAYS_PER_YEAR,
            -6.90460016972063023e-05 * DAYS_PER_YEAR
        ),
        9.54791938424326609e-04 * SOLAR_MASS
    )
    saturn = Planet(
        MVector(
            8.343_366_718_244_579_87,
            4.124_798_564_124_304_79,
            -4.03523417114321381e-01
        ),
        MVector(
            -2.767_425_107_268_624_11e-03 * DAYS_PER_YEAR,
            4.998_528_012_349_172_38e-03 * DAYS_PER_YEAR,
            2.304_172_975_737_639_29e-05 * DAYS_PER_YEAR
        ),
        2.858_859_806_661_308_12e-04 * SOLAR_MASS
    )
    uranus = Planet(
        MVector(
            1.289_436_956_213_913_10e+01,
            -1.511_115_140_169_863_12e+01,
            -2.233_075_788_926_557_34e-01
        ),
        MVector(
            2.964_601_375_647_616_18e-03 * DAYS_PER_YEAR,
            2.378_471_739_594_809_50e-03 * DAYS_PER_YEAR,
            -2.965_895_685_402_375_56e-05 * DAYS_PER_YEAR
        ),
        4.366_244_043_351_562_98e-05 * SOLAR_MASS
    )
    neptune = Planet(
        MVector(
            1.537_969_711_485_091_65e+01,
            -2.591_931_460_998_796_41e+01,
            1.792_587_729_503_711_81e-01
        ),
        MVector(
            2.680_677_724_903_893_22e-03 * DAYS_PER_YEAR,
            1.628_241_700_382_422_95e-03 * DAYS_PER_YEAR,
            -9.515_922_545_197_158_70e-05 * DAYS_PER_YEAR
        ),
        5.151_389_020_466_114_51e-05 * SOLAR_MASS
    )

    bodies = MVector(sun, jupyter, saturn, uranus, neptune)
    offset_momentum!(bodies)
    pre_energe = energe(bodies)
    @inbounds for _ in range(1, n)
        advance!(bodies, 0.01)
    end
    cur_energe = energe(bodies)

    pre_energe, cur_energe
end
end # module n_body_jl
