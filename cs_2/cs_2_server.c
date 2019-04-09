#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MSG_LENGTH 100 //maximum length of message


//expresses the type of the incoming message
typedef enum {
	REGULAR,
	COMMAND
} msg_type_t;


//structure for the message
typedef struct msg {

	msg_type_t type;
	int PID;
	char message_text[MSG_LENGTH];

} msg_t; //<--allows declaration with 'msg_t' rather than 'struct msg'


int current_client_count;

//function to decrement child count
void decrement_clients(int sig){
	current_client_count--;
	signal(SIGCHILD, decrement_clients); //so it will catch the next one
}


int main()
{
	int server_PID = getpid();
	int fd, client_PID;

	current_client_count = 0; //initially, no clients have connected
	signal(SIGCHILD, decrement_clients); //set up signal handler to decrement child count

	msg_t m;

	char * np = "server_np";

	printf("This is the CS_2 server process, started with PID %d\n", server_PID);


	//create the initial FIFO
	mkfifo(np, 0600); //owner has read and write permissions, group and other have none

	while(1){

		fd = open(np, O_RDONLY); //open the pipe in read only mode

		read(fd,&m,sizeof(msg_t)); //blocking read waits for a message

		close(fd); //close the file descriptor, now that we have a message from a client

		client_PID = m.PID; //get the PID from the message

	}



	return 0;
}
