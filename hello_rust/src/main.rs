mod guessing_game;
mod basic_structure;
mod ownership;
mod enums;
mod lifetime;
mod traits;
// use ownership::ownership;
// use basic_structure::test_if;
// use guessing_game::guess;

fn swap(a: &mut i32, b: &mut i32) {
    let temp = *a;
    *a = *b;
    *b = temp;
}

#[derive(Debug)]
struct RGB(usize, usize, usize);

impl RGB {
    fn print(&self) {
        println!("{self:?}");
    }

    fn moved(self) {

    }
}

static A: [i32; 10] = [0;10];
fn main() {
    println!("Hello, world!");
    let red = RGB(1, 0, 0);
    red.print();
    let green = RGB(0, 1, 0);
    green.moved();
    let a = Box::new(A);
    println!("A 的内容 {:?}", A);
    println!("A 的地址 {:p}", &A);
    println!("a 的内容 {:?}", a);
    println!("a 指向的地址 {:p}", a);
    println!("a 的地址 {:p}", &a);

    // green.0;
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
    enums::test_enums();    
    // ownership();
    // println!("ref_a = {ref_a}");
}
