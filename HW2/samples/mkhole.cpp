#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <cmath>
#include <limits>

int main(int argc, char** argv){

    // the name of file to create
    const char* filename = argv[1];

    // missing file name argument
    if(argc == 1){
        std::cerr << "Missing filename argument" << std::endl;
        exit(1);
    }

    // open file in truncate mode and for write-only
    int file = open(filename, O_TRUNC | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

    // something went wrong 
    if(file < 0){
        std::cerr << "Error while opening the file " << filename << ". " << strerror(errno) << std::endl;
        exit(errno);
    }

    // get a big buffer with data (10mb)
    long long bufferSize = 10485760;

    // create new char buffer for data
    char* buffer = new char[bufferSize];

    // init buffer with data
    for(int i = 0; i < bufferSize; ++i){
        buffer[i] = 'x';
    }

    // each argument is a size of either hole or data
    for(int i = 2; i < argc; ++i){
        
        // indicate if need to create hole or data
        bool isData = i % 2 == 0;

        // decide data or hole
        const char* type = isData ? "DATA: " : "HOLE: ";

        // take next argument
        const char* size = argv[i];

        // how many bytes to go
        long long bytes = 0;

        // turn into string stream and then to bytes
        std::stringstream stream(size);

        // write into integral value
        stream >> bytes;

        // could not parse due to some error consider zero
        if(stream.fail()){
            std::cout << type << "Warning: " << "Could not parse the value " << size << " as an integer. Will use 0 instead." << std::endl;
            bytes = 0;
        }

        // notify will create region
        std::cout << type << "Creating area of " << bytes << " bytes..." << std::endl;

        // in case if need to create hole
        if(!isData){
            
            // create a hole with given size
            long long segmentHole = 0;

            // create hole with given size
            while(segmentHole < bytes){

                // left to write
                long long leftToHole = std::min(bytes - segmentHole, bufferSize);

                // create next portion
                off_t holeCreated = lseek(file, (off_t) leftToHole, SEEK_END);

                // could not create a hole for some reason
                if(holeCreated < 0){
                    std::cerr << type << "Could not create a hole with size " << bytes << " because of error " << strerror(errno) << std::endl;
                    break;
                }

                // mark next hole ready
                segmentHole += holeCreated;
            }

            // go no next area
            continue;
        }

        // need to create data, start writing data bytes
        long long segmentWritten = 0;

        // start writing
        while(segmentWritten < bytes){
            
            // need to write this much
            size_t leftToWrite = std::min(bytes - segmentWritten, bufferSize);

            // write concrete data into file
            ssize_t written = write(file, buffer, leftToWrite);

            // do not continue writing if something failed
            if(written < 0){
                std::cerr << type << "Error while writing data into the file due to " << strerror(errno) << ". Written bytes for region: " << segmentWritten << std::endl;
                break;
            }

            // mark as written
            segmentWritten += written;
        }
    }

    // no longer need the file
    int closed = close(file);

    // deallocate buffer
    delete [] buffer;

    // close the program
    return closed < 0 ? errno : 0;
}