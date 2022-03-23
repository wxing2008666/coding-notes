// 给你一个字符串 s，找到 s 中最长的回文子串。

// 示例 1：

// 输入：s = "babad"
// 输出："bab"
// 解释："aba" 同样是符合题意的答案。
// 示例 2：

// 输入：s = "cbbd"
// 输出："bb"



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