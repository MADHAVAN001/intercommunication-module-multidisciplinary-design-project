#include "controller.h"

// Controller Variables
char *tempCommand;
char *inputPtr[2];
int i = 0;

void *control_main(){
	do{
		if(rpi_hasReceived && isReady()){
			rpi_hasReceived = 0;

			// printf("-------------------------------\n");
			printf("[RPI] Received: %s\n", input);

			// Split Command
			int result = splitString(input, "<", inputPtr, 2);
			if(result == 2){
				message_size = strlen(inputPtr[1]);
				tempCommand = inputPtr[1];

				// printf("Target: %s\n", inputPtr[0]);
				// printf("Target Length: %d\n", strlen(inputPtr[0]));
				// printf("Target Size: %d\n", sizeof(inputPtr[0]));
				// printf("Message: %s\n", inputPtr[1]);
				// printf("Message Length: %d\n", strlen(inputPtr[1]));
				// printf("Message Size: %d\n", sizeof(inputPtr[1]));
				// printf("-------------------------------\n");

				memset(output, 0, sizeof(output));
				strncpy(output, tempCommand, strlen(tempCommand));

				for(i = 0; i < strlen(inputPtr[0]); i++){
					setTarget((&input[0])[i]);
				}
			}
			// }else{
			// 	memset(output, 0, sizeof(output));
			// 	strncpy(output, "Invalid Command: ", 17);
			// 	strcat(output, input);
			// 	setTarget(sender);
			// }

			//printf("%d:%d:%d\n", arduino_isWriting, bt_isWriting, tcp_isWriting);
		}

		if(tcp_isReconnected){
			tcp_isReconnected = 0;
			onReady();
		}
	}while(1);
}

void onReady(){
	bzero(input, BUFFER_SIZE);
	strcat(input, "BP<ready");
	rpi_hasReceived = 1;
}

int isReady(){
	return ar_isConnected && bt_isConnected && tcp_isConnected && !arduino_isWriting && !bt_isWriting && !tcp_isWriting;
}

void setTarget(char target){
	switch(target){
		case 'A':
		arduino_isWriting = 1;
		break;

		case 'B':
		bt_isWriting = 1;
		break;

		case 'P':
		tcp_isWriting = 1;
		break;
	}
}

void printInput(){
	int i;
	int n = sizeof(input);
	printf("%i bytes read. Command contains: %s\n", n, input);
	for(i=0; i<n; i++)
	{
		printf("Byte %i:%i, ",i+1, (char)input[i]);
	}
	printf("\n");
}

void printOutput(){
	int i;
	int n = sizeof(output);
	printf("%i bytes read. Response contains: %s\n", n, output);
	for(i=0; i<n; i++)
	{
		printf("Byte %i:%i, ",i+1, (char)output[i]);
	}
	printf("\n");
}

int splitString(char *input, char *delim, char *results[], int limit){
	char *token;
	int i = 0;
	for (token = strtok(input, delim); token; token = strtok(NULL, delim)){
		results[i++] = token;
		if(i > limit) break;
	}
	return i;
}

char *substring(char *string, int position, int length) 
{
	char *pointer;
	int c;

	pointer = malloc(length+1);

	if (pointer == NULL)
	{
		printf("Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	for (c = 0 ; c < position -1 ; c++) 
		string++; 

	for (c = 0 ; c < length ; c++)
	{
		*(pointer+c) = *string;      
		string++;   
	}

	*(pointer+c) = '\0';

	return pointer;
}
