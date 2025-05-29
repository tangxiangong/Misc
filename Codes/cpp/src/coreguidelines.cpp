#include <cstddef>
#include <iostream>

/*
* Core Guidelines
*/

/*
* P.5: Prefer compile-time checking to run-time checking
*
* Reason: Code clarity and performance. You don't need to write error handlers for errors caught at compile time.
*/

using Int = std::size_t;
// Int is an alias used for integers
void example_1() {
    int bits = 0;  // don't: avoidable code
    for(Int i = 1; i; i <<= 1) 
        ++bits;
    if(bits < 32) 
        std::cerr << "Int too small\n";
    // This example fails to achieve what it is trying to achieve (because overflow is undefined) and should be replaced with a simple `static_assert`:
    static_assert(sizeof(Int) >= 4); // do: compile-time check
    // Or better still just use the type system and replace `Int` with `int32_t`.
}

// Example 2:
void read(int *p, int n); // read max n integers into *p

void example_2() {
    int a[100];
    read(a, 1000); // bad, off the end
    // Better 
    // void read(std::span<int> r);
    // read(a); // better: let the compiler figure out the number of elements
    // Alternative formulation: Don't postpone to run time what can be done well at compile time.
}

#include <initializer_list>
#include <type_traits>

namespace std_ {
    inline constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();

    template<class T>
    concept Integral_Constant_Like /*integral-constant-like*/ = // exposition only
    std::is_integral_v<decltype(T::value)> && 
    !std::is_same_v<bool, std::remove_const_t<decltype(T::value)>> &&
    std::convertible_to<T, decltype(T::value)> &&
    std::equality_comparable_with<T, decltype(T::value)> &&
    std::bool_constant<T() == T::value>::value &&
    std::bool_constant<static_cast<decltype(T::value)>(T()) == T::value>::value;
}
