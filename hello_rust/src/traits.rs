use num_traits::{Num, NumCast};

fn add<T, U, V>(a: T, b: U) -> V
where T: Num + NumCast,
      U: Num + NumCast,
      V: Num + NumCast
{
    let a: V = NumCast::from(a).unwrap();
    let b: V = NumCast::from(b).unwrap();
    a + b
}

// trait obj 

trait Number {
    fn value(&self) -> f64;
}

struct Float(f64);

struct Int(i32);

impl Number for Float {
    fn value(&self) -> f64 { self.0 }
}

impl Number for Int {
    fn value(&self) -> f64 { self.0 as f64 }
}

fn calculate(lst: Vec<Box<dyn Number>>) -> f64 {
    lst.iter().map(|x: &Box<dyn Number>| x.value().sqrt()).sum()
}

fn test() {
    let a = Box::new(Int(1));
    let b = Box::new(Float(3.0));
    let v: Vec<Box<dyn Number>> = vec![a, b];
    // let c = Box::new(1);
    // let d = Box::new(1.);
    // 
    // let v2: Vec<Box<dyn Num>> = vec![c, d];
    // let result: f64 = add(1, 2.0);
    // let c = 1f64 + 2.0;
}