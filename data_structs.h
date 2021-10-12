//
// Created by Raven O'Rourke on 10/9/21.
//

#ifndef OSLAB2_DATA_STRUCTS_H
#define OSLAB2_DATA_STRUCTS_H

class PCB {
public:
    PCB(){};
    PCB(int pid_, int arr_, int burst_, int prior_) : pid(pid_), arrival(arr_), burst(burst_), priority(prior_){}
    int pid;
    int arrival;
    int burst;
    int priority;
};

class ProcessArray { // for holding processes from file before added to ready queue
public:
    PCB *arr;
    int count = 0;
};

class Node {
public:
    explicit Node(PCB p) : process(p) {}
    PCB process;
    Node* prev = nullptr;
    Node* next = nullptr;
};

class Queue {
public:
    Node* head = nullptr;
    int length = 0;
    void enqueue(PCB p);
    void dequeue();
    void dequeue(int index);
};

#endif //OSLAB2_DATA_STRUCTS_H
