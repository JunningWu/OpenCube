//==========================================================================
//	uc_gui_connector.h
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

#ifndef UC_GUI_CONNECTOR_H
#define UC_GUI_CONNECTOR_H

#include <vector>
using namespace std;

#include "rtos/kernel/uc_gui_statistics.h"
#include "systemc.h"

class UC_rtos_class;
class UC_TLM_bus_class;
class UC_NoC_Interface;

class UC_gui_connector : public sc_module {
public:
    UC_gui_connector(sc_module_name name);

    ~UC_gui_connector();

	void new_rtos(UC_rtos_class *rtos);
	void new_hal(UC_TLM_bus_class *hal);
	void new_noc(UC_NoC_Interface *noc);

	void update_interval(sc_time interval);
	void set_total_time(sc_time time);

	void update_gui();

	void get_rtos_statistics(global_statistics &gui_packet);
	void get_hal_statistics(global_statistics &gui_packet);
	void get_noc_statistics(global_statistics &gui_packet);
	void send_package(global_statistics &gui_packet);

	vector<UC_rtos_class *> m_rtos_list; ///< List of rtos'
	vector<UC_TLM_bus_class *> m_hal_list; ///< List of hals
	vector<UC_NoC_Interface *> m_noc_list; ///< List of nocs

private:
	bool m_gui_active;		///< Checks if GUI is ready to receive data
	int m_fd;				///< The file descriptor of the fifo
	sc_time m_interval;		///< The update interval
	sc_time m_total_time;	///< The total time of the simulation
	char *m_scope_fifo;		///< The fifo to communicate with GUI
};

#endif
