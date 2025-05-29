pub fn split(arr: &mut [i32], index: usize) -> (&mut [i32], &mut [i32]) {
    let length = arr.len();
    assert!(index < length, "Index out of bounds");
    let arr_ptr = arr.as_mut_ptr();
    unsafe {
        (
            std::slice::from_raw_parts_mut(arr_ptr, index),
            std::slice::from_raw_parts_mut(arr_ptr.add(index), length - index),
        )
    }
}

#[link(name = "m")]
unsafe extern "C" {
    pub safe fn abs(input: i32) -> i32;

    pub unsafe fn lgammaf(input: f32) -> f32;

    pub unsafe fn lgamma(input: f64) -> f64;
}

pub fn gammaf(input: f32) -> f32 {
    if input <= 0.0 {
        panic!("Input must be positive");
    }
    unsafe { lgammaf(input) }
}

pub fn gamma(input: f64) -> f64 {
    if input <= 0.0 {
        panic!("Input must be positive");
    }
    unsafe { lgamma(input) }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_abs() {
        let x = -123;
        let y = abs(x);
        assert_eq!(y, 123);
    }

    #[test]
    fn test_gammaf() {
        let x = 1.0;
        let y = gammaf(x);
        assert_eq!(y, 1.0);
    }

    #[test]
    fn test_gamma() {
        let x = 1.0;
        let y = gamma(x);
        assert_eq!(y, 1.0);
    }
}