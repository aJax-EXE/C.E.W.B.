#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

const char delim = '|';

int main() {
    int fd;
    ssize_t mgSize = 0;
    string inData;

    fcntl(fd, F_SETFL);
    fd = open("/dev/ttyACM0", O_RDWR);
    char buffer[1024];

    

    char word[100];  // Buffer to store the word (up to 99 chars + '\0')

    
   
    while(true) {

        FILE *file = fopen("./data.txt", "r");  // Open file for reading
        if (file == NULL) {
        perror("Error opening file");
        return 1;
        }
        mgSize = read(fd, buffer, 1);

        

        if (fscanf(file, "%99s", word) == 1) {  // Read one word from the file

        printf("The word in the file is: %s\n", word);
         } else {
        printf("No word found or read error.\n");
        }
        fclose(file);  // Close the file



        if(buffer[0] != delim) {
            inData += buffer[0];
        } else {
            cerr << "Hit Delimiter" << endl;
             cout << "DEBUG: " << inData << endl;
        }
       

        // memset(buffer, 0, mgSize);
        // write(buffer, 0, mgSize);

    }
   
    close(fd);
    return 0;
}