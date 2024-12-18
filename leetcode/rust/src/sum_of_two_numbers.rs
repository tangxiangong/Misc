//! 给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target  的那 两个 整数，并返回它们的数组下标。
//! 你可以假设每种输入只会对应一个答案，并且你不能使用两次相同的元素。
//! 你可以按任意顺序返回答案。
//! 
use std::collections::HashMap;
pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
    let mut map : HashMap<i32, i32> = HashMap::with_capacity(nums.len());
    map.insert(nums[0], 0);
    let mut result = vec![0; 2];
    for (index, value) in nums.iter().enumerate().skip(1) {
        if map.contains_key(&(target - value)) {
            result[1] = index as i32;
            result[0] = map[&(target-value)];
        } else {
            map.insert(*value, index as i32);
        }
    }
    result
}