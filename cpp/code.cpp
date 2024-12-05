#include <random>
#include <string>
#include <array>
#include <ranges>
#include <cstdint>
#include <chrono>
#include <format>
#include <iostream>


int main()
{
    auto start = std::chrono::system_clock::now();

    {
        std::random_device rd {};
        std::mt19937 engine {rd()};
        std::uniform_int_distribution<int> dist1 {0, 9999};
        std::uniform_int_distribution<int> dist2 {1, 9};
        int const mod = dist2(engine);
        int const picked_number = dist1(engine);
        std::array<int, 10000> array {};
        for(int const i: std::ranges::views::iota(0, 10000)) {
            for(int const j: std::ranges::views::iota(0, 100000)) {
                array[i] += j % mod;
            }
            array[i] += picked_number;
        }
    }

    auto end = std::chrono::system_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds> (end-start);

    double duration = static_cast<double>(duration_ms.count()) / 1000000;

    std::cout << std::format("C++ 用时: {}s", duration) << std::endl;

    return 0;

}