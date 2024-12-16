//
// Created by 小雨 on 2024/12/16.
//
#include <iostream>
#include <functional>

template<typename K, typename V>
class Pair {
private:
    K m_key;
    V m_value;
public:
    Pair() = default;
    Pair(const K& key, const V& value) : m_key {key}, m_value {value} {}
    K& key() { return m_key; }
    V& value() { return m_value; }
};

template<typename K>
size_t calc_hash(const K& key) {
    return std::hash<K>{}(key);
}

template<typename K, typename V>
class HashMap {
private:
    size_t m_size;
    size_t m_capacity;
    size_t m_loadSize;
    double m_loadThreshold;
    size_t m_extendRatio;
};