use std::hint::black_box;
use criterion::{criterion_group, criterion_main, Criterion};
use fdesolver_rs::*;

fn criterion_benchmark(c: &mut Criterion) {
    let eq = FDE {
        init: 1.0,
        order: 0.7,
        inteval: 10.0,
        spectrum: -1.0,
        source: |t| (-t / 10.0).exp(),
    };
    let pro = Discretization { eq, step: 0.001 };
    
    c.bench_function("FDESolver", |b| b.iter(|| solve(black_box(&pro))));
}

println!("Rust 基准测试");
 
criterion_group!(benches, criterion_benchmark);
criterion_main!(benches);
println!("-----------------------------------");