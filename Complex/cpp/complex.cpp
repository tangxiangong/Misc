#include <iostream>
#include <cmath>
#include <numbers>
#include <limits>
#include <type_traits>

using std::numbers::pi;

template<typename T>
concept Real = std::is_integral_v<T> || std::is_floating_point_v<T>;

export bool approx_equal(double a, double b) {
    double diff { fabs(a - b) };
    return diff < std::numeric_limits<double>::epsilon();
}

export class Complex {
public:
    Complex() = default;
    Complex(double, double);
    static Complex zero();
    static Complex polar(double, double);
    bool is_zero();
    Complex conj();
    double real(); 
    double imag();
    double abs();
    double arg();
    Complex operator+(const Complex&);
    Complex operator+(Real auto);
    friend Complex operator+(Real auto, Complex);
    Complex operator-();
    Complex operator-(Complex);
    Complex operator-(Real auto);
    friend Complex operator-(Real auto, Complex);
private:
    double m_real { 0.0 };
    double m_imaginary { 0.0 };
};


Complex::Complex(double real, double imag) : m_real{ real }, m_imaginary { imag } {}

Complex Complex::zero() { return Complex { }; } 

Complex Complex::polar(double modulur, double arg) {
    if(approx_equal(modulur, 0.0))
        return Complex::zero();
    double real { modulur * cos(arg) };
    double imag { modulur * sin(arg) };
    return Complex { real, imag };
}

bool Complex::is_zero() { return approx_equal(this->abs(), 0.0); }

Complex Complex::conj() { return Complex { m_real, -m_imaginary }; }

double Complex::real() { return m_real; }

double Complex::imag() { return m_imaginary; }

double Complex::abs() { return sqrt(m_real * m_real + m_imaginary * m_imaginary); }

double Complex::arg() {
    double ratio { };
    if(approx_equal(m_real, 0.0))
        ratio = std::numeric_limits<double>::quiet_NaN();
    else
        ratio = m_imaginary/m_real;
    if(approx_equal(m_real, 0.0)) {
        if(approx_equal(m_imaginary, 0.0)) 
            return std::numeric_limits<double>::quiet_NaN();
        else if(m_imaginary > 0.0) 
            return pi/2.0;
        else
            return -pi/2.0;
    } else if(m_real > 0.0) {
        return atan(ratio);
    } else {
        if(m_imaginary >= 0.0) 
            return atan(ratio) + pi;
        else
            return atan(ratio) - pi;
    }        
}


Complex operator+(Real auto lhs, Complex rhs) { return Complex { lhs + rhs.real(), rhs.imag() }; }