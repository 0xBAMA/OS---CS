#include <stdio.h>
#include <unistd.h>
#include <string.h>


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
	int cs_pipe[2];
	pipe(cs_pipe);

//******************************************************************************
	//get the child's PID at the same time as the fork
	if((child_PID = fork())){//parent process - closes write end then read messages
		close(cs_pipe[1]); //close write end of the pipe

		msg_t recieved_message;

		//sleep(5);

		while(1)
		{
			read(cs_pipe[0],(char*)&recieved_message,sizeof(msg_t));

			if(recieved_message.type == REGULAR)
			{//print the string
				printf("\nRecieved string: \n %sfrom client with PID %d\n\n",recieved_message.message_text, child_PID);
			}

			if(recieved_message.type == COMMAND)
			{//exit the process
				printf("Exit signal recieved, goodbye\n");
				break;
			}
		}

//******************************************************************************
	}else{						//child process - closes read end then send messages
		close(cs_pipe[0]); //close read end of the pipe

		char child_message_string[MSG_LENGTH]; //holds the user input
		char buf[6];
		char exit_string[5] = "exit";
		char send_string[5] = "send";

		msg_t msg;

		printf("This is the child - enter \'send:\' followed by a message to have the server print, or enter \'exit\' to exit.\n");

		while(1)
		{

			fgets(child_message_string,MSG_LENGTH,stdin);//get the input
			sprintf(buf,"%.4s",child_message_string);//grab the first four characters

			if(!strcmp(buf,exit_string))
			{
				sprintf(msg.message_text, "%s", child_message_string);
				msg.type = COMMAND;//prepare a message telling it to exit

				write(cs_pipe[1],(char*)&msg,sizeof(msg_t));

				break;//exit the infinite while loop, if the user input started with "exit"
			}

			if(!strcmp(buf,send_string))
			{//we know the string starts with send - construct the message and send it
				sprintf(msg.message_text, "%s", child_message_string+5); //drop "send:"
				msg.type = REGULAR;//prepare a message telling it to output a string

				write(cs_pipe[1],(char*)&msg,sizeof(msg_t));
			}
			printf("This is the child - enter \'send:\' followed by a message to have the server print, or enter \'exit\' to exit.\n");
		}
	}
//******************************************************************************
	return 0;
}
