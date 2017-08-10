//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Functions to read the XML file with the System Destription
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

#include "xml_main.h"
#include "xml_configuration_file.h"
//#include "xml_metric_definition.h"

#include "xml_hierarchy.h"
#include "xml_if.h"

#include <stdio.h>
#include <string.h>

#include "expat.h"
//#include "xml_reader.h"

int xml_info_level = 0;
int xml_has_platform_description = 0;


/**
 * \brief Function to be called to manage a new xml clause
 *
 * \param userData Pointer to the user buffer
 * \param name Name of the clause
 * \param atts Arguments of the clause
 * \return
 */

static void XMLCALL
startElement(void *userData, const char *name, const char **atts)
{
	if(xml_info_level == 1){
		xml_add_parameter((char*)userData,name, atts);
	}
//	else if(xml_info_level == 3){
//		xml_add_metric((char*)userData,name, atts);
//	}

	if(strcasecmp_null(name,"Description"))	{
		printf("Loading platform description.\n");
		xml_info_level = 2;
		xml_has_platform_description = 1;
	}
	else if(strcasecmp_null(name,"simulator_input_interface")){
		printf("Loading configuration parameters.\n");
		xml_info_level = 1;
	}
//	else if(strcasecmp_null(name,"system_metrics")){
//		printf("Configuring metric extraction.\n");
//		xml_info_level = 3;
//	}
	if(xml_info_level == 2){
		xml_add_hierarchy((char*)userData,name, atts);
	}
}

/**
 * \brief Function to close a xml clause
 *
 * \param userData Pointer to the user buffer
 * \param name Name of the clause
 * \return
 */
static void XMLCALL
endElement(void *userData, const char *name)
{
	if(xml_info_level == 2){
		xml_up_hierarchy((char*)userData, name);
	}
	if(strcasecmp_null(name,"Description"))	{
		printf("Platform description loaded.\n");
		xml_info_level = 0;
	}
	else if(strcasecmp_null(name,"simulator_input_interface")){
		printf("Configuration parameters loaded.\n");
		xml_info_level = 0;
	}
//	else if(strcasecmp_null(name,"system_metrics")){
//		printf("Metric configuration finished.\n");
//		xml_info_level = 0;
//	}
	if(xml_info_level == 1){
		xml_end_parameter((char*)userData,name);
	}
//	else if(xml_info_level == 3){
//		xml_end_metric((char*)userData,name);
//	}
}

/**
 * \brief Function read the XML System Description file
 *
 * \param file Name of the file
 * \return 0 on success, otherwise error
 */
int
read_xml_file(char *file)
{
 char buf[BUFSIZ];
 XML_Parser parser = XML_ParserCreate(NULL);
 int done;
 char data[1000]="";
 XML_SetUserData(parser, data);
 XML_SetElementHandler(parser, startElement, endElement);

 FILE *fichero;
 fichero = fopen( file, "r" );
 printf( "File: %s -> ", file );
 if( fichero )
      printf( "(OPENED)\n" );
 else
 {
      printf( "Error \n" );
      return 1;
 }


 do {
   int len = (int)fread(buf, 1, sizeof(buf), fichero);
   done = len < sizeof(buf);
   if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
     fprintf(stderr,
//              "%s at line %" XML_FMT_INT_MOD "u\n",
//              XML_ErrorString(XML_GetErrorCode(parser)),
//              XML_GetCurrentLineNumber(parser));
              "%s at line %d\n",
              XML_ErrorString(XML_GetErrorCode(parser)),
              XML_GetCurrentLineNumber(parser));
     return 1;
   }
 } while (!done);
 XML_ParserFree(parser);

 if(xml_has_platform_description){
	printf("Creating platform.\n");
	xml_display_hierarchy(data);
	printf("Platform created.\n");

 }

 printf( "File: %s -> (LOADED)\n", file );

 return 0;
}
