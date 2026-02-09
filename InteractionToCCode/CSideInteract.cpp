#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <chrono>
#include <sys/select.h>

using namespace std;

int main() {

    const char* port = "/dev/ttyACM0";

    ofstream log1("log_primary.txt", ios::app);
    ofstream log2("log_backup.txt", ios::app);

    if (!log1 || !log2) {
        cerr << "Failed to open log files\n";
        return 1;
    }

    int fd = open(port, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    termios tty{};
    tcgetattr(fd, &tty);

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag = 0;
    tty.c_iflag = 0;
    tty.c_oflag = 0;

    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 1;

    tcsetattr(fd, TCSANOW, &tty);

    sleep(2);   // Arduino reset

    auto startTime = chrono::steady_clock::now();

    char serialBuf[256];
    char inputBuf[128];

    string line;

    cout << "Running. Type commands to send to Arduino.\n";

    while (true) {

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        int maxfd = max(fd, STDIN_FILENO) + 1;

        select(maxfd, &readfds, nullptr, nullptr, nullptr);

        /* ---------- SERIAL INPUT ---------- */

        if (FD_ISSET(fd, &readfds)) {

            int n = read(fd, serialBuf, sizeof(serialBuf));

            for (int i = 0; i < n; i++) {

                char c = serialBuf[i];

                if (c != '\n') {
                    line += c;
                } else {

                    auto now = chrono::steady_clock::now();
                    long ms = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();

                    // cout << ms << "," << line << endl;

                    log1 << ms << "," << line << "\n";
                    log2 << ms << "," << line << "\n";

                    log1.flush();
                    log2.flush();

                    line.clear();
                }
            }
        }

        /* ---------- TERMINAL INPUT ---------- */

        if (FD_ISSET(STDIN_FILENO, &readfds)) {

            int n = read(STDIN_FILENO, inputBuf, sizeof(inputBuf));

            if (n > 0) {
                write(fd, inputBuf, n);   // send to Arduino
            }
        }
    }

    close(fd);
}


