#include "hw_send_irq.h"

hw_send_irq::hw_send_irq(sc_module_name module_name, unsigned int begin, unsigned int end, int irq)
	: UC_hw_if(module_name, begin, end, irq)
{
	m_offset = irq;
	SC_THREAD(sending_irq);
}

int hw_send_irq::write(ADDRESS addr, DATA data, unsigned int size) {
	return size;
}

int hw_send_irq::read(ADDRESS addr, DATA data, unsigned int size) {
	return size;
}

void hw_send_irq::sending_irq() {
	while (true) {
		wait(1, SC_SEC);
		wait(m_offset, SC_MS);
		send_irq();
	}
}
