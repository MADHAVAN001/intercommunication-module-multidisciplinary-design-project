#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "bluetooth.c"
#include "arduino.c"
#include "controller.c"
#include "wireless.c"

void ctrl_c_handler(int signal);
void close_sockets();

int main(int argc, char *argv[])
{
	(void) signal(SIGINT,ctrl_c_handler);

	// Multithreading Variables
	pthread_t controller_controlT, ad_readT, ad_writeT, bt_readT, bt_writeT, tcp_readT, tcp_writeT, tcp_reconnectT, bt_reconnectT, ar_reconnectT;
	int iret[10];

	// Initialize
	bt_isWriting = 0; tcp_isWriting = 0; arduino_isWriting = 0; rpi_hasReceived = 0;
	bt_isConnected = 0; tcp_isConnected = 0, tcp_isReconnected = 0, ar_isConnected = 0;

	setupArduino();

	setupBluetooth();

	setupWireless(); 

	//Create independent threads each of which will execute function
	iret[0] = pthread_create(&controller_controlT, NULL, control_main, NULL);
	iret[1] = pthread_create(&ad_readT, NULL, arduino_read, NULL);
	iret[2] = pthread_create(&ad_writeT, NULL, arduino_write, NULL);
	iret[3] = pthread_create(&bt_readT, NULL, bluetooth_read, NULL);
	iret[4] = pthread_create(&bt_writeT, NULL, bluetooth_write, NULL);

	iret[5] = pthread_create(&tcp_readT, NULL, wireless_read, NULL);
	iret[6] = pthread_create(&tcp_writeT, NULL, wireless_write, NULL);
	iret[7] = pthread_create(&tcp_reconnectT, NULL, wireless_reconnect, NULL);
	iret[8] = pthread_create(&bt_reconnectT, NULL, bluetooth_reconnect, NULL);
	iret[9] = pthread_create(&ar_reconnectT, NULL, arduino_reconnect, NULL);

	// Broadcast System is Ready
	//onReady();
	printf("[RPI] System Ready\n");

	// Wait for Threads to Terminate
        pthread_join(controller_controlT, NULL);
        pthread_join(ad_readT, NULL);
   	pthread_join(ad_writeT, NULL);
	pthread_join(bt_readT, NULL);
	pthread_join(bt_writeT, NULL);
	pthread_join(tcp_readT, NULL);
	pthread_join(tcp_writeT, NULL);
	pthread_join(tcp_reconnectT, NULL);
	pthread_join(bt_reconnectT, NULL);
	pthread_join(ar_reconnectT, NULL);

	close_sockets();
	return 0;
}

void close_sockets(){
	closeArduino();
	closeBluetooth();
	closeWireless();
    printf("Sockets Closed\n");
}

void ctrl_c_handler(int signal) {
    printf("Catched signal: %d ... !!\n", signal);
    close_sockets();
    exit(0);
}
