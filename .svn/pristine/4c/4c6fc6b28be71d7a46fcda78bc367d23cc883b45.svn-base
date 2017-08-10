//==========================================================================
//	uc_icache_line.h
//	Author: Juan Castillo
//	Date: mar sep 8 2009
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
/**
 * \file uc_icache_line.h"
 * \brief Definitions for user code interface
 * This file must be C code since it is included automatically
 * as header in the application code
 */
#ifndef UC_ICACHE_LINE_H
#define UC_ICACHE_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct icache_line_s {
	char num_set;		///< The set where line should be allocated
	char hit;			///< 1=hit, 0=miss
} uc_icache_line_t;


typedef struct icache_tag_s {
	int tag;
	int iStart;
	int iEnd;
} uc_icache_tag_t;


	void ic_insert_line(uc_icache_line_t *line);
	void ic_search_tag(int tag, int iStart, int iEnd);
#ifdef __cplusplus
}
#endif 

#endif /* UC_ICACHE_LINE_H */




