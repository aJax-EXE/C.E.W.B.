#include <fcntl.h>    /* file open flags and open() */
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

int main() 
{
  struct termios serial_port_settings;
  char serial_read_buffer[100] = {}; //Data read from port stored in this array
  char dataToSend[10];
 
  int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY); //open a connection to serialport
  FILE *fp=fdopen(fd,"rw");
   
   
  sleep(3);  // Delay for 3 seconds,for Arduino to get stabilized,
             // Opening the port resets the Arduino
  
  int status = tcgetattr(fd, &serial_port_settings); // get the serial port settings from the termios structure
  
  /*****************     Configure the Baudrate       *******************/
  cfsetispeed(&serial_port_settings,B9600); //Set input Baudrate 
  cfsetospeed(&serial_port_settings,B9600); //Set output Baudrate
  /*****************     Configure the termios structure   ************************/
 
  serial_port_settings.c_lflag &= ~(ECHO | ECHOE | ISIG); // Enable NON CANONICAL Mode for Serial Port Comm
  serial_port_settings.c_iflag &= ~(IXON | IXOFF | IXANY);         // Turn OFF software based flow control (XON/XOFF).
  serial_port_settings.c_cflag &= ~CRTSCTS;                        // Turn OFF Hardware based flow control RTS/CTS 
  
  serial_port_settings.c_cflag |=  CREAD | CLOCAL;         // Turn ON  the receiver of the serial port (CREAD)
 
   // Set 8N1 (8 bits, no parity, 1 stop bit)
  serial_port_settings.c_cflag &= ~PARENB;      // No parity
  serial_port_settings.c_cflag &= ~CSTOPB;      // One stop bit
  serial_port_settings.c_cflag &= ~CSIZE;       
  serial_port_settings.c_cflag |=  CS8;          // 8 bits
  serial_port_settings.c_oflag &= ~OPOST;/*No Output Processing*/
  
  // serial_port_settings.c_cc[VMIN]  = 2; /* Read at least 20 characters */  
  // serial_port_settings.c_cc[VTIME] = 0; /* Wait for 10 *100ms = 1 second ,measured in increments of 100ms */
  status = tcsetattr(fd,TCSANOW,&serial_port_settings);  // update new settings to termios structure,
                                                         // TCSANOW tells to make the changes now without waiting
    if (tcflush(fd, TCIOFLUSH) != 0) {
     perror("tcflush");
    }
    int tcflushstatus = tcflush(fd, TCIOFLUSH);
    cout << "tcflushstatus: " << tcflushstatus << endl;
 
  while(true){
     /* Flush both input and output buffers to clear out garbage values */
  
    // Sending Data to the Arduino from the terminal
    cout << "Please Enter either 'A', 'B', or 'C': ";
    cin >> dataToSend;

    int bytes_written = write(fd,&dataToSend,strlen(dataToSend)); //Write data to Serial port 

    printf("\n\nCharacter Send       = %s" ,dataToSend);
    printf("\nNumber of Bytes Send = %d" ,bytes_written);
    int received_bytes ;
    char *readData=fgets((char *)serial_read_buffer,100,fp);
    char toBeRead[5];
    //read(fd, toBeRead, 2);
    //toBeRead[2]=0;
    //cout << "DEBUG: " << atoi(toBeRead) << endl;
    // Getting Data from the Arduino
    // int received_bytes = read(fd,serial_read_buffer, sizeof(serial_read_buffer)-1);
    // read(fd,serial_read_buffer, atoi(toBeRead));
    //printf("\n\nBytes Received from Serial Port = %d ",received_bytes);
    printf("\n\nData Received from Serial Port  = %s\n",readData );
  }
  fclose(fp);
  close(fd);  /* Close the file descriptor*/ 
}