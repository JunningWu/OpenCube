//==========================================================================
//	uc_gui_statistics.h
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

#ifndef UC_GUI_STATISTICS_H
#define UC_GUI_STATISTICS_H

struct cpu_statistics {
	long long running_time;
	long long instructions;
	long long instruction_misses;
	long long data_misses;
};

struct bus_statistics {
	long long bytes;
};

struct noc_statistics {
	long long bytes;
	long long transfers;
};

struct global_statistics {
	long long total_time;
	long long time;

	cpu_statistics cpu[8];
	bus_statistics bus[8];
	noc_statistics noc[1];

/*	global_statistics(){
		int cpus = 0;
		for (it_elem = m_rtos_list.begin(); it_elem != m_rtos_list.end(); it_elem++) {
			cpus += (*it_elem)->get_num_cpus();
	}*/
};

#endif
