pub mod sum_of_two_numbers;
pub use sum_of_two_numbers::*;
pub mod anagram_group;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_two_sum() {
        let nums = vec![2, 7, 11, 15];
        let target = 9;
        let result = two_sum(nums, target);
        assert_eq!(result, vec![0, 1])
    }
}
