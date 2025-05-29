//
// Created by 小雨 on 2024/11/24.
//

#include <iostream>
#include <vector>
#include "Matrix.hpp"

int main() {
    std::vector<int> data {1, 2, 3};
    Vec<int> vec1 {data};
    Vec<int> vec2 {3};
    Vec<int> vec3  {vec1 + vec2};
    return 0;
}
