#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h> //didn't realize bools required a header

#include "msg.h" //definitions of message types

int current_client_count;

//function to decrement child count
void decrement_clients(int sig){
	current_client_count--;
	// printf("A child process has terminated - leaving %d clients connected\n", current_client_count);
	signal(SIGCHLD, decrement_clients); //make sure the handler is registered
}


int main()
{
	int server_PID = getpid();
	int fd, client_PID;

	int client_send_pipe, client_recv_pipe;

	current_client_count = 0; //initially, no clients have connected
	signal(SIGCHLD, decrement_clients); //set up signal handler to decrement child count

	initial_msg_t m; //used as a buffer for the incoming initial message
	msg_t msg;

	char * np = "server_np"; //name of the named pipe


	// printf("This is the CS_2 server process, started with PID %d and %d chlidren\n", server_PID, current_client_count);


	//create the initial FIFO
	mkfifo(np, 0600); //owner has read and write permissions, group and other have none



	char client_send_str[12];//holds the name of the <PID>_send pipe
	char client_recv_str[12];//holds the name of the <PID>_recv pipe



	while(1){

		fd = open(np, O_RDONLY); //open the pipe in read only mode
		read(fd,(char*)&m,sizeof(initial_msg_t)); //blocking read waits for a message
		close(fd); //close the file descriptor, now that we have a message from a client
		//make sure to follow the pattern open-read-close to avoid complications

		client_PID = m.PID; //get the PID from the message

		sprintf(client_send_str,"%d_send",client_PID);
		sprintf(client_recv_str,"%d_recv",client_PID);


		//fork
		if(!fork()){ //here we enter the child function

			while(1){

				client_send_pipe = open(client_send_str,O_RDONLY);				//open
				read(client_send_pipe,(char*)&msg,sizeof(msg_t));					//read
				close(client_send_pipe);																	//close

				printf("I HAVE RECIEVED THE CLIENT'S MESSAGE\n");

				// client_recv_pipe =

				if(msg.type == STATUS){
				//response will include the number of children

						printf("Client process %d wants to know how many children I have.\n",client_PID);

						printf("and so it shall\n");

				}else if(msg.type == TIME){
				//response will tell the time to the client

						printf("Client process %d wants to know the time.\n",client_PID);

						printf("and so it shall\n");

				}else if(msg.type == STRING){
				//does not require a response

						printf("Client process %d wants me to echo a string.\n", client_PID);

				}else if(msg.type == COMMAND){
				//does not require a response

						printf("Client process %d is done.\n", client_PID);
						break;//exit loop

				}else{
						printf("Server recieved message of unknown type - exiting\n");
						break;
				}
			}

			printf("Child process handling client number %d has finished", client_PID);

			return 0; //once you have exited infinite while loop, exit this child process

		}else{

			//parent code
			current_client_count++; //increment - parent needs to retain this information

		}
	}

	unlink(np); //get rid of the fifo
	return 0;
}
