#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <cmath>
#include <limits>

int main(int argc, char** argv){

    // the name of file to analyze
    const char* filename = argv[1];

    // missing file name argument
    if(argc == 1){
        std::cerr << "Missing filename argument" << std::endl;
        exit(1);
    }

    // open file for read only
    int file = open(filename, O_RDONLY);

    // something went wrong 
    if(file < 0){
        std::cerr << "Error while opening the file " << filename << ". " << strerror(errno) << std::endl;
        exit(errno);
    }

    // keep the last index
    int last = 0;

    // go over file
    while(true){

        // try jump to next beginning of hole
        off_t data = lseek(file, last, SEEK_HOLE);

        // there was some data if we got to next hole
        if(data > 0){
            
            // report segment
            std::cout << "Data Area Detected from " << last << " to " <<  data << std::endl;

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
        off_t hole = lseek(file, last, SEEK_DATA);

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
            // report segment
            std::cout << "Hole Area Detected from " << last << " to " << hole << std::endl;

            // move pointer
            last = hole;
        }
    }

    // no longer need the file
    int closed = close(file);

    // close the program
    return closed < 0 ? errno : 0;
}