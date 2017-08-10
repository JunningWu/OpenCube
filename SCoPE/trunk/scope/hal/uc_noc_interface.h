//==========================================================================
//	uc_noc_interface.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié feb 6 2008
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

#ifndef _UC_NOC_INTERFACE_H
#define _UC_NOC_INTERFACE_H

#include <vector>
#include <systemc.h>
#include <sys/socket.h>

#include "hal/scuc_noc_sim_if.h"
//#include "sicosys/SC_Simul/SC_Simul.hpp"
#include "hal/scuc_noc_if.h"
#include "rtos/kernel/uc_gui_statistics.h"

using namespace std;
class UC_hw_NoC;

struct uc_packet_info{
	int dest;
	int source;
	int size;
	void *packet;
};

struct uc_task_info{
	int node;
	int priority;
	sc_event event;
};

class UC_NoC_Interface : public SCUC_NoC_IF, public sc_module {
private:
	vector<uc_packet_info*> received;
	vector<uc_packet_info*> sent;
	vector<uc_task_info *> rec_tasks;
	vector<uc_task_info *> send_tasks;
	vector<UC_hw_NoC *> bus_ifs;
	long long m_prev_clock;
	sc_event *m_event;
	int m_id;

	int m_x;
	int m_y;
	int m_z;
	int ***m_numbers;

//	SCUC_NoC_Sim_IF *sicosys;

	bool m_cond_sicosys;
	sc_event end_simul_ev;

	/**
	 * \brief This is the pointer to the NoC simulation
	 *
	 * It is used to write the results when the simulation ends
	 */
	SCUC_NoC_Sim_IF *m_sim;

	long long m_sent_bytes;
	long long m_sent_transfers;

public:
	int m_argc;
	char** m_argv;

	SC_HAS_PROCESS(UC_NoC_Interface);

	UC_NoC_Interface(sc_module_name module_name, int id, bool cond_sicosys);
	~UC_NoC_Interface() {
		delete m_event;
	}

	void set_structure(int *** numbers, int x, int y = 1, int z = 1);
	void add_bus_interface(UC_hw_NoC* bus_if) {
		bus_ifs.push_back(bus_if);
	}

	//Interface with SCoPE
	bool wait_package(struct uc_packet_info**, int node);
	bool send_package(struct uc_packet_info*, int node);
	int wait_event2(int node, int port);// wait for both wait_packet and send_packet

	bool trywait_package(struct uc_packet_info**, int nodo);
	bool trywait_packages(vector<struct uc_packet_info*> &packages, int nodo);
	bool trysend_package(struct uc_packet_info*, int nodo);

	//Interface with sicosys
	bool sim_messageReceived(int x,int y,int z, void *package);
	bool sim_toInjectMessages(long long &m_Clock);
	bool sim_waitForMessage(long long &m_Clock, long long timeout);

	void end_simulation();
	long double get_transport_lattency();

	noc_statistics get_statistics();

protected:
	void sim_main_function();

	void tryFull();
	void send_interrupt(int node, int type);
	bool node_XYZ(int node, int *x, int *y, int *z);
	bool XYZ_node(int x, int y, int z,int *node);
	bool is_broadcast(int node);
};

#endif
