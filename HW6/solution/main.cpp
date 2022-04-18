#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <sys/ucontext.h>

// get the username by uid
std::string getUser(uid_t uid)
{
    struct passwd *pws;
    pws = getpwuid(uid);

	// safety check
	if(pws == NULL)
	{
		return "NULL username";
	}

    return pws->pw_name;
}

// the SIGUSR1 signal handler
void handleSigusr(int signum, siginfo_t* siginfo, void* vcontext)
{
	std::cout << "The PID of sender: " << siginfo->si_pid << std::endl;

	// print sender uid
	uid_t uid = siginfo->si_uid;
	std::cout << "The UID of the sender: " << uid << std::endl;

	// get the sender User Name and print
	std::cout << "The username of the sender: " << getUser(uid) << std::endl;

	// print the current values of EIP, EAX, EBX registers
	ucontext_t *context = (ucontext_t*)vcontext;
	std::cout << "The value of EIP register: " << context->uc_mcontext.gregs[14] << std::endl;
	std::cout << "The value of EAX register: " << context->uc_mcontext.gregs[12] << std::endl;
	std::cout << "The value of EBX register: " << context->uc_mcontext.gregs[8] << std::endl;
//	exit(0);
}

int main()
{ 
	// sigaction struct
	struct sigaction new_sigaction;
	// init with zeros
	memset(&new_sigaction, 0, sizeof(struct sigaction));

	// initialize sa_sigaction handler
	new_sigaction.sa_sigaction = handleSigusr;

	// handle the SIGUSR1 signal action
	sigaction(10, &new_sigaction, NULL);

	// infinite loop with some delays
	while(true){
		sleep(2);
		std::cout << "Working..." << std::endl;	
	}

	return 0;
}