#include <iostream>
#include <iomanip>
using namespace std;

struct process{
    int PID;
    double arrival_time;
    double burst_time;
    double waiting_time;
    double turn_around_time;

    process() : PID(0), arrival_time(0), burst_time(0), waiting_time(0), turn_around_time(0) {}
};

class ShortestJobFirst {
    public:
    ShortestJobFirst(int process_ids[], double arrival_times[], double burst_times[], int total_processes) : n(total_processes), avg_waiting_time(0.0), avg_turn_around_time(0.0) {
        sjf = new process*[n];
        
        for(int i=0; i<n; i++) {
            sjf[i] = new process();
            sjf[i]->PID = process_ids[i];
            sjf[i]->arrival_time = arrival_times[i];
            sjf[i]->burst_time = burst_times[i];
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
            cout << setw(13) << sjf[i]->PID << setw(15) << sjf[i]->arrival_time << setw(13) << sjf[i]->burst_time << setw(14) << sjf[i]->waiting_time << setw(19) << sjf[i]->turn_around_time << endl;
        }

        cout << "\nAverage Waiting Time = " << avg_waiting_time << endl;
        cout << "Average Turn-around Time = " << avg_turn_around_time << endl;
    }
    
    private:
    int n;
    process **sjf; //table of process ids, arrival times, burst times, waiting times, turn around times
    double avg_waiting_time, avg_turn_around_time;

    void sort() {
        // sorting according to arrival times then burst times (a low-burst process may arrive late)
        // use PIDs to resolve clashes
        
        for(int i=0; i<n; i++) {
            int min_index = i;
            for(int j=i+1; j<n; j++) {
                if(sjf[j]->arrival_time < sjf[min_index]->arrival_time ||
                  (sjf[j]->arrival_time == sjf[min_index]->arrival_time && sjf[j]->burst_time < sjf[min_index]->burst_time) ||
                  (sjf[j]->burst_time == sjf[min_index]->burst_time && sjf[j]->PID < sjf[min_index]->PID)) {
                    min_index = j;
                }
            }
            swap(sjf[i], sjf[min_index]);
        }
    }

    void calculate_waiting_times() {
        // waiting time for the first process is 0
        sjf[0]->waiting_time = 0;
        
        // calculating waiting time
        for(int i = 1; i<n; i++) {
            //if the CPU is idle in between, then it's zero
            avg_waiting_time += sjf[i]->waiting_time = max(0.0, sjf[i-1]->arrival_time + sjf[i-1]->burst_time + sjf[i-1]->waiting_time - sjf[i]->arrival_time);
        }

        avg_waiting_time /= n;
    }

    void calculate_turn_around_times() {
        for(int i=0; i<n; i++) {
            avg_turn_around_time += sjf[i]->turn_around_time = sjf[i]->burst_time + sjf[i]->waiting_time;
        }

        avg_turn_around_time /= n;
    }
};

int main() {
    int PIDs[] = {1, 2, 3};
    double arrival_times[] = {0.0, 0.4, 1};
    double burst_times[] = {8, 4, 1};
    
    ShortestJobFirst sjf(PIDs, arrival_times, burst_times, sizeof(PIDs)/sizeof(int));
    sjf.display();

    return 0;
}