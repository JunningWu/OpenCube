/*!
 * \file sc_main.cpp
 * \brief Vocoder gsm with NoC (gui demo)
 */

#include <iostream>
using namespace std;

#include "sc_scope.h"

extern "C" void * coder_main(void *);
extern "C" void * decoder_main(void *);

/*
 * sc_main
 */
#define NUM_NODES 2
int sc_main(int argc, char **argv) {
	UC_rtos_class *rtos;
	vector<UC_rtos_class *> rtos_list;
	UC_TLM_bus_class *bus;

	// NoC creation
	UC_NoC_Interface *simulator = new UC_NoC_Interface("simulator", 1, true);
	int numbers[2][1][1];
	numbers[0][0][0] = 0;
	numbers[1][0][0] = 1;
	simulator->set_structure((int ***)numbers,2,1,1);

	UC_gui_connector gui_connector("gui_connector");
	gui_connector.update_interval(sc_time(20, SC_MS));
	gui_connector.new_noc(simulator);

	for (int node = 0; node < NUM_NODES; node++) {
		rtos = new UC_rtos_class(1);
		if (node == 0) {
			(*rtos)[0]->new_process(coder_main, NULL);
		}
		else {
			(*rtos)[0]->new_process(decoder_main, NULL);
		}

		rtos_list.push_back(rtos);
		gui_connector.new_rtos(rtos);

		bus = new UC_TLM_bus_class(sc_gen_unique_name("HAL"), 100000000);
		bus->set_memory_address(RAM_START);

		gui_connector.new_hal(bus);

		// Processor to HAL binding
		(*rtos)[0]->bind(bus);

		UC_hw_NoC *eth = new UC_hw_NoC(sc_gen_unique_name("eth"), NET_START, NET_START + 0x3F, NET_IRQ/*irq*/, 10/*delay*/, simulator);
		bus->bind(eth);

		UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name("mem"), RAM_START, RAM_START + RAM_SIZE - 1, 100/*resp.time(ns)*/);
		bus->bind(mem);

		bus->generate_memory_map();

		insmod(rtos, tun_init);
	}

	sc_time total_simulation_time(30000, SC_MS);
	gui_connector.set_total_time(total_simulation_time);

	sc_start(total_simulation_time);
	cout << "Main finish" << endl;
	cout << "Simulated time: " << sc_time_stamp() << endl;

	simulator->end_simulation();
	cout << endl;

	for (int i = 0; i < NUM_NODES; i++) {
		delete rtos_list[i];
	}

	return 0;
}
