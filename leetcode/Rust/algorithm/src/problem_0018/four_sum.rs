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