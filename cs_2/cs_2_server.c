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
	signal(SIGCHLD, decrement_clients); //set up the handler again so it will catch the next one
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

		client_PID = m.PID; //get the PID from the message

		current_client_count++; //increment before the fork - parent needs to retain this information

		//fork
		if(!fork()){ //here we enter the child function

			//we're going to look at this tomorrow

			//handle the message
			//open fifos for the particular process
			//handle further messages
			//continue till it exits
		}
	}



	return 0;
}
