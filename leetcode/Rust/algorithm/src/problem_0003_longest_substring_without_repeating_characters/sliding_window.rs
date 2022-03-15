pub struct Solution;

use std::mem;

impl Solution {
    pub fn length_longest_substring(s: String) -> i32 {
        3
    }
}

impl super::Solution for Solution {
    fn length_longest_substring(s: String) -> i32 {
        Self::length_longest_substring(s)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0003() {
        //
    }
}