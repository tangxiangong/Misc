export module complex;

export class Complex {
public:
    Complex() = default;
    Complex(double, double);
    static Complex Polar(double, double);
    static Complex zero();
private:
    double m_real;
    double m_imag;
    bool m_polar;
    double m_mod;
    double m_arg;
};