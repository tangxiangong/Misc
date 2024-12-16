////
//// Created by 小雨 on 2024/12/16.
////
//#include <iostream>
//#include <functional>
//#include <vector>
//
//using std::vector;
//
//template<typename K, typename V>
//class Pair {
//private:
//    K m_key;
//    V m_value;
//public:
//    Pair() = default;
//    Pair(const K& key, const V& value) : m_key {key}, m_value {value} {}
//    K& key() { return m_key; }
//    V& value() { return m_value; }
//};
//
//template<typename K>
//size_t calc_hash(const K& key) {
//    return std::hash<K>{}(key);
//}
//
//template<typename K, typename V>
//class HashMap {
//private:
//    size_t m_size;
//    size_t m_capacity;
//    size_t m_loadSize;
//    double m_loadThreshold;
//    size_t m_extendRatio;
//    vector<vector<Pair<K, V>>> m_buckets;
//
//public:
//    HashMap() = default;
////    HashMap()
//};
#include <vector>
#include <optional>
#include <stdexcept>
#include <functional>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include <format>


template<typename K, typename V>
class HashMap {
private:
    struct Pair {
        K key;
        V value;

        Pair(K k, V v) : key(std::move(k)), value(std::move(v)) {}
    };

    size_t size_;
    size_t capacity_;
    size_t load_size_;
    double load_thres_;
    size_t extend_ratio_;
    std::vector<std::optional<std::vector<Pair>>> buckets_;

    // ... 其他私有方法声明

public:
    // ... 公共方法声明

    HashMap(size_t capacity = 10)
            : size_(0), capacity_(capacity), load_size_(0),
              load_thres_(0.75), extend_ratio_(2),
              buckets_(capacity) {}

    void add(const K& key, const V& value) {
        if (load_factor() > load_thres_) {
            extend();
        }
        size_t idx = calc_idx(key);
        auto& bucket = buckets_[idx];
        if (bucket) {
            auto it = std::find_if(bucket->begin(), bucket->end(),
                                   [&key](const Pair& p) { return p.key == key; });
            if (it != bucket->end()) {
                if (it->value != value) {
                    it->value = value;
                    size_++;
                }
                return;
            }
            bucket->emplace_back(key, value);
        } else {
            bucket = std::vector<Pair>{{key, value}};
            load_size_++;
        }
        size_++;
    }

    void remove(const K& key) {
        size_t idx = calc_idx(key);
        auto& bucket = buckets_[idx];
        if (bucket) {
            auto it = std::find_if(bucket->begin(), bucket->end(),
                                   [&key](const Pair& p) { return p.key == key; });
            if (it != bucket->end()) {
                bucket->erase(it);
                size_--;
                if (bucket->empty()) {
                    bucket.reset();
                    load_size_--;
                }
                return;
            }
        }
        throw KeyError(std::to_string(key));
    }

    class Iterator;
    Iterator begin() const;
    Iterator end() const;
};

// KeyError 异常类
class KeyError : public std::runtime_error {
public:
    explicit KeyError(const std::string& key)
            : std::runtime_error(std::format("'{}' 不存在！", key)) {}
};

// ... Iterator 类的实现
template<typename K, typename V>
class HashMap<K, V>::Iterator {
private:
    const HashMap<K, V>* map_;
    size_t bucket_index_;
    size_t pair_index_;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<const K&, const V&>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    Iterator(const HashMap<K, V>* map, size_t bucket = 0, size_t pair = 0)
            : map_(map), bucket_index_(bucket), pair_index_(pair) {
        find_next_valid();
    }

    value_type operator*() const {
        const auto& bucket = map_->buckets_[bucket_index_];
        const auto& pair = (*bucket)[pair_index_];
        return {pair.key, pair.value};
    }

    Iterator& operator++() {
        advance();
        return *this;
    }

    bool operator==(const Iterator& other) const {
        return map_ == other.map_ &&
               bucket_index_ == other.bucket_index_ &&
               pair_index_ == other.pair_index_;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }

private:
    void find_next_valid() {
        while (bucket_index_ < map_->buckets_.size()) {
            const auto& bucket = map_->buckets_[bucket_index_];
            if (bucket && pair_index_ < bucket->size()) {
                return;
            }
            bucket_index_++;
            pair_index_ = 0;
        }
    }

    void advance() {
        pair_index_++;
        find_next_valid();
    }
};

