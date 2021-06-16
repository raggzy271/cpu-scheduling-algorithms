#include <iostream>
#include <iomanip>
#include <deque>
using namespace std;

struct process{
    int PID;
    int arrival_time;
    int burst_time;
    int completion_time;
    int waiting_time;
    int turn_around_time;
    int response_time;

    process() : PID(-1), arrival_time(0), burst_time(0), completion_time(0), waiting_time(0), turn_around_time(0), response_time(-1) {}
};

class RoundRobin {
    public:
    RoundRobin(int process_ids[], int arrival_times[], int burst_times[], int total_processes, int time_quantum) : n(total_processes), avg_waiting_time(0.0), avg_turn_around_time(0.0), avg_response_time(0), Q(time_quantum) {
        rr = new process*[n];
        
        for(int i=0; i<n; i++) {
            rr[i] = new process();
            rr[i]->PID = process_ids[i];
            rr[i]->arrival_time = arrival_times[i];
            rr[i]->burst_time = burst_times[i];
        }

        sort();
        calculate();
    }

    void display() {
        cout << endl;

        cout << setw(12) << "Process ID" << setw(14) << "Arrival Time" << setw(12) << "Burst Time" << setw(17) << "Completion Time" << setw(14) << "Waiting Time" << setw(18) << "Turn Around Time" << setw(15) << "Response Time" << endl;

        for(int i=1; i<=102; i++)
            cout << "-";
        cout << endl;

        for(int i=0; i<n; i++) {
            cout << setw(12) << rr[i]->PID << setw(14) << rr[i]->arrival_time << setw(12) << rr[i]->burst_time << setw(17) << rr[i]->completion_time << setw(14) << rr[i]->waiting_time << setw(18) << rr[i]->turn_around_time << setw(15) << rr[i]->response_time << endl;
        }

        cout << "\nAverage Waiting Time = " << avg_waiting_time << endl;
        cout << "Average Turn-around Time = " << avg_turn_around_time << endl;
        cout << "Average Response Time = " << avg_response_time << endl;
    }
    
    private:
    int n;
    process **rr; // table of process ids, arrival, burst, completion, waiting, turn around and response times
    double avg_waiting_time, avg_turn_around_time, avg_response_time;
    double Q; // time quantum

    void sort() {
        // sorting according to arrival times
        // use PIDs to resolve clashes
        
        for(int i=0; i<n; i++) {
            int min_index = i;

            for(int j=i+1; j<n; j++) {
                if(rr[j]->arrival_time < rr[min_index]->arrival_time ||
                  (rr[j]->arrival_time == rr[min_index]->arrival_time && rr[j]->PID < rr[min_index]->PID)) {
                    min_index = j;
                }
            }

            swap(rr[i], rr[min_index]);
        }
    }

    void calculate() {
        deque<int> ready; // indices (as per 'rr') of the processes in the ready queue
        int current = -1; // index (as per 'rr') of the currently executing process
        int next = 0; // index (as per 'rr') of the next process to arrive 
        int terminated = 0; // number of processes terminated

        int *remaining = new int[n]; // stores the remaining burst times of each process
        for(int i=0; i<n; i++) {
            remaining[i] = rr[i]->burst_time;
        }

        int timer = 0; // timer from start to end of execution of all the processes
        
        while(terminated!=n) { // until all the processes are terminated

            // check if processes have arrived
            while(next<n && timer>=rr[next]->arrival_time) {
                ready.push_back(next);

                // update their waiting times while the previous process was executing
                rr[next]->waiting_time = timer - rr[next]->arrival_time;

                next++; 
            }

            // if there was a process running before whose time quantum has completed but, the process has not terminated, add it to the queue
            if(current!=-1) {
                ready.push_back(current);
            }

            // Right now, no process is running (reason: termination or context switch)...
            
            // if the ready queue is empty, wait for the processes to arrive
            if(ready.empty()) {
                timer++;
                continue;
            }
                
            // otherwise, get a process to execute
            current = ready.front();
            ready.pop_front();

            // if this is the first response, calculate response time
            if(rr[current]->response_time==-1) {
                rr[current]->response_time = timer - rr[current]->arrival_time;
            }

            // finally, execute the process
            for(int i=0; i<Q && remaining[current]; i++) {
                remaining[current]--;
                timer++;

                // while this process is executing, increase the waiting time of all the other processes in the ready queue (this will happen only when the ready queue is not empty)
                for(int j : ready) {
                    ++(rr[j]->waiting_time);
                }
            }

            // check if the process has completed execution
            if(remaining[current] == 0) {
                terminated++;
                rr[current]->completion_time = timer;
                current = -1;
            }       
        }

        for(int i=0; i<n; i++) {
            avg_waiting_time += rr[i]->waiting_time;
            avg_response_time += rr[i]->response_time;
            avg_turn_around_time += rr[i]->turn_around_time = rr[i]->burst_time + rr[i]->waiting_time;
        }

        avg_waiting_time /= n;
        avg_response_time /= n;
        avg_turn_around_time /= n;
    }
};

int main() {
    int PIDs[] = {1, 2, 3, 4};
    int arrival_times[] = {0, 1, 2, 4};
    int burst_times[] = {5, 4, 2, 1};
    
    RoundRobin rr(PIDs, arrival_times, burst_times, sizeof(PIDs)/sizeof(int), 2);
    rr.display();

    return 0;
}