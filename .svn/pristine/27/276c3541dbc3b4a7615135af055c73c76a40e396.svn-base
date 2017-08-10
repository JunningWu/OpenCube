/**
 * \file uc_interrupt.h
 * \brief Support for Linux interrupts
 */
#ifndef UC_INTERRUPT_H
#define UC_INTERRUPT_H

#include "rtos/drivers/linux/uc_irqreturn.h"

typedef irqreturn_t (*irq_handler_t)(int, void *);

int request_irq(unsigned int irq, irqreturn_t (*)(int, void *, struct pt_regs *),
				unsigned long irqflags, const char * devname, void *dev_id);
void free_irq(unsigned int irq, void *dev_id);

#endif /* UC_INTERRUPT_H */

