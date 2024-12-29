//
// Created by 小雨 on 2024/12/26.
//
module;
#include <utility>
export module smart_pointer;

/* 智能独有指针
 * 只能进行移动操作，无法进行拷贝操作！
 * Box<T>::make(args...): 分配 T(args...) 并返回地址
 *
 * */
export template<typename Tp>
struct Box {
    // DELETE 默认无参构造
    Box() = delete;

    // DELETE 默认拷贝构造
    Box(const Box &) = delete;

    // DELETE 默认拷贝赋值运算符
    Box &operator=(const Box &) = delete;

    // 移动构造函数
    Box(Box &&other) noexcept : m_ptr{other.m_ptr} {
        other.m_ptr = nullptr;
    }

    // 移动赋值运算符
    Box &operator=(Box &&rhs) noexcept {
        if (this != &rhs) [[likely]] {
            delete m_ptr;
            m_ptr = std::exchange(rhs.m_ptr, nullptr);
        }
        return *this;
    }

    Tp *get_raw() {
        return m_ptr;
    }

    Tp *release() {
        Tp *ptr = std::exchange(m_ptr, nullptr);
        return ptr;
    }

    void swap(Box &other) noexcept {
        std::swap(m_ptr, other.m_ptr);
    }

    void reset(Tp *src) {
        const Tp *ptr = std::exchange(m_ptr, src);
        delete ptr;
    }

    // 重载 ->
    Tp *operator->() {
        return m_ptr;
    }

    // 解引用
    Tp &operator*() {
        return *m_ptr;
    }

    bool is_nullptr() {
        return m_ptr == nullptr;
    }

    template<typename... Args>
    static Box make(Args &&... args) {
        return Box(new Tp(std::forward<Args>(args)...));
    }

    ~Box() {
        if (m_ptr != nullptr) {
            delete m_ptr;
            m_ptr = nullptr;
        }
    }

private:
    Tp *m_ptr = nullptr;

    explicit Box(Tp *ptr) : m_ptr{ptr} {
    }
};


template<typename Tp>
struct RcBase {
    Tp *m_ptr = nullptr;
    size_t m_ref_count = 0;

    explicit RcBase(Tp *ptr) : m_ptr{ptr} {
    }
};

export template<typename Tp, typename Base = RcBase<Tp> >
struct Rc {
    Rc() = delete;

    Rc(const Rc &other) : m_base{other.m_base} { ++m_base->m_ref_count; }

    Rc &operator=(const Rc<Tp> &other) {
        if (m_base != other.m_base) [[likely]] {
            delete m_base->m_ptr;
            m_base->m_ptr = nullptr;
            delete m_base;
            m_base = other.m_base;
            ++m_base->m_ref_count;
        }
        return *this;
    }

    Rc(Rc &&other) noexcept : m_base{other.m_base} {
        other.m_base = nullptr;
    }

    Rc &operator=(Rc<Tp> &&other) {
        if (m_base != other.m_base) {
            delete m_base->m_ptr;
            m_base->m_ptr = nullptr;
            m_base = std::exchange(other.m_base, nullptr);
        }
        return *this;
    }

    bool operator==(const Rc<Tp> &other) {
        return m_base == other.m_base;
    }

    Tp *operator->() {
        return m_base->m_ptr;
    }

    Tp &operator*() {
        return *m_base->m_ptr;
    }

    template<typename... Args>
    static Rc<Tp> make(Args... args) {
        Base *base = new Base(new Tp(std::forward<Args>(args)...));
        return Rc(base);
    }

    [[nodiscard]] size_t count() const {
        return m_base->m_ref_count;
    }

    Tp *get_raw() {
        return m_base->m_ptr;
    }

    ~Rc() {
        --m_base->m_ref_count;
        if (m_base->m_ref_count == 0) {
            delete m_base->m_ptr;
            m_base->m_ptr = nullptr;
            delete m_base;
            m_base = nullptr;
        }
    }

private:
    Base *m_base;

    explicit Rc(Base *base) : m_base{base} {
        ++m_base->m_ref_count;
    }
};
