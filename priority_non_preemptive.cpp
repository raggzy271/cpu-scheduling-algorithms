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

    process() : PID(-1), arrival_time(-1), burst_time(-1), priority_no(-1), completion_time(-1), waiting_time(-1), turn_around_time(-1) {}
};

class PriorityNonPreemptive {
    public:
    PriorityNonPreemptive(int process_ids[], int arrival_times[], int burst_times[], int priority_nos[], int total_processes) : n(total_processes), avg_waiting_time(0.0), avg_turn_around_time(0.0) {
        pnp = new process*[n];
        
        for(int i=0; i<n; i++) {
            pnp[i] = new process;
            pnp[i]->PID = process_ids[i];
            pnp[i]->arrival_time = arrival_times[i];
            pnp[i]->burst_time = burst_times[i];
            pnp[i]->priority_no = priority_nos[i];
        }

        sort();
        calculate();
    }

    void display() {
        cout << endl;

        cout << setw(5) << "PID" << setw(13) << "Priority No" << setw(9) << "Arrival" << setw(7) << "Burst" << setw(12) << "Completion" << setw(9) << "Waiting" << setw(13) << "Turn Around" << endl;

        for(int i=1; i<=68; i++)
            cout << "-";
        cout << endl;

        for(int i=0; i<n; i++) {
            cout << setw(5) << pnp[i]->PID << setw(13) << pnp[i]->priority_no << setw(9) << pnp[i]->arrival_time << setw(7) << pnp[i]->burst_time << setw(12) << pnp[i]->completion_time << setw(9) << pnp[i]->waiting_time << setw(13) << pnp[i]->turn_around_time << endl;
        }

        cout << "\nAverage Waiting Time = " << avg_waiting_time << endl;
        cout << "Average Turn-around Time = " << avg_turn_around_time << endl;
    }
    
    private:
    int n;
    process **pnp; // table of process ids, arrival, priorities, burst, completion, waiting and turn around times
    double avg_waiting_time, avg_turn_around_time;

    void sort() {
        // sorting according to arrival time then priority numbers
        // use PIDs to resolve clashes
        
        for(int i=0; i<n; i++) {
            int min_index = i;

            for(int j=i+1; j<n; j++) {
                if(pnp[j]->arrival_time < pnp[min_index]->arrival_time ||

                  (pnp[j]->arrival_time == pnp[min_index]->arrival_time &&
                  pnp[j]->priority_no < pnp[min_index]->priority_no) ||
                  
                  (pnp[j]->priority_no == pnp[min_index]->priority_no) ||
                  pnp[j]->PID < pnp[min_index]->PID) {
                    min_index = j;
                }
            }

            swap(pnp[i], pnp[min_index]);
        }
    }

    void calculate() {
        set<int> ready; // indices (as per 'pnp') of the processes in the ready queue
        int current; // index (as per 'pnp') of the currently executing process
        int next = 0; // index (as per 'pnp') of the next process to arrive 
        int terminated = 0; // number of processes terminated
        int timer = pnp[next]->arrival_time; // timer from start to end of execution of all the processes
        
        while(terminated!=n) { // until all the processes are terminated

            // check if processes have arrived
            while(next<n && timer>=pnp[next]->arrival_time) {
                ready.insert(next);
                next++;
            }

            // Right now, no process is running...

            // if the ready queue is empty, wait for a process to arrive
            if(ready.empty()) {
                timer = pnp[next]->arrival_time;
                continue;
            }
                
            // otherwise, select a process to execute
            current = *(ready.begin());
            for(int i : ready) {
                if(pnp[i]->priority_no < pnp[current]->priority_no ||
                (pnp[i]->priority_no == pnp[current]->priority_no &&
                (pnp[i]->arrival_time < pnp[current]->arrival_time || 
                (pnp[i]->arrival_time == pnp[current]->arrival_time &&
                pnp[i]->PID < pnp[current]->PID)))) {
                    current = i;
                }
            }

            // finally, execute the process
            ready.erase(current);
            timer += pnp[current]->burst_time;
            terminated++;

            // calculations
            pnp[current]->completion_time = timer;
            pnp[current]->turn_around_time = pnp[current]->completion_time - pnp[current]->arrival_time;
            pnp[current]->waiting_time = pnp[current]->turn_around_time - pnp[current]->burst_time;
        }

        // finding out averages
        for(int i=0; i<n; i++) {
            avg_waiting_time += pnp[i]->waiting_time;
            avg_turn_around_time += pnp[i]->turn_around_time;
        }
        avg_waiting_time /= n;
        avg_turn_around_time /= n;
    }
};

int main() {
    int PIDs[] = {1, 2, 3, 4};
    int arrival_times[] = {0, 1, 2, 4};
    int burst_times[] = {5, 4, 2, 1};
    int priorities[] = {40, 30, 20, 10};
    
    PriorityNonPreemptive pnp(PIDs, arrival_times, burst_times, priorities, sizeof(PIDs)/sizeof(int));
    pnp.display();

    return 0;
}