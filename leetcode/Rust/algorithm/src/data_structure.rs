use std::cell::RefCell;
use std::rc::Rc;

#[derive(Debug, PartialEq, Eq)]
pub struct ListNode {
    pub val: i32,
    pub next: Option<Box<Self>>,
    // pub next: Option<Box<ListNode>>,
}

impl ListNode {
    pub fn new(val: i32) -> Self {
        Self {next: None, val}
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct TreeNode {
    pub val: i32,
    pub left: Option<Rc<RefCell<Self>>>,
    pub right: Option<Rc<RefCell<Self>>>,
}

impl TreeNode {
    pub fn new(val: i32) -> Self {
        Self {
            val,
            left: None,
            right: None,
        }
    }
}

#[allow(variant_size_differences)]
#[derive(Debug, PartialEq, Eq)]
pub enum NestedInteger {
    Int(i32),
    List(Vec<Self>),
}

#[cfg(test)]
mod tests {
    use super::{ListNode};

    #[test]
    fn test_debug_list_node() {
        assert_eq!(format!("{:?}", ListNode::new(3)),
                    "ListNode { val: 3, next: None }");
    }

    #[test]
    fn test_debug_tree_node() {
        assert_eq!(format!("{:?}", TreeNode::new(2)),
                    "TreeNode { val: 2, left: None, right: None }");
    }

    #[test]
    fn test_debug_nested_interger() {
        assert_eq!(format!("{:?}", NestedInteger::Int(2)),
                    "Int(2)");

        assert_eq!(format!("{:?}", NestedInteger::List(vec![NestedInteger::Int(3), NestedInteger::Int(5)])),
                    "List([Int(3), Int(5)])");
    }
}