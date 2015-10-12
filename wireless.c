#include "wireless.h"

// Wireless Variables
int  newsockfd, n, tcp_isReconnecting = 0, tcp_hasReconnected = 1;
char buffer[BUFFER_SIZE];
struct sockaddr_in serv_addr, cli_addr;
socklen_t clilen;

void setupWireless(){

	printf("[TCP] Setting Up\n");
	//printf("TCP_isreconnecting value: %d", tcp_isReconnecting);
	fflush(stdout);
	if(!tcp_isReconnecting){
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0){
			printf("[TCP] ERROR opening socket\n");
			return;
		}

		bzero((char *)&serv_addr, sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(WIRELESS_PORT);

		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			tcp_isReconnecting = 0;
			printf("[TCP] ERROR on binding - %s\n", strerror(errno));
			return;
		}

		listen(sockfd, 3);
		printf("[TCP] Waiting for Client\n");
	}

	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0){
		tcp_isReconnecting = 0;
		printf("[TCP] ERROR on accept - %s\n", strerror(errno));
		return;
	}
	
	bzero(buffer, BUFFER_SIZE);

	if(!tcp_isReconnecting){
		printf("[TCP] Connected\n");
		tcp_isConnected = 1;
	}
}

//Re-Connection After 6 seconds
void *wireless_reconnect(){

	while (1)
	{
		
		if(!tcp_isConnected){
			tcp_isReconnecting = 1;
			tcp_hasReconnected = 0;
			printf("[TCP] Connection Lost\n");

			closeWireless();
			usleep(6000000);
			setupWireless();

			printf("[TCP] Attempting to Reconnect...\n");

			// Send Reconnection Message
			write(newsockfd, "Reconnecting", 12);
			write(newsockfd, "\n", 1);
			usleep(500000);

			// Listen for Reconnection Message
			//n = read(newsockfd, buffer, BUFFER_SIZE);
			//strtok(buffer, "\n");
			//strtok(buffer, "\r");
			//if(strcmp(buffer, "Reconnected") == 0){
				printf("[TCP] Connection Restored\n");
				tcp_isConnected = 1;
				tcp_isReconnected = 1;
			//}
		}
	}
}

void *wireless_read(){
	do{
		if (tcp_isConnected){
			n = read(newsockfd, buffer, BUFFER_SIZE);
			if (n > 0){
				strtok(buffer, "\n");
				//strtok(buffer, "\r");
				strncpy(input, buffer, sizeof(buffer));
				fprintf(stdout, "[TCP] Read: %s (%d Bytes)\n", buffer, n);
				bzero(buffer, BUFFER_SIZE);
				sender = 'P';
				rpi_hasReceived = 1;
			}else{
				tcp_isConnected = 0;
				usleep(10000000);
			}
		}
	} while (1);
}

void *wireless_write(){

	int status;

	do{
		//strcpy(output,"SE");
		//tcp_isWriting = 1;
		//tcp_isConnected = 1;
		//printf("\nEnter the string to be sent: ");		
		//scanf("%s",output);
		if (tcp_isWriting && tcp_isConnected){
			strcat(output,"\n");
			status = write(newsockfd, output, strlen(output));
			//status += write(newsockfd, "\n", 1);
			if (status > 0) {
				tcp_isWriting = 0;
				fprintf(stdout, "[TCP] Sent: %s (%d Bytes)\n", output, status);
			}else{
				tcp_isConnected = 0;
				usleep(10000000);
			}
		}
	} while (1);
}

void closeWireless(){
	if(tcp_isReconnecting){
		shutdown(newsockfd, SHUT_RDWR);
		close(newsockfd);
		return;
	}
	shutdown(newsockfd, SHUT_RDWR);
	close(newsockfd);
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);
}
