include("n_body_jl.jl")
using .n_body_jl

nbody(10);

n = 50_000_000;

duration = @elapsed nbody(n)

println("Julia: ", duration, "s")
