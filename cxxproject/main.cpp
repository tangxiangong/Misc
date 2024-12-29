#include <print>
#include <memory>
#include <iostream>
import smart_pointer;
import forwardlist;

int main() {
    int a = 10;
    List<int> list {1, 2, 3, 4, 5, a};
    const auto n1 = List<int>::Node::make(1);
    const auto n2 = List<int>::Node::make(2);
    const auto n3 = List<int>::Node::make(3);
    const auto n4 = List<int>::Node::make(4);
    const auto n5 = List<int>::Node::make(5);

    list.append(n1);
    list.append(n2);
    list.append(n3);
    list.append(n4);
    list.append(n5);
    list.append(n1);

    int& node = list.get_mut(4);
    node = 10;
    list.display();
    std::cout << "Capacity: " << list.capacity() << std::endl;
    std::cout << "Circular: " << std::boolalpha << list.has_circle() << std::endl;

    return 0;
}

namespace DEL {
#include <string>
    struct Student {
        std::string name;
        size_t id;
    };
    void smart_pointer() {
        {
            auto stuPtr1 = Box<Student>::make("stu1", 100001);
            auto stuPtr2 = Box<Student>::make("stu2", 100002);
            std::println("name: {}; id: {}", stuPtr1->name, stuPtr1->id);
            stuPtr1 = std::move(stuPtr2);
            std::println("name: {}; id: {}", stuPtr1->name, stuPtr1->id);
        }
        std::println();
        {
            auto stuPtr1 = Rc<Student>::make("stu1", 100001);
            std::println("Reference Count: {}", stuPtr1.count());
            Rc<Student> stuPtr2 {std::move(stuPtr1)};
            std::println("Reference Count: {}", stuPtr2.count());
            size_t n = stuPtr1.count();
            std::println("moved {}", n);
        }
    }
}