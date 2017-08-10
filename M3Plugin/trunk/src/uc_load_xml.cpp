//==========================================================================
//
// Author(s): Hector Posadas
// Contributors: Gerardo de Miguel González
// Description:   Main file of the plug-in. Contain the sc_main and the functions to manage the XML files, ...
//
// Modifications:
// $19-09-2008 - Add global metrics variables
// $22-09-2008 - Implement open_metrics_output_file
// $23-09-2008 - Implement load_metric_definition
// $27-10-2008 - Parser, Handler, Metric object and std::container (Queue)
// $28-10-2008 - Parameter Object and std::container (List)
// $29-10-2008 - libmiexpat integration (from expat parser source)
// $07-11-2008 - libmiexpat (expat + expatmm class wrapper)
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
//  http://www.teisa.unican.essc_main/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================

//::GMG::SCoPe Library
#include "sc_scope.h"
//::GMG::XmlReader (includes Expat Parser)
#include "xml_main.h"
//::GMG::ArchitectureReader
#include "uc_load_xml.h"
//::GMG::PlatformBuilder
#include "uc_create_xml_platform.h"

#include "xml_hierarchy.h"

#include "io_access/io_access.h"

#include "../include/xml_plugin_functions.h"

//::GMG::SCoPe globals
UC_gui_connector *gui_connector;
xml_scope_simulation_info_t xml_scope_simulation_info(-2, SC_NS, 0);
char *executable_name;

//::GMG:: Variables for the extraction of metrics from SCoPe
vector<UC_rtos_class *> rtos_list;
vector<UC_NoC_Interface *> simulator_list;

/**
 * \brief SystemC main function. It decodes the input arguments, reads the xml files, builds the system model, and starts the simulation
 *
 * \param argc Number of arguments received from the terminal
 * \param argv Arguments received from the terminal
 * \return Program result
 */

//#ifndef NO_MAIN
int __attribute__ ((weak)) sc_main(int argc, char **argv){
	int status = uc_xml_init_plugin(argc, argv);
	if(status == -1) return -1;
	
	if(status == 2){
		cout << "\nNo simulation time limit found. Running sc_start (-1)" << endl;
		sc_start(-1);

		//::GMG::Report end of simulaton and simulation time
		cout << "\nMain finish" << endl;
		cout << "Simulated time: " << sc_time_stamp() << "\n\n" << endl;
		
	}

	return uc_xml_close_plugin();
}
//#endif
static char *xml_file=NULL;
static char *xml_config=NULL;
static char *xml_metrics=NULL;
static MetricReader *mr;

int uc_xml_init_plugin(int argc, char **argv){

//::GMG:: Loop counter and file name holders
  int i;

//::GMG::Program command-line parsing
        executable_name = argv[0];
	for(i = 0;i < argc; i++){
		if(strcmp(argv[i],"-xml") == 0 ){
			xml_file = (char*)malloc(strlen(argv[i+1])+1);
			strcpy(xml_file,argv[i+1]);
			i++;
		}
		else if(strcmp(argv[i],"-bt") == 0 ){
			uc_activate_backtrace(3);
		}
		else if( strncmp(argv[i],"--xml-system-description=",25) == 0 || strncmp(argv[i],"--xml_system_description=",25) == 0 ){ // xml platform description file
			char *ptr = argv[i] + 25;
			xml_file = (char*)malloc(strlen(ptr)+1);
			strcpy(xml_file,ptr);
		}
		else if(strcmp(argv[i],"-xsc") == 0 ){
			xml_config = (char*)malloc(strlen(argv[i+1])+1);
			strcpy(xml_config,argv[i+1]);
			i++;
		}
		else if(strncmp(argv[i],"--xml-system-configuration=",27) == 0 || strncmp(argv[i],"--xml_system_configuration=",27) == 0){ // xml system configuration file
			char *ptr = argv[i] + 27;
			xml_config = (char*)malloc(strlen(ptr)+1);
			strcpy(xml_config,ptr);
		}
		else if(strcmp(argv[i],"-xmd") == 0 ) {
			xml_metrics = (char*)malloc(strlen(argv[i+1])+1);
			strcpy(xml_metrics,argv[i+1]);
			i++;
		}
		else if(strncmp(argv[i],"--xml-metric-definition=",24) == 0 || strncmp(argv[i],"--xml_metric_definition=",24) == 0){ // xml metrics definition file
			char *ptr = argv[i] + 24;
			xml_metrics = (char*)malloc(strlen(ptr)+1);
			strcpy(xml_metrics,ptr);
		}
		else if(strcmp(argv[i],"-xof") == 0 ){
			xml_scope_simulation_info.m_xml_output = (char*)malloc(strlen(argv[i+1])+1);
			strcpy(xml_scope_simulation_info.m_xml_output,argv[i+1]);
			i++;
		}
		else if(strncmp(argv[i],"--xml-system-metrics=",21) == 0 || strncmp(argv[i],"--xml_system_metrics=",21) == 0){ // xml output file
			char *ptr = argv[i] + 21;
			xml_scope_simulation_info.m_xml_output = (char*)malloc(strlen(ptr)+1);
			strcpy(xml_scope_simulation_info.m_xml_output,ptr);
			i++;
		}
		else if(strcmp(argv[i],"--help") == 0 || strcmp(argv[i],"-h") == 0){ // xml output file
		        usage(argv[0]);
			return 0;
		}
	}

  //::GMG::MetricReader(Writer) reads the metrics from XML DSE file
  mr = new MetricReader(xml_metrics);
  if( xml_metrics != NULL ) {
     if (mr->Ready()) {
         mr->Parse();
     } else {
         mr->writeXMLErrorFile (xml_scope_simulation_info.m_xml_output, "Parser Error", "fatal");
         return -1;
     }
  } else {
    cerr << "XML Metric Definition file not found. \n"/*
         << "Please, use '-xmd file_name' or use '--help' "
         << "to get more information" */<< endl;
    /*mr->writeXMLErrorFile ("output.xml", "Metric Definition file not found", "fatal");
    return 1;*/
  }
  //::GMG::NOTE:: Here we've got the metrics we must get from simulation
  //       We could use this information to simplify the simulation

  //::GMG::ParameterReader reads the parameters from a input m3 file
  if(xml_config !=NULL ){
     load_xml_configuration_file(xml_config);
  }
else {
    cerr << "XML System Configuration file not found. \n"/*
         << "Please, use '-xsc file_name' or use '--help' "
         << "to get more information" */
	 << endl;
/*    mr->writeXMLErrorFile ("output.xml", "System Configuration file not found", "fatal");
    return 1;*/

  }
  //::GMG::NOTE:: Here we've got the parameters from the design exploration
  //       tool that should match with the parameters from DSE XML file

  //::GMG::Custom ArchitectureReader and Builder
  if(xml_file !=NULL ){
     load_xml_platform_file(xml_file);
  } else {
    cerr << "XML System Description file not found. \n"
	 << "Please, use '-xml file_name' or use '--help' "
	 << "to get more information" << endl;
    mr->writeXMLErrorFile (xml_scope_simulation_info.m_xml_output, "System Description file not found", "fatal");
    return -1;
  }

  //::GMG::SCoPe Simulation
  if(xml_scope_simulation_info.m_simulation_time == -2){
	return 2;
  }
  if(xml_scope_simulation_info.m_simulation_time > 0) {
     sc_time total_simulation_time (xml_scope_simulation_info.m_simulation_time,
                                    xml_scope_simulation_info.m_sim_time_unit);
     cout << "Maximum simulation time: "
          << total_simulation_time
          << "\n\n" <<endl;
     if(xml_scope_simulation_info.m_debug_level == 1) {
        gui_connector->set_total_time(total_simulation_time);
     }
     sc_start(total_simulation_time);
  } else {
     cout << "\nNo simulation time limit found. Running sc_start (-1)"
          << endl;
     sc_start(-1);
  }

  //::GMG::Report end of simulaton and simulation time
  cout << "\nMain finish"
       << endl;
  cout << "Simulated time: "
       << sc_time_stamp()
       << "\n\n"
       << endl;
  return 1;
}

int uc_xml_close_plugin(){

  //::GMG::Get the metrics we need from simulation results
  obtain_xml_metrics(*mr);

  //::GMG::Write the output XML file with the simulation results (metrics)
  if( xml_scope_simulation_info.m_xml_output != NULL && xml_metrics != NULL) {
     if (!mr->writeXMLFile(xml_scope_simulation_info.m_xml_output)) {
       cerr << "writeXMLFile::error writing output file: "
            << xml_scope_simulation_info.m_xml_output << endl;
       mr->writeXMLErrorFile ( xml_scope_simulation_info.m_xml_output, "Error writing output file", "fatal");
       destroy_objects();
       //return 1;
     }
  } else {
     cerr << "XML Output file not created. \n"/*
          << "Please, use '-xof file_name' or use '--help' "
          << "to get more information"*/ << endl;/*
     cerr << "Default to output.xml" << endl;
     if (!mr->writeXMLFile("output.xml")) {
       cerr << "writeXMLFile::error writing output file: "
            << xml_output << endl;
       mr->writeXMLErrorFile ("output.xml", "Error writing output file", "fatal");
       destroy_objects();
       return 1;
     }*/
  }

  //::GMG::Destroy simulation objects and print out the results on screen
  destroy_objects();

  //::GMG::We end up successfully
  return 0;
}

/**
 * \brief Destroy the dynamically allocated simulation objects and print out the results on screen
 *
 */
void destroy_objects() {
  //::GMG::We destroy the objects we had used for simulation
  //       and as a side effect the results get printed out on screen
//   for (int i = 0; i < rtos_list.size(); i++) {
//        delete rtos_list[i];
//   }

//  for (int i = uc_simulation_config.rtos_list->size()-1; i >=0 ; i--) {	

#ifndef NO_XML_PLATFORM
  for (int i = 0; i<uc_simulation_config.rtos_list->size(); i++) {
	UC_rtos_class *rtos = (*uc_simulation_config.rtos_list)[i];
	(*uc_simulation_config.rtos_list)[i]=NULL;
	delete rtos;
  }

  for (int i = 0; i < simulator_list.size(); i++) {
       simulator_list[i]->end_simulation();
  }
  cout << endl;
  //::GMG::NOTE::We should be able to switch off the result's printing!
#endif
}

/**
 * \brief Prints out the program command-line help
 *
 */
void usage (char *programa) {
  cout << "\nUsage: " << programa << " <option(s)> \n";
  cout << "Simulate and analyze the performance of a SystemC "
       << "system description\n";
  cout << "Options are:\n";
  cout << "\t-xml file_name\n\t--xml-system-desription=file_name     "
       << "File with the platform and allocation information\n\n";
  cout << "\t-xsc file_name\n\t--xml-system-configuration=file_name  "
       << "File with the system configuration parameters\n\n";
  cout << "\t-xmd file_name\n\t--xml-metric-definition=file_name     "
       << "File with the definition of the metrics to be reported\n\n";
  cout << "\t-xof file_name\n\t--xml-system-metrics=file_name        "
       << "File name for the output metric report\n\n";
}

/**
 * \brief Execute all the functions required to read the XML System Description file and create the system model
 *
 * \param file Name of the XML System Description file
 *
 */
void load_xml_platform_file(char * file){

	// Read the xml file
	read_xml_file(file);

	// Create SW instances
	instanciate_sw_components();

	// Create HW instances
	instanciate_hw_components();

	// Create HW connections
	connect_hw_components();

	// Create SW tasks
	allocate_sw_tasks();

}

/**
 * \brief Load the XML System Configuration file
 *
 * \param file Name of the XML System Configuration file
 *
 */
void load_xml_configuration_file(char *xml_file){

        read_xml_file(xml_file);

}

