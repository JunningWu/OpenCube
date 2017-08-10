/*!
 * \file sc_main.cpp
 * \brief Start routines for H.264 video coder
 */

#include <iostream>
using namespace std;

#include "sc_scope.h"

int uc_main(void);
/*
 * User application
 */
void * user_code(void *) {
	uc_main();
}

/*
 * sc_main
 */
#define NUM_NODES 1
int sc_main(int argc, char **argv) {
	UC_rtos_class *rtos;
	vector<UC_rtos_class *> rtos_list;
	UC_TLM_bus_class *bus;
	UC_TLM_bus_class *hub;

	for (int node = 0; node < NUM_NODES; node++) {
		rtos = new UC_rtos_class(1, "arm926t");
		(*rtos)[0]->new_process(user_code, NULL, "bubble");
		rtos_list.push_back(rtos);

		bus = new UC_TLM_bus_class(sc_gen_unique_name("HAL"), 100000000, RAM_START);
		hub = new UC_TLM_bus_class(sc_gen_unique_name("hub"), 100000000);

		// Processor to bus binding
		(*rtos)[0]->bind(bus);

		bus->bind(hub);

		UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name("mem"), RAM_START, RAM_START + RAM_SIZE - 1, 100/*resp.time(ns)*/);
		hub->bind(mem);

		bus->generate_memory_map();
	}

	sc_start(100,SC_MS);
	cout << "Main finish" << endl;
	cout << "Simulated time: " << sc_time_stamp() << endl;

	for (int i = 0; i < NUM_NODES; i++) {
		delete rtos_list[i];
	}

	return 0;
}
