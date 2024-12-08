use std::{
    f64::{consts::PI, NAN},
    ops,
};

#[inline]
pub fn approx_equal(a: f64, b: f64) -> bool {
    (a - b).abs() <= f64::EPSILON
}

#[derive(Copy, Clone, Debug)]
pub struct Complex {
    real: f64,
    imaginary: f64,
}

impl Complex {
    pub fn new(real: f64, imaginary: f64) -> Complex {
        Complex { real, imaginary }
    }

    pub fn abs(&self) -> f64 {
        (self.real * self.real + self.imaginary * self.imaginary).sqrt()
    }

    pub fn arg(&self) -> f64 {
        let temp = if approx_equal(self.real, 0.) {
            NAN
        } else {
            self.imaginary / self.real
        };
        if approx_equal(self.real, 0.) {
            if approx_equal(self.imaginary, 0.) {
                NAN
            } else if self.imaginary > 0. {
                PI / 2.
            } else {
                -PI / 2.
            }
        } else if self.real > 0. {
            temp.atan()
        } else {
            if self.imaginary >= 0. {
                temp.atan() + PI
            } else {
                temp.atan() - PI
            }
        }
    }

    pub fn zero() -> Complex {
        Complex {
            real: 0.,
            imaginary: 0.,
        }
    }

    pub fn polar(rad: f64, arg: f64) -> Complex {
        if approx_equal(rad, 0.) {
            Complex::zero()
        } else {
            let real = rad * arg.cos();
            let imaginary = rad * arg.sin();
            Complex { real, imaginary }
        }
    }

    pub fn to_complex(value: f64) -> Complex {
        Complex::new(value, 0.)
    }

    pub fn is_zero(&self) -> bool {
        approx_equal(self.abs(), 0.)
    }
    // 共轭
    pub fn conj(&self) -> Complex {
        Complex::new(self.real, -self.imaginary)
    }
}

impl ops::Add for Complex {
    type Output = Complex;
    fn add(self, rhs: Self) -> Self::Output {
        Complex::new(self.real + rhs.real, self.imaginary + rhs.imaginary)
    }
}

impl ops::Add<f64> for Complex {
    type Output = Complex;
    fn add(self, rhs: f64) -> Self::Output {
        Complex::new(self.real + rhs, self.imaginary)
    }
}

impl ops::Add<Complex> for f64 {
    type Output = Complex;
    fn add(self, rhs: Complex) -> Self::Output {
        Complex::new(self + rhs.real, rhs.imaginary)
    }
}

impl ops::Add<i32> for Complex {
    type Output = Complex;
    fn add(self, rhs: i32) -> Self::Output {
        Complex::new(self.real + rhs as f64, self.imaginary)
    }
}

impl ops::Add<Complex> for i32 {
    type Output = Complex;
    fn add(self, rhs: Complex) -> Self::Output {
        Complex::new(self as f64 + rhs.real, rhs.imaginary)
    }
}

impl ops::Neg for Complex {
    type Output = Complex;
    fn neg(self) -> Self::Output {
        if self.is_zero() {
            return Complex::zero();
        }
        Complex::new(-self.real, -self.imaginary)
    }
}

impl ops::Sub for Complex {
    type Output = Complex;
    fn sub(self, rhs: Self) -> Self::Output {
        self + (-rhs)
    }
}

impl ops::Sub<f64> for Complex {
    type Output = Complex;
    fn sub(self, rhs: f64) -> Self::Output {
        self + (-rhs)
    }
}

impl ops::Sub<Complex> for f64 {
    type Output = Complex;
    fn sub(self, rhs: Complex) -> Self::Output {
        self + (-rhs)
    }
}

impl ops::Sub<i32> for Complex {
    type Output = Complex;
    fn sub(self, rhs: i32) -> Self::Output {
        self + (-rhs)
    }
}

impl ops::Sub<Complex> for i32 {
    type Output = Complex;
    fn sub(self, rhs: Complex) -> Self::Output {
        self + (-rhs)
    }
}

impl ops::Mul for Complex {
    type Output = Complex;
    fn mul(self, rhs: Self) -> Self::Output {
        if self.is_zero() || rhs.is_zero() {
            Complex::zero()
        } else {
            let real = self.real * rhs.real - self.imaginary * rhs.imaginary;
            let imaginary = self.real * rhs.imaginary + self.imaginary * rhs.real;
            Complex::new(real, imaginary)
        }
    }
}

impl ops::Mul<f64> for Complex {
    type Output = Complex;
    fn mul(self, rhs: f64) -> Self::Output {
        if self.is_zero() || approx_equal(rhs, 0.) {
            Complex::zero()
        } else {
            Complex::new(self.real * rhs, self.imaginary * rhs)
        }
    }
}

impl ops::Mul<Complex> for f64 {
    type Output = Complex;
    fn mul(self, rhs: Complex) -> Self::Output {
        if rhs.is_zero() || approx_equal(self, 0.) {
            Complex::zero()
        } else {
            Complex::new(rhs.real * self, rhs.imaginary * self)
        }
    }
}

impl ops::Mul<i32> for Complex {
    type Output = Complex;
    fn mul(self, rhs: i32) -> Self::Output {
        if self.is_zero() || rhs == 0 {
            Complex::zero()
        } else {
            Complex::new(self.real * rhs as f64, self.imaginary * rhs as f64)
        }
    }
}

impl ops::Mul<Complex> for i32 {
    type Output = Complex;
    fn mul(self, rhs: Complex) -> Self::Output {
        if rhs.is_zero() || self == 0 {
            Complex::zero()
        } else {
            Complex::new(rhs.real * self as f64, rhs.imaginary * self as f64)
        }
    }
}
