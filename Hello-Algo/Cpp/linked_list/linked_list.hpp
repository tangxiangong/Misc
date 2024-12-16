//
// Created by 小雨 on 2024/12/16.
//

#ifndef LINKED_LIST_LINKED_LIST_HPP
#define LINKED_LIST_LINKED_LIST_HPP
#include <iostream>
#include <format>
#include <sstream>
//#include <type_traits>

using std::shared_ptr;
using std::make_shared;

template<typename T>
class Node {
private:
    T m_data;
    shared_ptr<Node<T>> m_next;

public:
    Node() = default;
    explicit Node(const T& data) : m_data { data }, m_next { nullptr } {}
    T& data() { return m_data; }
    shared_ptr<Node<T>>& next() { return m_next; }
};

template<typename T>
class LinkedList {
private:
    size_t m_size;
    shared_ptr<Node<T>> m_head;
    shared_ptr<Node<T>> m_tail;

public:
    LinkedList() : m_size { 0 }, m_head { nullptr }, m_tail {nullptr } {}
    bool is_empty() { return m_size == 0; }
    void push(const T&);
    void print();
    size_t size() { return m_size; }

};

template<typename T>
void LinkedList<T>::push(const T& data) {
    auto node { make_shared<Node<T>>(data) };
    if(this->is_empty()) {
        m_head = node;
    } else {
        m_tail->next() = node;
    }
    m_tail = node;
    m_size++;
}

template<typename T>
void LinkedList<T>::print() {
    if(this->is_empty()) {
        std::cout << "empty" << std::endl;
    } else {
        auto current = m_head;
        while(current != m_tail) {
            std::cout << std::format("{} -> ", current->data());
            current = current->next();
        }
        std::cout << std::format("{}", current->data()) << std::endl;
    }
}

#endif //LINKED_LIST_LINKED_LIST_HPP
