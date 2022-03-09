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