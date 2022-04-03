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