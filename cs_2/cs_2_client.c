#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "msg.h" //definitions of message types


int main()
{
	int client_PID = getpid();
	int server_initial_pipe;
	int client_send_pipe,client_recv_pipe;

	msg_t m;

	printf("This is an instance of the CS_2 client process, started with PID %d\n", client_PID);


	//initalize the message
	m.type = INITIAL;
	m.PID = client_PID;
	sprintf(m.message_text,"blah blah");

	//before writing to the server_np, open this process's pipes, i.e. <PID>_send and <PID>_recieve
		//avoids a race condition, makes sure that the pipes are set up BEFORE the
		//forked server process tries to used them to communicate with this process

	char send_pipe[12]; //pid is up to 5 digits, plus underscore, plus "send" and termination
	char recv_pipe[12]; //using recv instead of "recieve", since it's also 4 chars


	//create the two pipes to allow bidirectional client-server communication
	sprintf(send_pipe,"%d_send",client_PID);
	sprintf(recv_pipe,"%d_recv",client_PID);

	mkfifo(send_pipe, 0600);
	mkfifo(recv_pipe, 0600);

	//make sure to follow the pattern open-write-close

	//open the pipe that allows the initial contact between client and server
	server_initial_pipe = open("server_np", O_WRONLY);
	//write a message to server indicating the client's PID, allowing further communication
	write(server_initial_pipe,(char*)&m,sizeof(msg_t));
	//close the initial pipe
	close(server_initial_pipe);


	//client_send_pipe = open(send_pipe,O_WRONLY);
	//client_recv_pipe = open(recv_pipe,O_RDONLY);



	//here is where the pipes are used by the client process

	//unlink pipes
	unlink(send_pipe);
	unlink(recv_pipe);

	//fin
	return 0;
}
