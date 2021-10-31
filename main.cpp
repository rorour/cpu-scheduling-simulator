//CSCI 3453 Lab 2 - Raven O'Rourke
//CPU Scheduling Simulator

#include <iostream>
#include <fstream>
#include <string>
#include "data_structs.h"
using namespace std;

const double CONTEXT_SWITCHING_COST = 0.5; //all times are in milliseconds

//todo
// implement FCFS
// implement SJF
// implement RR
// time unit MILLISECONDS
// RR: FCFS breaks tie
// check time quantum
// assume context switching cost, one CPU, only CPU activities
// context switch ONLY when current process moved to ready queue, not on termination
// if same time, NEW process, queued before OLD REQUEUED
// select process from ready queue
// execute process by given scheduling algo
// display activities and eval performance based on avg turnaround, waiting time, response time
// 2.3 specific info to collect
// make sure runs on csegrid

void fcfs(const string &filename);
void sjf(const string &filename);
void rr(const string &filename);
void print_history(const string algo_type, const ProcessArray *all_processes);

int main(int argc, char *argv[]) {
    //todo rm this section
    cout << "argc is " << argc << endl;
    for (int i = 0; i < argc; i++) { // 0=unneeded, 1=input file, 2=algo, 3(optional)=quantum size
        cout << "argv[" << i << "] is " << argv[i] << endl;
    }

    if (argc < 3 || (*argv[2] == '2' && argc < 4)) {
        cout << "Not enough arguments detected\n";
        cout << "Enter arguments as: inputfilename algorithm quantumsize(RR only)\n";
        cout << "0=FCFS, 1=SJF, 2=RR\n";
        return 0;
    }

    switch(*argv[2]) {
        case '0': // FCFS
            cout << "FCFS selected\n";
            fcfs(argv[1]);
            break;
        case '1': // SJF
            cout << "SJF selected\n";
            break;
        case '2': // RR
            cout << "RR selected\n";
            break;
        default:
            cout << "Invalid algorithm specified. 0=FCFS, 1=SJF, 2=RR\n";
            break;
    }

    return 0;
}

void process_file(const string &filename, ProcessArray *all_processes) {

    cout << "Opening file named " << filename << endl;

    //read in from file
    string lines[100];
    ifstream infile(filename);
    int num_lines = 0;
    if (infile.is_open()) {
        string line;
        while ( getline (infile,line)) {
            cout << line << '\n';
            lines[num_lines] = line;
            num_lines++;
        }
        infile.close();
    } else cout << "Unable to open file";

    all_processes->arr = new PCB[num_lines];
    all_processes->count = num_lines;
    cout << "Read in " << num_lines << " lines\n";


    //process lines from file
    for (int i=0; i < num_lines; i++) {
        string line = lines[i];
        //convert line to char array
        char line_arr[line.length()];
        for (int m = 0; m < line.length(); m++) {
            line_arr[m] = line[m];
        }

        //separate data pieces in line
        string tokens[4];
        char *token = strtok(line_arr, " ");
        int a = 0;
        while (token != NULL) {
            tokens[a] = token;
            a++;
            token = strtok(NULL, " ");
        }

        //add data to PCB
        PCB *process = new PCB(stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]));
        all_processes->arr[i] = *process;
    }
}

void fcfs(const string &filename) {
    Queue ready_queue;
    int processes_queued = 0;
    int completed_processes = 0;
    double current_time = 0;
    PCB* currently_running_process = nullptr;
    ProcessArray *all_processes = new ProcessArray();
    
    process_file(filename, all_processes);

    while (completed_processes < all_processes->count) {
        //get arriving processes at this time from all_processes & add to ready queue
        while (processes_queued < all_processes->count && all_processes->arr[processes_queued].arrival <= current_time) {
            ready_queue.enqueue(all_processes->arr[processes_queued]);
            processes_queued++;
        }
        //check if currently running process - if not,get next process from ready queue
        if (currently_running_process == nullptr && ready_queue.head != nullptr) { //start new process
            currently_running_process = &ready_queue.head->process;
            ready_queue.dequeue();
            Burst* new_burst = new Burst(current_time); //begin new burst
            currently_running_process->bursts[currently_running_process->num_bursts] = *new_burst;
        }
        //else we are continuing with currently running process

        //run process - decrement time still needed
        currently_running_process->time_left_to_run--;

        //increment current time
        current_time += 1;

        //if time still needed is 0, update burst, increment completed processes & set currently running to null
        if (currently_running_process->time_left_to_run == 0) {
            currently_running_process->bursts[currently_running_process->num_bursts].end_time = current_time;

            cout << "Process " << currently_running_process->pid << " ran from "
                << currently_running_process->bursts[currently_running_process->num_bursts].start_time << " to "
                << currently_running_process->bursts[currently_running_process->num_bursts].end_time << endl;

            completed_processes ++;
            cout << "Completed process " << completed_processes << endl;

            currently_running_process->completion_time = currently_running_process->bursts[currently_running_process->num_bursts].end_time;
            //waiting time for fcfs is start time minus arrival time
            currently_running_process->waiting_time = currently_running_process->bursts[0].start_time - currently_running_process->arrival;
            //turnaround time for fcfs is end time minus arrival time
            currently_running_process->turnaround_time = currently_running_process->bursts[0].end_time - currently_running_process->arrival;
            //response time for fcfs is waiting time
            currently_running_process->response_time = currently_running_process->waiting_time;

            //update all_processes with completed process info
            for (int a = 0; a < all_processes->count; a++) {
                if (all_processes->arr[a].pid == currently_running_process->pid) {
                    all_processes->arr[a] = *currently_running_process;
                }
            }

            currently_running_process = nullptr;
        }
    }

    cout << "Done running all processes." << endl;
    print_history("FCFS", all_processes);
}

void print_history(const string algo_type, const ProcessArray *all_processes) {
    //calculated measurements
    float avg_cpu_burst = 0;
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    float avg_response_time = 0;
    int total_context_switches = 0;

    cout << "\n--------------------------------------------------Scheduling Algorithm: "
        << algo_type << "---------------------------------------------------\n";

    printf ("%5s ", "pid | ");
    printf ("%5s ", "arrival | ");
    printf ("%5s ", "CPU burst | ");
    printf ("%5s ", "priority | ");
    printf ("%5s ", "completion time | ");
    printf ("%5s ", "wait time | ");
    printf ("%5s ", "turnaround time | ");
    printf ("%5s ", "response time | ");
    printf ("%5s ", "context switches ");
    cout << endl;

    for (int i = 0; i < all_processes->count; i++) {
        PCB proc = all_processes->arr[i];
        printf ("%3d |", proc.pid);
        printf ("%9d |", proc.arrival);
        printf ("%11d |", proc.total_time_needed);
        printf ("%10d |", proc.priority);
        printf ("%17.1f |", proc.completion_time);
        printf ("%11.1f |", proc.waiting_time);
        printf ("%17.1f |", proc.turnaround_time);
        printf ("%15.1f |", proc.response_time);
        printf ("%17d ", proc.num_context_occurred);
        cout << endl;

        avg_cpu_burst += proc.total_time_needed;
        avg_waiting_time += proc.waiting_time;
        avg_turnaround_time += proc.turnaround_time;
        avg_response_time += proc.response_time;
        total_context_switches += proc.num_context_occurred;
    }

    avg_cpu_burst /= all_processes->count;
    avg_waiting_time /= all_processes->count;
    avg_turnaround_time /= all_processes->count;
    avg_response_time /= all_processes->count;

    cout << "average CPU burst time: " << avg_cpu_burst << "ms\n";
    cout << "average wait time: " << avg_waiting_time << "ms\n";
    cout << "average turnaround time: " << avg_turnaround_time << "ms\n";
    cout << "average response time: " << avg_response_time << "ms\n";
    cout << "total number of context switches: " << total_context_switches << "\n";
    cout << endl;
}



void sjf();
void rr();