//==========================================================================
//	CxxSearchContext.cpp
//	Author: Juan Castillo, David Quijano
//	Date: lun dic 3 2007
//	Description:
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
 * \file CxxSearchContext.cxx
 * \brief Method implementation for CxxSearchContext class
 * \author Juan Castillo
 */
#include <stdlib.h>
#include "CxxSearchContext.h"

extern int line_number;

CxxSearchContext *CxxSearchContext::_current = 0;
CxxSearchContext *CxxSearchContext::_free = 0;
size_t CxxSearchContext::actual_searches = 0;
size_t CxxSearchContext::advances[16] = { 0 };
size_t CxxSearchContext::max_search_depth = 0;
size_t CxxSearchContext::nested_searches = 0;
size_t CxxSearchContext::releases = 0;
size_t CxxSearchContext::search_advances;
size_t CxxSearchContext::unnested_searches;

/*!
 * \fn CxxSearchContext::CxxSearchContext(CxxSearchContext *nextSearch)
 * \brief Constructor
 */
CxxSearchContext::CxxSearchContext(CxxSearchContext *nextSearch) {
	_next = nextSearch;
	_index = 0;
	_depth = 0;
	_size = sizeof(_status)/sizeof(_status[0]);
	_mark = 0;
	_enable_type1 = false;
	_line = 0;
	_advances = 0;
}

//
//	Implements a binary search counter, performing the increment at the
//	_index of othe failed search.
//
bool CxxSearchContext::advance() {
	_advances++;
	size_t i = _depth;
	if (i <= 0) {
		return false;
	}
	while (--i > _index) {
		_status[i] = false;
	}
	while (true) {
		if (!_status[i]) {
			_status[i] = true;
			_index = 0;
			return true;
		}
		if (i <= 0) {
			return false;
		}
		_status[i--] = false;
	}
}


void CxxSearchContext::initialise(size_t markIndex, bool enableType1) {
	_index = 0;
	_depth = 0;
	_mark = markIndex;
	_enable_type1 = enableType1;
	_line = line_number;
	_advances = 0;
}


bool CxxSearchContext::is_template() {
	if (_index >= _depth) {
		if (_depth >= _size) {
			ERRMSG("Binary search depth exceeded.");
			return false;
		}
		_status[_depth++] = false;
		if (_depth > max_search_depth) {
			max_search_depth = _depth;
		}
	}
	return _status[_index++] ? false : true;
}

//
//	Put this element onto listHead, returning element under this one.
//
CxxSearchContext *CxxSearchContext::queue(CxxSearchContext *& listHead) {
	CxxSearchContext *oldNext = _next;
	_next = listHead;
	listHead = this;
	return oldNext;
}

//
//	Release the current search buffer.
//
void CxxSearchContext::release() {
	if (_current) {
		releases++;
		_current->reset();
		_current = _current->queue(_free);
	}
}

void CxxSearchContext::reset() {
	if (did_search()) {
		_advances++;
		actual_searches++;
	}
	if (_advances >= sizeof(advances)/sizeof(advances[0])) {
		advances[sizeof(advances)/sizeof(advances[0])-1]++;
	}
	else {
		advances[_advances]++;
	}
}


/*!
 * \fn CxxSearchContext::start(YACC_MARK_TYPE anIndex, bool enableType1)
 * \brief Starts a new search for gramatical sense
 */
void CxxSearchContext::start(YACC_MARK_TYPE anIndex, bool enableType1) {
	if (_current == NULL) {
		//There is not currently a search. The new search is unnested.
		unnested_searches++;
	}
	else {
		nested_searches++;
	}
	if (!_free) {
		_current = new CxxSearchContext(_current);
	}
	else {
		_free = _free->queue(_current);
	}
	_current->initialise(anIndex, enableType1);
}
