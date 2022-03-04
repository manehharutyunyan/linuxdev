#include <iostream>
#include <unistd.h>		
#include <stdio.h>		
#include <fcntl.h>		
#include <fstream>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;

// get the logical size of the file
int getLogicalSize(char* file)
{
    // open the file
    int fd = open(file,O_RDWR); 

    // file doesn't exists
    if(fd == -1){
        cout << "couldn't open the file" << endl;
        exit(errno);
    }

    // calculate and print the logical size of source file
    int logicalSize = lseek(fd, 0, SEEK_END); 
    
    // move pointer to the strat
    lseek(fd, 0, SEEK_SET);

    // close connection
    close(fd);

    return logicalSize;
}

// get the physical size of the file
int getPhysicalSize(char* file)
{
    // open the file
    int fd = open(file,O_RDWR); 

    // move pointer to the strat
    lseek(fd, 0, SEEK_SET);

    // keep the last index
    int last = 0; 

    // physical size of source
    int physSize = 0;

    // go over file
    while(true){
        // try jump to next beginning of hole
        off_t data = lseek(fd, last, SEEK_HOLE);

        // there was some data if we got to next hole
        if(data > 0){
            // add physical data
            physSize += data - last;

            // move pointer
            last = data;
        }

        // reached the end of file
        if(data == 0){
            break;
        }

        // something went wrong
        if(data < 0){
            std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
            exit(errno);
        }

        // try find next data jumping over a hole
        off_t hole = lseek(fd, last, SEEK_DATA);

        // reached the end no data to read
        if(hole == -1 && errno == ENXIO){
            break;
        }

        // something went wrong
        if(hole == -1){
            std::cerr << "Something went wrong. " << strerror(errno) << std::endl;
            exit(errno);
        }

        if(hole > 0){
            // move pointer
            last = hole;
        }
    }
    // close connection
    close(fd);

    return physSize;
}

// copy the source to the destinantion
void copy(char* source, char* dest)
{
    // open the source file
    int source_fd = open(source, O_RDWR);
    //lseek(source_fd, 0, SEEK_SET);

    // open the dest file
    int dest_fd = open(dest, O_RDWR | O_CREAT, 0777);
    //lseek(dest_fd, 0, SEEK_SET);

    // the buffer to read 
    char* buffer = new char[1];

    // go over source and write in destinantion
    while(true){
        // read 1 byte in a buffer
        ssize_t  readedBytes = read(source_fd, buffer,1);

        // error while reading
        if(readedBytes == -1){
            cout << "error reading the file" << endl;
            exit(errno);
        }

        // end of file
        if(readedBytes == 0){
            break;
        }

        // seek in case of hole
        if(buffer[0] == '\0')
        {
            lseek(dest_fd, 1 , SEEK_CUR);
        }
        else
        {
            write(dest_fd, buffer, readedBytes);
        }

        // write bytes from the buffer
        //ssize_t  writedBytes = write(dest_fd, buffer, readedBytes);

        // // do not continue writing if something failed
        // if(writedBytes < 0){
        //     cout << "Error while writing data into the file due to " << strerror(errno) << endl;
        //     break;
        // }
    }

    // close the source
    close(source_fd);
    
    // close the dest
    close(dest_fd);
}

int main(int argc, char** argv) {
    if(argc < 3){
        cout << "give the path of source and destination files" << "\n";
        exit(1);
    }

    // read source and destination file paths from given arguments
    char* source = argv[1];
    char* dest = argv[2];

    // count the logical size of the source file
    int sourceLogicalSize = getLogicalSize(source);
    cout << "The logical size of source file is " << sourceLogicalSize << endl;

    // count the physical size of the source file
    int sourcePhysicalSize = getPhysicalSize(source);
    cout << "The physical size of source file is " << sourcePhysicalSize << endl;

    // copy the source file to dest
    copy(source, dest);
    cout << "The file is copied" << endl;  

    // count the logical size of the destinantion file
    int destLogicalSize = getLogicalSize(dest);
    cout << "The logical size of destinantion file is " << destLogicalSize << endl;

    // count the physical size of the destinantion file
    int destPhysicalSize = getPhysicalSize(dest);
    cout << "The physical size of destinantion file is " << destPhysicalSize << endl;

    return 0;
}