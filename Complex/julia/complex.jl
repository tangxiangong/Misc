struct ComplexNum <: Number
    real::Float64
    imaginary::Float64
    function ComplexNum(real = 0.0, imag = 0.0)
        new(real, imag)
    end
end

Base.zero(::Type{ComplexNum})::ComplexNum = ComplexNum()

Base.iszero(z::ComplexNum)::Bool = iszero(z.real) && iszero(z.imaginary)

Base.abs2(z::ComplexNum)::Float64 = z.real * z.real + z.imaginary * z.imaginary

Base.abs(z::ComplexNum)::Float64 = sqrt(abs2(z))

Base.:+(lhs::ComplexNum, rhs::ComplexNum)::ComplexNum = ComplexNum(lhs.real + rhs.real, lhs.imaginary + rhs.imaginary)

function Base.:+(lhs::ComplexNum, rhs::T)::ComplexNum where {T <: Real}
    ComplexNum(lhs.real + rhs, lhs.imaginary)
end

function Base.:+(lhs::T, rhs::ComplexNum)::ComplexNum where {T <: Real} 
    ComplexNum(rhs.real + lhs, rhs.imaginary)
end

Base.:-(z::ComplexNum)::ComplexNum = ComplexNum(-z.real, -z.imaginary)

Base.:-(lhs::ComplexNum, rhs::ComplexNum)::ComplexNum = lhs + (-rhs)

function Base.:-(lhs::ComplexNum, rhs::T)::ComplexNum where {T <: Real} 
    lhs + (-rhs)
end

function Base.:-(lhs::T, rhs::ComplexNum)::ComplexNum where {T <: Real} 
    lhs + (-rhs)
end