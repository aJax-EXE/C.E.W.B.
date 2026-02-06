#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <chrono>

using namespace std;

int main() {
  // Establishing communication with the arduino board and the text file
  const char* port = "/dev/ttyACM0";
  const char* logfile = "arduino_log.txt";

  // Opening the serial port
  int fd = open(port, O_RDWR | O_NOCTTY);

  if (fd < 0) {
    perror("open serial");
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

  tty.c_cc[VMIN]  = 1;
  tty.c_cc[VTIME] = 0;

  tcsetattr(fd, TCSANOW, &tty);

  sleep(2);   // Arduino reset delay

  ofstream out(logfile, ios::app);

  if (!out.is_open()) {
    cerr << "Failed to open output file\n";
    return 1;
  }

  char buffer[256];
  string lineBuffer;
  
  auto startTime = chrono::steady_clock::now();


  cout << "Logging serial with timestamps...\n";

   while (true) {
      int n = read(fd, buffer, sizeof(buffer));

      if (n > 0) {

        for (int i = 0; i < n; i++) {

          char c = buffer[i];

          if (c != '\n') {
            lineBuffer += c;
          } else {
            auto now = chrono::steady_clock::now();
            long ms = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();

            cout << ms << "," << lineBuffer << endl;
            out  << ms << "," << lineBuffer << "\n";
            out.flush();

            lineBuffer.clear();
          }
        }
      }

      close(fd);
      out.close();
    }
}

