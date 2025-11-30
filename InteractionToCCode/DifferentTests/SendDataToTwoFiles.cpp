// This code will send a timestamp and Values to the 2 .txt files
#include <fstream> // Required for file stream operations
#include <iostream> // Required for console output

using namespace std;

int main() {
    // Create and open the first output file stream
    ofstream file1("output1.txt"); 

    // Create and open the second output file stream
    ofstream file2("output2.txt");

    // Check if both files opened successfully
    if (file1.is_open() && file2.is_open()) {
        // Write data to the first file
        file1 << "This is the first line for file 1." << endl;
        file1 << "Another line in file 1." << endl;

        // Write data to the second file
        file2 << "Data for file 2 starts here." << endl;
        file2 << "More data in file 2." << endl;

        // Close the files
        file1.close();
        file2.close();

        cout << "Data successfully written to output1.txt and output2.txt" << endl;
    } else {
        cerr << "Error: Unable to open one or both files." << endl;
    }

    return 0;
}