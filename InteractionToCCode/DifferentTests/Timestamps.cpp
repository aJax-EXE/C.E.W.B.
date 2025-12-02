// This code will be a test of timestamping the data that is sent to the 2 .txt files
#include <chrono>
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    // Record the starting timestamp
    auto start_time = chrono::high_resolution_clock::now();

    // Your program's code goes here
    // For demonstration, a simple loop is used
    for (long long i = 0; i < 100000000; ++i) {
        // Do some work
    }

    // Output the elapsed time
    while(1 == 1) {

        // Record the ending timestamp
        auto end_time = chrono::high_resolution_clock::now();

        // Calculate the duration
        auto duration = chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        cout << "Program ran for: " << duration.count() << " milliseconds." << endl;
        
        // Pausing the code for a bit to see the changes
        usleep(1000);
    }

    return 0;
}