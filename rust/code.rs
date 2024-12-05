use rand::{thread_rng, Rng};
use std::time::Instant;

fn main() {
    let start = Instant::now();

    {   
        let n = thread_rng().gen_range(1..10);
        let r = thread_rng().gen_range(1..10000);

        let mut a = [0; 10000];
        for i in 0..10000 {
            for j in 0..100000 {
                a[i] += j % n
            }
            a[i] += r;
        }
    }    
    let duration = start.elapsed();
    println!("Rust 用时: {:?}", duration);
}