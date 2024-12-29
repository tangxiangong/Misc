template <typename T>
struct is_number {
    constexpr static bool value = false;
};

template <typename T>
constexpr bool is_number_v = is_number<T>::value;

#define DEFINE_IS_NUMBER_FOR_TYPE(T) \
    template <> \
    struct is_number<T> { \
        constexpr static bool value = true; \
    };

DEFINE_IS_NUMBER_FOR_TYPE(short);
DEFINE_IS_NUMBER_FOR_TYPE(int);
DEFINE_IS_NUMBER_FOR_TYPE(long);
DEFINE_IS_NUMBER_FOR_TYPE(long long);
DEFINE_IS_NUMBER_FOR_TYPE(unsigned short);
DEFINE_IS_NUMBER_FOR_TYPE(unsigned int);
DEFINE_IS_NUMBER_FOR_TYPE(unsigned long);
DEFINE_IS_NUMBER_FOR_TYPE(unsigned long long);
DEFINE_IS_NUMBER_FOR_TYPE(float);
DEFINE_IS_NUMBER_FOR_TYPE(double);


template<typename T>
concept Num = is_number_v<T> && requires (T a, T b) {
    {a + b};
    {a - b};
    {a * b};
    {a / b};
};

template<Num T, Num U> requires requires (T a, U b) {
    a + b;
}