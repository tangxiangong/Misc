module;

#include <type_traits>
#include <numeric>

export module rational;

template <typename T>
concept Integer = std::is_integral_v<T>;

export enum class Sign {
    Positive = 1,
    Negative = -1,
    Zero = 0
};

export template <Integer I>
class RationalNumber {
public:
    RationalNumber() = default;
    RationalNumber(const I&, const I&); 
    Sign sign() const;
private:
    I numeritor = 0;
    I denominator = 1;
};

export template<Integer I>
using rational = RationalNumber<I>;


template <Integer I>
RationalNumber<I>::RationalNumber(const I& p, const I& q) : numeritor(p), denominator(q) {
    if (q == 0)
        throw("Error: the denominator could not be zero!");
    auto divisor = std::gcd(numeritor, denominator);
    numeritor /= divisor;
    denominator /= divisor;
}

template <Integer I>
Sign RationalNumber<I>::sign() const {
    
}