//
// Created by 小雨 on 2024/11/24.
//

#ifndef CPP_20_MATRIX_HPP
#define CPP_20_MATRIX_HPP
#include <iostream>
#include <format>
#include <vector>
using namespace std;

template<typename T>
class Vec {
private:
    vector<T> m_data;
    size_t m_length;
public:
    Vec() = default;
    Vec(size_t);
    Vec(const vector<T>&);

    T& operator[](size_t) const;
    Vec<T> operator+(const Vec<T>&) const;
};

template<typename T>
Vec<T>::Vec(size_t length) {
    vector<T> data(length);
    m_length = length;
    m_data = data;
}

template<typename T>
Vec<T>::Vec(const vector<T>& data) {
    size_t length { data.size() };
    m_length = length;
    m_data = std::move(data);
}

template<typename T>
T &Vec<T>::operator[](size_t index) const { return m_data[index]; }

template<typename T>
Vec<T> Vec<T>::operator+(const Vec<T>& rhsVec) const {
    if(this->m_length != rhsVec.m_length)
        throw logic_error { " Dimension not match. " };
    size_t length { this->m_length };
    vector<T> data(length);
    for(int i {0}; i<length; i++)
        data[i] = this->m_data[i] + rhsVec.m_data[i];
    return Vec<T>(data);
}

#endif //CPP_20_MATRIX_HPP
