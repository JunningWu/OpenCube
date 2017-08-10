# Stub simulator for m3explorer; it creates the correct output XML document,
# multiplying some of the given parameters as output metrics; whenever one of
# those parameters equals 1, it returns an error document.
# This script expects two parameters, as a specific:
# --xml_system_configuration=input_filename.xml
# --xml_system_metrics=output_filename.xml
# 
# @author Sivieri Alessandro

import xml.dom.minidom as minidom
import os.path
import os
import sys
import getopt

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
		if name=="partest_1":
			partest_1=int(param.getAttribute("value"))
			partest_1_bool=True
		if name=="partest_2":
			partest_2=int(param.getAttribute("value"))
			partest_2_bool=True
		if name=="partest_3":
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
		errorNode.setAttribute("kind", "non-fatal")
		root.appendChild(errorNode)
	elif partest_3==1:
		# error output
		if partest_2==1:
			kind="non-fatal"
		else:
			kind="non-fatal"
		errorNode=doc.createElement("error")
		errorNode.setAttribute("reason", "partest_3 == 1")
		errorNode.setAttribute("kind", kind)
		root.appendChild(errorNode)
	else:
		# metric1
		node=doc.createElement("system_metric")
		node.setAttribute("name", "metric1")
		node.setAttribute("value", str(partest_1*partest_2))
		root.appendChild(node)
		# metric2
		node=doc.createElement("system_metric")
		node.setAttribute("name", "metric2")
		node.setAttribute("value", str(partest_2*partest_3))
		root.appendChild(node)
	# writing output in the same directory of input
	outputString=doc.toprettyxml()
	doc.unlink()
	f=open(outputFilename, "w")
	f.write(outputString)
	f.close()

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
