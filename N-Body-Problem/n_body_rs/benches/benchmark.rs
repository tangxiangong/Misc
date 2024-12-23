use std::hint::black_box;
use criterion::{criterion_group, criterion_main, Criterion};
use n_body_rs::nbody;

fn criterion_benchmark(c: &mut Criterion) {
    c.bench_function("N Body Problem", |b| b.iter(|| nbody(black_box(50_000_000))));
}

criterion_group!(benches, criterion_benchmark);
criterion_main!(benches);