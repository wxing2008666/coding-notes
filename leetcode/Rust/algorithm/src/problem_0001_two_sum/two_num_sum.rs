// 给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target  的那 两个 整数，并返回它们的数组下标。

// 你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。

// 你可以按任意顺序返回答案

// 示例 1：

// 输入：nums = [2,7,11,15], target = 9
// 输出：[0,1]
// 解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 


pub struct Solution;

use std::collections::HashMap;

impl Solution {
    pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
        let (last, nums) = nums.split_last().unwrap();
        let mut indices = HashMap::new();

        for (i, &num) in (0..).zip(nums) {
            match indices.get(&(target - num)) {
                None => indices.insert(num, i),
                Some(&index) => return vec![index, i],
            };
        }

        vec![indices[&(target - last)], nums.len() as _]
    }
}

// 
impl super::Solution for Solution {
    fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
        Self::two_sum(nums, target)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0001() {
        super::super::tests::run::<super::Solution>();
    }
}