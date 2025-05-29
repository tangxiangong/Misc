use std::arch::asm;

fn main() {
    let x = 3;
    let y = 2;
    let result = add(x, y);
    println!("Result: {}", result);
}

#[inline(always)]
#[cfg(target_arch = "aarch64")]
fn add(x: i32, y: i32) -> i32 {
    let mut result: i32;
    unsafe {
        asm!(
            "add {0:w}, {1:w}, {2:w}",
            out(reg) result,
            in(reg) x,
            in(reg) y,
        );
    }
    result
}

#[inline(always)]
#[cfg(target_arch = "x86_64")]
fn add(x: i32, y: i32) -> i32 {
    let mut result: i32;
    unsafe {
        asm!(
            "addl {1}, {0}",
            inout(reg) x => result,
            in(reg) y,
        );
    }
    result
}

#[inline(always)]
#[cfg(not(any(target_arch = "aarch64", target_arch = "x86_64")))]
fn add(x: i32, y: i32) -> i32 {
    x + y
}
