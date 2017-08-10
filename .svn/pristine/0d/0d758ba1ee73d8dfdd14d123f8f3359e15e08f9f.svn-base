#include <iostream>
using namespace std;

#include "sc_scope.h"
#include "hw_send_irq.h"

int bus_estress(int argc, char *argv[]);
/*
 * User application
 */
void * user_code(void *) {
	char *argv[] = {"bus.x"};
	bus_estress(1, argv);
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
		rtos = new UC_rtos_class(2);
		(*rtos)[0]->new_process(user_code, NULL, "bus_estress");
		(*rtos)[1]->new_process(user_code, NULL, "bus_estress2");
		rtos_list.push_back(rtos);

		bus = new UC_TLM_bus_class(sc_gen_unique_name("HAL"), 1000000000);
		bus->set_memory_address(RAM_START);

		// Processor to bus binding
		(*rtos)[0]->bind(bus);
		(*rtos)[1]->bind(bus);

		UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name("mem"), RAM_START, RAM_START + RAM_SIZE - 1, 1/*resp.time(ns)*/);
		bus->bind(mem);

		hw_send_irq *hw_irq = new hw_send_irq("hw_irq", 0x1000, 0x1001, 2);
		bus->bind(hw_irq);

		bus->generate_memory_map();
	}

	sc_start(130 ,SC_SEC);
	cout << "Main finish" << endl;
	cout << "Simulated time: " << sc_time_stamp() << endl;

	for (int i = 0; i < NUM_NODES; i++) {
		delete rtos_list[i];
	}

	return 0;
}
