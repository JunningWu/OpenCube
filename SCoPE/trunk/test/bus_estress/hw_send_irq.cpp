/**
 * \file hw_send_irq.cpp
 */
#include <iostream>
#include "hw_send_irq.h"
using namespace std;

hw_send_irq::hw_send_irq(sc_module_name module_name, unsigned int begin, unsigned int end, int irq)
	: UC_hw_if(module_name, begin, end, irq)
{
	SC_THREAD(sending_irq);
}

int hw_send_irq::write(ADDRESS addr, DATA data, unsigned int size) {
	return size;
}

int hw_send_irq::read(ADDRESS addr, DATA data, unsigned int size) {
	return size;
}

void hw_send_irq::sending_irq() {
	int i = 1;
	while (true) {
		wait(5, SC_MS);
		cout << "Sending IRQ " << i << "..." << endl;
		send_irq();
		i++;
	}
}
