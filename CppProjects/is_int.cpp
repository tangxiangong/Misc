#include <iostream>

template <typename T>
struct is_integer {
    static constexpr bool value = false;
};

template <typename T>
constexpr bool is_integer_v = is_integer<T>::value;

template <>
struct is_integer<short> {
    static constexpr bool value = true;
};

template <>
struct is_integer<int> {
    static constexpr bool value = true;
};

template <>
struct is_integer<long> {
    static constexpr bool value = true;
};

template <>
struct is_integer<long long> {
    static constexpr bool value = true;
};

int main() {
    std::cout << std::boolalpha << is_integer_v<int> << std::endl;
}