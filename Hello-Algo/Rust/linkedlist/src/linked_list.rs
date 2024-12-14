use std::rc::Rc;
use std::cell::RefCell;

pub struct LinkedList<'a, T> {
    head: Option<&'a ListNode<T>>,
    tail: Option<&'a ListNode<T>>,
    pub size: usize,
}


impl<'a, T> LinkedList<'a, T> {
    pub fn new() -> Self {
        LinkedList { head: None, tail: None, size: 0 }
    }

    pub fn push(&mut self, node: &'a ListNode<T>) {
        if self.tail.is_none() {
            self.head = Some(node)
        } else {
            self.tail.unwrap().address.borrow_mut().next = Some(node.address.clone());
        }
        self.tail = Some(node);
        self.size += 1;
    }
}
// TODO Box<dyn Traits> 替换 T 实现可以存储不同类型的链表
pub struct ListNode<T> {
    address: Rc<RefCell<_ListNode<T>>>,
}

impl<T> ListNode<T> {
    pub fn new(value: T) -> Self {
        let address = Rc::new(RefCell::new(_ListNode::new(value)));
        ListNode { address }
    }

    pub fn data(&self) -> T where T: Copy {
        self.address.borrow().value
    }

    pub fn next(&self) -> Option<T>  where T: Copy {
        let v = self.address.borrow().next.clone();
        match v {
            Some(address) => Some(address.borrow().value),
            _ => None,
        }
    }

    // #[warn(private_interfaces)]
    // pub fn next(&self) -> Option<Rc<RefCell<_ListNode>>> {
    //     self.address.borrow_mut().next.clone()
    // }
}


struct _ListNode<T> {
    value: T,  // 节点值
    next: Option<Rc<RefCell<_ListNode<T>>>>,  // 指向下一个节点的指针
}

impl<T> _ListNode<T> {
    fn new(value: T) -> Self {
        _ListNode{ value,  next: None }
    }
}

/* 在节点 target 后面插入一个节点 node */
// pub fn insert(target: &Rc<RefCell<ListNode>>, node: Rc<RefCell<ListNode>>) {
//     let successor_of_target = target.borrow_mut().next.take();
//     node.borrow_mut().next = successor_of_target;
//     target.borrow_mut().next = Some(node)
// }

/* 删除节点 target 后的首个节点 */
// pub fn remove(target: &Rc<RefCell<ListNode>>) {
//     if target.borrow().next.is_none() {
//         return;
//     }
//     let p = target.borrow_mut().next.take();

// }

