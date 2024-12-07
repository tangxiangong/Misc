mod complex;
use complex::Complex;

fn main() {
    let z1 = Complex::new(1 as f64, 0 as f64);
    let z2 = Complex::polar(1.1, 2.2);
    let z3 = z1 + z2;
    println!("{:?}", z1);
    println!("{:?}", z3);
}
