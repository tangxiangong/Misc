//
// Created by 小雨 on 2024/12/29.
//
module;

#include <print>
#include <string>
#include <memory>
#include <variant>
#include <exception>
#include <unordered_set>
#include <initializer_list>

export module forwardlist;
using std::print;
using std::string;
using std::println;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::unordered_set;
using std::initializer_list;

struct CircularListError final : std::exception {
    CircularListError() : message{"Error: caused by the circular linked list."} {
    }

    explicit CircularListError(const string &str) : message{"Error: " + str} {
    }

    ~CircularListError() override = default;

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

private:
    string message;
};

struct DuplicateNodeError final : std::exception {
    DuplicateNodeError() : message{"Error: caused by the duplicate node."} {
    }

    explicit DuplicateNodeError(const string &str) : message{"Error: " + str} {
    }

    ~DuplicateNodeError() override = default;

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

private:
    string message;
};

struct IndexBoundsError final : std::exception {
    IndexBoundsError() : message{"Error: caused by the index out of bounds."} {
    }

    explicit IndexBoundsError(const string &str) : message{"Error: " + str} {
    }

    ~IndexBoundsError() override = default;

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

private:
    string message;
};

export template<typename T>
struct List {
    struct Node {
        friend List;

        explicit Node(const T &data) : m_data{data}, m_next{nullptr}, m_weak_next{m_next} {
        }

        static shared_ptr<Node> make(const T &data) {
            return make_shared<Node>(data);
        }

        const T &data() { return m_data; }

        const shared_ptr<Node> &next() { return m_next; }

    private:
        T m_data;
        shared_ptr<Node> m_next;
        weak_ptr<Node> m_weak_next;
    };

    struct Iterator {
        friend List;

        Iterator &operator++() {
            if (m_raw) {
                m_raw = m_raw->next();
            }
            return *this;
        }

        Iterator operator++(int) {
            ++*this;
            auto tmp = *this;
            return tmp;
        }

        bool operator!=(const Iterator &other) {
            return m_raw != other.m_raw;
        }

        const T &operator*() {
            return m_raw->data();
        }

        explicit Iterator(const shared_ptr<Node> &node) : m_raw{node} {
        }

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

    List(const initializer_list<T> &list) {
        for (auto value: list) {
            append(value);
        }
    }

    [[nodiscard]] [[maybe_unused]] bool empty() const { return m_head == nullptr; }

    [[nodiscard]] [[maybe_unused]] size_t capacity() const { return m_capacity; }

    [[nodiscard]] [[maybe_unused]] bool has_circle() const { return m_circular; }

    [[maybe_unused]] void append(const T &data) {
        auto node = Node::make(data);
        update(node);
    }

    [[maybe_unused]] void append(const shared_ptr<Node> &node_ptr) {
        if (m_circular)
            throw CircularListError
                    {"This list can not be appended, as it has already been circular."};
        // 如果新插入的节点已在链表中，则表示这个链表已成环/重复插入尾节点
        if (m_nodes.contains(node_ptr)) {
            if (node_ptr == m_tail) {
                throw DuplicateNodeError{"Duplicated tail node"}; // 重复插入尾节点
            }
            m_tail->m_weak_next = node_ptr;
            m_circular = true;
        } else {
            update(node_ptr);
        }
    }

    size_t circular_index() {
        if (!m_circular) {
            throw CircularListError{"This list doesn't have circular node"};
        }
        Iterator it = begin();
        size_t index = 0;
        auto circular_node = m_tail->m_weak_next.lock();
        for (; index < m_capacity; ++index) {
            if (it.m_raw == circular_node) {
                break;
            }
            ++it;
        }
        return index;
    }

    [[maybe_unused]] T &get_mut(const int &index) const {
        if (index >= m_capacity && index < -m_capacity) {
            throw IndexBoundsError
                    {"Index out of bounds. The length of this list is" + std::to_string(m_capacity) + "."};
        }
        const int modified_index = index >= 0 ? index : m_capacity + index;
        auto current = m_head;
        for (size_t i = 1; i <= modified_index; ++i) {
            current = current->m_next;
        }
        return current->m_data;
    }

    [[maybe_unused]] const T &operator[](const int &index) {
        return get_mut(index);
    }

    [[maybe_unused]] void display(this List &self) {
        if (self.empty()) {
            println("Empty list");
        }
        if (!self.m_circular) {
            println("List with length = {} :", self.m_capacity);
            print("{}", self[0]);
            for (size_t i = 1; i < self.capacity(); ++i) {
                print(" -> {}", self[i]);
            }
            println(".");
        } else {
            println("Circular list (the part in '()' is circular part): ");
            const size_t circular_index = self.circular_index();
            for (size_t i = 0; i < circular_index; ++i) {
                print("{} -> ", self[i]);
            }
            print("({}", self[circular_index]);
            for (size_t k = circular_index + 1; k < self.capacity(); ++k) {
                print(" -> {}", self[k]);
            }
            println(").");
        }
    }

private:
    shared_ptr<Node> m_head = nullptr;
    shared_ptr<Node> m_tail = nullptr;
    size_t m_capacity = 0;
    unordered_set<shared_ptr<Node> > m_nodes;
    bool m_circular = false;

    void update(const shared_ptr<Node> &node_ptr) {
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
