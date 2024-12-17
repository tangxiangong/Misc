use std::cell::RefCell;
use std::fmt::{Debug, Display};
use std::rc::Rc;

type NodePtr<T> = Rc<RefCell<Node<T>>>;

#[derive(Debug)]
pub(crate) struct Node<T> {
    pub(crate) data: T,
    pub(crate) left: Option<NodePtr<T>>,
    pub(crate) right: Option<NodePtr<T>>,
}

impl<T> Node<T>
where T: Copy + PartialOrd + PartialEq + Debug + Display
{
    pub(crate) fn new(data: T) -> NodePtr<T> {
        Rc::new(RefCell::new(Node {
            data,
            left: None,
            right: None,
        }))
    }
}

impl<T> Display for Node<T>
where T: Copy + PartialOrd + PartialEq + Debug + Display
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let left_data = if self.left.is_none() {
            "None"
        } else {
            & self.left.clone().unwrap().borrow().data.to_string()
        };
        let right_data = if self.right.is_none() {
            "None"
        } else {
            & self.left.clone().unwrap().borrow().data.to_string()
        };
        write!(f, "{}, left: {}, right: {}.", self.data, left_data, right_data)
    }
}

pub struct Tree<T> {
    pub(crate) root: Option<NodePtr<T>>,
    pub(crate) size: usize,
}

impl<T> Tree<T>
where T: Copy + PartialOrd + PartialEq + Debug + Display
{
    pub fn new(data: T) -> Self {
        Tree {
            root: None,
            size: 0,
        }
    }

    pub fn is_empty(&self) -> bool {
        self.root.is_none()
    }

    pub fn add(&mut self, data: T) {
        if self.is_empty() {
            let node = Node::new(data);
            self.root = Some(node.clone());
        }
    }
}
