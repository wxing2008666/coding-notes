pub mod longest_palindromic_substring;

pub trait Solution {
    fn longest_palindromic(s: String) -> String;
}

#[cfg(test)]
mod tests {
    use super::Solution;

    pub fn run<S: Solution>() {
        let test_cases = [("babad", &["bab", "aba"] as &[_]), ("cbbd", &["bb"])];

        for (s, expected) in test_cases {
            assert!(expected.contains(&S::longest_palindromic(s.to_string()).as_str()));
        }
    }
}