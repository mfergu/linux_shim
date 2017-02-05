//http://bit.ly/2lckxRB
//this example shows how to use the ptrace API to
//determine which system calls are being made by the child
//process. Note, this example, only catches the first system
//call.
//we want to watch all system calls using the ptrace API
//Strace and ltrace tools can be used to sanity check your results
//
//bare minimum reqs
//PTRACE_TRACEME,
//PTRACE_SYSCALL,
//PTRACE_PEEKUSER,
//PTRACE_SETOPTIONS requests
//And the PTRACE_O_TRACESYSGOOD option
//use fork to create the new process
//The child process should call ptrace(PTRACE_TRACEME) before calling exec*
//Once child process completed, program will write the results into the output file
//Each system call should be on its own line
//you should report the system call number
//followed by a tab character
//followed by the number of times that system call was called
//


#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int wait_for_syscall( pid_t );
int do_trace( pid_t , char*);

int main(int argc, char **argv) {

	char* cla[argc];
	if( argc > 1) {
		int i = 1;
		while(i < argc) {
			cla[i-1] = argv[i];
			++i;
		}
	}

	//create an array to break up the second cla
	char* instr_split[10] = {0}; 
	char* temp = strtok(cla[0], " ");
	int i = 0;
	while( temp != NULL) {
		instr_split[i] = temp;
		temp = strtok(NULL, " ");	
		++i;
	}
	instr_split[i] = NULL;

	//spawn child 
    pid_t child = fork();
    if (child == 0) {

		ptrace(PTRACE_TRACEME);
		kill(getpid(), SIGSTOP);
		return execvp(instr_split[0], instr_split);
		//end child process
    } else {
	
		return do_trace(child, cla[1]);
    }//end parent process
	
	//pipe the output to a file
	//cat > file_name
}

int do_trace(pid_t child, char* write_to_me) {

	int status, syscall, retval;
	unsigned int calls[512] = {0};
	waitpid(child, &status, 0);
	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	while(1) {

        if (wait_for_syscall(child) != 0) break;
		syscall = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX);
		++calls[syscall];	
		if (wait_for_syscall(child) != 0) break;
	}
	
	FILE* output_file = fopen( write_to_me, "w");
	for( int i = 0; i < 512; i++) {
		if(calls[i] > 0) {
			fprintf( output_file, "%d\t%u\n", i, calls[i]);
		}
	}
	fclose(output_file);
    return 0;
}

int wait_for_syscall( pid_t child) {
	int status;
	while( 1) {
		ptrace(PTRACE_SYSCALL, child, 0, 0);
		waitpid(child, &status, 0);
		if(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
			return 0;
		if( WIFEXITED(status))
			return 1;
	}
}
