use std::arch::x86_64::_mm256_max_epi8;

// 所有权 //
    // 数据/内存有且仅有一个所有者
// 不可变引用（共享引用）：只保留可读权
// 可变引用（独占引用）：数据被引用暂时独占
// 引用的流动权限
pub fn ownership() {
    let value: i32;
    if true {
        value = 0;
    } else {
        value = 1;
    }
    println!("value: {}", value);
    // Box 指针，分配堆内存，返回指向该内存的指针
    // 如果一个变量拥有一个box，当释放该变量的栈内存时，会自动释放对应box的堆内存
    // let mut a = Box::new([1;100]);  // a 拥有这个 box
    // let b = a;  // a 的所有权转移给了 b
    // 为了避免发生所有权转移，可以使用 .clone()
    // assert_eq!(a[0], 1); // ERROR
    // let my_box = create_box();
    // let box_ref = &my_box;
    // change_box(&mut a);
    // println!("a[1] = {}", a[1]);
    // let x = my_box[1];
    // let mut x = 1;
    // let x_ref1 = &x;
    // let x_ref2 = &x; // 只读
    //     x += 1;
    // println!("{x}");
    // println!("x_ref1 = {}", x_ref1);
    // x += 1;
    // *x_ref1 = 2;
    let x = create_box();
    println!("地址: {:p}", x);
}

fn create_box() -> Box<[i32]> {
    let a = Box::new([1;100]);
    println!("地址 {:p}", a);
    a
}

fn change_box(my_box: &mut Box<[i32; 100]>) {
    my_box[1] = 20;
}