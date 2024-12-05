function code() 
    a = zeros(Int32, 10^4)
    u::Int32 = rand(1:10)
    r::Int32 = rand(1:10^4)
    @inbounds for i in 1:10^4 
        @inbounds for j in 1:10^5
            @fastmath a[i] += j % u
        end
        a[i] += r
    end
    nothing
end

duration = @elapsed code()
println("Julia 用时: ", duration, "s")
