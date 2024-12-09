mod guessing_game;
mod basic_structure;
mod ownership;

use ownership::ownership;
// use basic_structure::test_if;
// use guessing_game::guess;

fn swap(a: &mut i32, b: &mut i32) {
    let temp = *a;
    *a = *b;
    *b = temp;
}

fn main() {
    println!("Hello, world!");
    // let mut a = 1;
    // let mut b = 2;
    // swap(&mut a, &mut b);
    // println!("a = {a}, b = {b}");
    // let ref_a = &mut a;
    // let ref_b = &mut b;
    // swap(ref_a, ref_b);
    // println!("a = {}, b = {b}", a);
    // println!("{}", ref_a);
    // a += 1;
    
    ownership();
    // println!("ref_a = {ref_a}");
}
