pub enum Real {
    Int(i32),
    Float(f64),
}

pub fn test_enums() {
    let mut vec: Vec<Real> = Vec::new();
    vec.push(Real::Int(1));
    vec.push(Real::Int(2));
    // vec.push(Real::Float(3.14));
    vec.push(Real::Float(4.2));
    
    for value in &vec {
        match value {
            Real::Int(i) => println!("Int: {}", i),
            Real::Float(f) => println!("Float: {}", f),
        }
    }
}