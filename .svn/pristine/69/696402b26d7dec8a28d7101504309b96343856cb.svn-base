/*!
 * \file sc_main.cpp
 * \brief Start routines for H.264 video coder
 */

#include <iostream>
using namespace std;

#include "sc_scope.h"

int uc_main(int argc, char *argv[]);
/*
 * User application
 */
void * user_code(void *) {
	cout << "Entrada en la función user_code" << endl;
	char *argv[] = {"coder.x"};
	uc_main(1, argv);
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
		rtos = new UC_rtos_class(2, "arm926t");
		(*rtos)[0]->new_process(user_code, NULL);
		(*rtos)[1]->new_process(user_code, NULL);
		rtos_list.push_back(rtos);

		bus = new UC_TLM_bus_class(sc_gen_unique_name("HAL"), 100000000);
		bus->set_memory_address(RAM_START);

		// Processor to HAL binding
		(*rtos)[0]->bind(bus);
		(*rtos)[1]->bind(bus);

		UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name("mem"), RAM_START, RAM_START + RAM_SIZE - 1, 100/*resp.time(ns)*/);
		bus->bind(mem);

		bus->generate_memory_map();
	}

	uc_load_stop_sim_handler();

// 	uc_gui_init();

	sc_start(80,SC_SEC);
	cout << "Main finish" << endl;
	cout << "Simulated time: " << sc_time_stamp() << endl;

	for (int i = 0; i < NUM_NODES; i++) {
		delete rtos_list[i];
	}

	return 0;
}
