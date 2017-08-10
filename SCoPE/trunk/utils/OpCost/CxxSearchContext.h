//
// C++ Interface: CxxSearchContext
//
// Description:
//
//
// Author: Juan Castillo <castillo@teisa.unican.es>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
 * \file CxxSearchContext.hxx
 * \brief CxxSearchContext class definition
 */
#ifndef CXX_SEARCH_CONTEXT_HXX
#define CXX_SEARCH_CONTEXT_HXX

#include "CxxToken.h"

class CxxSearchContext {
//    friend class ios;        // Suppress GNU error message for no public constructors.
	CxxSearchContext *_next;
	size_t _index;
	size_t _depth;
	size_t _size;
	size_t _mark;
	bool _enable_type1;
	size_t _line;
	size_t _advances;
	bool _status[32];
private:
	CxxSearchContext(CxxSearchContext *nextSearch);
	CxxSearchContext(const CxxSearchContext&);
	CxxSearchContext& operator=(const CxxSearchContext&);
	bool did_search() const { return _depth > 0 ? true : false; }
	void initialise(size_t markIndex, bool enableType1);
	CxxSearchContext *queue(CxxSearchContext *& listHead);
	void reset();
public:
	bool advance();
	bool enable_type1() const { return _enable_type1; }
	bool is_template();
	size_t mark() const { return _mark; }
private:
	static CxxSearchContext *_current;		//!< The current search context
	static CxxSearchContext *_free;
public:
	static size_t actual_searches;
	static size_t advances[16];
	static size_t max_search_depth;
	static size_t nested_searches;
	static size_t releases;
	static size_t search_advances;
	static size_t unnested_searches;
public:
	static CxxSearchContext *current() { return _current; }
	static void release();
	static void start(YACC_MARK_TYPE anIndex, bool enableType1);
};

#endif
