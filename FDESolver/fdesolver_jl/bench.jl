include("src/fdesolver_jl.jl")
using .fdesolver_jl

begin
    α = 0.7
    T = 10.0
    x₀ = 1.0
    λ = -1.0
    τ = 0.001
    f(t) = exp(-t/10)
end
fde = FDE(α, T, x₀, λ, f)
discretization = Discretization(fde, τ)

solve(Discretization(fde, 0.1))

using BenchmarkTools
bench = @benchmark solve($discretization)
println("Julia 基准测试")
show(stdout, MIME("text/plain"), bench)

println("-----------------------------------")