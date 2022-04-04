// 18. 四数之和
// 给你一个由 n 个整数组成的数组 nums ，和一个目标值 target 。请你找出并返回满足下述全部条件且不重复的四元组 [nums[a], nums[b], nums[c], nums[d]] （若两个四元组元素一一对应，则认为两个四元组重复）：

// 0 <= a, b, c, d < n
// a、b、c 和 d 互不相同
// nums[a] + nums[b] + nums[c] + nums[d] == target
// 你可以按 任意顺序 返回答案 。

// 示例 1：
// 输入：nums = [1,0,-1,0,-2,2], target = 0
// 输出：[[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]

// 示例 2：
// 输入：nums = [2,2,2,2,2], target = 8
// 输出：[[2,2,2,2]]


pub struct Solution;

impl Solution {
    pub fn four_sum(nums: Vec<i32>, target: i32) -> Vec<Vec<i32>> {
        Vec::new()
    }
}

impl super::Solution for Solution {
    fn four_sum(nums: Vec<i32>, target: i32) -> Vec<Vec<i32>> {
        Self::four_sum(nums, target)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0018() {
        super::super::tests::run::<super::Solution>();
    }
}