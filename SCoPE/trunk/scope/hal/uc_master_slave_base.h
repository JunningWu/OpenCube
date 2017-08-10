#ifndef UC_MASTER_SLAVE_BASE_H
#define UC_MASTER_SLAVE_BASE_H

#include "hal/uc_hal_base_if.h"
#include "hal/uc_tlm_bus_sockets.h"
#include "hal/uc_hw_if.h"
#include "hal/uc_address_manager_class.h"

class UC_TLM_bus_class;

/**
 * \brief Functions shared by all bus masters
 */
class UC_master_slave_base : public sc_module, 
	public UC_hw_if_b, 
	public UC_cpu_if,
	public UC_address_manager_class
{

	int m_bus_priority;
	int m_irq_num;
	int m_burst_size;		  ///< The burst size accepted by the peripheral

	UC_tlm_target_socket<> m_target_socket;


public:

//	virtual unsigned int get_start_address(){return UC_address_manager_class::get_start_address();}

	UC_master_slave_base(sc_module_name module_name, unsigned int begin, unsigned int end, int irq_num = -1);

	UC_master_slave_base(sc_module_name module_name, int irq_num = -1);

	virtual void set_bus_priority(int prio);
	virtual int get_bus_priority();
	virtual void bind(UC_TLM_bus_class * bus);

	// Receive request
	virtual void b_transport(tlm_generic_payload &trans, sc_time &delay);

	// Send Interruption
	virtual void send_irq();
	virtual void send_irq(int id);

	virtual UC_tlm_target_socket<> * get_target_port();

	// Specific peripheral functions (DATA = void *, ADRESS = unsigned int)
	virtual int read(ADDRESS addr, DATA data, unsigned int size, void *extension);
	virtual int write(ADDRESS addr, DATA data, unsigned int size, void *extension);
	virtual int read(ADDRESS addr, DATA data, unsigned int size);
	virtual int write(ADDRESS addr, DATA data, unsigned int size);

	// Burst size control
	virtual void set_burst_size( int burst_size );
	virtual int get_burst_size();

public:

	UC_tlm_initiator_socket<> m_port; ///< Master's port

};
#endif
