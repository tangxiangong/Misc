#include <iostream>
#include "linked_list.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    LinkedList<int> list {};
    list.push(1);
    list.push(2);
    list.push(3);
    list.push(4);
    list.push(5);
    list.push(6);
    list.print();
    return 0;
}
