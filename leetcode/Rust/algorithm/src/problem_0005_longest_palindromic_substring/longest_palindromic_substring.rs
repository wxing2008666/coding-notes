pub struct Solution;

impl Solution {
    pub fn longest_palindromic(s: String) -> String {
        "test".to_string()
    }
}

impl super::Solution for Solution {
    fn longest_palindromic(s: String) -> String {
        Self::longest_palindromic(s)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0005() {
        super::super::tests::run::<super::Solution>();
    }
}