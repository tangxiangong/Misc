#include <iostream>

/*
#include <memory>
#include <iostream>
#include <optional>

template<typename T>
class LinkedList {
private:
    // 节点结构定义
    struct Node {
        T data;
        std::shared_ptr<Node> next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    size_t size;

public:
    // 构造函数
    LinkedList() : head(nullptr), size(0) {}

    // 在链表头部插入元素
    void push_front(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        new_node->next = head;
        head = new_node;
        size++;
    }

    // 在链表尾部插入元素
    void push_back(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        if (!head) {
            head = new_node;
        } else {
            auto current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
        size++;
    }

    // 删除头部元素
    bool pop_front() {
        if (!head) return false;
        head = head->next;
        size--;
        return true;
    }

    // 获取指定位置的元素
    std::optional<T> at(size_t index) const {
        if (index >= size) return std::nullopt;
        
        auto current = head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    // 获取链表大小
    size_t get_size() const {
        return size;
    }

    // 判断链表是否为空
    bool empty() const {
        return size == 0;
    }

    // 清空链表
    void clear() {
        head = nullptr;
        size = 0;
    }
};

这个实现具有以下特点：
使用了模板类 template<typename T>，使得链表可以存储任意类型的数据
使用 std::shared_ptr 来管理节点的内存，避免内存泄漏
使用 std::optional 来处理可能失败的操作返回值
提供了基本的链表操作：
push_front(): 头部插入
push_back(): 尾部插入
pop_front(): 删除头部元素
at(): 访问指定位置的元素
get_size(): 获取大小
empty(): 判断是否为空
clear(): 清空链表
使用示例：
int main() {
    LinkedList<int> list;
    
    // 插入一些元素
    list.push_back(1);
    list.push_back(2);
    list.push_front(0);
    
    // 访问元素
    if (auto value = list.at(1)) {
        std::cout << "索引 1 的值是: " << *value << std::endl;
    }
    
    // 删除头部元素
    list.pop_front();
    
    // 检查大小
    std::cout << "链表大小: " << list.get_size() << std::endl;
    
    return 0;
}
这个实现是线程不安全的。如果需要在多线程环境中使用，需要添加适当的同步机制。另外，你还可以根据需求添加更多功能，比如：
迭代器支持
删除指定位置的元素
在指定位置插入元素
查找元素
反转链表等
需要添加其他功能的话，我可以继续帮你实现。
修改代码，使得链表可以存储不同类型的数据

我明白你的需求。我们可以使用 C++ 的 std::variant 来实现一个可以存储不同类型数据的链表。下面是修改后的代码：

#include <memory>
#include <iostream>
#include <optional>
#include <variant>
#include <string>

// 定义可以存储的类型
using DataVariant = std::variant<int, double, std::string, bool>;

class VariantLinkedList {
private:
    struct Node {
        DataVariant data;
        std::shared_ptr<Node> next;
        
        template<typename T>
        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    size_t size;

public:
    VariantLinkedList() : head(nullptr), size(0) {}

    // 支持任意允许类型的 push_front
    template<typename T>
    void push_front(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        new_node->next = head;
        head = new_node;
        size++;
    }

    // 支持任意允许类型的 push_back
    template<typename T>
    void push_back(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        if (!head) {
            head = new_node;
        } else {
            auto current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
        size++;
    }

    // 获取指定位置的元素
    std::optional<DataVariant> at(size_t index) const {
        if (index >= size) return std::nullopt;
        
        auto current = head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    // 打印元素的辅助函数
    void print() const {
        auto current = head;
        while (current) {
            std::visit([](const auto& value) {
                std::cout << value << " ";
            }, current->data);
            current = current->next;
        }
        std::cout << std::endl;
    }

    // 其他基本操作保持不变
    bool pop_front() {
        if (!head) return false;
        head = head->next;
        size--;
        return true;
    }

    size_t get_size() const { return size; }
    bool empty() const { return size == 0; }
    void clear() {
        head = nullptr;
        size = 0;
    }
};

主要的改动：
使用 std::variant 定义了 DataVariant，可以存储多种类型（这里示例使用了 int、double、string 和 bool）
移除了类模板，改为具体的类
添加了 print() 方法使用 std::visit 来打印不同类型的值
使用示例：
int main() {
    VariantLinkedList list;
    
    // 插入不同类型的数据
    list.push_back(42);           // int
    list.push_back(3.14);         // double
    list.push_back("Hello");      // string
    list.push_back(true);         // bool
    
    // 打印所有元素
    std::cout << "链表内容: ";
    list.print();
    
    // 访问特定位置的元素
    if (auto value = list.at(1)) {
        std::visit([](const auto& v) {
            std::cout << "索引 1 的值是: " << v << std::endl;
        }, *value);
    }
    
    std::cout << "链表大小: " << list.get_size() << std::endl;
    
    return 0;
}
如果要获取特定类型的值，可以使用 std::get_if：
if (auto value = list.at(0)) {
    if (auto int_val = std::get_if<int>(&(*value))) {
        std::cout << "这是一个整数: " << *int_val << std::endl;
    }
}
你可以根据需要修改 DataVariant 来支持更多类型：
// 扩展支持的类型
using DataVariant = std::variant<
    int, 
    double, 
    std::string, 
    bool,
    long,
    unsigned int,
    char
    // 添加更多需要的类型...
>;
这个实现允许在同一个链表中存储不同类型的数据，同时保持类型安全。使用 std::variant 和 std::visit 可以优雅地处理不同类型的值。


*/