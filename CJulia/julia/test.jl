import Libdl:dlopen, dlext

dlopen("./lib/libtest."*dlext)

func(x::Cdouble)::Cdouble = x * x

function trapezoid(func, a, b, dx)
    cfunc = @cfunction(func, Cdouble, (Cdouble,))
    result = @ccall trapezoid(cfunc::Ptr{Cvoid}, a::Cdouble, b::Cdouble, dx::Cdouble)::Cdouble
    return result
end

trapezoid(func, 0, 1, 1e-3)