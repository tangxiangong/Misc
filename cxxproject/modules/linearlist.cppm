//
// Created by 小雨 on 2024/12/29.
//
module;
#include <memory>
export module linearlist;
using std::shared_ptr;
using std::make_shared;

export template<typename T>
class List {
public:
    struct Node {
        T m_data;
        shared_ptr<Node> m_next = nullptr;
        static shared_ptr<Node> make(const T& data, shared_ptr<Node> node = nullptr) {
            return make_shared<Node>(data, node);
        }
    };
    struct Iterator {
        Iterator& operator++() {
            if(m_raw) {
                m_raw = m_raw->m_next;
            }
            return *this;
        }
        Iterator operator++(int) {
            ++(*this);
            auto tmp = *this;
//            Iterator tmp;
//            if (m_raw) {
//                tmp = Iterator(m_raw->m_next);
//            } else {
//                tmp = *this;
//            }
            return tmp;
        }
        bool operator!=(const Iterator& other) {
            return m_raw != other.m_raw;
        }
        T& operator*() {
            return m_raw->m_data;
        }
        explicit Iterator(const shared_ptr<Node>& node) : m_raw {node} {}
    private:
        shared_ptr<Node> m_raw;
    };
    Iterator begin() {
        return Iterator(m_head);
    }
    Iterator end() {
        return Iterator(m_tail->m_next);
    }
    List() = default;
    [[nodiscard]] bool is_empty() const { return m_head == nullptr; }
    [[nodiscard]] size_t len() const { return m_len; }
    void push(const T& data) {
        auto node = Node::make(data);
        if (m_head == nullptr) {
            m_head = node;
            m_tail = node;
        } else {
            m_tail->m_next = node;
            m_tail = node;
        }
        m_len++;
    }

private:
    shared_ptr<Node> m_head = nullptr;
    shared_ptr<Node> m_tail = nullptr;
    size_t m_len = 0;
};