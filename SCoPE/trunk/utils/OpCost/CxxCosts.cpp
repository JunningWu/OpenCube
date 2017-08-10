//==========================================================================
//	CxxCosts.cxx
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
 * \file CxxCosts.cxx
 * \brief Static Parser for execution time estimation and cache modeling
 */

#include <iostream>
#include "fstream"
#include "operators.h"
#include "CxxCosts.h"
#include "types.h"
#include "tinyxml.h"
using namespace std;

extern parsing_struct_t parsing_struct;
extern parsing_options_t parsing_options;
extern asm_sentences_t asm_sentences;
extern char for_mark[];
proc_data_costs_t proc_data_costs;  //!< Global structure with processor parameters


/*!
 * \fn load_default_processor(void)
 * \brief Load default costs when processor is not specified
 */
void load_default_processor(void) {
	parsing_struct.asm_sentence = asm_sentences.asm_arm; //FIXME
	parsing_struct.for_mark = for_mark;
}


/*!
 * \fn load_processor(char *proc_type)
 * \brief Load the operators costs from the specified processor
 * \param proc_type The name of the selected processor
 */
void load_processor(char *proc_type) {
	FILE *fd, *fd_strout;
	char *file;
	TiXmlDocument hDoc;
	TiXmlElement *root;
	TiXmlElement *procElem;
	TiXmlNode *l1Node;
	TiXmlElement *l1Elem;
	const char *l1Tag;
	char *data; //Stores the cost of the operator just before convert from string to int
	double clock, mips, cpi;
	
	// Check if default processor:
	if (strcmp(proc_type, "default") == 0) {
		load_default_processor();
		return;
	}

	//Open the default collection of processors from file:
	file = (char *)malloc(256);
	data = (char *)malloc(64);
	strcpy(file, parsing_options.compiler_path);
	strcat(file, "/config/processors.xml");
	fd_strout = fopen("/dev/stdout", "r");
	fd = fopen(file, "r");
	if (fd == NULL) {
		fprintf(stderr, "\n#ERROR: cannot open %s\n\n", file);
		exit(-1);
	}
	if (hDoc.LoadFile(file) == false) {
		fprintf(stderr, "\n#ERROR: cannot read %s\n\n", file);
		exit(-1);
	}
	root = hDoc.RootElement();
	//Search the selected processor:
	procElem = root->FirstChildElement();
	while (procElem != NULL) {
		if (strcmp(procElem->Attribute("type"), proc_type) == 0) {
			break;
		}
		procElem = procElem->NextSiblingElement();
	}
	if (procElem == NULL) {
		fprintf(stderr, "\n#ERROR: Processor %s not found in processors.xml file!\n\n", proc_type);
		exit(-1);
// 		load_default_processor();
// 		return;
	}
	//We have in procElem the xml element corresponding to the selected processor.
	//We read its parameters: time, instructions, caches, etc.
	cpi = 1.0; //By default
	l1Node = procElem->FirstChild();
	while (l1Node != NULL) {
		l1Elem = l1Node->ToElement();
		l1Tag = l1Elem->Value();
		if (strcmp(l1Tag, "common") == 0) {
			strcpy(data, l1Elem->Attribute("mips"));
			mips = atof(data);
			if (mips <= 0.0) {
				std::cerr << "ERROR: MIPS must be greater than zero!" << std::endl;
				exit(-1);
			}
			strcpy(data, l1Elem->Attribute("clock"));
			clock = atof(data);
			if (clock <= 0.0) {
				std::cerr << "ERROR: Clock must be greater than zero!" << std::endl;
				exit(-1);
			}
			cpi = clock / mips;
		}
		else if (strcmp(l1Tag, "time") == 0) {
			load_time_params(l1Node);
		}
		else if (strcmp(l1Tag, "instructions") == 0) {
			load_instr_params(l1Node);
		}
		else if (strcmp(l1Tag, "caches") == 0) {
			load_cache_params(l1Node);
		}
		l1Node = l1Node->NextSibling();
	}

	parsing_struct.asm_sentence = asm_sentences.asm_arm; //FIXME
	parsing_struct.for_mark = for_mark;
	free(file);
	free(data);
	fclose(fd);
	fclose(fd_strout);
}


/*!
 * \fn load_time_params(TiXmlNode *timeNode)
 * \brief Load the times of the processor from the xml node and stores them into a global structure
 * \param timeNode The xml where temporal data are stored
 */
void load_time_params(TiXmlNode *timeNode) {
	TiXmlNode *typeNode, *weightsNode;
	const char *typeTag;
	const char *timeInt, *timeFloat, *timeChar, *timeBool, *timeAccess, *timeControl;	//Time weights
	char *data; //Stores the cost of the operator just before convert from string to int
	data = (char *)malloc(64);

	typeNode = timeNode->FirstChild();
	while (typeNode != NULL) {
		typeTag = typeNode->Value();
		if (strcmp(typeTag, "int") == 0) {
			weightsNode = typeNode->FirstChild();
			timeInt = weightsNode->ToText()->Text();
			//Get all times for int operations into an array:
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (timeInt[j] != ' ' && timeInt[j] != '\0') {
					data[k] = timeInt[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.time_data[INT_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "float") == 0) {
			weightsNode = typeNode->FirstChild();
			timeFloat = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (timeFloat[j] != ' ' && timeFloat[j] != '\0') {
					data[k] = timeFloat[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.time_data[FLOAT_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "char") == 0) {
			weightsNode = typeNode->FirstChild();
			timeChar = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (timeChar[j] != ' ' && timeChar[j] != '\0') {
					data[k] = timeChar[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.time_data[CHAR_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "bool") == 0) {
			weightsNode = typeNode->FirstChild();
			timeBool = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (timeBool[j] != ' ' && timeBool[j] != '\0') {
					data[k] = timeBool[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.time_data[BOOL_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "access") == 0) {
			weightsNode = typeNode->FirstChild();
			timeAccess = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_ACCESS_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (timeAccess[j] != ' ' && timeAccess[j] != '\0') {
					data[k] = timeAccess[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.time_access[i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "control") == 0) {
			weightsNode = typeNode->FirstChild();
			timeControl = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_CONTROL_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (timeControl[j] != ' ' && timeControl[j] != '\0') {
					data[k] = timeControl[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.time_control[i] = atoi(data);
			}
		}
		else {
			fprintf(stderr, "Error: Unknown element: %s\n", typeTag);
		}
		typeNode = typeNode->NextSibling();
	}
	free(data);
}


/*!
 * \fn load_instr_params(TiXmlNode *instrNode)
 * \brief Load the num of instructions of the processor from the xml node and stores them into a global structure
 * \param instrNode The xml where instructions data are stored
 */
void load_instr_params(TiXmlNode *instrNode) {
	TiXmlNode *typeNode, *weightsNode;
	const char *typeTag;
	const char *instrInt, *instrFloat, *instrChar, *instrBool, *instrAccess, *instrControl;	//Time weights
	char *data; //Stores the cost of the operator just before convert from string to int
	data = (char *)malloc(64);
	typeNode = instrNode->FirstChildElement();
	while (typeNode != NULL) {
		typeTag = typeNode->Value();
		if (strcmp(typeTag, "int") == 0) {
			weightsNode = typeNode->FirstChild();
			instrInt = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (instrInt[j] != ' ' && instrInt[j] != '\0') {
					data[k] = instrInt[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.instr_data[INT_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "float") == 0) {
			weightsNode = typeNode->FirstChild();
			instrFloat = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (instrFloat[j] != ' ' && instrFloat[j] != '\0') {
					data[k] = instrFloat[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.instr_data[FLOAT_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "char") == 0) {
			weightsNode = typeNode->FirstChild();
			instrChar = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (instrChar[j] != ' ' && instrChar[j] != '\0') {
					data[k] = instrChar[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.instr_data[CHAR_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "bool") == 0) {
			weightsNode = typeNode->FirstChild();
			instrBool = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_DATA_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (instrBool[j] != ' ' && instrBool[j] != '\0') {
					data[k] = instrBool[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.instr_data[BOOL_TYPE_OP][i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "access") == 0) {
			weightsNode = typeNode->FirstChild();
			instrAccess = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_ACCESS_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (instrAccess[j] != ' ' && instrAccess[j] != '\0') {
					data[k] = instrAccess[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.instr_access[i] = atoi(data);
			}
		}
		else if (strcmp(typeTag, "control") == 0) {
			weightsNode = typeNode->FirstChild();
			instrControl = weightsNode->ToText()->Text();
			for (int i=0, j=0, k=0 ; i<TOTAL_CONTROL_OP ; i++, j++, k=0) {
				data[0] = '\0'; //Clear previous value
				while (instrControl[j] != ' ' && instrControl[j] != '\0') {
					data[k] = instrControl[j];
					j++;
					k++;
				}
				data[k] = '\0';
				proc_data_costs.instr_control[i] = atoi(data);
			}
		}
		else {
			fprintf(stderr, "Error: Unknown element: %s\n", typeTag);
		}
		typeNode = typeNode->NextSibling();
	}
	free(data);
}


/*!
 * \fn load_cache_params(TiXmlNode *cacheNode)
 * \brief Load the cache caracteristics from the xml node
 * \param cacheNode The xml node where cache caracteristicas are stored
 */
void load_cache_params(TiXmlNode *cacheNode) {
	TiXmlNode *typeNode;
	const char *typeTag;
	typeNode = cacheNode->FirstChild();
	while (typeNode != NULL) {
		typeTag = typeNode->Value();
		if (strcmp(typeTag, "icache") == 0) {
			if(parsing_options.instruction_cache == false && parsing_options.annotate_time == true){
				
				fprintf(stderr,"\n***********************************************************************\n");
				fprintf(stderr,"***** Warning: Instruction cache disabled for a cached processor  *****");
				fprintf(stderr,"\n***********************************************************************\n\n");
			}
//  			printf("Instruction cache\n");
		}
		else if (strcmp(typeTag, "dcache") == 0) {
//  			printf("Data cache\n");
			//TODO:LUMY:Igual aqui hay que meter algo
		}
		else {
			fprintf(stderr, "Error: Unknown cache type: %s\n", typeTag);
		}
		typeNode = typeNode->NextSibling();
	}
}


int get_data_op_time(int op, int type) {
	if (op >= TOTAL_DATA_OP) { fprintf(stderr, "Error: %d exceeds range of valid operators", op); return 0; }
	if (type >= TOTAL_DATA_TYPES) { fprintf(stderr, "Error: %d exceeds range of valid data types", type); return 0; }
	return proc_data_costs.time_data[type][op];
}


int get_access_op_time(int op) {
	if (op >= TOTAL_DATA_OP) { fprintf(stderr, "Error: %d exceeds range of valid operators", op); return 0; }
	return proc_data_costs.time_access[op];
}


int get_control_op_time(int op) {
	if (op >= TOTAL_DATA_OP) { fprintf(stderr, "Error: %d exceeds range of valid operators", op); return 0; }
	return proc_data_costs.time_control[op];
}


int get_data_op_instr(int op, int type) {
	if (op >= TOTAL_DATA_OP) { fprintf(stderr, "Error: %d exceeds range of valid operators", op); return 0; }
	if (type >= TOTAL_DATA_TYPES) { fprintf(stderr, "Error: %d exceeds range of valid data types", type); return 0; }
	return proc_data_costs.instr_data[type][op];
}


int get_access_op_instr(int op) {
	if (op >= TOTAL_DATA_OP) { fprintf(stderr, "Error: %d exceeds range of valid operators", op); return 0; }
	return proc_data_costs.instr_access[op];
}


int get_control_op_instr(int op) {
	if (op >= TOTAL_DATA_OP) { fprintf(stderr, "Error: %d exceeds range of valid operators", op); return 0; }
	return proc_data_costs.instr_control[op];
}
