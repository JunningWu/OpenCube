
#include <iostream>
using namespace std;

#include "scope.h"
#include "unistd.h"

/*
 * User application
 */

void * send_data(void *) {
	cout << "Function send_data" << endl;

	int fd;
	char data_to_send[140];

	if ((fd = open("/dev/net/tun", 0)) < 0) {
		printf("Error, opening /dev/net/tun\n");
		exit(1);
	}

// 	if (read(fd, data, 14) <= 0) {
// 		printf("Error, reading /dev/net/tun\n");
// 		exit(1);
// 	}

	unsigned int data[25];
	for (int i = 0; i < 25; i++) {
		data[i] = -i-1;
	}
	// "dest, source, data"
	memcpy(data_to_send, "\0\0\0\0\0\1\0\0\0\0\0\0", 12);
	memcpy(&(data_to_send[12]), data, 100);
	if (write(fd, data_to_send, 140) <= 0) {
		printf("Error, writing /dev/net/tun\n");
		exit(1);
	}
}

void * receive_data(void *) {
	cout << "Function receive_data" << endl;

	int fd;
	char data[140];

	if ((fd = open("/dev/net/tun", 0)) < 0) {
		printf("Error, opening /dev/net/tun\n");
		exit(1);
	}

	if (read(fd, data, 140) <= 0) {
		printf("Error, reading /dev/net/tun\n");
		exit(1);
	}

	int *int_data = (int*) &(data[12]);
	cout << "Read data:" << endl;
	for (int i = 0; i < 25; i++) {
		cout << int_data[i] << endl;
	}
}
