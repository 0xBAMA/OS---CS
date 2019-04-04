#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


#define MSG_LENGTH 100 //maximum length of message


//expresses the type of the incoming message
typedef enum {
	REGULAR,
	COMMAND
} msg_type_t;


//structure for the message
typedef struct msg {

	msg_type_t type;
	char message_text[MSG_LENGTH];

} msg_t; //<--allows declaration with 'msg_t' rather than 'struct msg'


int main(){

	int child_PID;
	int parent_PID = getpid(); //getpid() for pid, getppid() for parent pid

	//set up pipe


	//get the child's PID at the same time as the fork
	if((child_PID = fork())){//parent process
		//close write end then read messages


	}else{						//child process
		//close read end then send messages


	}

	return 0;
}
