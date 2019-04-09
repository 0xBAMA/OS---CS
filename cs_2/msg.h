//this would all be the same in both files, so I went ahead and made it a header

#define MSG_LENGTH 100 //maximum length of message


//expresses the type of the incoming message
typedef enum {
	INITIAL,
	REGULAR,
	COMMAND
} msg_type_t;


//structure for the message
typedef struct msg {

	msg_type_t type;
	int PID;
	char message_text[MSG_LENGTH];

} msg_t; //<--allows declaration with 'msg_t' rather than 'struct msg'
