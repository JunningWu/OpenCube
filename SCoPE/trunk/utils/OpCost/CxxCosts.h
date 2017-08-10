//==========================================================================
//	CxxCosts.hxx
//	Author: Hector Posadas, Juan Castillo, David Quijano
//	Date: 12-12-2007
//	Description: Static Parser for execution time estimation and cache modeling.
//  Original grammar obtained from http://willink.me.uk/projects/fog/index.html
//  These files comprises a pre-built demonstration of the superset C++ grammar
//  from FOG.
//==========================================================================
//  Copyright (C) 2008 Design of Systems on Silicon(DS2)
//  Main developer: University of Cantabria
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 2 of the License, or
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
//  For more information about SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

/*!
 * \file CxxCosts.hxx
 * \brief Static Parser for execution time estimation and cache modeling
 */
#ifndef _CXXCOSTS_H_
#define _CXXCOSTS_H_

#include "operators.h"
#include "tinyxml.h"

void load_default_processor(void);
void load_processor(char *proc_type);
void load_time_params(TiXmlNode *timeNode);
void load_instr_params(TiXmlNode *instrNode);
void load_cache_params(TiXmlNode *cacheNode);
int get_data_op_time(int op, int type);
int get_access_op_time(int op);
int get_control_op_time(int op);
int get_data_op_instr(int op, int type);
int get_access_op_instr(int op);
int get_control_op_instr(int op);

/*!
 * \struct proc_data_s
 * \brief Contains arrays to store processor parameters
 */
typedef struct proc_data_s {
	 // execution time of operators:
	int time_data[TOTAL_DATA_TYPES][TOTAL_DATA_OP];
	int time_access[TOTAL_ACCESS_OP];
	int time_control[TOTAL_CONTROL_OP];
	// Number of instructions of operators:
	int instr_data[TOTAL_DATA_TYPES][TOTAL_DATA_OP];
	int instr_access[TOTAL_ACCESS_OP];
	int instr_control[TOTAL_CONTROL_OP];
} proc_data_costs_t;

#endif
