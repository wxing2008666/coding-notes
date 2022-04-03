// 15. 三数之和
// 给你一个包含 n 个整数的数组 nums, 判断 nums 中是否存在三个元素 a,b,c ，使得 a + b + c = 0 ？请你找出所有和为 0 且不重复的三元组。

// 注意：答案中不可以包含重复的三元组。

// 示例 1:

// 输入：nums = [-1,0,1,2,-1,-4]
// 输出：[[-1,-1,2],[-1,0,1]]
// 示例 2：

// 输入：nums = []
// 输出：[]
// 示例 3：

// 输入：nums = [0]
// 输出：[]


pub struct Solution;

use std::cmp::Ordering;

impl Solution {
    pub fn three_sum(nums: Vec<i32>) -> Vec<Vec<i32>> {
        let mut result = Vec::new();
        // Must have at least 3 elements.
        if nums.len() > 2 {
            let nums = {
                let mut temp = nums;
                temp.sort_unstable();
                temp
            };

            for i in 0..nums.len() - 2 {
                let first_value = nums[i];

                if i == 0 || first_value != nums[i - 1] {
                    let target = -first_value;

                    // Do two sum of target `target` from `i + 1` to `nums.len()`
                    let mut j = i + 1;
                    let mut k = nums.len() - 1;

                    while j < k {
                        let second_value = nums[j];
                        let third_value = nums[k];

                        match (second_value + third_value).cmp(&target) {
                            Ordering::Less => j += 1,
                            Ordering::Equal => {
                                result.push(vec![first_value, second_value, third_value]);

                                // Skip duplicates.
                                loop {
                                    j += 1;

                                    if j >= k || nums[j] != nums[j - 1] {
                                        break;
                                    }
                                }

                                // Skip duplicates.
                                loop {
                                    k -= 1;

                                    if j >= k || nums[k] != nums[k + 1] {
                                        break;
                                    }
                                }
                            }
                            Ordering::Greater => k -= 1,
                        }
                    }
                }
            }
        }

        result
    }
}

impl super::Solution for Solution {
    fn three_sum(nums: Vec<i32>) -> Vec<Vec<i32>> {
        Self::three_sum(nums)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0015() {
        super::super::tests::run::<super::Solution>();
    }
}