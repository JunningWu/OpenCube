#include "uc_posix.h"

#include "rtos/kernel/uc_rtos_class.h"

extern __thread UC_rtos_class *qt_parent_rtos;

int UC_posix_class::uc_smp_processor_id(){
	return qt_parent_rtos->get_current_cpu()->m_id;
}
