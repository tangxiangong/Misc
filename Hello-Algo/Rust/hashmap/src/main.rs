use std::collections::HashMap;

// fn inner(s: &str) -> usize {
//     s.len()
// }

// fn outer(s: &str) -> usize {
//     inner(s) + 1
// }

fn main() {
    // let s = "Rust".to_owned();
    // let l = outer(&s);
    // println!("{s}, {}", l);
    // let mut test = vec![None; 10];
    // test.push(Some(RGB(1, 1, 1)));
    let mut map = HashMap::with_capacity(10);
    let k = "aaaa";
    let v = "aaa".to_string();
    map.insert(k, v);
    // println!("{}", v);
    // let a = ("str".to_string(), 1);
    // let v = vec![a.clone()];
    // println!("{} -> {}", a.0, a.1);
    // println!("Hello, world!");
}
