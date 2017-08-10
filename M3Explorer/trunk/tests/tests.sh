#
# Simplified tests for the XML driver; every single file has some
# peculiarity for testing purposes.
# This script needs one mandatory parameter, the path to m3explorer binary.
# 
# @author: Sivieri Alessandro
#

if [ $# -ne 1 ]; then
	echo "Usage: $0 path/to/m3explorer"
	exit 1
fi
if [ $# -eq 1 ]; then
	if [ ! -x $1 ]; then
		echo "m3explorer cannot be executed; please check $1"
		exit 1
	fi
	$1 -x multicube_design_space_simplified_1.xml -f test_xml_1.scr > test_results_1.txt
	$1 -x multicube_design_space_simplified_2.xml -f test_xml_2.scr > test_results_2.txt
	$1 -x multicube_design_space_simplified_3.xml -f test_xml_3.scr > test_results_3.txt
	$1 -x multicube_design_space_simplified_4.xml -f test_xml_4.scr > test_results_4.txt
	$1 -x multicube_design_space_simplified_5.xml -f test_xml_5.scr > test_results_5.txt
	$1 -x multicube_design_space_simplified_6.xml -f test_xml_6.scr > test_results_6.txt
fi
