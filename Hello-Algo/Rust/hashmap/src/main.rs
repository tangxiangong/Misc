use std::collections::HashMap;

fn main() {
    let mut map = HashMap::with_capacity(10);
    let k = "aaaa";
    let v = "aaa".to_string();
    map.insert(k, v);
    println!("{}", v);
    // let a = ("str".to_string(), 1);
    // let v = vec![a.clone()];
    // println!("{} -> {}", a.0, a.1);
    // println!("Hello, world!");
}
