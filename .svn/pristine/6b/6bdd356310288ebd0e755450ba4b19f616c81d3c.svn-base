#include "systemc.h"
#include "hal/uc_master_slave_base.h"
#include "hal/uc_hw_if.h"
#include "hal/uc_master_slave_base.h"
#include "hal/uc_tlm_bus_class.h"

/**
 * \brief Constructor
 *
 * \param module_name the module name
 * \param begin the initial HW address
 * \param end the final HW address
 * \param irq_num the irq number
 */
UC_master_slave_base::UC_master_slave_base(sc_module_name module_name, unsigned int begin, unsigned int end, int irq_num)
	: sc_module(module_name),UC_hw_if_b(module_name),UC_address_manager_class(NULL),m_port("initiator_socket"),m_target_socket("target_socket")
{
	m_irq_num = irq_num;

	m_target_socket.bind(*this);
	m_port.bind(*this);

	m_burst_size = 1;

	set_range(begin, end);
}

/**
 * \brief Constructor
 *
 * It is required to set the address ranges with the function 'set_range'
 * \param module_name the module name
 * \param irq_num the irq number
 */
UC_master_slave_base::UC_master_slave_base(sc_module_name module_name, int irq_num)
	: sc_module(module_name),UC_hw_if_b(module_name),UC_address_manager_class(NULL),m_port("initiator_socket"),m_target_socket("target_socket")
{
	m_irq_num = irq_num;

	m_target_socket.bind(*this);
	m_port.bind(*this);
}

/**
 * \brief Sends irq to the initiator through the bus
 */
void UC_master_slave_base::send_irq() {
	if (m_irq_num != -1) {
		m_target_socket->send_irq(m_irq_num);
	}
}

/**
 * \brief Sends irq to the initiator through the bus
 *
 * \param id irq number
 */
void UC_master_slave_base::send_irq(int id) {
	if (id != -1) {
		m_target_socket->send_irq(id);
	}
}

/**
 * \brief Receives a transaction from the bus and transform the interface to read/write
 *
 * \param trans The transaction packet
 * \param delay Transaction delay (actually ignored)
 */
void UC_master_slave_base::b_transport(tlm_generic_payload &trans, sc_time &delay) {
	unsigned int addr = trans.get_address();
	DATA data = trans.get_data_ptr();
	unsigned int size = trans.get_data_length();

	int bytes;
	if (trans.is_read() == true) {
		add_read_access (size);
		bytes = this->read(addr, data, size);
	}
	else if (trans.is_write() == true) {
		add_write_access (size);
		bytes = this->write(addr, data, size);
	}
	else {
		cout << "HW error: TLM command unrecognized" << endl;
		bytes = -3;
	}

	switch (bytes) {
		case 0:
			trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
			break;
		case -1:
			trans.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
			break;
		case -2:
			trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
			break;
		case -3:
			trans.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
			break;
		default:
			trans.set_response_status(TLM_OK_RESPONSE);
			break;
	}
}


/**
 * \brief Write function to be reimplemented by the HW
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \param extension Unused. For compatibility reasons
 * \return The number of bytes written
 */
int UC_master_slave_base::write(ADDRESS addr, DATA data, unsigned int size, void* extension) {
	printf("Warning: UC_hw_if_b_base::write function not overloaded for this component\n"); 
	return -1;
}

/**
 * \brief Read function to be reimplemented by the HW
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \param extension Unused. For compatibility reasons
 * \return The number of bytes read
 */
int UC_master_slave_base::read(ADDRESS addr, DATA data, unsigned int size, void* extension) {
	printf("Warning: UC_hw_if_b_base::read function not overloaded for this component\n"); 
	return -1;
}


/**
 * \brief Write function to be reimplemented by the HW
 * Should not be used. Included for compatibility reasons with previous versions
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \return The number of bytes written
 */
int UC_master_slave_base::write(ADDRESS addr, DATA data, unsigned int size) {
	return this->write(addr, data, size, NULL);
}

/**
 * \brief Read function to be reimplemented by the HW
 * Should not be used. Included for compatibility reasons with previous versions
 *
 * \param addr The transaction address
 * \param data The transaction data
 * \param size The transaction size
 * \return The number of bytes read
 */
int UC_master_slave_base::read(ADDRESS addr, DATA data, unsigned int size) {
	return this->read(addr, data, size, NULL);
}

/**
 * \brief Gets the target socket
 *
 * \return The target socket
 */
UC_tlm_target_socket<> * UC_master_slave_base::get_target_port() {
	return &m_target_socket;
}

/**
 * \brief Gets the burst size corresponding to an address
 *
 * This is used for special hardware that has a burst size associated
 * \return the burst size
 */
int UC_master_slave_base::get_burst_size(){
	return m_burst_size;
}

/**
 * \brief Assign the burst size associated to the peripheral
 *
 * \param burst_size the maximum burst size accepted by the module.
 */
void UC_master_slave_base::set_burst_size(int burst_size)
{
	m_burst_size = burst_size;
}

/**
 * \brief Function to set the bus access priority of the master
 *
 * @param prio The priority
 */
void UC_master_slave_base::set_bus_priority(int prio) {
	m_bus_priority = prio;
}

/**
 * \brief Function to get the bus access priority of the master
 *
 * @return prio The priority
 */
int UC_master_slave_base::get_bus_priority() {
	return m_bus_priority;
}

/**
 * \brief Binds the bus master with the TLM bus
 *
 * @param bus The bus to connect the master
 */
void UC_master_slave_base::bind(UC_TLM_bus_class * bus){
	UC_tlm_target_socket<> *socket = bus->new_target_port();
	m_port.bind(*socket);
	bus->set_master_priority(socket, m_bus_priority);
}

