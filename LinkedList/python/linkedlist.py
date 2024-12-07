from typing import Optional, Any

class Node(object):
    def __init__(self, data: Any):
        self._data = data
        self._next: Optional[Node] = None
    
    @property
    def data(self):
        return self._data
    
    @property
    def next(self):
        return self._next


def to_node(data) -> Optional[Node]:
    if not isinstance(data, Optional[Node]):
        data = Node(data)
    return data


class LinkedList(object):
    def __init__(self, head = None):
        head = to_node(head)
        self._head: Optional[Node] = head
        self._tail: Optional[Node] = head
        self._size: int = 0 if head is None else 1
    
    @property
    def size(self) -> int:
        return self._size
    
    def isempty(self) -> bool:
        return self._size == 0
    
    def add(self, data):
        node = to_node(data)
        if self.isempty():
            self._head = node
            self._tail = node
        else:
            self._tail._next = node
            self._tail = node
        self._size += 1


if __name__ == "__main__":
    linkedlist = LinkedList()
    linkedlist.add(1)
    linkedlist.add("2")
    