// 11. 盛最多水的容器
// 给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。

// 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。

// 返回容器可以储存的最大水量。

// 说明：你不能倾斜容器

// 示例 1:
// 输入：[1,8,6,2,5,4,8,3,7]
// 输出:49 
// 解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。


pub struct Solution;

impl Solution {
    pub fn max_area(height: Vec<i32>) -> i32 {
        let mut result = 0;
        let (mut head, rest) = height.split_first().unwrap();
        let (mut tail, mut body) = rest.split_last().unwrap();
        let mut length = rest.len() as i32;

        loop {
            if head < tail {
                result = result.max(length * head);

                if let Some((new_head, new_body)) = body.split_first() {
                    head = new_head;
                    body = new_body;
                } else {
                    break;
                }
            } else {
                result = result.max(length * tail);

                if let Some((new_tail, new_body)) = body.split_last() {
                    tail = new_tail;
                    body = new_body;
                } else {
                    break;
                }
            }

            length -= 1;
        }

        result
    }
}

impl super::Solution for Solution {
    fn max_area(height: Vec<i32>) -> i32 {
        Self::max_area(height)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0011() {
        super::super::tests::run::<super::Solution>();
    }
}