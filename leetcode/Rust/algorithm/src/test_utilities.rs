use crate::data_structure::{ListNode, TreeNode};
use std::cell::RefCell;
use std::cmp::Ordering;
use std::collections::VecDeque;
use std::fmt::Debug;
use std::iter;
use std::rc::Rc;

// pub fn iter_list(list: &Option<Box<ListNode>>) -> impl Iterator<Item = &i32> {
//     iter::successors(list.as_deref(), |node| node.next.as_deref()).map(|node| &node.val)
// }

pub fn make_list<I: IntoIterator<Item = i32>>(values: I) -> Option<Box<ListNode>> {
    None
}