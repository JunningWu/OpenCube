//==========================================================================
//	uc_noc_interface.cpp
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

#include "hal/uc_noc_interface.h"
#include "hal/uc_hw_noc.h"
//#include "sicosys/ATCSimul/inc/PRZSimulation.hpp"
#include "hal/uc_simple_noc.h"

SCUC_NoC_Sim_IF *(*CreateNoCSimulatorFunction)(SCUC_NoC_IF *UC_NoC_If);

UC_NoC_Interface::UC_NoC_Interface(sc_module_name module_name,int id, bool cond_sicosys) :
		sc_module(module_name), m_numbers(NULL) {
	m_prev_clock = 0;
	m_id = id;
	m_event = new sc_event;
	m_argc = 3;
	m_argv = new char*[3];
	m_argv[0] = new char[10];
	strcpy(m_argv[0], "sicosys");
	m_argv[1] = new char[20];
	strcpy(m_argv[1], "-s");
	m_argv[2] = new char[20];
	strcpy(m_argv[2], "M88-BDOR");

	m_sim = NULL;
	m_sent_bytes = 0;
	m_sent_transfers = 0;

	m_cond_sicosys = cond_sicosys;
//	end_simul = 0;

	if (cond_sicosys == true) {
		m_sim = CreateNoCSimulatorFunction(this);
	}
	else {
		m_sim = UCSCCreateSimpleNoCSimulator(this);
	}

	SC_THREAD(sim_main_function);
}

void UC_NoC_Interface::sim_main_function()
{
	m_sim->sim_main_function(m_argc, m_argv, 1514);
}

bool UC_NoC_Interface::is_broadcast(int node){
	return node==0xffffffff;
}

bool UC_NoC_Interface::node_XYZ(int node, int *x, int *y, int *z){
	int m_n[m_x][m_y][m_z];
	memcpy(m_n,m_numbers,sizeof(int)*m_x*m_y*m_z);
	for(int i=0;i<m_x;i++) {
		for(int j=0;j<m_y;j++) {
			for(int k=0;k<m_z;k++) {
				if(m_n[i][j][k]==node){
					*x = i;
					*y = j;
					*z = k;
					return true;
				}
			}
		}
	}
	return false;
}

bool UC_NoC_Interface::XYZ_node(int x, int y, int z,int *node ){
	int m_n[m_x][m_y][m_z];
	memcpy(m_n, m_numbers, sizeof(int) * m_x * m_y * m_z);
	if (x < m_x && y < m_y && z < m_z) {
		*node = m_n[x][y][z];
		return true;
	}
	return false;
}

void UC_NoC_Interface::set_structure(int ***numbers, int x, int y, int z ){
	m_numbers = (int ***)malloc(sizeof(int) * x * y * z);
	m_x = x;
	m_y = y;
	m_z = z;
	memcpy(m_numbers, numbers, x * y * z * sizeof(int));
}

bool UC_NoC_Interface::wait_package(struct uc_packet_info **packet, int node){
	struct uc_task_info *task;
	struct uc_packet_info *d_packet;
	unsigned int flag;
	while (true) {
		for (flag = 0; flag < received.size(); flag++) {
			d_packet = (struct uc_packet_info*)received[flag];
			if (d_packet->dest == node) {
				*packet = d_packet;
				received.erase(received.begin() + flag);
				return true;
			}
		}

		// NOTE is this new deleted? (there are other two news)
		task = new struct uc_task_info;
		task->node = node;
		rec_tasks.push_back(task);
		wait(task->event);
	}
	return true;
}

bool UC_NoC_Interface::send_package(uc_packet_info *packet, int node){
	struct uc_task_info *task;
	int x,y,z,x_d,y_d,z_d;
	unsigned int flag;
	double t_val;

	int m_n[m_x][m_y][m_z];

	if (is_broadcast(packet->dest)) {
		memcpy(m_n, m_numbers, sizeof(int) * m_x * m_y * m_z);
		for (int i = 0; i < m_x; i++) {
			for (int j = 0; j < m_y; j++) {
				for (int k = 0; k < m_z; k++) {
					int node_dest = m_n[i][j][k];
					if (node_dest >= bus_ifs[0]->get_num_nodes()) {
						continue;
					}

					if (node_dest == node) {
						continue;
					}

					uc_packet_info *pack = (uc_packet_info*)malloc(sizeof(uc_packet_info));
					memcpy(pack, packet, sizeof(uc_packet_info));
					pack->dest = node_dest;
					send_package(pack,node);
				}
			}
		}
		return true;
	}

	if (node == packet->dest) {
		for (flag = 0; flag < rec_tasks.size(); flag++){
			task = (struct uc_task_info*)rec_tasks[flag];
			if (task->node == node) {
				task->event.notify();
				rec_tasks.erase(rec_tasks.begin() + flag);
				break;
			}
		}
		return true;
	}

	task = new struct uc_task_info;
	task->node=node;
	if (!node_XYZ(packet->source, &x, &y, &z)) {
		printf("Error: node not in the network");
	}

	if (!node_XYZ(packet->dest, &x_d, &y_d, &z_d)) {
		printf("Error: node not in the network");
	}

	flag = m_sim->m_messagesInNetwork();
	t_val = sc_time_stamp().value() / 1000;
	while (!m_sim->trysend_package(x, y, z, x_d, y_d, z_d, t_val, packet->size, packet->packet)) {
		send_tasks.push_back(task);
		wait(task->event);
		flag = m_sim->m_messagesInNetwork();
		t_val = sc_time_stamp().value() / 1000;
	}

	if (flag == 0) {
		m_event->notify();
	}

	sent.push_back(packet);
	return true;
}

bool UC_NoC_Interface::trywait_package(uc_packet_info **packet, int node){
	struct uc_packet_info *d_packet;
	unsigned int flag;
	for (flag = 0; flag < received.size(); flag++) {
		d_packet = (struct uc_packet_info*)received[flag];
		if (d_packet->dest == node) {
			*packet = d_packet;
			received.erase(received.begin() + flag);
			return true;
		}
	}
	return false;
}

bool UC_NoC_Interface::trywait_packages(std::vector<uc_packet_info *> &packets, int node){
	struct uc_packet_info *d_packet;
	int flag;
	int ret = false;
	for (flag = received.size() - 1; flag != 0; flag--) {
		d_packet = (struct uc_packet_info*)received[flag];
		if (d_packet->dest == node) {
			received.erase(received.begin() + flag);
			packets.push_back(d_packet);
			ret = true;
		}
	}
	return ret;
}

bool UC_NoC_Interface::trysend_package(uc_packet_info *packet, int node){
	struct uc_task_info *task;
	double t_val;
	unsigned int flag;
	int x,y,z,x_d,y_d,z_d;

	int m_n[m_x][m_y][m_z];

	m_sent_bytes += packet->size;
	m_sent_transfers++;

	if (is_broadcast(packet->dest)) {
		memcpy(m_n, m_numbers, sizeof(int) * m_x * m_y * m_z);
		for (int i = 0; i < m_x; i++) {
			for (int j = 0; j < m_y; j++) {
				for (int k = 0; k < m_z; k++) {
					uc_packet_info *pack = (uc_packet_info*)malloc(sizeof(uc_packet_info));
					memcpy(pack, packet,sizeof(uc_packet_info));
					pack->dest = m_n[i][j][k];
					trysend_package(pack,node);
				}
			}
		}
		return true;
	}

	if(node == packet->dest) {
		received.push_back(packet);
		for (flag = 0; flag < rec_tasks.size(); flag++) {
			task = (struct uc_task_info*)rec_tasks[flag];
			if (task->node == node) {
				task->event.notify();
				rec_tasks.erase(rec_tasks.begin() + flag);
				break;
			}
		}
		if(!flag) {
			send_interrupt(node,3);
		}
		return true;
	}

	if (!node_XYZ(packet->source, &x, &y, &z)) {
		printf("Error: node not in the network");
	}

	if (!node_XYZ(packet->dest, &x_d, &y_d, &z_d)) {
		printf("Error: node not in the network");
	}

	flag = m_sim->m_messagesInNetwork();
	t_val = sc_time_stamp().value() / 1000;
	if (m_sim->trysend_package(x, y, z, x_d, y_d, z_d, t_val, packet->size, packet->packet)) {
		if (flag == 0) {
			m_event->notify();
		}
		sent.push_back(packet);
		return true;
	}

	return false;
}

bool UC_NoC_Interface::sim_toInjectMessages(long long &m_Clock){
	long long ck;

	ck = m_Clock + 10 - (m_Clock % 10);

	if (rec_tasks.size() > 0) {
		tryFull();
	}

	m_prev_clock = (sc_time_stamp().value() / 1000);

	m_prev_clock &= 0xffffffffL;
	if (m_prev_clock > 0x0fffffffL && ck < m_prev_clock) {
		ck += 0xffffffffL - m_prev_clock;
	}
	else {
		ck -= m_prev_clock;
	}

	wait(ck, SC_NS);
	m_prev_clock = (sc_time_stamp().value() / 1000);

	m_Clock = m_prev_clock;
	return true;
}

bool UC_NoC_Interface::sim_waitForMessage(long long &m_Clock, long long timeout){
	if (timeout < 0) {
		wait(*m_event);
	}
	else {
		sc_time time(timeout - m_prev_clock, SC_NS);
		wait(time, *m_event);
	}

	m_prev_clock = (sc_time_stamp().value() / 1000);
	m_Clock = m_prev_clock;
	return true;
}

bool UC_NoC_Interface::sim_messageReceived(int x,int y,int z, void *package){
	struct uc_packet_info *pk;
	struct uc_task_info *task;
	int node;
	unsigned int flag,size,ff;
	size = sent.size();
	for (flag = 0; flag < size; flag++) {
		pk = (struct uc_packet_info*)sent[flag];
		if (!XYZ_node(x,y,z,&node)) {
			printf("Error: node not in the network");
		}

		if (pk->packet == package && node == pk->dest) {
			sent.erase(sent.begin() + flag);
			received.push_back(pk);
			break;
		}
	}

	if (flag == size){
		return false;
	}

	if (node != pk->dest) {
		printf("Error: message not received in the correct node");
	}

	ff = 0;
	for (flag = 0; flag < rec_tasks.size(); flag++) {
		task = (struct uc_task_info*)rec_tasks[flag];
		if (task->node == node) {
			task->event.notify();
			rec_tasks.erase(rec_tasks.begin() + flag);
			ff = 1;
		}
	}
	if (!ff) {
		send_interrupt(node, 3);
	}
	return true;
}

int UC_NoC_Interface::wait_event2(int node, int port){
	struct uc_task_info *task,*aux;
	unsigned int flag;
	int ret = 3;
	task= new struct uc_task_info;
	task->node = node;
	rec_tasks.push_back(task);
	send_tasks.push_back(task);
	wait(task->event);

	for (flag = 0; flag < rec_tasks.size(); flag++) {
		aux = (struct uc_task_info*)rec_tasks[flag];
		if (task == aux) {
			rec_tasks.erase(rec_tasks.begin() + flag);
			ret = 2;
			break;
		}
	}

	for (flag = 0; flag < send_tasks.size(); flag++) {
		aux = (struct uc_task_info*)send_tasks[flag];
		if (task == aux) {
			send_tasks.erase(rec_tasks.begin() + flag);
			return ret - 2;
		}
	}
	return ret;
}

void UC_NoC_Interface::tryFull(){
   int i,j,k,l,node,x,y,z,flag;

	for (i = 0; i < m_x; i++) {
		for (j = 0; j < m_y; j++) {
	    	for (k = 0; k < m_z; k++) {
				flag = m_sim->bufferFull(x,y,z);
				if(!flag){
		    		flag=0;
		    		XYZ_node(x,y,z,&node);
					for (l = send_tasks.size() - 1; l >= 0; l--) {
						if (node == send_tasks[l]->node) {
							send_tasks[l]->event.notify();
							send_tasks.erase(rec_tasks.begin() + flag);
							flag = 1;
						}
					}
					if(flag == 0) {
						send_interrupt(node,1);
					}
				}

	    	}
		}
    }
}

//type: 0:fatal error, 1:send buffer not full, 3: package rec.
//types not implemented: 2:send buffer empty, 4: rec. buffer full
void UC_NoC_Interface::send_interrupt(int node, int type){
	unsigned int i;
	for (i = 0; i < bus_ifs.size(); i++) {
		if (bus_ifs[i]->get_node() == node) {
			bus_ifs[i]->send_irq();
		}
	}
}

void UC_NoC_Interface::end_simulation() {

	if (m_sim != NULL) {
		m_sim->writeResults();
	}
}

noc_statistics UC_NoC_Interface::get_statistics() {
	noc_statistics stats;
	stats.bytes = m_sent_bytes;
	stats.transfers = m_sent_transfers;
	return stats;
}

long double UC_NoC_Interface::get_transport_lattency(){
	if (m_sim != NULL) {
		return m_sim->get_transport_lattency();
	}
	return 0;
}
