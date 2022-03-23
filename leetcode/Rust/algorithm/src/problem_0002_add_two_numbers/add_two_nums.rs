// 给你两个 非空 的链表，表示两个非负的整数。它们每位数字都是按照 逆序 的方式存储的，并且每个节点只能存储 一位 数字。

// 请你将两个数相加，并以相同形式返回一个表示和的链表。

// 你可以假设除了数字 0 之外，这两个数都不会以 0 开头。

// 示例 2：

// 输入：l1 = [0], l2 = [0]
// 输出：[0]
// 示例 3：

// 输入：l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
// 输出：[8,9,9,9,0,0,0,1]



use crate::data_structure::ListNode;

pub struct Solution;

impl Solution {
    pub fn add_two_nums(l1: Option<Box<ListNode>>, l2: Option<Box<ListNode>>) -> Option<Box<ListNode>> {
        let mut head = ListNode::new(0);
        let mut cur = &mut head.next;
        let (mut x, mut y) = (l1, l2);
        let mut upper = 0;
        let node_val = |node: &Option<Box<ListNode>>| node.as_ref().map_or(0, |x| x.val);
        let node_next = |node: Option<Box<ListNode>>| node.map_or(None, |node| node.next);
        while x.is_some() || y.is_some() || upper == 1 {
            let sum = node_val(&x) + node_val(&y) + upper;
            upper = 0;
            if sum >= 10 {
                upper = 1;
            }
            let node = ListNode::new(sum % 10);
            *cur = Some(Box::new(node));
            cur = &mut cur.as_mut().unwrap().next;
            x = node_next(x);
            y = node_next(y);
        }
        return head.next;
    }
}

//
impl super::Solution for Solution {
    fn add_two_nums(l1: Option<Box<ListNode>>, l2: Option<Box<ListNode>>) -> Option<Box<ListNode>> {
        Self::add_two_nums(l1, l2)
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_problem_0002() {
        super::super::tests::run::<super::Solution>();
    }
}