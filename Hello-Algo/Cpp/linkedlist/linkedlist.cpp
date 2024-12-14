#include <iostream>
#include <variant>
#include <string>

using Tp = std::variant<int, long int, float, double, std::string, bool>;


typedef struct Node {
    Tp m_data;
    std::shared_ptr<Node> m_next;
    template<typename T>
    Node(const T& data) : m_data {data}, m_next {nullptr} {}
} Node;


class LinkedList {
private:
    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
    
};
