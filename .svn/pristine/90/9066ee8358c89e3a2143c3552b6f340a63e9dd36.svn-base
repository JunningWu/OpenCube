//==========================================================================
//	sc_main.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar feb 26 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from PERFidiX,
//  released June 30, 2006. Copyright (C) 2006 Design of Systems on Silicon(DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCope you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCope, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================
#include <iostream>
#include <systemc.h>
using namespace std;

#include "sc_scope.h"

void * send_data(void *);
void * receive_data(void *);

/*
 * sc_main
 */
#define NUM_NODES 2
int sc_main(int argc, char **argv) {
	UC_rtos_class *rtos;
	vector<UC_rtos_class *> rtos_list;
	UC_TLM_bus_class *bus;

	UC_NoC_Interface *simulator = new UC_NoC_Interface("simulator", 0, false);
	int numbers[2][1][1];
	numbers[0][0][0] = 0;
	numbers[1][0][0] = 1;
	simulator->set_structure((int ***)numbers,2,1,1);

	for (int node = 0; node < NUM_NODES; node++) {
		rtos = new UC_rtos_class(1);

		if (node == 0) {
			(*rtos)[0]->new_process(send_data, NULL);
		}
		else {
			(*rtos)[0]->new_process(receive_data, NULL);
		}
		rtos_list.push_back(rtos);

		bus = new UC_TLM_bus_class(sc_gen_unique_name("HAL"), 100000000);
		bus->set_memory_address(RAM_START);

		// Processor to HAL binding
		(*rtos)[0]->bind(bus);

		UC_hw_NoC *eth = new UC_hw_NoC(sc_gen_unique_name("eth"), NET_START, NET_START + 0x3F, NET_IRQ/*irq*/, 10/*delay*/, simulator);
		bus->new_initiator_port()->bind(*eth->get_target_port());

		UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name("mem"), RAM_START, RAM_START + RAM_SIZE - 1, 100/*resp.time(ns)*/);
		bus->bind(mem);

		bus->generate_memory_map();

		insmod(rtos, tun_init);
	}

	sc_start(20, SC_US);
	cout << "Main finish" << endl;
	cout << "Simulated time: " << sc_time_stamp() << endl;

	simulator->end_simulation();
	cout << endl;

	for (int i = 0; i < NUM_NODES; i++) {
		delete rtos_list[i];
	}

	return 0;
}

