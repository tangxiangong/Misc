use std::io;
use std::cmp::Ordering;
use rand::Rng;      // cargo add rand@x.x.x
// cargo update 更新库

pub fn guess() {
    // 宏
    println!("Guess the number!");
    // 随机数生成器 thread_rng()
    // Range: 1..101 == 1..=100
    let secret_number = rand::thread_rng().gen_range(1..101);
    // loop （死）循环
    loop {
        println!("Please input your guess.");
        // let 声明（不可变）变量，mut 表示可变变量
        let mut guess = String::new();

        io::stdin()  // 输入流 -> stdin
            // 读取内容，存入guess中，&mut表示可变引用
            .read_line(&mut guess)
            // 返回 Result 枚举类型: 成功 Ok 或者失败 Err
            .expect("Failed to read line"); // 如果失败，异常处理
        println!("You guessed: {}", guess); // {} 占位符
        // trim 去除字符串的首尾空白
        // parse 解析字符串为数字 返回 Result (Ok, Err)
        // let guess: u32 = guess.trim().parse().expect("Please type a number!");
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        // match 匹配枚举结果
        match guess.cmp(&secret_number) {
            // 枚举 Ordering
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            },
        }
    }
}