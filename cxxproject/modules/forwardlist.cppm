//
// Created by 小雨 on 2024/12/29.
//
module;
#include <string>
#include <memory>
#include <variant>
#include <exception>
#include <unordered_set>
export module forwardlist;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::unordered_set;

struct CircularListError: public std::exception {
public:
    CircularListError() : message { "Error: caused by the circular linked list." } {}
    explicit CircularListError(const string& str) : message {"Error: " + str} {}
    ~CircularListError() override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
private:
    string message;
};

struct DuplicateNodeError: public std::exception {
public:
    DuplicateNodeError() : message { "Error: caused by the deplicate node." } {}
    explicit DuplicateNodeError(const string& str) : message {"Error: " + str} {}
    ~DuplicateNodeError() override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
private:
    string message;
};

export template<typename T>
class List {
public:
    struct Node {
        friend List;
        explicit Node(const T& data) :
                        m_data{data}, m_next{nullptr}, m_weak_next {m_next} {}
        static shared_ptr<Node> make(const T& data) {
            return make_shared<Node>(data);
        }
        const T& data() { return m_data; }
        const shared_ptr<Node>& next() { return m_next; }
    private:
        T m_data;
        shared_ptr<Node> m_next;
        weak_ptr<Node> m_weak_next;
    };
    struct Iterator {
        Iterator& operator++() {
            if(m_raw) {
                m_raw = m_raw->next();
            }
            return *this;
        }
        Iterator operator++(int) {
            ++(*this);
            auto tmp = *this;
            return tmp;
        }
        bool operator!=(const Iterator& other) {
            return m_raw != other.m_raw;
        }
        const T& operator*() {
            return m_raw->data();
        }
        explicit Iterator(const shared_ptr<Node>& node) : m_raw {node} {}
    private:
        shared_ptr<Node> m_raw;
    };
    Iterator begin() {
        return Iterator(m_head);
    }
    Iterator end() {
        return Iterator(m_tail->next());
    }
    List() = default;
    [[nodiscard]] [[maybe_unused]] bool empty() const { return m_head == nullptr; }
    [[nodiscard]] [[maybe_unused]] size_t capacity() const { return m_capacity; }
    [[nodiscard]] [[maybe_unused]] bool has_circle() const { return m_circular; }

    [[maybe_unused]] void append(const T& data) {
        auto node = Node::make(data);
        update(node);
    }

    [[maybe_unused]] void append(const shared_ptr<Node>& node_ptr) {
        if(m_circular)
            throw CircularListError {"This list can not be appended, as it has already been circular."};
        // 如果新插入的节点已在链表中，则表示这个链表已成环/重复插入尾节点
        if(m_nodes.contains(node_ptr)) {
            if(node_ptr == m_tail) {
                throw DuplicateNodeError {"Duplicated tail node"};   // 重复插入尾节点
            } else {
                m_tail->m_weak_next = node_ptr;
                m_circular = true;
            }
        } else {
            update(node_ptr);
        }
    }

    [[maybe_unused]] shared_ptr<Node> circular_node() {
        if(!m_circular) {
            return nullptr;
        } else {
            return m_tail->m_weak_next.lock();
        }
    }

private:
    shared_ptr<Node> m_head = nullptr;
    shared_ptr<Node> m_tail = nullptr;
    size_t m_capacity = 0;
    unordered_set<shared_ptr<Node>> m_nodes;
    bool m_circular = false;
    void update(const shared_ptr<Node>& node_ptr) {
        m_nodes.insert(node_ptr);
        if (m_head == nullptr) {
            m_head = node_ptr;
            m_tail = node_ptr;
        } else {
            m_tail->m_next = node_ptr;
            m_tail = node_ptr;
        }
        m_capacity++;
    }
};