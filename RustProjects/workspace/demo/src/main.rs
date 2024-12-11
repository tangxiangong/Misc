// use std::{cell::{self, Cell, RefCell}, rc::Rc};


fn main() {
    // let a  = (1 + 1 as i16)?;
    // let p = RefCell::new(1);
    
    // let a = &p;
    // let b = &p;

    // println!("inner p = {}", p.borrow());
    // println!("inner a = {}", a.borrow());
    // println!("inner b = {}", b.borrow());
    
    // *a.borrow_mut() += 1;
    // println!("inner p = {}", p.borrow());
    // println!("inner a = {}", a.borrow());
    // println!("inner b = {}", b.borrow());
    
    // *b.borrow_mut() += 1;
    // println!("inner p = {}", p.borrow());
    // println!("inner a = {}", a.borrow());
    // println!("inner b = {}", b.borrow());
    

    // let cell_ptr = Cell::new(1);
    // let ref_one = &cell_ptr;
    // let ref_two = &cell_ptr;

    // let value = ref_two.get();
    // ref_one.set(10);
    // let now_value = ref_two.get();
    // println!("{value} == {now_value} ?");
    // // let one = refcell_ptr.borrow_mut();
    
    // println!("one = {one}");
    // println!("ptr = {}", **refcell_ptr);
    // let mut rc_ptr = Rc::new(1);
    // let rc_ptr_clone = rc_ptr.clone();
    // // rc_ptr = Rc::new(2);
    // println!("rc_ptr = {}", rc_ptr);
    // println!("rc_ptr 指向的地址 {:p}", rc_ptr);
    // println!("rc_ptr_clone = {}", rc_ptr_clone);
    // println!("rc_ptr_clone 指向的地址 {:p}", rc_ptr_clone);
    
    // let box_ptr = Box::new(1);
    // let box_ptr_clone = box_ptr.clone();
    // println!("box_ptr = {}", box_ptr);
    // println!("box_ptr 指向的地址 {:p}", box_ptr);
    // println!("box_ptr_clone = {}", box_ptr_clone);
    // println!("box_ptr_clone 指向的地址 {:p}", box_ptr_clone);
    // let mut a = Box::new(1);
    // *a = 2;
    // let b = &mut a;
    // *b = Box::new(3);
    // **b = 4;
    // println!("a = {a}");
    // let c = a;
    // println!("b = {}", **b);
    // a = Box::new(2);
    // println!("b={}", b);
    // let a = "Hello World";
    // let b = &a;
    // let c = a;
    
    // println!("a 的地址 {:p}", &a);
    // // println!("a 的首元素地址 {:p}", &a[0]);
    // println!("b 的地址 {:p}", &b);
    // println!("c 的地址 {:p}", &c);
    // // println!("c 的首元素地址 {:p}", &c[0]);
    // let (_a, _b, .., _, _d) = (1, 2, 3, 4, 5);
}