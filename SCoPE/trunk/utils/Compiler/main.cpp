/**
 * \file main.cpp
 */

#include <iostream>
#include "command_line.h"
using namespace std;

int main(int argc, char *argv[]) {
	int ret = 0;
	command_analyzer_class *command_analyzer = new command_analyzer_class();
	command_analyzer->analyze_command_line(argc, argv);
	command_analyzer->run();
	delete command_analyzer;
	return ret;
}

