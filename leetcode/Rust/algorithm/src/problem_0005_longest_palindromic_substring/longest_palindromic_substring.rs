pub struct Solution;

impl Solution {
    pub fn longest_palindrome(s: String) -> String {
        "test"
    }
}

impl super::Solution for Solution {
    fn longest_palindrome(s: String) -> String {
        Self::longest_palindrome(s)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0005() {
        super::super::tests::run<super::Solution>();
    }
}