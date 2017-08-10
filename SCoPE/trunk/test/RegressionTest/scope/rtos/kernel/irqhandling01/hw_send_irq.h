#ifndef _hw_send_irq_H
#define _hw_send_irq_H

#include "uc_hw_if.h"

class hw_send_irq : public UC_hw_if
{
public :
	hw_send_irq(sc_module_name module_name, unsigned int begin, unsigned int end, int irq);
	int read(ADDRESS addr, DATA data, unsigned int size);
	int write(ADDRESS addr, DATA data, unsigned int size);
	
	void sending_irq();
	
	SC_HAS_PROCESS(hw_send_irq);
	
private:
	int m_offset;
};

#endif
