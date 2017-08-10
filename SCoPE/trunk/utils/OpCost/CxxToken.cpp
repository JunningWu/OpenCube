/**********************************************************************************
*
* Static Parser for execution time estimation and cache modeling
*
* Author: Hector Posadas
* mailto: posadash@teisa.unican.es
* University of Cantabria
*
************************************************************************************
* Original grammar obtained from http://willink.me.uk/projects/fog/index.html
*
* These files comprises a pre-built demonstration of the superset C++ grammar
* from FOG.
*
*************************************************************************************/
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

#include <memory.h>
#include "CxxToken.h"

CxxToken::CxxToken() {
}


CxxToken::CxxToken(const CxxToken&) {
}


CxxToken::CxxToken(int tokenValue , var_type type , const char* name, int leng) {
	_value = tokenValue;
	_type = type;
	if (name == NULL) {
		_name = NULL;
		_leng = -1;
	}
	else {
		if (leng == -1) {
			_leng=strlen(name);
		}
		else {
			_leng = leng;
		}
		_name = (char*)malloc(_leng+1);
		strncpy(_name, name, _leng);
		_name[_leng] = '\0';
	}
	_scope = NULL;
}


CxxToken::~CxxToken()  {
	free(_name);
	free(_scope);
}


int CxxToken::value() const {
	return _value;
}


void CxxToken::set_value(int val) {
	_value=val;
}


enum var_type CxxToken::type() const {
	return _type;
}


void CxxToken::set_type(enum var_type type) {
	_type = type;
}


bool CxxToken::is_name(const char *name) {
	bool ret;
	ret = (strcmp(name,_name) == 0);
	return ret;
}


char * CxxToken::name() const {
	return _name;
}


void CxxToken::set_name(const char *name) {
	if (_name != 0) {
		free(_name);
	}
	_leng = strlen(name);
	_name = (char*)malloc(_leng+1);
	strcpy(_name, name);
}


char * CxxToken::scope() const {
	return _scope;
}


void CxxToken::set_scope(const char *scope) {
	if (_scope != NULL) {
		char *_new_scope = (char*)malloc(strlen(_scope)+ strlen(scope)+1);
		strcpy(_new_scope, scope);
		strcat(_new_scope, _scope);
		free(_scope);
		_scope = _new_scope;
	}
	else {
		_scope = (char*)malloc(strlen(scope)+1);
		strcpy(_scope, scope);
	}
}

