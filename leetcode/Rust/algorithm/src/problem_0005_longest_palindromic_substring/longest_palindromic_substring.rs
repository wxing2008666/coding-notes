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
        let s = s.into_bytes();
        let mut max_left = 0;
        let mut max_length = 0;

        let mut update_result = |mut i: usize, mut j: usize| {
            while let (Some(lhs), Some(rhs)) = (s.get(i), s.get(j)) {
                if lhs == rhs {
                    i = i.wrapping_sub(1);
                    j += 1;
                } else {
                    break;
                }
            }

            let length = j.wrapping_sub(i) - 1;

            if length > max_length {
                max_left = i.wrapping_add(1);
                max_length = length;
            }
        };

        for i in 0..s.len() {
            update_result(i.wrapping_sub(1), i + 1);
            update_result(i, i + 1);
        }

        String::from_utf8(s[max_left..max_left + max_length].to_vec()).unwrap()
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