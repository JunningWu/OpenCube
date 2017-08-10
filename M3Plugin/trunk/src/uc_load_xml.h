//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Header file
//
// Modifications:
//
//==========================================================================
//
//  The following code is derived, directly or indirectly, from XML plug-in for SCoPE,
//  released June 30, 2008. Copyright (C) 2008 Univeristy of Cantabria(UC)
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
//  For more information about SCoPE and the XML plug-in for SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================
#ifndef _UC_LOAD_XML_H_
#define _UC_LOAD_XML_H_

//::GMG::SCoPe Library
#include "sc_scope.h"
//::GMG::Std::Vector container
#include <vector>
//::GMG::MetricReader(Writer)
//       (includes Parser, Handler(s), Metric object
//       and its std::container(s) Queue)
#include "expat.h"
#include "expatmm.h"
#include "metric.h"
#include "parse.h"

using namespace std;

//::GMG::Basic SCoPe simulation data structure
class xml_scope_simulation_info_t{
public:
	long long m_simulation_time;
	enum sc_time_unit m_sim_time_unit;
	int m_debug_level;
	int m_warnings;
	int m_report_generated;
	char *m_xml_output;


	xml_scope_simulation_info_t(long long time, enum sc_time_unit unit, int debug = 0, int warnings = 0){
		m_simulation_time = time;
		m_sim_time_unit = unit;
		m_debug_level = debug;
		m_warnings = warnings;
		m_xml_output = NULL;
		m_report_generated = 0;
	}
	~xml_scope_simulation_info_t(){
		if(m_report_generated == 0){
			FILE *f=fopen (m_xml_output,"w");
			fprintf(f,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			fprintf(f,"<simulator_output_interface xmlns=\"http://www.multicube.eu/\" version=\"1.3\">\n");
			if(uc_simulation_config.m_sigint>0){
				fprintf(f,"\t<error reason=\"sigint received\" kind=\"fatal\"/>\n");
			}
			else{
				fprintf(f,"\t<error reason=\"abnormal termination\" kind=\"non-fatal\"/>\n");
			}
			fprintf(f,"</simulator_output_interface>");
			fclose(f);
		}
	}
	
};

//::GMG::SCoPe simulation variables
extern xml_scope_simulation_info_t xml_scope_simulation_info;
extern vector<UC_rtos_class *> rtos_list;
extern vector<UC_NoC_Interface *> simulator_list;
extern UC_gui_connector *gui_connector;

//::GMG::SystemC main entry function
int sc_main(int argc, char **argv);

//::GMG::Utlity functions
void usage(char *program);
void load_xml_platform_file(char *xml_config);
void load_xml_configuration_file(char *xml_file);
// void load_xml_metric_definition(char *xml_metrics);
// void open_metrics_output_file(char *xml_output);
int obtain_xml_metrics(MetricReader &mr);
void destroy_objects();

#endif
