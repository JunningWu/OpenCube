//==========================================================================
//
// Author(s): Gerardo de Miguel González
// Contributors:
// Description: (Expat + expatmm) MetricReader Header file
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
#ifndef _PARSE_H_
#define _PARSE_H_

#include <fstream>
#include <queue>

#include "ExpatXMLParser.h"

/**
 * \class MetricReader
 * \brief Metric (expat + expatmm) Parser Class
 *
 * \author Gerardo de Miguel González
 * \version 1.0
 * \date    June 2008
 */
class MetricReader : public expatmm::ExpatXMLParser {
public:
  MetricReader ();
  MetricReader(const char* xmlFile);
  ~MetricReader ();

  //bool readXMLFile(const char* xmlInputFile);
  bool writeXMLFile(const char* xmlOutputFile);
  bool writeXMLErrorFile (const char* xmlOutputFile, const char* reason, const char* kind);

  Metric getMetric();
  void setMetric (Metric& m);
  bool isMetricLeft() const;
  int size () const;

protected:
  virtual ssize_t read_block(void);
  virtual void StartElement(const XML_Char* name, const XML_Char** attrs);
  virtual void EndElement(const XML_Char* name);

private:
  ifstream is;
  unsigned long int length;
  queue<Metric> myMetricStack;
};

#endif
