// 将一个给定字符串 s 根据给定的行数 numRows ，以从上往下、从左到右进行 Z 字形排列。

// 比如输入字符串为 "PAYPALISHIRING" 行数为 3 时，排列如下：

// P   A   H   N
// A P L S I I G
// Y   I   R
// 之后，你的输出需要从左往右逐行读取，产生出一个新的字符串，比如："PAHNAPLSIIGYIR"。

// 示例 1：

// 输入：s = "PAYPALISHIRING", numRows = 3
// 输出："PAHNAPLSIIGYIR"
// 示例 2：
// 输入：s = "PAYPALISHIRING", numRows = 4
// 输出："PINALSIGYAHRPI"
// 解释：
// P     I    N
// A   L S  I G
// Y A   H R
// P     I


pub struct Solution;

impl Solution {
    pub fn convert(s: String, num_rows: i32) -> String {
        if num_rows == 1 {
            return s;
        }

        let num_rows = num_rows as usize;
        let cycle_length = num_rows * 2 - 2;
        let chunks = s.as_bytes().chunks(cycle_length);
        let mut result = vec![0; s.len()];
        let mut iter = result.iter_mut();

        for &c in chunks.clone().filter_map(<[_]>::first) {
            *iter.next().unwrap() = c;
        }

        for row in 1..num_rows - 1 {
            for chunk in chunks.clone() {
                if let Some(&c) = chunk.get(row) {
                    *iter.next().unwrap() = c;

                    if let Some(&c) = chunk.get(cycle_length - row) {
                        *iter.next().unwrap() = c;
                    }
                }
            }
        }

        for &c in chunks.filter_map(|chunk| chunk.get(num_rows - 1)) {
            *iter.next().unwrap() = c;
        }

        String::from_utf8(result).unwrap()
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