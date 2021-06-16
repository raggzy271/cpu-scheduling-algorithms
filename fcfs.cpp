#include <iostream>
#include <iomanip>
using namespace std;

class FirstComeFirstServe {
    public:
    FirstComeFirstServe(int process_ids[], int burst_times[], int total_processes) : n(total_processes), avg_waiting_time(0), avg_turn_around_time(0) {
        fcfs = new int*[n];
        
        for(int i=0; i<n; i++) {
            fcfs[i] = new int[4];
            fcfs[i][0] = process_ids[i];
            fcfs[i][1] = burst_times[i];
        }
        
        calculate_waiting_times();
        calculate_turn_around_times();
    }

    void display() {
        cout << endl;

        cout << setw(13) << "Process ID" << setw(13) << "Burst Time" << setw(14) << "Waiting Time" << setw(19) << "Turn Around Time" << endl;

        for(int i=1; i<=60; i++)
            cout << "-";
        cout << endl;

        for(int i=0; i<n; i++) {
            cout << setw(13) << fcfs[i][0] << setw(13) << fcfs[i][1] << setw(14) << fcfs[i][2] << setw(19) << fcfs[i][3] << endl;
        }

        cout << "\nAverage Waiting Time = " << avg_waiting_time << endl;
        cout << "Average Turn-around Time = " << avg_turn_around_time << endl;
    }

    private:
    int n;
    int **fcfs; //table of process ids, burst times, waiting times, turn around times
    double avg_waiting_time, avg_turn_around_time;

    void calculate_waiting_times() {
        // waiting time for the first process is 0
        fcfs[0][2] = 0;
        
        // calculating waiting time
        for(int i = 1; i<n; i++) {
            //burst of prev + waiting of prev
            avg_waiting_time += fcfs[i][2] = fcfs[i-1][1] + fcfs[i-1][2];
        }

        avg_waiting_time /= n;
    }

    void calculate_turn_around_times() {
        avg_turn_around_time = 0;

        for(int i=0; i<n; i++) {
            avg_turn_around_time += fcfs[i][3] = fcfs[i][1] + fcfs[i][2]; //burst + waiting
        }

        avg_turn_around_time /= n;
    }
};

int main()
{
    int PIDs[] = {1, 2, 3, 4, 5};
    int burst_times[] = {7, 11, 2, 5, 6};
    
    FirstComeFirstServe fcfs(PIDs, burst_times, 5);
    fcfs.display();

    return 0;
}