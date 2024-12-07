use std::{f64::{consts::PI, NAN}, ops};

#[inline]
pub fn appro_equal(a: f64, b: f64) -> bool {
    (a-b).abs() <= f64::EPSILON
}

#[derive(Copy, Clone, Debug)]
pub struct Complex {
    real: f64,
    imag: f64,
    polar_form: bool,
    modulur: f64,
    argument: f64,
}

// 关联函数
impl Complex {
    pub fn new(real: f64, imag: f64) -> Complex {
        let polar_form = false;
        let modulur = (real * real + imag * imag).sqrt();
        let temp = if appro_equal(real, 0.0) { NAN } else { imag/real };
        let argument = if appro_equal(imag, 0.0) { 
            if appro_equal(imag, 0.) { NAN } else if imag > 0. { PI/2. } else { -PI/2. }
        } else if real > 0.0 { temp.atan() } else {
            if imag >= 0. { temp.atan() + PI } else { temp.atan() - PI }
        };
        Complex {real, imag, polar_form, modulur, argument}
    }

    pub fn zero() -> Complex { Complex {real: 0., imag: 0., polar_form: true, modulur: 0., argument: NAN} }
    
    pub fn polar(rad: f64, arg: f64) -> Complex {
        if appro_equal(rad, 0.) { Complex::zero() }
        else {
            let real = rad * arg.cos();
            let imag = rad * arg.sin();
            Complex {real: real, imag: imag, polar_form: true, modulur: rad, argument: arg}
        }
    }
}


impl Complex {
    pub fn is_zero(&self) -> bool {
        self.argument.is_nan()
    }
    // 共轭
    pub fn conj(&self) -> Complex {
        if self.is_zero() { Complex::zero() } 
        else if self.polar_form { Complex::polar(self.modulur, -self.argument) }
        else { Complex::new(self.real, -self.imag) }
    }
    // 模
    pub fn abs(&self) -> f64 { self.modulur }
    // 浮角
    pub fn arg(&self) -> f64 { self.argument }
}

impl ops::Add for Complex {
    type Output = Complex;
    fn add(self, rhs: Self) -> Self::Output {
        Complex::new(self.real + rhs.real, self.imag + rhs.imag)
    }
}

impl ops::Neg for Complex {
    type Output = Complex;
    fn neg(self) -> Self::Output {
        if self.is_zero() { return Complex::zero() }       
        Complex::new(-self.real, -self.imag)
    }
}

impl ops::Add<f64> for Complex {
    type Output = Complex;
    fn add(self, rhs: f64) -> Self::Output {
        Complex::new(self.real + rhs, self.imag)
    }
}

impl ops::Add<Complex> for f64 {
    type Output = Complex;
    fn add(self, rhs: Complex) -> Self::Output {
        Complex::new(self + rhs.real, rhs.imag)
    }
}