//==========================================================================
//	sc_main.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
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
using namespace std;

#include "sc_scope.h"

void * test1(void *);
		 
int test_return;

#define NUM_NODES 1
int sc_main(int argc, char **argv) {
	UC_rtos_class *rtos;
	vector<UC_rtos_class *> rtos_list;
	UC_TLM_bus_class *bus;

	test_return = -1;
	
	for (int node = 0; node < NUM_NODES; node++) {
		rtos = new UC_rtos_class(1);

		(*rtos)[0]->new_process(test1, NULL);
		rtos_list.push_back(rtos);

		bus = new UC_TLM_bus_class(sc_gen_unique_name("bus"), 100000000);
		bus->set_memory_address(RAM_START);

		// Processor to HAL binding
		(*rtos)[0]->bind(bus);
		
		// HW creation and binding
		UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name("mem"), RAM_START, RAM_START + RAM_SIZE - 1, 100);
		bus->bind(mem);

		bus->generate_memory_map();
	}

	sc_start(10, SC_SEC);
	
	cout << "Main finish" << endl;
	cout << "Simulated time: " << sc_time_stamp() << endl;

	for (int i = 0; i < NUM_NODES; i++) {
		delete rtos_list[i];
	}

	return test_return;
}

