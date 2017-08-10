/**
 * \file uc_irqreturn.h
 * \Brief Support for Linux interrupt handler
 */
#ifndef UC_IRQRETURN_H
#define UC_IRQRETURN_H

typedef int irqreturn_t;

#define IRQ_NONE	(0)
#define IRQ_HANDLED	(1)
#define IRQ_RETVAL(x)	((x) != 0)


#endif /* UC_IRQRETURN_H */

