# Stub simulator for m3explorer; it creates the correct output XML document,
# generating the surface and internal area of a beverage can as output metrics.
# This script expects two parameters, as a specific:
# --xml_system_configuration=input_filename.xml
# --xml_system_metrics=output_filename.xml
# 
# @author Sivieri Alessandro

import xml.dom.minidom as minidom
import os.path
import os
import time
import sys
import getopt
import math

#parameters
partest_1=0
partest_2=0
partest_3=0
partest_1_bool=False
partest_2_bool=False
partest_3_bool=False
# files
outputFilename=None
inputFilename=None

def main():
	# reading input params
	doc2=minidom.parse(inputFilename)
	params=doc2.getElementsByTagName("parameter")
	for param in params:
		name=param.getAttribute("name")
		if name=="radius":
			partest_1=int(param.getAttribute("value"))
			partest_1_bool=True
		if name=="height":
			partest_2=int(param.getAttribute("value"))
			partest_2_bool=True
		if name=="thickness":
			partest_3=int(param.getAttribute("value"))
			partest_3_bool=True
	# writing output params
	doc=minidom.Document()
	root=doc.createElementNS("http://www.multicube.eu/", "simulator_output_interface")
	root.setAttribute("xmlns", "http://www.multicube.eu/")
	root.setAttribute("version", "1.3")
	doc.appendChild(root)
	if not partest_1_bool or not partest_2_bool or not partest_3_bool:
		# error output
		errorNode=doc.createElement("error")
		errorNode.setAttribute("reason", "missing required parameter")
		errorNode.setAttribute("kind", "fatal")
		root.appendChild(errorNode)
	else:
		# metric1
		node=doc.createElement("system_metric")
		node.setAttribute("name", "surface")
		node.setAttribute("value", str(2*math.pi*partest_1*partest_1+2*math.pi*partest_1*partest_2))
		root.appendChild(node)
		# metric2
		node=doc.createElement("system_metric")
		node.setAttribute("name", "volume")
		node.setAttribute("value", str(math.pi*(partest_1-float(partest_3)/10)*(partest_1-float(partest_3)/10)*partest_2))
		root.appendChild(node)
        #print "------> BEVERAGE CAN is running: Received "+str(partest_1)+" "+str(partest_2)+" "+str(partest_3)+" "
# writing output in the same directory of input
	outputString=doc.toprettyxml()
	doc.unlink()
	f=open(outputFilename, "w")
	f.write(outputString)
	f.close()
#	time.sleep(1.5)

def usage(program):
	print "Usage: "+program+" --xml_system_configuration=input_filename.xml --xml_system_metrics=output_filename.xml --reference_xsd=input_filename.xsd"
	sys.exit(1)

if __name__=="__main__":
	try:
		opts, args=getopt.getopt(sys.argv[1:], "", ["xml_system_configuration=", "xml_system_metrics=", "reference_xsd="])
	except getopt.GetoptError, err:
		print err
		usage(sys.argv[0])
	for o, a in opts:
		if o == "--xml_system_configuration":
			inputFilename=a
		elif o == "--xml_system_metrics":
			outputFilename=a
		elif o == "--reference_xsd":
			pass
		else:
			usage(sys.argv[0])
	if(inputFilename==None or outputFilename==None):
	   usage(sys.argv[0])
	main()
	sys.exit(0)
