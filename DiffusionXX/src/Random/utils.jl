module utils
    import Base.Threads:@threads
    export vectorize
    function vectorize(RNG::Function, N::Integer, args...)
        N > 0 || throw(ArgumentError("第二个参数需大于 1"))
        N == 1 && return RNG(args...)
        x = zeros(N)
        @threads for k in eachindex(x)
            @inbounds x[k] = RNG(args...)
        end
        return x
    end
end