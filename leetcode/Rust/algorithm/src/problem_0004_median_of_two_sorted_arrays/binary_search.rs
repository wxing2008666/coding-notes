pub struct Solution;

impl Solution {
    pub fn find_median_sorted_arrays(nums1: Vec<i32>, nums: Vec<i32>) -> f64 {
        1.0
    }
}

impl super::Solution for Solution {
    fn find_median_sorted_arrays(nums1: Vec<i32>, nums2: Vec<i32>) -> f64 {
        Self::find_median_sorted_arrays(nums1, nums2)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0004() {
        //
    }
}