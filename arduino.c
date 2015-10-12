#include "arduino.h"
#include<string.h>

char buffer[BUFFER_SIZE] = "";
char temp[BUFFER_SIZE] = "";
int  n, i;
int arduino_isWaiting = 0;
//int port;
void setupArduino(){
	struct termios toptions;

	printf("[Arduino] Setting Up\n");

  	/* open serial port */
	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
	//printf("%d",fd);
	//n = read(fd, buffer, 2);
	//port = fd;
	//printf("As read during initialization: %s",buffer);	
	// Check Serial Connection Status
	if (fd != -1){

  		/* wait for the Arduino to reboot */
		usleep(3000000);

  		/* get current serial port settings */
		if (tcgetattr(fd, &toptions) < 0) {
			printf("[Arduino] Couldn't get term attributes\n");
		}
		
  		/* set 9600 baud both ways */
		cfsetispeed(&toptions, B9600);
		cfsetospeed(&toptions, B9600);

  		/* 8 bits, no parity, no stop bits */
		toptions.c_cflag &= ~PARENB;
		toptions.c_cflag &= ~CSTOPB;
		toptions.c_cflag &= ~CSIZE;
		toptions.c_cflag |= CS8;
		toptions.c_cflag &= ~CRTSCTS;
		toptions.c_cflag |= CREAD | CLOCAL;
		toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
		toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		toptions.c_oflag &= ~OPOST;
		toptions.c_cc[VMIN]  = 0;
		toptions.c_cc[VTIME] = 100;

		usleep(2000000); 
		//tcflush(fd,TCIOFLUSH);

  		/* commit the serial port settings */
		if (tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
			printf("[Arduino] Couldn't set term attributes\n");
		}

		printf("[Arduino] Connected\n");
		ar_isConnected = 1;
	}else{
		printf("[Arduino] Unable to open Serial Port\n");
	}
}

void *arduino_reconnect(){
	while(1){
		if(!ar_isConnected){
			closeArduino();
			usleep(5000000);
			setupArduino();
		}
	}
}


void *arduino_read(){
	int size = 0;
	do{
		if(arduino_isWaiting){
			//printf("In arduino");
			n  = read(fd, buffer, 1);
			if(n <= 0) continue;

        		// Append Temp
			size += n;
			strcat(temp, buffer);
			//bzero(buffer, BUFFER_SIZE);

			if(temp[size-1] != '\n'){
				//printf("[Arduino] Message Incomplete, read again: %s buffer: %s \n",temp, buffer);
				bzero(buffer,BUFFER_SIZE);
				continue;
			}
			
			strtok(temp, "\n");
			memset(input, '\0', sizeof(input));
			if(strlen(temp) == 0){
				bzero(temp, BUFFER_SIZE);
				memset(output, 0, sizeof(output));
				strncpy(output, "S", 1);
				arduino_isWriting = 1;
				arduino_isWaiting = 0;
				continue;
			}
			strncpy(input, temp, size);
			printf("[Arduino] Read: %s (%d) - %d\n", temp, strlen(temp), n);
			bzero(temp, BUFFER_SIZE);
			size = 0;
			sender = 'A';
			arduino_isWaiting = 0;
			rpi_hasReceived = 1;
		}
	}while(1);
}
			

void *arduino_write(){
	//int tmp_var = 1;
	do{
		//arduino_isWriting = 1;
		//if(tmp_var%2 == 1)
		//{
		//strcpy(output,"a");
		//tmp_var++;
		//}
		//else
		//if(tmp_var%2 ==0)
		//{
		//strcpy(output,"c");
		//tmp_var++;
		//}
		//else
		//arduino_isWriting = 0;
		//strcpy(output,"a");
		//arduino_isWriting = 1;
		//strcpy(output,"2");
		//fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NONBLOCK);
		if(arduino_isWriting && !arduino_isWaiting){
			strcat(output,"\n");
			//strtok(output,"\0");
			int status = write(fd,output, BUFFER_SIZE);
			if(status != -1) {
				arduino_isWriting = 0;
				arduino_isWaiting = 1;
				printf("[Arduino] Sent: %s (%d) - %d\n", output, strlen(output), status);
			}else{
				ar_isConnected = 0;
				printf("[Arduino] Error Writing\n");
			}
			//printf("[Arduino] Sent: %s (%d) - %d\n", output, strlen(output), status);
		}
		//arduino_isWriting = 0;
		//arduino_isWaiting = 1;
		//usleep(2000000);
	}while(1);

	return 0;
}

void closeArduino(){
	usleep(2000000); //required to make flush work, for some reason
	tcflush(fd,TCIOFLUSH);
	close(fd);
}
