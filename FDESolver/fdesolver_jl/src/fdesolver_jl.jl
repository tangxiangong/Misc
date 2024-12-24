module fdesolver_jl

import SpecialFunctions: gamma

export FDE, Discretization, solve

struct FDE{F<:Function}
    fractional_order::Float64
    time_length::Float64
    initial_value::Float64
    spectral_value::Float64
    source_term::F
end

struct Discretization
    eq::FDE
    τ::Float64
end

function solve(discretization::Discretization)
    τ, eq = discretization.τ, discretization.eq
    α, T, x₀, λ = eq.fractional_order, eq.time_length, eq.initial_value, eq.spectral_value
    f = eq.source_term
    nums = floor(Int, T/τ)
    b = map(0:nums-1) do j 
        ((j+1)^(1-α) - j^(1-α))/gamma(1-α)
    end
    c = diff(b)
    t = zeros(nums + 1)
    x = zeros(nums + 1)
    x[1] = x₀
    a = b[1] - τ^α * λ
    @fastmath @inbounds for n in 2:nums+1
        t[n] = (n-1) * τ 
        temp = mapreduce((x,y)->x*y, +, view(c, 1:n-2), Iterators.reverse(view(x, 2:n-1)), init=0.0)
        x[n] = (τ^α * f(t[n]) + b[n-1] * x[1] + temp) / a
    end
    t, x
end

end # module fdesolver_jl
