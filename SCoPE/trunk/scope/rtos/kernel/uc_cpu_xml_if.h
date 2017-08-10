//==========================================================================
//	uc_cpu_xml_if.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie ago 8 2008
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
/*!
 * \file uc_cpu_xml_if.h
 */
#ifndef UC_CPU_XML_IH_H
#define UC_CPU_XML_IH_H

#include <string>
#include "tinyxml.h"
using namespace std;

class UC_cpu_class;
class UC_icache_class;
class UC_dcache_class;

/*!
 * \class uc_cpu_xml_if_class
 * \brief Class to get cpu patameters from xml file
 */
class uc_cpu_xml_if_class {
public:
	uc_cpu_xml_if_class(const char *cpu_name);
	~uc_cpu_xml_if_class();

	int load_cpu(UC_cpu_class *cpu);

private:
// 	int load_cache(TiXmlElement *cacheElem, UC_icache_class **icache_p, UC_cpu_class *cpu);
	int load_cache(TiXmlElement *cacheElem, UC_icache_class **icache_p, UC_dcache_class **dcache_p, UC_cpu_class *cpu);

	string m_cpu_name;			//!< Name of the cpu for current project
	string m_cpu_fileName;		//!< Xml file with processor description (default: $SCOPE_HOME/config/processors.xml)
	TiXmlDocument m_doc;		//!< Xml document of processors collection
	TiXmlElement *m_root;		//!< Root node of processors xml tree
	TiXmlElement *m_cpu;		//!< The current cpu Xml node
};

#endif
