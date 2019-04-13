#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "msg.h" //definitions of message types

int current_client_count;

//function to decrement child count
void decrement_clients(int sig){
	current_client_count--;
	printf("a great tragedy, a child has died - leaving %d\n", current_client_count);
	signal(SIGCHLD, decrement_clients); //make sure the handler is registered
}


int main()
{
	int server_PID = getpid();
	int fd, client_PID;

	current_client_count = 0; //initially, no clients have connected
	signal(SIGCHLD, decrement_clients); //set up signal handler to decrement child count

	msg_t m; //used as a buffer for the incoming message

	char * np = "server_np"; //name of the named pipe

	printf("This is the CS_2 server process, started with PID %d and %d chlidren\n", server_PID, current_client_count);


	//create the initial FIFO
	mkfifo(np, 0600); //owner has read and write permissions, group and other have none

	while(1){

		fd = open(np, O_RDONLY); //open the pipe in read only mode

		read(fd,(char*)&m,sizeof(msg_t)); //blocking read waits for a message

		close(fd); //close the file descriptor, now that we have a message from a client

		//make sure to follow the pattern open-read-close to avoid complications

		client_PID = m.PID; //get the PID from the message


		//fork
		if(!fork()){ //here we enter the child function

			//child code

			//handle the message
			//open fifos for the particular process
			//handle further messages
			//continue till it exits
		}else{
			//parent code
			current_client_count++; //increment - parent needs to retain this information
			printf("I, the server, have given birth and now posess %d children\n", current_client_count);
		}
	}


	unlink(np); //get rid of the fifo
	return 0;
}
