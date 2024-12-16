use hashmap::HashMap;

fn main() {
    let mut map = HashMap::new(5);
    for k in 1..10 {
        map.add(format!("no. {k}"), k);
    }
    let res = map.find(&"no. 1".to_string()).unwrap();
    // *res = 10;
    println!("{}", map);
}
