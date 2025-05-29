import Random:randexp
import .utils:vectorize

# include("./utils.jl")

@doc raw"""
稳定分布随机数
"""
randstable(α, N::Integer=1; skewed::Bool=false) = skewed ? _rand_skewed_stable(α, N) : _rand_stable(α, N)

@doc raw"""
单边无偏移量的 ``\alpha``--稳定分布随机数
"""
function _rand_skewed_stable(α)
    0< α < 1 || throw(ArgumentError("参数在 (0,1) 之间"))
    v = (rand()-1/2)*π
    w = randexp()
    c₁ = (cospi(α/2))^(-1/α)
    c₂ = π/2
    temp₁ = sin(α*(v+c₂))
    temp₂ = (cos(v-α*(v+c₂))/w)^(1/α-1)
    temp₃ = (cos(v))^(1/α)
    return c₁*temp₁*temp₂/temp₃
end

_rand_skewed_stable(α, N::Integer) = vectorize(_rand_skewed_stable, N, α)


@doc raw"""
双边对称的 ``\alpha``--稳定分布随机数
"""
function _rand_stable(α)
    0 < α <= 2 || throw(ArgumentError("参数在 (0,2] 之间")) 
    α == 2 && return randn()
    v = rand()-1/2
    w = randexp()
    temp₁ = sinpi(α*v)
    temp₂ = (cospi((1-α)*v)/w)^(1/α-1)
    temp₃ = cospi(v)
    return temp₁ * temp₂ / temp₃
end

_rand_stable(α, N::Integer) = vectorize(_rand_stable, N, α)

