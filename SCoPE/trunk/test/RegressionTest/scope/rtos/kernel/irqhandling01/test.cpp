
#include <iostream>
#include <vector>
using namespace std;

#include "linux/ioport.h"
#include "linux/interrupt.h"
#include "../../time_class.h"

extern int test_return;

vector<int> report;

void UC_eat_time(unsigned long long time);

irqreturn_t irq_handler(int irq, void *args, struct pt_regs *regs) {
	report.push_back(irq);

	UC_eat_time(100000000);

	report.push_back(irq);
	return 0;
}

void * test1(void *)
{
	request_irq(2, irq_handler, 0, "/dev/irq2", NULL);
	request_irq(3, irq_handler, 0, "/dev/irq3", NULL);

	sleep(2);

	if ((report[0] == 2) && (report[1] == 3) && (report[2] == 3) && (report[3] == 2)) {
		test_return = 0;
	}

	return NULL;
}
