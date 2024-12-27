#include <iostream>
#include <print>
#include <string>
import smart_pointer;

struct Student {
    std::string name;
    size_t id;
};
int main() {
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
    return 0;
}
