pub fn basic() {
    //# 变量 & 可变性//

//     let 声明变量，默认是不可变的
    let some_number = 1;  // 整数默认为 i32
    let some_number = some_number.to_string();  // 变量遮蔽，新变量，只不过名字相同
//     mut 声明可变变量
    let mut another_number = 2.0;  // 浮点数默认为 f64
//     const 声明常量，必须标注类型，不可以使用mut，仅可以使用常量表达式赋值
    const CONSTANT: i32 = 8 * 2;

    //# 数据类型//
    // 1. 标量类型：整数、浮点数、布尔、字符
    // 2. 复合类型
//     整数：i8, u8, i16, u16, i32(default), u32, i64, u64, i128, u128, isize, usize
//     i 有符号，u 无符号
//     isize, usize：与系统架构相关，32(64)位系统为 i32/u32(i64/u64)，
//     十进制: 123_456, 十六/八/二进制: 0x/0o/0b， 字节: b'A'
//     浮点数：f32，f64(default)
//     布尔：true, false
//     原始复合类型：元组，数组（编译时必须知道大小）

    // 元组：固定长度，可含不同类型的元素
    let my_tuple = (1, "hello", CONSTANT);
    // 用 .0, .1, ... 索引
    let t1 = my_tuple.0;
    let t2 = my_tuple.1;
    // 模式匹配
    let (x, y, z) = my_tuple;

    // 数组：固定长度，元素类型相同
    let my_array = [1, 2, 3, 4, 5];
    let a = [3; 5];  // == [3, 3, 3, 3, 3]
    // 使用[]索引
    let first = a[0];
}

    // # 函数 //
    // 函数&变量命名规范：小写，单词之间用 _ 连接
fn my_function() -> i32 {
        let x = 5;
        let y =  {
            let x = 3.0;
            x + 1.0
        };
        println!("The value of y is: {}", y);
        println!("The value of x is: {}", x);
        x  // == return x; 作为返回值时不能加分号 ";"
}

    // # 控制流 //
    // 1. if -- else if -- else
pub fn test_if() {
        let cond = true;
        let x = if cond { 1 } else { 2 };
        let mut y = 0;
        if cond { y = 1; } else { y = 2; }

        println!("The value of x is: {}", x);
        println!("The value of y is: {}", y);
}

    // 2. loop 死循环：break continue
    // 返回值: break + 表达式
pub fn test_loop() {
        let mut counter = 0;
        let result = loop {
            counter += 1;
            if counter == 10 {
                break counter * 2;
            }
        };
        println!("The result is {}", result);
    }

    // loop 标签
pub fn loop_label() {
        let mut counter = 0;
        'outer_loop: loop {
            println!("The value of counter is: {}", counter);
            let mut remaining = 10;
            loop {
                println!("The value of remaining is: {}", remaining);
                if remaining == 9 {
                    break;
                }
                if counter == 2 {
                    break 'outer_loop;  //终止 外部循环
                }
                remaining -= 1;
            }
            counter += 1;
        }
        println!("The end count is {}", counter);
    }
    

    // # for 循环遍历

pub fn test_for() {
    let a = [1, 2, 3, 4, 5];
    for element in a {
        println!("The value of element is: {element}");
    }
    for number in (1..4).rev() { // Range 1..4 == 1..=3
        println!("{number}");
    }
}
