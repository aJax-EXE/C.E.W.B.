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
    // int fd;
    ssize_t mgSize = 0;
    string inData;

    // fcntl(fd, F_SETFL);
    // fd = open("/dev/ttyACM0", O_RDWR);
    // char buffer[1024];
    string buffer;
    fstream fs;
    fs.open("/dev/ttyACM0", ios_base::in|ios_base::out|ios_base::binary);

    

    char word[100];  // Buffer to store the word (up to 99 chars + '\0')

   
    while(true) {

        FILE *file = fopen("./data.txt", "r");  // Open file for reading
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }
        // mgSize = read(fd, buffer, 1);
        getline(fs, buffer);
        // if(buffer == "") continue;
        

        if (fscanf(file, "%99s", word) == 1) {  // Read one word from the file

        printf("The word in the file is: %s\n", word);
        rewind(file);
         } else {
        printf("No word found or read error.\n");
        }
        fclose(file);  // Close the file



        // while(buffer[0] != delim)
        // {
        //     inData += buffer[0];
        //     cout << "DEBUG: " << inData;
        // }
        // if(buffer[0] != delim) {
            
        // } else {
        cerr << "Hit Delimiter" << endl;
        cout << "DEBUG: " << buffer << endl;
        // }
       

        // memset(buffer, 0, mgSize);
        // write(buffer, 0, mgSize);

    }
   
    
    fs.close();
    // close(fd);
    return 0;
}