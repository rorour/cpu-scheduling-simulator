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
    int time = 0;
    //calculated measurements
    int avg_cpu_burst;
    int avg_waiting_time;
    int avg_turnaround_time;
    int avg_response_time;
    int total_context_switches;
    ProcessArray *all_processes = new ProcessArray();
    
    process_file(filename, all_processes);

    int processes_queued = 0;
    int completed_processes = 0;
    int current_time = 0;
    PCB* currently_running_process;

    //todo remove
    int safe = 100;

    while (completed_processes < all_processes->count && safe > 0) {
        //get arriving processes from all_processes & add to ready queue
        while (all_processes->arr[processes_queued].arrival <= current_time) {
            ready_queue.enqueue(all_processes->arr[processes_queued]);
            processes_queued++;
        }
        //check if currently running process - if not, add context switching time and get next process from ready queue
        if (currently_running_process == nullptr) {

        } else {

        }
        //begin new burst
        //run process - decrement time still needed
        //if time still needed is 0, update burst, increment completed processes & set currently running to null

        //increment current_time
        current_time += 1;
        safe--;
    }

}


void sjf();
void rr();