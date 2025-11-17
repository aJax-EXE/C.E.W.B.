#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>


using namespace std;

// ------------------ Timestamp Function ------------------

string getTimestamp() {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);

    tm local_tm{};
    localtime_r(&t, &local_tm);

    stringstream ss;
    ss << put_time(&local_tm, "[%Y-%m-%d %H:%M:%S] ");
    return ss.str();
}

// ------------------ Serial Setup ------------------

int openSerial(const char* port) {
    int serial = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial < 0) {
        cerr << "Failed to open port " << port << "\n";
        exit(1);
    }

    termios tty{};
    tcgetattr(serial, &tty);

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    tcsetattr(serial, TCSANOW, &tty);

    return serial;
}

// ------------------ Main Program ------------------

int main() {
    const char* serialPort = "/dev/ttyACM0";  // Change if needed
    const string filename = "data.txt";

    int serial = openSerial(serialPort);
    cout << "Connected to Arduino.\n";

    off_t lastSize = 0;

    while (true) {
        struct stat st{};
        if (stat(filename.c_str(), &st) == 0) {
            if (st.st_size > lastSize) {

                ifstream file(filename);
                file.seekg(lastSize);

                string line;
                while (getline(file, line)) {
                    string msg = line + "\n";
                    write(serial, msg.c_str(), msg.size());
                }

                lastSize = st.st_size;
            }
        }

        // Read Arduino reply, if any
        char buffer[256];
        int n = read(serial, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            string response = buffer;

            // Print to terminal
            cout << "[Arduino] " << response;

            // Append timestamped line to file
            ofstream fout(filename, ios::app);
            fout << getTimestamp() << response;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }

    close(serial);
    return 0;
}

// // Without Timestamp Version
// #include <iostream>
// #include <string>
// #include <fstream>
// #include <chrono>
// #include <thread>
// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <termios.h>

// using namespace std;

// // ------------------ Serial Setup ------------------

// int openSerial(const char* port) {
//     int serial = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
//     if (serial < 0) {
//         cerr << "Failed to open port " << port << "\n";
//         exit(1);
//     }

//     termios tty{};
//     tcgetattr(serial, &tty);

//     cfsetospeed(&tty, B9600);
//     cfsetispeed(&tty, B9600);

//     tty.c_cflag |= (CLOCAL | CREAD);
//     tty.c_cflag &= ~CSIZE;
//     tty.c_cflag |= CS8;
//     tty.c_cflag &= ~PARENB;
//     tty.c_cflag &= ~CSTOPB;
//     tty.c_cflag &= ~CRTSCTS;

//     tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//     tty.c_iflag &= ~(IXON | IXOFF | IXANY);
//     tty.c_oflag &= ~OPOST;

//     tcsetattr(serial, TCSANOW, &tty);

//     return serial;
// }

// // ------------------ Main Program ------------------

// int main() {
//     const char* serialPort = "/dev/ttyACM0";  // Change to /dev/ttyUSB0 if needed
//     const string filename = "data.txt";       // The file being monitored

//     int serial = openSerial(serialPort);
//     cout << "Connected to Arduino.\n";

//     // Track file size to detect changes
//     off_t lastSize = 0;

//     while (true) {
//         struct stat st{};
//         if (stat(filename.c_str(), &st) == 0) {
//             if (st.st_size > lastSize) {
//                 // New data added to the file
//                 ifstream file(filename);
//                 file.seekg(lastSize);   // jump to previous end of file

//                 string line;
//                 while (getline(file, line)) {
//                     string msg = line + "\n";
//                     write(serial, msg.c_str(), msg.size());
//                 }

//                 lastSize = st.st_size;
//             }
//         }

//         // Read Arduino reply
//         char buffer[256];
//         int n = read(serial, buffer, sizeof(buffer) - 1);
//         if (n > 0) {
//             buffer[n] = '\0';
//             string response = buffer;

//             // Print to terminal
//             cout << "[Arduino] " << response;

//             // Append to the same file
//             ofstream fout(filename, ios::app);
//             fout << response;
//         }

//         this_thread::sleep_for(chrono::milliseconds(100));
//     }

//     close(serial);
//     return 0;
// }
