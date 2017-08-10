/*!
 * \file sc_main.cpp
 * \brief Jpeg
 */

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
				 
#include "sc_scope.h"
#include "stream.h"

// int uc_main(int argc, char *argv[]);
extern "C" {
	int ejpgl_main(stream_t *data_in, stream_t *data_out);
}

/*
 * User application
 */
void * user_code(void *) {
	char *argv[] = { "cjpeg", "test.bmp", "test.jpg" };
	FILE *infile, *outfile;
	stream_t data_in, data_out;
	struct stat file_status;
	
	//Open input file:
	if (stat(argv[1], &file_status) != 0) {
		return NULL;
	}
// 	if (!S_ISREG(file_status)) {
// 		printf("BMP image %s is not a regular file\n", argv[1]);
// 		return 1;
// 	}
	infile = fopen(argv[1], "rb");
	if (infile == NULL) {
		printf("Cannot open intput file %s\n", argv[1]);
		return NULL;
	}
		
	//Open output file:
	outfile = fopen(argv[2], "wb");
	if (outfile == NULL) {
		printf("Cannot open output file %s\n", argv[2]);
		return NULL;
	}
	
	//Initialize streams:
	data_in.size = file_status.st_size;
	data_in.data = (byte_t *)malloc(file_status.st_size);
	data_in.pos = data_in.data;
	fread(data_in.data, sizeof(byte_t), file_status.st_size, infile);
	data_out.data = (byte_t *)malloc(file_status.st_size);
	data_out.pos = data_out.data;
	data_out.size = 0;
		
	ejpgl_main(&data_in, &data_out);
	
	fwrite(data_out.data, sizeof(byte_t), data_out.size, outfile);
	
	free(data_in.data);
	free(data_out.data);
	return NULL;
}

/*
 * sc_main
 */
#define NUM_NODES 1
int sc_main(int argc, char **argv) {
	UC_rtos_class *rtos;
	vector<UC_rtos_class *> rtos_list;
	UC_TLM_bus_class *bus;

	for (int node = 0; node < NUM_NODES; node++) {
		rtos = new UC_rtos_class(1, "arm926t");
		(*rtos)[0]->new_process(user_code, NULL);
		rtos_list.push_back(rtos);
		// Bus:
		bus = new UC_TLM_bus_class(sc_gen_unique_name("HAL"), 100000000);
		bus->set_memory_address(RAM_START);
		// Processor to HAL binding:
		(*rtos)[0]->bind(bus);
		//Memory:
		UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name("mem"), RAM_START, RAM_START + RAM_SIZE - 1, 100/*resp.time(ns)*/);
		bus->bind(mem);
		//Generate memory map:
		bus->generate_memory_map();
	}
	uc_load_stop_sim_handler();
	sc_start(100,SC_MS);
	cout << "Main finish" << endl;
	cout << "Simulated time: " << sc_time_stamp() << endl;
	for (int i = 0; i < NUM_NODES; i++) {
		delete rtos_list[i];
	}
	return 0;
}

