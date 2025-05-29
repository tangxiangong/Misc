#include <iostream>
#include <array>
#include <vector>
#include <string>
//#include <format>
//#include <compare>
//import employee;

class MyString {
private:
    char* data;
    size_t length;
public:
//    构造函数
    MyString(const char* str) {
    length = std::strlen(str);
    data = new char[length + 1];
    std::strcpy(data, str);
    }
//    移动构造函数
    MyString(MyString&& other) noexcept {
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
    }
//    复制构造函数，进行深拷贝
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
    }
//    赋值运算符重载，进行深拷贝
    MyString& operator=(const MyString& other) {
        if(this != &other) {
            delete[] this->data;
            this->length = other.length;
            this->data = new char[this->length + 1];
            std::strcpy(this->data, other.data);
        }
        return *this;
    }
//    移动赋值运算符重载
    MyString& operator=(MyString&& other) noexcept {
        if(this != &other) {
            delete[] this->data;
            this->data = other.data;
            this->length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }
    ~MyString() { delete[] data; }
};

int main() {
    MyString str1("hello");
    MyString str2 = str1;    // 复制构造函数
    MyString str3("word");
    str3 = str1;    // 赋值运算符
    MyString str4 = std::move(str1);
    return 0;
}


namespace DEL {
    template<typename T>
    void null(T *arr, int length) {
        for (int i = 0; i < length; i++)
            arr[i] = static_cast<T>(0);
    }

    template<typename T>
    void null(std::vector<T> &vec) {
        for (auto &v: vec)
            v = static_cast<T>(0);
    }

    int main() {
        std::vector<double> vec{1, 2, 3, 4};
        null<double>(vec);
        for (const auto &v: vec)
            std::cout << v << std::endl;
        double a[] = {1, 2, 3, 4};
        null<double>(a, 4);
        for (const auto &k: a)
            std::cout << k << std::endl;
        return 0;
    }
}

namespace test{

    template <typename T>
    class Auto_ptr {
    public:
        explicit Auto_ptr(T* ptr) : _ptr(ptr) {}
        T& operator*() { return *_ptr; }
        T* operator->() { return _ptr; }
        ~Auto_ptr()  { delete _ptr; std::cout << "release" << std::endl; }
    private:
        T* _ptr;
    };
    int main() {
        //    {
////    shared_ptr 共享指针
//        std::shared_ptr<int> age(new int(10));
//        std::cout << *age << std::endl;
//        {
////    auto temp = age;
//            std::shared_ptr<int> temp = age;
////        引用计数
//            std::cout << std::format("计数1: {}", age.use_count()) << std::endl;
//        } // release temp
//        std::cout << *age << std::endl;
//        std::cout << std::format("计数2: {}", age.use_count()) << std::endl;
///*    reset
// *.   1. 没有参数，放弃所有权，引用计数减一，若变为零，则释放内存
// *
// * */
//        age.reset();
//        std::cout << std::format("计数3: {}", age.use_count()) << std::endl;
//    }

        //    原生指针
    //    int* page = new int(19);
    //    std::cout << *page << std::endl;
    //    delete page;
        Auto_ptr<int> age(new int(20));
        std::cout << *age << std::endl;
    //    智能指针 RAII

    //    int a {1};
    //    std::strong_ordering result_of_int {a <=> 2};
    //    if(result_of_int == std::strong_ordering::less) { std::cout << " integer less " << std::endl; }
    //    if(result_of_int == std::strong_ordering::greater) { std::cout << " integer greater " << std::endl; }
    //    if(result_of_int == std::strong_ordering::equal) { std::cout << " integer equal " << std::endl; }
    //
    //    float b {0.1 + 0.2};
    //    std::partial_ordering result_of_float {b <=> 0.3};
    //    if(result_of_float == std::partial_ordering::less) { std::cout << " float less" << std::endl; }
    //    if(result_of_float == std::partial_ordering::greater) { std::cout << " float greater" << std::endl; }
    //    if(result_of_float == std::partial_ordering::equivalent) { std::cout << " float equal" << std::endl; }

    //    std::array<int, 5> a {1, 2, 3, 4, 5};
    //    for(const auto& i: a)
    //        std::cout << i << std::endl;
    //    std::cout << std::format("There are {} ways I love you.", 520) << std::endl;
        std::cout << "Hello, World!" << std::endl;
        return 0;
}}