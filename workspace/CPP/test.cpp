// #include <iostream>
// #include <concepts>

template<typename T>
struct is_integral {
    constexpr static bool value = false;
};

template<typename T>
constexpr static bool is_integral_v = is_integral<T>::value;

template<>
struct is_integral<short> {
    constexpr static bool value = true;
};

template<>
struct is_integral<int> {
    constexpr static bool value = true;
};

template<>
struct is_integral<long> {
    constexpr static bool value = true;
};

template<>
struct is_integral<long long> {
    constexpr static bool value = true;
};

template<typename T>
struct is_float {
    constexpr static bool value = false;
};

template<typename T>
constexpr static bool is_float_v = is_float<T>::value;

template<>
struct is_float<float> {
    constexpr static bool value = true;
};

template<>
struct is_float<double> {
    constexpr static bool value = true;
};

template<bool Cond, typename T>
struct enable_if {};

template<typename T>
struct enable_if<true, T> {
    using type = T;
};

template<bool Cond, typename T>
using enable_if_t = typename enable_if<Cond, T>::type;

template<typename T>
using Real = enable_if_t<is_float_v<T> || is_integral_v<T>, T>;

template<typename T, typename U>
struct Complex {
    Real<T> real;
    Real<U> imag;
};

int main() {
    Complex<int, int> z1 { 1, 2 };
    Complex<int, double> z2 { 2, 2 };
    return 0;
}
