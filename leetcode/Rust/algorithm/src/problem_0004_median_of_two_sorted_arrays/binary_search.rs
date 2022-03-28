// 给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的 中位数 。

// 算法的时间复杂度应该为 O(log (m+n)) 

// 示例 1：

// 输入：nums1 = [1,3], nums2 = [2]
// 输出：2.00000
// 解释：合并数组 = [1,2,3] ，中位数 2
// 示例 2：

// 输入：nums1 = [1,2], nums2 = [3,4]
// 输出：2.50000
// 解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5



pub struct Solution;

impl Solution {
    pub fn find_median_sorted_arrays(nums1: Vec<i32>, nums2: Vec<i32>) -> f64 {
        let (m,n) = (nums1.len(), nums2.len());

        if m > n {
            return Solution::find_median_sorted_arrays(nums2, nums1);
        }

        let (mut left, mut right) = (0, m);

        let mid_sum = (m + n + 1) / 2;

        while left < right {
            // 前一部分包含 nums1[0 .. i-1] 和 nums2[0 .. j-1]
            // 后一部分包含 nums1[i .. m-1] 和 nums2[j .. n-1]
            let i = (left + right + 1) / 2;
            let j = mid_sum - i;

            if nums1[i - 1] > nums2[j] {
                right = i - 1;
            } else {
                left = i;
            }
        }

        let (i, j) = (left, mid_sum - left);

        // nums_im1, nums_i, nums_jm1, nums_j 分别表示 nums1[i-1], nums1[i], nums2[j-1], nums2[j]
        let nums_im1 = match i {
            0 => i32::MIN,
            _ => nums1[i - 1]
        };
        let nums_i = match i {
            i if i == m => i32::MAX,
            _ => nums1[i]
        };
        let nums_jm1 = match j {
            0 => i32::MIN,
            _ => nums2[j - 1]
        };
        let nums_j = match j {
            j if j == n => i32::MAX,
            _ => nums2[j]
        };

        // median1：前一部分的最大值
        let median1 = nums_im1.max(nums_jm1);
        // median2：后一部分的最小值
        let median2 = nums_i.min(nums_j);

        if (m + n) % 2 == 0 {
            (median1 + median2) as f64 / 2.0
        } else {
            median1 as f64
        }
    }
}

impl super::Solution for Solution {
    fn find_median_sorted_arrays(nums1: Vec<i32>, nums2: Vec<i32>) -> f64 {
        Self::find_median_sorted_arrays(nums1, nums2)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0004() {
        super::super::tests::run::<super::Solution>();
    }
}