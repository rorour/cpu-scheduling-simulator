//
// Created by Raven O'Rourke on 10/10/21.
//
#include <iostream>
#include "data_structs.h"
using namespace std;

void Queue::enqueue(PCB p) {
    Node *new_node = new Node(p);
    if (this->length == 0) {
        head = new_node;
    } else {
        Node *current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        new_node->prev = current;
        new_node->next = nullptr;
        current->next = new_node;
    }
    this->length += 1;
}

void Queue::dequeue() {
    dequeue(0);
}

void Queue::dequeue(int index) {
    if (index > length - 1) {
        cout << "Invalid index, there are " << length << " items.\n";
        return;
    }

    Node current = *head;
    for (int i=0; i < index; i++) {
        current = *current.next;
    }
    if (index != 0) {
        current.prev->next = current.next;
    } else if (index != length - 1){
        head = current.next;
    } else {
        head = nullptr;
    }

    if (index != length - 1) {
        current.next->prev = current.prev;
    } else if (index != 0) {
        current.prev->next = nullptr;
    }

    this->length -= 1;
}
