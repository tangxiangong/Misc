use std::{env, process};
// use std::error::Error;
// use std::fs;
use minigrep::Config;

fn main() {
    let args: Vec<String> = env::args().collect();  // 读取命令行参数

    let config = Config::new(&args)
        .unwrap_or_else(|err| {
            eprintln!("Problem parsing arguments: {err}");
            process::exit(1);
        });
    if let Err(e) = minigrep::run(config) {
        eprintln!("Application error: {e}");
        process::exit(1);
    }
}

