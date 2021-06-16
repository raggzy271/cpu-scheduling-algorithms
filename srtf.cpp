#include <iostream>
#include <iomanip>
#include <set>
using namespace std;

struct process{
    int PID;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turn_around_time;

    process() : PID(0), arrival_time(0), burst_time(0), waiting_time(0), turn_around_time(0) {}
};

class ShortestRemainingTimeFirst {
    public:
    ShortestRemainingTimeFirst(int process_ids[], int arrival_times[], int burst_times[], int total_processes) : n(total_processes), avg_waiting_time(0), avg_turn_around_time(0) {
        srtf = new process*[n];
        
        for(int i=0; i<n; i++) {
            srtf[i] = new process();
            srtf[i]->PID = process_ids[i];
            srtf[i]->arrival_time = arrival_times[i];
            srtf[i]->burst_time = burst_times[i];
        }

        sort();
        calculate_waiting_times();
        calculate_turn_around_times();
    }

    void display() {
        cout << endl;

        cout << setw(13) << "Process ID" << setw(15) << "Arrival Time" << setw(13) << "Burst Time" << setw(14) << "Waiting Time" << setw(19) << "Turn Around Time" << endl;

        for(int i=1; i<=75; i++)
            cout << "-";
        cout << endl;

        for(int i=0; i<n; i++) {
            cout << setw(13) << srtf[i]->PID << setw(15) << srtf[i]->arrival_time << setw(13) << srtf[i]->burst_time << setw(14) << srtf[i]->waiting_time << setw(19) << srtf[i]->turn_around_time << endl;
        }

        cout << "\nAverage Waiting Time = " << avg_waiting_time << endl;
        cout << "Average Turn-around Time = " << avg_turn_around_time << endl;
    }
    
    private:
    int n;
    process **srtf; //table of process ids, arrival times, burst times, waiting times, turn around times
    double avg_waiting_time, avg_turn_around_time;

    void sort() {
        // sorting according to arrival times then burst times (a low-burst process may arrive late)
        // use PIDs to resolve clashes
        
        for(int i=0; i<n; i++) {
            int min_index = i;
            for(int j=i+1; j<n; j++) {
                if(srtf[j]->arrival_time < srtf[min_index]->arrival_time ||
                  (srtf[j]->arrival_time == srtf[min_index]->arrival_time && srtf[j]->burst_time < srtf[min_index]->burst_time) ||
                  (srtf[j]->burst_time == srtf[min_index]->burst_time && srtf[j]->PID < srtf[min_index]->PID)) {
                    min_index = j;
                }
            }
            swap(srtf[i], srtf[min_index]);
        }
    }

    void calculate_waiting_times() {
        set<int> ready; // indices (as per 'srtf') of the processes in the ready queue
        int terminated = 0; // number of processes terminated
        int next = 0; // index (as per 'srtf') of the next process to arrive 
        int current_time = 0;
        int shortest = -1; // process with the shortest remaining time at an instant
        
        int *remaining = new int[n]; // stores the remaining burst times of each process
        for(int i=0; i<n; i++) {
            remaining[i] = srtf[i]->burst_time;
        }

        while(terminated!=n) { // until all the processes are terminated 
            // check at every instant of time if a process has arrived
            if(next<n && current_time == srtf[next]->arrival_time) {
                ready.insert(next++);

                // if a new process arrives and there is a possibility of preemption, the current process re-enters the ready queue
                if(shortest!=-1 && remaining[shortest]!=0) {
                    ready.insert(shortest);
                }

                // since the ready queue is updated, we must now check for the process with the shortest remaining time
                for(int i : ready) {
                    if(shortest==-1 || remaining[i]<remaining[shortest]) {
                        shortest = i;
                    }
                }

                ready.erase(shortest); //the shortest process will now execute... therefore, remove it from the ready queue
            }

            // when no process is running, this means that either the ready queue is empty or the process executing before has terminated
            if(shortest==-1 && !ready.empty()) {
                shortest = *(ready.begin());
                for(int i : ready) {
                    if(remaining[i]<remaining[shortest]) {
                        shortest = i;
                    }
                }

                ready.erase(shortest);
            }

            // execute the 'shortest' process for this time instant (if there is one)
            if(shortest!=-1 && --remaining[shortest] == 0) {
                terminated++;
                shortest = -1;
            }

            // while this process is executing, increase the waiting time of all the other processes in the ready queue (this will happen only when the ready queue is not empty)
            for(int i : ready) {
                ++(srtf[i]->waiting_time);
            }

            current_time++;
        }

        for(int i=0; i<n; i++) {
            avg_waiting_time += srtf[i]->waiting_time;
        }

        avg_waiting_time /= n;
    }

    void calculate_turn_around_times() {
        for(int i=0; i<n; i++) {
            avg_turn_around_time += srtf[i]->turn_around_time = srtf[i]->burst_time + srtf[i]->waiting_time;
        }

        avg_turn_around_time /= n;
    }
};

int main() {
    int PIDs[] = {1, 2, 3, 4};
    int arrival_times[] = {0, 1, 2, 4};
    int burst_times[] = {5, 3, 4, 1};
    
    ShortestRemainingTimeFirst srtf(PIDs, arrival_times, burst_times, sizeof(PIDs)/sizeof(int));
    srtf.display();

    return 0;
}