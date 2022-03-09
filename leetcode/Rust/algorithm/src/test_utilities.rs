use crate::data_structure::{ListNode};
// use std::rc::Rc;
// use std::cell::RefCell;
// use std::cmp::Ordering;
// use std::collections::VecDeque;
// use std::fmt::Debug;
use std::iter;

pub fn iter_list(list: &Option<Box<ListNode>>) -> impl Iterator<Item = &i32> {
    iter::successors(list.as_deref(), |node| node.next.as_deref()).map(|node| &node.val)
}

pub fn make_list<I: IntoIterator<Item = i32>>(values: I) -> Option<Box<ListNode>> {
    let mut result = None;
    let mut node_ref = &mut result;

    for value in values {
        *node_ref = Some(Box::new(ListNode::new(value)));
        node_ref = &mut node_ref.as_mut().unwrap().next;
    }

    result
}