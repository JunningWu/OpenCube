//==========================================================================
//	uc_gui_connector.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié mar 5 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from SCoPE,
//  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
//
//  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "rtos/kernel/uc_gui_connector.h"
#include "rtos/kernel/uc_gui_statistics.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "hal/uc_tlm_bus_class.h"
#include "hal/uc_noc_interface.h"

UC_gui_connector::UC_gui_connector(sc_module_name name) : sc_module(name)
{
	m_interval = sc_time(20, SC_MS);
	SC_HAS_PROCESS(UC_gui_connector);
	SC_THREAD(update_gui);
	m_scope_fifo = (char *)malloc(256);
	char *gui_active = getenv("SCOPE_GUI");
	if (gui_active == NULL) {
		m_gui_active = false;
	}
	else {
		m_gui_active = true;
	}
	char *scope_path = getenv("SCOPE_HOME");

	if (scope_path == NULL) {
		fprintf(stderr, "#ERROR. SCOPE_HOME environment variable not defined!!\n\n");
		exit(-1);
	}
	strcpy(m_scope_fifo, scope_path);
	strcat(m_scope_fifo, "/gui/scope_fifo");
}

UC_gui_connector::~UC_gui_connector() {
	free(m_scope_fifo);
	if (m_gui_active == true) {
		close(m_fd);
	}
}

void UC_gui_connector::new_rtos(UC_rtos_class *rtos) {
	m_rtos_list.push_back(rtos);
}

void UC_gui_connector::new_hal(UC_TLM_bus_class *hal) {
	m_hal_list.push_back(hal);
}

void UC_gui_connector::new_noc(UC_NoC_Interface *noc) {
	m_noc_list.push_back(noc);
}

void UC_gui_connector::update_interval(sc_time interval) {
	m_interval = interval;
}

void UC_gui_connector::set_total_time(sc_time time) {
	m_total_time = time;
}

void UC_gui_connector::update_gui() {
	global_statistics gui_packet;
	gui_packet.total_time = m_total_time.value() / 1000;
	if (m_gui_active == true) {
		m_fd = open(m_scope_fifo, O_RDWR);
		if (m_fd < 0) {
			fprintf(stderr, "#ERROR. Cannot open scope_fifo for GUI interface!!\n");
			fprintf(stderr, "%s\n\n", m_scope_fifo);
			exit(-1);
		}
	}

	while (true) {
		wait(m_interval);
		//Block this thread to advance simulation:
		wait(SC_ZERO_TIME);
		wait(SC_ZERO_TIME);
		wait(SC_ZERO_TIME);
		wait(SC_ZERO_TIME);
		wait(SC_ZERO_TIME);
		gui_packet.time = sc_time_stamp().value() / 1000;
		get_rtos_statistics(gui_packet);
		get_hal_statistics(gui_packet);
// 		get_noc_statistics(gui_packet);
		send_package(gui_packet);
	}
}

void UC_gui_connector::get_rtos_statistics(global_statistics &gui_packet) {
	int cpus;
	UC_cpu_class *cpu_class;
	int i = 0;
	vector<UC_rtos_class *>::iterator it_elem;
	for (it_elem = m_rtos_list.begin(); it_elem != m_rtos_list.end(); it_elem++) {
		cpus = (*it_elem)->get_num_cpus();
		for (int cpu = 0; cpu < cpus; cpu++) {
			cpu_class = (*it_elem)->get_processor(cpu);
			gui_packet.cpu[i] = cpu_class->get_statistics();
			i++;
		}
	}
}

void UC_gui_connector::get_hal_statistics(global_statistics &gui_packet) {
	int i = 0;
	vector<UC_TLM_bus_class *>::iterator it_elem;
	for (it_elem = m_hal_list.begin(); it_elem != m_hal_list.end(); it_elem++) {
		gui_packet.bus[i] = (*it_elem)->get_statistics();
		i++;
	}
}

void UC_gui_connector::get_noc_statistics(global_statistics &gui_packet) {
	int i = 0;
	vector<UC_NoC_Interface *>::iterator it_elem;
	for (it_elem = m_noc_list.begin(); it_elem != m_noc_list.end(); it_elem++) {
		gui_packet.noc[i] = (*it_elem)->get_statistics();
		i++;
	}
}

void UC_gui_connector::send_package(global_statistics &gui_packet) {
	if (m_gui_active == true) {
		write(m_fd, (void *)&gui_packet, sizeof(gui_packet));
	}
// 	cout << "Total time: " << gui_packet.total_time << endl;
// 	cout << "Time: " << gui_packet.time << endl;
// 	cout << "Running time: " << gui_packet.cpu[0].running_time << endl;
// 	cout << "Instructions: " << gui_packet.cpu[0].instructions << endl;
// 	cout << "Instruction misses: " << gui_packet.cpu[0].instruction_misses << endl;
// 	cout << "Data misses: " << gui_packet.cpu[0].data_misses << endl;
// 	cout << "Running time: " << gui_packet.cpu[1].running_time << endl;
// 	cout << "Instructions: " << gui_packet.cpu[1].instructions << endl;
// 	cout << "Instruction misses: " << gui_packet.cpu[1].instruction_misses << endl;
// 	cout << "Data misses: " << gui_packet.cpu[1].data_misses << endl;
// 	cout << "Bus transmited bytes: " << gui_packet.bus[0].bytes << endl;
// 	cout << "Bus transmited bytes: " << gui_packet.bus[1].bytes << endl;
// 	cout << "NoC transmited bytes: " << gui_packet.noc[0].bytes << endl;
// 	cout << "NoC transfers: " << gui_packet.noc[0].transfers << endl;
}
