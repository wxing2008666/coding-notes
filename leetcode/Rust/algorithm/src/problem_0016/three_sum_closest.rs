// 16. 最接近的三数之和
// 给你一个长度为 n 的整数数组 nums 和 一个目标值 target。请你从 nums 中选出三个整数，使它们的和与 target 最接近。

// 返回这三个数的和。

// 假定每组输入只存在恰好一个解。

// 示例 1：

// 输入：nums = [-1,2,1,-4], target = 1
// 输出：2
// 解释：与 target 最接近的和是 2 (-1 + 2 + 1 = 2) 。
// 示例 2：

// 输入：nums = [0,0,0], target = 1
// 输出：0


pub struct Solution;

impl Solution {
    pub fn three_sum_closest(nums: Vec<i32>, target: i32) -> i32 {
        1
    }
}

impl super::Solution for Solution {
    fn three_sum_closest(nums: Vec<i32>, target: i32) -> i32 {
        Self::three_sum_closest(nums, target)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0016() {
        super::super::tests::run::<super::Solution>();
    }
}