// use std::rc::Rc;
// use std::cell::RefCell;
use hello_algo::linked_list::{ListNode, LinkedList};

fn main() {
    let n1 = ListNode::new(1);
    let n2 = ListNode::new(2);

    let mut list = LinkedList::new();

    list.push(&n1);
    list.push(&n2);

    println!("list size = {}", list.size);
    assert!(n1.next().unwrap() == n2.data());
    // let n0 = Rc::new(RefCell::new(_ListNode {value: 1, next: None}));
    // let n1 = Rc::new(RefCell::new(ListNode {val: 2, next: None}));
    // let n2 = Rc::new(RefCell::new(ListNode {val: 3, next: None}));
    // let n3 = Rc::new(RefCell::new(ListNode {val: 4, next: None}));
    
    // n0.borrow_mut().next = Some(n1.clone());
    // n1.borrow_mut().next = Some(n2.clone());
    // n2.borrow_mut().next = Some(n3.clone());
    println!("Hello, world!");
}
