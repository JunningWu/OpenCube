/**
 * \file bus_estress.cpp
 */

#include <iostream>
using namespace std;
#include <linux/io.h>
#include <linux/interrupt.h>
#include <pthread.h>

irqreturn_t irq_handler(int irq, void *args, struct pt_regs *regs) {
	int clear = 0xffffffff;
	iowrite32(clear, (void *)0x80000000);
	return 0;
}

int bus_estress(int argc, char *argv[]) {
// 	char *data = (char *)malloc(50000);
	
	request_irq(1, irq_handler, 0, "/dev/irq1", NULL);
	request_irq(2, irq_handler, 0, "/dev/irq2", NULL);

	for (int i = 0; i < 100000; i++) {
		for (int j=0 ; j<50000 ; j++) {
			iowrite32(0x12345678, (void *)0x80000000);
		}
	}

// 	free(data);
	
	cout << "Finished" << endl;
	return 0;
}
