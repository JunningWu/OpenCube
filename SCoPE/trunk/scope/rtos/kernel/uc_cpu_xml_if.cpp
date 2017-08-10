//==========================================================================
//	uc_cpu_xml_if.cpp
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
 * \file uc_cpu_xml_if.cpp
 */

#include <stdio.h>
#include "rtos/kernel/uc_cpu_xml_if.h"
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_icache.h"
#include "rtos/kernel/uc_dcache.h"

/*!
 * \fn uc_cpu_xml_if_class
 * \brief Constructor. Receives the processors file
 * \param *fileName The name of the xml file with cpu's descriptions
 */
uc_cpu_xml_if_class::uc_cpu_xml_if_class(const char *cpu_name) {
	const char *aux = getenv("SCOPE_HOME");
	if (aux == NULL) {
		fprintf(stderr, "\n#ERROR: SCOPE_HOME not set!\n\n");
		exit(1);
	}
	m_cpu_name = cpu_name;
	m_cpu_fileName = aux;
	m_cpu_fileName += "/config/processors.xml";

	if (m_doc.LoadFile(m_cpu_fileName.c_str()) == false) {
		cerr << "#ERROR: cannot read processors file: " << m_cpu_fileName << endl;
		exit(1);
	}
	m_root = m_doc.RootElement();
	//Search the current cpu:
	m_cpu = m_root->FirstChildElement();
	while (m_cpu != NULL) {
		if (strcmp(m_cpu->Attribute("type"), m_cpu_name.c_str()) == 0) {
			break;
		}
		m_cpu = m_cpu->NextSiblingElement();
	}
	if (m_cpu == NULL) {
		cerr << "#ERROR: Processor " << m_cpu_name << " not found in " << m_cpu_fileName << endl;
		exit(1);
	}
}


/*!
 * \fn ~uc_cpu_xml_if_class
 * \brief Destructor
 */
uc_cpu_xml_if_class::~uc_cpu_xml_if_class() {
}


int uc_cpu_xml_if_class::load_cpu(UC_cpu_class *cpu) {
	int ret = 0;
	TiXmlElement *l1Elem;
	const char *l1Tag;
	char *data;
	float mips, cpi;
	float instr_energy;
	float IC_access_energy, IC_fetch_energy;
	long long clock;
	data = (char *)malloc(2048);
	//Ok. By constructor, we know that m_cpu is the xml element that points to current cpu
	//We read its parameters: time, instructions, caches, etc.
	l1Elem = m_cpu->FirstChildElement();
	while (l1Elem != NULL) {
		l1Tag = l1Elem->Value();
		if (strcmp(l1Tag, "common") == 0) {
			//Clock:
			strcpy(data, l1Elem->Attribute("clock"));
			clock = (long long)atol(data);
			if (clock <= 0) {
				std::cerr << "ERROR: CPU Clock must be greater than zero!" << std::endl;
				exit(-1);
			}
			//MIPS:
			strcpy(data, l1Elem->Attribute("mips"));
			mips = (float)atof(data);
			if (mips < 0.0) {
				std::cerr << "ERROR: CPU MIPS must be greater than zero!" << std::endl;
				exit(-1);
			}
			//Energy per instruction:
			strcpy(data, l1Elem->Attribute("instrEnergy"));
			instr_energy = (float)atof(data);
			if (instr_energy < 0.0) {
				std::cerr << "ERROR: CPU energy cannot be negative!" << std::endl;
				exit(-1);
			}
			//Energy per cache access:
			strcpy(data, l1Elem->Attribute("ICAccessEnergy"));
			IC_access_energy = (float)atof(data);
			if (IC_access_energy < 0.0) {
				std::cerr << "ERROR: Cache energy cannot be negative!" << std::endl;
				exit(-1);
			}
			//Energy per cache miss:
			strcpy(data, l1Elem->Attribute("ICFetchEnergy"));
			IC_fetch_energy = (float)atof(data);
			if (IC_fetch_energy < 0.0) {
				std::cerr << "ERROR: Cache energy cannot be negative!" << std::endl;
				exit(-1);
			}

			//Calculate useful parameters:
			cpi = clock / mips;

			// FIXME: Apply units properly!!!
			clock *= (long long)1e6;
			
			//Update parameters to uc_cpu_class:
			cpu->m_parsed_clock = clock;
			cpu->m_cpi = cpi;
			cpu->m_instr_energy = instr_energy;
		}
		else if (strcmp(l1Tag, "caches") == 0) {
// 			load_cache(l1Elem, &(cpu->m_icache), cpu);
			load_cache(l1Elem, &(cpu->m_icache), &(cpu->m_dcache), cpu);
		}
		l1Elem = l1Elem->NextSiblingElement();
	}
	free(data);
	return ret;
}


/*!
 * \fn uc_cpu_xml_if_class::load_cache(TiXmlElement *cacheElem, UC_icache_class *icache)
 * \brief Load the cache parameters from processors.xml file and builds the caches
 * \param *cacheElem The xml element that contains the cache parameters
 * \param **icache Address of pointer to instruction cache
 * FIXME: data cache must be implemented. Must check what happend if cpu has no cache
 */
// int uc_cpu_xml_if_class::load_cache(TiXmlElement *cacheElem, UC_icache_class **icache_p, UC_cpu_class *cpu) {
int uc_cpu_xml_if_class::load_cache(TiXmlElement *cacheElem, UC_icache_class **icache_p, UC_dcache_class **dcache_p, UC_cpu_class *cpu) {
	int ret = 0;
	TiXmlElement *typeElem;
	const char *typeTag;
	int ic_size, ic_assoc, ic_line_size, ic_instr_size, dc_size, dc_assoc, dc_line_size, dc_instr_size;
// 	UC_dcache_class **dcache_p;
	typeElem = cacheElem->FirstChildElement();
	while (typeElem != NULL) {
		typeTag = typeElem->Value();
		if (strcmp(typeTag, "icache") == 0) {
			ic_size = atoi(typeElem->Attribute("size"));
			ic_assoc = atoi(typeElem->Attribute("associativity"));
			ic_line_size = atoi(typeElem->Attribute("line_size"));
			ic_instr_size = atoi(typeElem->Attribute("instr_size"));
			//Initializes the Instruction Cache:
			(*icache_p) = new UC_icache_class(ic_size, ic_assoc, ic_line_size, ic_instr_size, cpu);
		}
		else if (strcmp(typeTag, "dcache") == 0) {
			dc_size = atoi(typeElem->Attribute("size"));
			dc_assoc = atoi(typeElem->Attribute("associativity"));
			dc_line_size = atoi(typeElem->Attribute("line_size"));
			dc_instr_size = atoi(typeElem->Attribute("instr_size"));
			//printf("Data cache\n");
			//Initializes the data cache:
			(*dcache_p) = new UC_dcache_class(dc_size, dc_assoc, dc_line_size, dc_instr_size, cpu);
		}
		else {
			fprintf(stderr, "\n#ERROR: Unknown cache type: %s\n", typeTag);
			fprintf(stderr, "Accepted types: icache, dcache\n\n");
			exit(-1);
		}
		typeElem = typeElem->NextSiblingElement();
	}
	return ret;
}


