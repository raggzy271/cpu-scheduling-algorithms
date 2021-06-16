#include <iostream>
#include <iomanip>
using namespace std;

class FirstComeFirstServe {
    public:
    FirstComeFirstServe(int process_ids[], double arrival_times[], double burst_times[], int total_processes) : n(total_processes), avg_waiting_time(0), avg_turn_around_time(0) {
        fcfs = new double*[n];
        
        for(int i=0; i<n; i++) {
            fcfs[i] = new double[5];
            fcfs[i][0] = process_ids[i];
            fcfs[i][1] = arrival_times[i];
            fcfs[i][2] = burst_times[i];
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
            cout << setw(13) << fcfs[i][0] << setw(15) << fcfs[i][1] << setw(13) << fcfs[i][2] << setw(14) << fcfs[i][3] << setw(19) << fcfs[i][4] << endl;
        }

        cout << "\nAverage Waiting Time = " << avg_waiting_time << endl;
        cout << "Average Turn-around Time = " << avg_turn_around_time << endl;
    }
    
    private:
    int n;
    double **fcfs; //table of process ids, arrival times, burst times, waiting times, turn around times
    double avg_waiting_time, avg_turn_around_time;

    void sort() {
        //sorting according to arrival times (if they clash then PIDs)
        for(int i=0; i<n; i++) {
            int min_index = i;
            for(int j=i+1; j<n; j++) {
                if(fcfs[j][1] < fcfs[min_index][1] ||
                  (fcfs[j][1] == fcfs[min_index][1] && fcfs[j][0] < fcfs[min_index][0])) {
                    min_index = j;
                }
            }
            swap(fcfs[i], fcfs[min_index]);
        }
    }

    void calculate_waiting_times() {
        // waiting time for the first process is 0
        fcfs[0][3] = 0;
        
        // calculating waiting time
        for(int i = 1; i<n; i++) {
            //arrival of prev + burst of prev + waiting of prev - arrival or if the CPU is idle then it's zero
            avg_waiting_time += fcfs[i][3] = max(0.0, fcfs[i-1][1] + fcfs[i-1][2] + fcfs[i-1][3] - fcfs[i][1]);
        }

        avg_waiting_time /= n;
    }

    void calculate_turn_around_times() {
        avg_turn_around_time = 0;

        for(int i=0; i<n; i++) {
            avg_turn_around_time += fcfs[i][4] = fcfs[i][2] + fcfs[i][3]; //burst + waiting
        }

        avg_turn_around_time /= n;
    }
};

int main()
{
    int PIDs[] = {1, 2, 3};
    double arrival_times[] = {0, 0.4, 1};
    double burst_times[] = {8, 4, 1};
    
    FirstComeFirstServe fcfs(PIDs, arrival_times, burst_times, sizeof(PIDs)/sizeof(int));
    fcfs.display();

    return 0;
}