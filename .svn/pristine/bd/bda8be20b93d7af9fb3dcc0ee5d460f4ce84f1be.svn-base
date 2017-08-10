//==========================================================================
//	scuc_noc_sim_if.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue feb 28 2008
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

#ifndef _SCUC_NOC_SIM_IF_H_
#define _SCUC_NOC_SIM_IF_H_

class SCUC_NoC_IF;

class SCUC_NoC_Sim_IF{
public:
	virtual int sim_main_function(int argc, char* argv[], int flit_length = 32)=0;
	virtual bool trysend_package(int x,int y,int z,int x_d,int y_d,int z_d,
									double time,int size,void *packet)=0;
	virtual bool bufferFull(int x,int y,int z)=0;
	virtual int m_messagesInNetwork()=0;
	virtual long double get_transport_lattency(){return 0;};
	void writeResults(){};
};

SCUC_NoC_Sim_IF *UCSCCreateNoCSimulator(SCUC_NoC_IF *UC_NoC_If);

#endif
