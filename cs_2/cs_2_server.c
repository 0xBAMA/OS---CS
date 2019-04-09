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
	int PID;
	char message_text[MSG_LENGTH];

} msg_t; //<--allows declaration with 'msg_t' rather than 'struct msg'




int main()
{
	int server_PID = getpid();

	printf("This is the CS_2 server process, started with PID %d\n", server_PID);



	return 0;
}
