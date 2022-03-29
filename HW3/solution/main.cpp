#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <vector>
#include <sys/wait.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

using namespace std;

// split given string into parts
std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

// get the program path by program name
std::string getProgramPath(std::string programName)
{
    // get "PATH" env variable
    char *envVarPath = getenv("PATH");

    // get all paths from env variable
    std::vector<std::string> envPaths = split(envVarPath, ':');

    // get program full path
    for (int i = 0; i < envPaths.size(); ++i)
    {
        std::string curr = envPaths[i] + "/" + programName;

        if (access(curr.c_str(), F_OK) == 0)
        {
            return curr;
            std::cout << curr << std::endl;
            // break;
        }
    }
    return "";
}

int main()
{
    while (true)
    {
        std::cout << "Shell has started... " << std::endl;

        // read command from shell
        std::string command;
        // std::getline(std::cin, command);

        // curr commaand
        command = "echo a b";
        std::vector<std::string> commandParts = split(command, ' ');

        // the program to be executed
        std::string program = commandParts[0];

        // if command is by name
        if (program.find('/') == std::string::npos)
        {
            program = getProgramPath(program);
        }

        // extra room for sentinel
        const char **argv = new const char *[commandParts.size() + 1];

        argv[0] = program.c_str();

        // add sentinel
        argv[commandParts.size()] = NULL;

        for (int i = 1; i < commandParts.size(); ++i)
        {
            argv[i] = commandParts[i].c_str();
        }

        // create child process
        pid_t child = fork();

        // could not create
        if (child == -1)
        {
            return errno;
        }

        // child will be equal to 0 only in child process
        if (child == 0)
        {
            std::cout << "Child process... " << std::endl;

            // the pid of child procces
            pid_t pid = getpid();

            // the directory 
            std::string fileDir = "/opt/silentshell/" + to_string(pid);
            
            // make direcotry if not exists
            mkdir(fileDir.c_str() , 0777);

            // the paths of std files
            std::string outPath = fileDir + "/out.std";
            std::string inPath = fileDir + "/in.std";
            std::string errPath = fileDir + "/err.std";

            freopen(outPath.c_str(), "w", stdout);
            freopen(inPath.c_str(), "r", stdin);
            freopen(errPath.c_str(), "w+", stderr);

            // execute given program
            int status_code = execv(argv[0], (char **)argv);

            if (status_code == -1)
            {
                printf("Process did not terminate correctly\n");
                exit(1);
            }

            fclose(stdout);
            fclose(stdin);
            fclose(stderr);

            exit(0);
        }
        // parent process
        else if (child > 0)
        {
            std::cout << "Parent process․․․ " << std::endl;
            int childStatus = -1;
            pid_t waitingChildPid = waitpid(child, &childStatus, 0);
        }

        exit(0);
    }

    std::cout << "Completing process " << getpid() << std::endl;

    return 0;
}
