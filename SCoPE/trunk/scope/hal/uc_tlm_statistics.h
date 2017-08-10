//==========================================================================
//	uc_tlm_bus.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun ago 25 2008
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

#ifndef _UC_TLM_STATISTICS_H_
#define _UC_TLM_STATISTICS_H_

class UC_tlm_statistics{

	public:
		UC_tlm_statistics(){
			number_of_read_acceses = 0;
			number_of_write_acceses = 0;
			size_of_read_acceses = 0;
			size_of_write_acceses = 0;
		}

		void add_read_access(unsigned int size){
			number_of_read_acceses++;
			size_of_read_acceses += size ;
		}
		void add_write_access(unsigned int size){
			number_of_write_acceses++;
			size_of_write_acceses += size;
		}

	unsigned long long number_of_read_acceses;
	unsigned long long number_of_write_acceses;
	unsigned long long size_of_read_acceses;
	unsigned long long size_of_write_acceses;

	
};
#endif
