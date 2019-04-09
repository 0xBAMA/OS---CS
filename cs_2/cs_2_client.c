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

	msg_t m;

	printf("This is an instance of the CS_2 client process, started with PID %d\n", client_PID);

	server_initial_pipe = open("server_np", O_WRONLY); //open the initial pipe

	//initalize the message
	m.type = INITIAL;
	m.PID = client_PID;
	sprintf(m.message_text,"blah blah");

	write(server_initial_pipe,(char*)&m,sizeof(msg_t)); //write a message to server


	return 0;
}
