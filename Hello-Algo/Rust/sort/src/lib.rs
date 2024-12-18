pub mod sort;



#[cfg(test)]
mod test {
    use sort::select_sort;

    use crate::*;
    #[test]
    fn test_select_sort() {
        let mut a = [9, 7, 5, 3, 2, 10, 8]; 
        select_sort(&mut a);
        assert_eq!(a, [2, 3, 5, 7, 8, 9, 10]);
    }
}