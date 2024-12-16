use hashmap::HashMap;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut map = HashMap::new(5);
    for k in 1..10 {
        map.add(format!("no. {k}"), k);
    }

    println!("{}", map);

    for (key, value) in map.iter() {
        println!("{} => {}", key, value);
    }

    map.remove(&"111".to_string())?;
    // map[&"no. 1".to_string()] = 100;
    // *res = 10;
    // println!("{}", map);
    // map.remove(&"no. 6".to_string())?;
    // println!("{}", map);
    Ok(())
}
