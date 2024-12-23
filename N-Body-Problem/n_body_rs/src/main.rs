use n_body_rs::nbody;
use std::time::Instant;

fn main() {
    let n = 50_000_000;
    let start = Instant::now();
    let (_, _) = nbody(n);
    let duration = start.elapsed();
    println!("Rust: {:?}", duration);
}
