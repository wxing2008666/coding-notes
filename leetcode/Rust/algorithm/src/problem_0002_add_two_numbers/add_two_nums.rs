use crate::data_structure::ListNode;

pub struct Solution;

impl Solution {
    pub fn add_two_nums(l1: Option<Box<ListNode>>, l2: Option<Box<ListNode>>) -> Option<Box<ListNode>> {
        Some(Box::new(ListNode::new(2)))
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