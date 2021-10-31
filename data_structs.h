//
// Created by Raven O'Rourke on 10/9/21.
//

#ifndef OSLAB2_DATA_STRUCTS_H
#define OSLAB2_DATA_STRUCTS_H

class Burst {
public:
    Burst(){};
    Burst(double start): start_time(start) {};
    double start_time;
    double end_time;
};

class PCB {
public:
    PCB(){};
    PCB(int pid_, int arr_, int total_time_needed_, int prior_) : pid(pid_), arrival(arr_),
        total_time_needed(total_time_needed_), priority(prior_){
            bursts = new Burst[total_time_needed_]; // allocate space for maximum number of possible bursts
            time_left_to_run = total_time_needed_;
    }
    //data from file below
    int pid;
    int arrival;
    int total_time_needed;
    int priority;
    //collected measurements below
    double completion_time;
    double waiting_time;
    double turnaround_time;
    double response_time;
    int num_context_occurred = 0;

    int time_left_to_run;
    int num_bursts = 0;
    Burst *bursts;
};

class ProcessArray { // for holding processes from file before added to ready queue
public:
    PCB *arr = nullptr;
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
