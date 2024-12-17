//
// Created by 小雨 on 2024/12/17.
//
#include <iostream>

using std::shared_ptr;

template<typename T>
class Node {
private:
    T m_data;
    shared_ptr<Node<T>> m_left = nullptr;
    shared_ptr<Node<T>> m_right = nullptr;
public:
    Node() = default;
    explicit Node(const T&);
    T& data();
    shared_ptr<Node<T>> left();
    shared_ptr<Node<T>> right();
};

template <typename T>
using NodePtr = shared_ptr<Node<T>>;


template<typename T>
T &Node<T>::data() {
    return m_data;
}

template<typename T>
NodePtr<T> Node<T>::right() {
    return m_right;
}

template<typename T>
NodePtr<T> Node<T>::left() {
    return m_left;
}

template<typename T>
Node<T>::Node(const T& data) : m_data {data} {}


template<typename T>
class Tree {
private:
    NodePtr<T> m_root = nullptr;
    size_t m_size = 0;
public:
    Tree() = default;
    explicit Tree(NodePtr<T>);
};

template<typename T>
Tree<T>::Tree(NodePtr<T> root) : m_root {root} {}
