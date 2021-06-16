#include <iostream>
#include <iomanip>
#include <set>
using namespace std;

struct process{
    int PID;
    int arrival_time;
    int burst_time;
    int priority_no; // Lower the priority number, higher the priority
    int completion_time;
    int waiting_time;
    int turn_around_time;
    int response_time;

    process() : PID(-1), arrival_time(-1), burst_time(-1), priority_no(-1), completion_time(-1), waiting_time(-1), turn_around_time(-1), response_time(-1) {}
};

class PriorityPreemptive {
    int n; // number of processes
    process **table; // array of pointers to processes
    double avg_waiting_time, avg_turn_around_time, avg_response_time;

    void sort() {
        // sorting according to arrival time then priority numbers
        // use burst times & then PIDs to resolve clashes
        // sorting in order to know the next process to arrive in the ready queue
        
        for(int i=0; i<n; i++) {
            int min_index = i;

            for(int j=i+1; j<n; j++) {
                if(table[j]->arrival_time < table[min_index]->arrival_time ||

                  (table[j]->arrival_time == table[min_index]->arrival_time &&
                  table[j]->priority_no < table[min_index]->priority_no) ||
                  
                  (table[j]->priority_no == table[min_index]->priority_no) ||
                  table[j]->PID < table[min_index]->PID) {
                    min_index = j;
                }
            }

            swap(table[i], table[min_index]);
        }
    }

    void calculate() {
        // calculates each process' completion, waiting, response and turn around times 

        set<int> ready; // indices (as per 'table') of the processes in the ready queue
        int current; // index (as per 'table') of the currently executing process
        int next = 0; // index (as per 'table') of the next process to arrive 
        int terminated = 0; // number of processes terminated
        int timer = table[next]->arrival_time; // timer from start to end of execution of all the processes
        
        int *remaining = new int[n]; // stores the remaining burst times of each process
        for(int i=0; i<n; i++) {
            remaining[i] = table[i]->burst_time;
        }

        while(terminated!=n) { // until all the processes are terminated 

            // check if processes have arrived
            while(next<n && timer>=table[next]->arrival_time) {
                ready.insert(next++);
            }

            // if the ready queue is empty, wait for a process to arrive
            if(ready.empty()) {
                timer = table[next]->arrival_time;
                continue;
            }

            // now, select a process to execute
            current = *(ready.begin());
            for(int i : ready) {
                if(table[i]->priority_no < table[current]->priority_no ||
                (table[i]->priority_no == table[current]->priority_no &&
                (table[i]->arrival_time < table[current]->arrival_time || 
                (table[i]->arrival_time == table[current]->arrival_time &&
                table[i]->PID < table[current]->PID)))) {
                    current = i;
                }
            }
            
            // if this is the first response, calculate response time
            if(table[current]->response_time == -1) {
                table[current]->response_time = timer - table[current]->arrival_time;
            }

            // execute this process
            ready.erase(current);
            int time_executed = (next < n) ? min(table[next]->arrival_time - timer, remaining[current]) : remaining[current];
            remaining[current] -= time_executed;
            timer += time_executed;

            // if the process is still pending, preempt the resources
            if(remaining[current]!=0) {
                ready.insert(current);
            }

            // otherwise calculate times
            else {
                terminated++;
                table[current]->completion_time = timer;
                table[current]->turn_around_time = table[current]->completion_time - table[current]->arrival_time;
                table[current]->waiting_time = table[current]->turn_around_time - table[current]->burst_time;

                avg_waiting_time += table[current]->waiting_time;
                avg_turn_around_time += table[current]->turn_around_time;
                avg_response_time += table[current]->response_time;
            }
        }

        avg_waiting_time /= n;
        avg_turn_around_time /= n;
        avg_response_time /= n;
    }

    public:
    PriorityPreemptive(int process_ids[], int arrival_times[], int burst_times[], int priority_nos[], int total_processes) : n(total_processes), avg_waiting_time(0.0), avg_turn_around_time(0.0), avg_response_time(0.0) {
        table = new process*[n];
        
        for(int i=0; i<n; i++) {
            table[i] = new process();
            table[i]->PID = process_ids[i];
            table[i]->arrival_time = arrival_times[i];
            table[i]->burst_time = burst_times[i];
            table[i]->priority_no = priority_nos[i];
        }

        sort();
        calculate();
    }

    void display() {
        cout << endl;

        cout << setw(5) << "PID" << setw(13) << "Priority No" << setw(9) << "Arrival" << setw(7) << "Burst" << setw(12) << "Completion" << setw(9) << "Waiting" << setw(13) << "Turn Around" << setw(10) << "Response" << endl;

        for(int i=1; i<=78; i++)
            cout << "-";
        cout << endl;

        for(int i=0; i<n; i++) {
            cout << setw(5) << table[i]->PID << setw(13) << table[i]->priority_no << setw(9) << table[i]->arrival_time << setw(7) << table[i]->burst_time << setw(12) << table[i]->completion_time << setw(9) << table[i]->waiting_time << setw(13) << table[i]->turn_around_time << setw(10) << table[i]->response_time << endl;
        }

        cout << "\nAverage Waiting Time = " << avg_waiting_time << endl;
        cout << "Average Turn-around Time = " << avg_turn_around_time << endl;
        cout << "Average Response Time = " << avg_response_time << endl;
    }
};

int main() {
    int PIDs[] = {1, 2, 3, 4};
    int arrival_times[] = {0, 1, 2, 4};
    int burst_times[] = {5, 4, 2, 1};
    int priorities[] = {40, 30, 20, 10};
    
    PriorityPreemptive pp(PIDs, arrival_times, burst_times, priorities, sizeof(PIDs)/sizeof(int));
    pp.display();

    return 0;
}