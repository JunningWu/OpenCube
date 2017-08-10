//==========================================================================
//
// Author(s): Gerardo de Miguel González
// Contributors: Hector Posadas
//
// Description: (Expat + expatmm) MetricReader implementation file
//
// Modifications:
// $12-01-2009 - Modified XML output file version from 1.0 to 1.3
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

#include <cstddef>     // ::GMG::size_t
#include <cstring>     // ::GMG::memset
#include <sys/types.h> // ::GMG::ssize_t
#include <strings.h>   // ::GMG::strcasecmp

#include "metric.h"    // ::GMG::Metric Class
#include "expat.h"     // ::GMG::C XML Parser
#include "expatmm.h"   // ::GMG::C++ XML Parser Wrapper
#include "parse.h"     // ::GMG::Metric Reader and Writer

using namespace expatmm;
using namespace std;

/**
 * \brief MetricReader default constructor
 *
 * \return A MetricReader Object
 * \warning The default constructor is disabled declaring it private
 */
MetricReader::MetricReader() : ExpatXMLParser() {
   setReadiness(false);
}

/**
 * \brief MetricReader constructor (from a XML file source)
 *
 * \param xmlfile XML file name where the MetricReader object is built from
 * \return A MetricReader object
 */
MetricReader::MetricReader(const char* xmlFile) : ExpatXMLParser() {
  //setReadiness(false);
  //if (!readXMLFile (xmlFile))
  //    cerr << "::readXMLFile::opening/reading/parsing file error: "
  //         << xmlFile << endl;
  //cout << "::GMG::DEBUG::Open " << xmlFile << " file" << endl;
  if (!xmlFile)
      setReadiness(false);
  else {
         is.open (xmlFile, ios::binary);
         //::GMG::input stream error checking if we don't use exceptions
         if (is.is_open()) {
             //cout << "::GMG::DEBUG::File " << xmlFile << " opened" << endl;
             is.seekg (0, ios::end);
             length = is.tellg();
             is.seekg (0, ios::beg);
             //cout << "::GMG::DEBUG::File length: " << length << " bytes" << endl;
             setReadiness(true);
         } else {
            setReadiness(false);
            //cout << "::GMG::DEBUG::Cannot open " << xmlFile << " file" << endl;
         }
  }
}

/**
 * \brief Metric default destructor
 *
 */
MetricReader::~MetricReader() {
  //::GMG::empty the stack
  while (!myMetricStack.empty())
     myMetricStack.pop();
  if (is.is_open()) {
      //cout << "::GMG::DEBUG::Close the file" << endl;
      is.close();
  }
}

/**
 * \brief StartElement Handler (SAX Parsing style) which triggers after parsing a starting tag in the XML file
 * The data got from the attribute/value pairs is used to build a Metric object
 * and the Metric object is pushed into a Stack (queued) container
 *
 * \param name constant char array which holds the tag's name which is parsed
 * \param attrs constant Nx2 array which holds the attribute/value pairs within the XML tag which is parsed
 * \return
 */
void MetricReader::StartElement(const XML_Char* name, const XML_Char** attrs) {

  int i;
  Metric metric;

  if(strcasecmp (name,"system_metric") == 0) {
     //::GMG::Get the metric attributes: name, type and unit
     for (i = 0; attrs[i]; i +=2){
         if(strcasecmp (attrs[i],"name") == 0){
            metric.setName(attrs[i+1]);
         }
         else if(strcasecmp (attrs[i],"type") == 0){
            metric.setType(attrs[i+1]);
         }
         else if(strcasecmp (attrs[i],"unit") == 0){
            metric.setUnit(attrs[i+1]);
         }
      }
      //::GMG::push he metric into the stack
      myMetricStack.push(metric);
  }
}

/**
 * \brief EndElement Handler (SAX Parsing style) which triggers after parsing an ending tag in the XML file
 *
 * \param name constant char array which holds the tag's name which is parsed
 * \return
 */
void MetricReader::EndElement(const XML_Char* name) {
}

/**
 * \brief Metric object acccesor
 * Destructive retrieval of a Metric object from the Stack
 *
 * \return A Metric object
 */
Metric MetricReader::getMetric () {
      Metric m = myMetricStack.front();
      myMetricStack.pop();
      return m;
}

/**
 * \brief Metric object setting method
 * Insertion of a Metric object on the Stack
 *
 * \param m the Metric object that is going to be pushed into the Stack
 */
void MetricReader::setMetric (Metric& m) {
      myMetricStack.push(m);
}

/**
 * \brief checks whether there is some Metric object left in the Stack
 *
 * \return A boolean result 'True' if there is some Metric object left
 */
bool MetricReader::isMetricLeft() const {
  return !myMetricStack.empty();
}

/**
 * \brief size of the Stack holding the Metric objects
 *
 * \return the number of Metric objects which are in the Stack
 */
int MetricReader::size () const {
  return myMetricStack.size();
}

/**
 * \brief expatmm (expat C++ wrapper) read_block method implementation
 * Read the XML source (i.e a file) getting blocks of characters from
 * a predefined block size which are then parsed and reported as events
 * (SAX parsing style) which are handled with the start and end tag event handlers
 *
 * \return the number of blocks read or '-1' if there is some error
 */
ssize_t MetricReader::read_block(void) {
  if (!is.is_open()) {
      //cout << "::GMG::DEBUG::read_block:: file is not open" << endl;
      return -1;
  }
  //cout << "::GMG::DEBUG::Reading " << getBlockSize() << " bytes" << endl;
  is.read (getBuffer(), getBlockSize());
  //::GMG::it has reached the end of file
  if (is.eof()) {
      //cout << "::GMG::DEBUG::Reached the end!" << endl;
      setStatus(XML_STATUS_OK);
      setLastError(XML_ERROR_FINISHED);
      return (is.gcount() ? static_cast<ssize_t>(is.gcount()) : -1);
  }
  //::GMG::some error other than reaching the End-Of-File has occurred
  //       during the input operation
  if (is.fail()) {
      //cout << "::GMG::DEBUG::reading operation failed!" << endl;
      setStatus(XML_STATUS_ERROR);
      setLastError(XML_ERROR_NO_ELEMENTS);
      is.seekg (0, ios::beg);
      return -1;
  }
  //cout << "::GMG::DEBUG::Read " << is.gcount() << " bytes" << endl;
  return static_cast<ssize_t>(is.gcount());
}


// bool MetricReader::readXMLFile (const char *xmlInputFile) {
//
//   //::GMG::open the DSE XML file as binary stream
//   is.open (xmlInputFile, ios::binary);
//   cout << "::GMG::DEBUG::File " << xmlInputFile << " opened" << endl;
//   //::GMG::input stream error checking if we don't use exceptions
//   if (is.fail()) {
//       setReadiness(false);
//       setLastError(XML_ERROR_NO_ELEMENTS);
//       setStatus(XML_STATUS_ERROR);
//       return false;
//   }
//   is.seekg (0, ios::end);
//   length = is.tellg();
//   is.seekg (0, ios::beg);
//   cout << "::GMG::DEBUG::File length: " << length << " bytes" << endl;
//   //::GMG:: Set the buffer length to the file size
//   if (!setBlockSize(static_cast<size_t>(length))) {
//       setStatus(XML_STATUS_ERROR);
//       setLastError(XML_ERROR_NO_MEMORY);
//       return -1;
//   }
//   cout << "::GMG::DEBUG::Calling read_block" << endl;
//   setReadiness(true);
//   if (read_block() == -1) {
//       is.close();
//       cout << "::GMG::DEBUG::File " << xmlInputFile << " closed" << endl;
//       cout << "::GMG::DEBUG::read_block " << xmlInputFile << " error: "
//            << getLastError() << "::" << getStatus() << endl;
//       return false;
//   }
//   cout << "::GMG::DEBUG::Called read_block" << endl;
//   is.close();
//   cout << "::GMG::DEBUG::File " << xmlInputFile << " closed" << endl;
//   return true;
// }

/**
 * \brief writes an XML file with the Metric objects as specified in MULTICUBE
 *
 * \param xmlOutputFile constant char array holding the name of the XML output file
 * \return A boolean result 'True' if the writing process has been successful
 */
bool MetricReader::writeXMLFile(const char* xmlOutputFile) {

  ofstream os;

  os.open (xmlOutputFile);
  if (os.is_open()) {
     os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
     os << "<simulator_output_interface "
        << "xmlns=\"http://www.multicube.eu/\" "
        << "version=\"1.3\">" << endl;
     //  while (this.isMetricLeft()) {
     //      os << "\t" << this.getMetric() << endl;
     //}
     while (!myMetricStack.empty()) {
       os << "\t" << myMetricStack.front() << endl;
       myMetricStack.pop();
     }
     os << "</simulator_output_interface>" << endl;
     os.close();
     return true;
  } else {
     return false;
  }
}

/**
 * \brief writes an XML error file as specified in MULTICUBE
 *
 * \param xmlOutputFile constant char array holding the name of the XML output file
 * \param reason a word description of the exception or error reported
 * \param kind the severity grade of the error reported (i.e fatal)
 * \return A boolean result 'True' if the writing process has been successful
 */
bool MetricReader::writeXMLErrorFile (const char* xmlOutputFile, const char* reason, const char* kind) {

  ofstream os;
  os.open (xmlOutputFile);
  if (os.is_open()) {
     os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
     os << "<simulator_output_interface "
        << "xmlns=\"http://www.multicube.eu/\" "
        << "version=\"1.0\">" << endl;
     os << "\t<error reason=\"" << reason << "\" "
        << "kind=\"" << kind << "\" />" << endl;
     os << "</simulator_output_interface>" << endl;
     os.close();
     return true;
  } else {
     return false;
  }
}
