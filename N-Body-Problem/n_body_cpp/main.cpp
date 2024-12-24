#include <iostream>
#include <chrono>
#include <format>
#include "nbody.hpp"

int main() {
    size_t n = 50000000;
    auto start = std::chrono::system_clock::now();
    auto[p, c] = nbody(n);
    auto end = std::chrono::system_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds> (end-start);
    double duration = static_cast<double>(duration_ms.count()) / 1000000;
    std::cout << std::format("C++ 用时: {}s", duration) << std::endl;
    // std::printf("%f\n", p);
    // std::printf("%f\n", c);
    return 0;
}