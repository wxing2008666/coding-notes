// 给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。

// 示例 1:

// 输入: s = "abcabcbb"
// 输出: 3 
// 解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
// 示例 2:

// 输入: s = "bbbbb"
// 输出: 1
// 解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。



pub struct Solution;

use std::mem;

impl Solution {
    pub fn length_longest_substring(s: String) -> i32 {
        const INVALID_INDEX: isize = -1;
        let s = s.into_bytes();
        let mut result = 0;
        let mut start = 0;
        let mut byte_locations = [INVALID_INDEX; 256];

        for (i, byte) in s.into_iter().enumerate() {
            let i = i as isize;
            let old_index = mem::replace(&mut byte_locations[byte as usize], i);

            if old_index < start {
                result = result.max(i - start + 1);
            } else {
                start = old_index + 1;
            }
        }

        result as _
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
        super::super::tests::run::<super::Solution>();
    }
}