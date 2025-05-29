import .utils:vectorize


@doc raw"""
幂律分布随机数
"""
function randpower(α)
    α > 0 || throw(ArgumentError("参数需为正"))
    r = rand()
    return (1-r)^(-1/α)-1
end
randpower(α, N::Integer) = vectorize(randpower, N, α)