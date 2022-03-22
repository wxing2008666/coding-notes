pub struct Solution;

impl Solution {
    pub fn convert(s: String, num_rows: i32) -> String {
        "test".to_string()
    }
}

impl super::Solution for Solution {
    fn convert(s: String, num_rows: i32) -> String {
        Self::convert(s, num_rows)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0006() {
        super::super::tests::run::<super::Solution>();
    }
}