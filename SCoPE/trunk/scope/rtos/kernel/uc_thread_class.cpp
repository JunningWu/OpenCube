//==========================================================================
//	uc_thread_class.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar oct 30 2007
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from SCoPE,
//  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//
//
//
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/utils/uc_gui_interface.h"
#include "rtos/kernel/uc_tic_timer_class.h"
#include "rtos/utils/uc_backtrace.h"
#include "rtos/utils/uc_randn.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern unsigned long long uc_segment_time;
extern unsigned long long uc_segment_instr;
extern __thread UC_rtos_class *qt_parent_rtos;


/**
 * \brief Constructor
 *
 * \param task The parent task of this process
 */
UC_thread_class::UC_thread_class(UC_task_class *task)
{
	int pid;

	m_parent_task = task;
	m_stp = NULL;
	m_initial_function = NULL;
	m_initial_function_args = NULL;
	m_RR_counter = NICE_TO_TICKS(task->get_nice());
	m_priority = 0;
	m_state = CREATED;
	m_next_state = (UC_process_states)-1;
	m_preempt_state = (UC_process_states)-1;
	m_kernel_mode = true;
	m_parser_time = 0;
	m_switches_time = 0;
	m_parser_instr = 0;
	m_tid = 0;
	m_func_name = NULL;
	m_timeout_expired = false;
	m_in_rwlock = 0;
	m_signal_executed = 0;
	m_signal_received = 0;
	m_in_signal = 0;
	m_waiting_processor_in_signal = 0;
	m_is_waiting_signal = 0;
	m_waiting_pid = 0;
	m_end_value = NULL;

	m_num_backtrace = 0;
	m_backtrace = NULL;
	m_backtrace_p = NULL;

	m_bus_transfer_info.m_HAL_return = 0;
	m_bus_transfer_info.m_HAL_function_id = -1;
	m_bus_transfer_info.m_HAL_arg_stack = malloc(MAX_HAL_ARGS_SIZE);
	m_bus_transfer_info.m_HAL_done = 0;
	m_bus_transfer_list.clear();


	MicroCOS_flags_ready = 0;
	m_microcos_unblocker = UNKNOWN;
	m_microcos_unblocker_id = -1;

	task->m_parent_process->m_parent_rtos->m_tic_timer->create_clock(this);

	m_time_created = sc_time_stamp().value();

	m_timeout_blocked = false;

	m_cpu = -1;
	m_user_time = 0;
	if(m_parent_task->m_cpu > 0){
		m_current_cpu = m_parent_task->m_parent_process->m_parent_rtos->get_processor(m_parent_task->m_cpu);
	}
	else{
		m_current_cpu = m_parent_task->m_parent_process->m_parent_rtos->get_processor(0);
	}

// Non operative: dynamic threads cannot be printed as traces in vcd files in SystemC. Values are wrong
#if 0
	if(uc_backtrace_required() && 0x200){
		if(strcmp(task->m_parent_process->m_process_name,"interruption_handler")!=0){
			char str[15];
			int index = 7;
			int id=m_tid;
			strcpy(str,"thread_");
			str[index++]=(id/100) + '0';
			id %= 100;
			str[index++]=(id/10) + '0';
			id %= 10;
			str[index++]=id + '0';
			str[index]='\0';
			sc_trace(uc_simulation_config.vcd_file,m_state, str);
		}
	}
#endif
}


UC_thread_class::~UC_thread_class()
{

	class UC_rtos_statistics_class *statistics;
	statistics = m_parent_task->m_parent_process->m_parent_rtos->m_statistics;
	statistics->add_thread_user_time(this,m_user_time);

	if (m_func_name != NULL) {
		delete [] m_func_name;
	}

	free(m_bus_transfer_info.m_HAL_arg_stack);

	m_parent_task->m_parent_process->m_parent_rtos->m_tic_timer->destroy_clock(this);

	// test
// 	if (m_lifo_cancel_func.size() != 0) cout << "Error1" << endl;
// 	if (m_lifo_cancel_args.size() != 0) cout << "Error2" << endl;
// 	if (m_end_value != NULL) cout << "end_value" << endl;
// 	if (m_blocked_thread_list.size() != 0) cout << "Error3" << endl;
// 	if (m_semaphore_list.size() != 0) cout << "Error4" << endl;
// 	if (m_rwlock_holds.size() != 0) cout << "Error5" << endl;
// 	if (m_semaphore_list_fork.size() != 0) cout << "Error6" << endl;
// 	if (m_backtrace != NULL) cout << "Error7" << endl;
}

/**
 * \brief Initializes the thread
 *
 * This funtions is called by UC_user_code_wrapper that executes the user code.\n
 * It must be executed before the user code.
 * \param stp The pointer to the qt execution structure
 */
void UC_thread_class::init_thread(stp_t *stp) {
	m_stp = stp;

	set_next_state(CREATED, READY);
	update_state();
// 	stp_yield();
	set_next_state(READY, SUPER_USER);
	update_state();
// 	stp_yield();
	set_next_state(SUPER_USER, WAITING);
	update_state();
	stp_yield();
// 	set_next_state(WAITING, USER);
}

/**
 * \brief Destroy the thread
 *
 * This funtions is called by UC_user_code_wrapper that executes the user code.\n
 * It must be executed after the user code.
 */
void UC_thread_class::destroy_thread() {

	UC_rtos_class *rtos = m_parent_task->m_parent_process->m_parent_rtos;

	if(rtos->m_additional_destroy_thread != NULL){
		if( rtos->m_additional_destroy_thread(this, rtos)) {
			// FIXME why the return?
			return;
		}
	}

//	current_cpu -> m_dcache->flush(); // send all lines in write back

	if(m_current_cpu -> m_dcache)m_current_cpu -> m_dcache->sync_misses(m_current_cpu, this);
	if(m_current_cpu -> m_icache)m_current_cpu -> m_icache->sync_misses(m_current_cpu, this);

	set_next_state(USER, SUPER_USER);
	update_state();
	set_next_state(SUPER_USER, ZOMBIE);
// 	stp_yield();
 	//update_state();

	// Destroy the clock associated
	m_parent_task->m_parent_process->m_parent_rtos->m_tic_timer->destroy_clock(this);

	// Delete from lists if blocked
	while (m_blocked_in_list.size() != 0) {
		vector<UC_thread_class *>::iterator it_thread;
		for (it_thread = m_blocked_in_list[0]->begin(); it_thread != m_blocked_in_list[0]->end(); it_thread++) {
			if ((*it_thread) == this) {
				m_blocked_in_list[0]->erase(it_thread);
				break;
			}
		}

		m_blocked_in_list.erase(m_blocked_in_list.begin());
	}

	// Awake blocked threads
	while (m_blocked_thread_list.size() != 0) {
		m_blocked_thread_list[0]->unblock();
	}

	// Destroying clocks
	m_parent_task->m_parent_process->m_parent_rtos->m_tic_timer->destroy_clock(this);
	m_parent_task->m_parent_process->m_parent_rtos->m_tic_timer->thread_destroyed(this);
}

/**
 * \brief Changes the thread state and update the task state
 *
 * \param initial_state The initial state of the task for checking
 * \param final_state The final state of the task
 * \return Returns true if success and false if failure
 */
bool UC_thread_class::set_next_state(UC_process_states initial_state, UC_process_states final_state) {
	// USER annotation
	annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;

	if( m_parent_task->m_parent_process->m_parent_rtos->m_additional_change_status != NULL){
		if(m_parent_task->m_parent_process->m_parent_rtos->m_additional_change_status(this, m_parent_task->m_parent_process->m_parent_rtos,
		initial_state, final_state)) {
			// FIXME why the return?
			return true;
		}
	}

	if (initial_state != m_state) {
// 		cout << "Error thread's new state: " << m_func_name << " - " << initial_state << final_state << m_state << endl;
		return false;
	}

	switch (initial_state) {
		case ZOMBIE:
			break;
		case BLOCKED:
			if (final_state == READY) {
				m_next_state = final_state;
				return true;
			}
			break;
		case WAITING:
			if (final_state == USER) {
				m_next_state = final_state;
				return true;
			}
			break;
		case CREATED:
			if (final_state == READY) {
				m_next_state = final_state;
				return true;
			}
			break;
		case READY:
			if (final_state == SUPER_USER) {
				m_next_state = final_state;
				return true;
			}
			break;
		case USER:
			if (final_state == SUPER_USER) {
				m_next_state = final_state;
				return true;
			}
			break;
		case SUPER_USER:
			if (final_state == WAITING ||
				final_state == BLOCKED ||
				final_state == READY ||
				final_state == ZOMBIE)
			{
				m_next_state = final_state;
				return true;
			}
			break;
	}
	return false;
}

/**
 * \brief Changes the thread state and update the task state
 *
 * \return Returns true if success and false if failure
 */
bool UC_thread_class::update_state() {
	if (m_state == m_next_state) {
		return false;
	}

	UC_process_states prev_state = m_state;
	m_state = m_next_state;

	if (UC_rtos_class::m_gui_fd != -1) {
		create_gui_packet();
	}

	return m_parent_task->m_thread_scheduler->change_state(this, prev_state, m_next_state);
}

/**
 * \brief Adds time to the user time of the thread
 *
 * \param time The time to annotate
 */
void UC_thread_class::annotate_user_time(unsigned long long time, unsigned long long instr) {
	float ftime, parsed_clock, current_clock, cpi;

	if(time > 0 && (m_state != USER && m_state != SUPER_USER)){
//		cout << "Annotating user time in a thread not in USER or SU state"<<endl;
		uc_print_backtrace();
//		assert(0);
	}

	// Adapt the received time to the current clock and cpi of the processor

	// Check that it is not in inizialization process
	// Check that it is not in inizialization process
	if(qt_parent_rtos == NULL || qt_parent_rtos->get_current_cpu() == NULL){
		assert(m_current_cpu != NULL);

		// Adapt the received time to the current clock and cpi of the processor
		current_clock = m_current_cpu->m_current_clock;
		parsed_clock = m_current_cpu->m_parsed_clock;
		cpi = m_current_cpu->m_cpi;
	}
	else{
		// Adapt the received time to the current clock and cpi of the processor
		current_clock = qt_parent_rtos->get_current_cpu()->m_current_clock;
		parsed_clock = qt_parent_rtos->get_current_cpu()->m_parsed_clock;
		cpi = qt_parent_rtos->get_current_cpu()->m_cpi;

	}

	if ( current_clock != 0.0) {
		ftime = (float)time;
		ftime *= (parsed_clock / current_clock );
		time = (unsigned long long)ftime;
	}
	if ( cpi != 0.0) {
		ftime = (float)time;
		ftime *= cpi;
		time = (unsigned long long)ftime;
	}
	m_parser_time += time;
	m_parser_instr += instr;
	if( m_state == USER ){
		m_user_time += time;
	}
	else{
		m_parent_task->m_parent_process->m_parent_rtos->m_statistics->m_rtos_time += time;
	}

}

/**
 * \brief Adds time to the switches time of the thread
 *
 * \param time The time to annotate
 */
void UC_thread_class::annotate_rtos_time(unsigned long long time, unsigned long long instr) {
	float ftime, parsed_clock, current_clock, cpi;

	// Print backtrace if required
	if( (uc_backtrace_required() & 8) != 0){
		uc_print_backtrace(this);
	}

	// Check that it is not in inizialization process
	if(qt_parent_rtos == NULL || qt_parent_rtos->get_current_cpu() == NULL){
		assert(m_current_cpu != NULL);
		// Adapt the received time to the current clock and cpi of the processor
		current_clock = m_current_cpu->m_current_clock;
		parsed_clock = m_current_cpu->m_parsed_clock;
		cpi = m_current_cpu->m_cpi;
	}
	else{
		// Adapt the received time to the current clock and cpi of the processor
		current_clock = qt_parent_rtos->get_current_cpu()->m_current_clock;
		parsed_clock = qt_parent_rtos->get_current_cpu()->m_parsed_clock;
		cpi = qt_parent_rtos->get_current_cpu()->m_cpi;
	}

	if ( current_clock != 0.0) {
		ftime = (float)time;
		ftime *= (parsed_clock / current_clock);
		time = (unsigned long long)ftime;
	}
	if ( cpi != 0.0) {
		ftime = (float)time;
		ftime *= cpi;
		time = (unsigned long long)ftime;
	}

	// FIXME: evaluate using a gaussian distribution for model swich time instead of a constant time
	//time +=randn (5000, 50000, 0) ;
	m_switches_time += time;
// 	m_parser_instr += instr; // No switches_instr variable needed
	m_parent_task->m_parent_process->m_parent_rtos->m_statistics->m_rtos_time += time;
}

/**
 * \brief Blocks the thread in a list (semaphores, mutexes...)
 *
 * \param block_list The list to block the thread in
 */
void UC_thread_class::block(vector<UC_thread_class *> *block_list) {
	block_list->push_back(this);
	m_blocked_in_list.push_back(block_list);
	set_next_state(SUPER_USER, BLOCKED);
	stp_yield();
}

/**
 * \brief Blocks the thread in a list (semaphores, mutexes...) until it is awaked or a timeout
 *
 * \param block_list The list to block the thread in
 * \param abs_timeout The maximum time to be blocked
 * \return If the time has expired returns true
 */
bool UC_thread_class::block(vector<UC_thread_class *> *block_list, const struct timespec *abs_timeout) {
	if (abs_timeout == NULL) {
		block(block_list);
		return false;
	}
//	block_list->push_back(this);
//	m_blocked_in_list.push_back(block_list);
//	set_next_state(SUPER_USER, BLOCKED);

	m_timeout_remaining_time = *abs_timeout;

	m_timeout_blocked = true;
	qt_parent_rtos->POSIX->uc_clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, abs_timeout, NULL);
	m_timeout_blocked = false;

	if (m_timeout_remaining_time.tv_sec == 0 && m_timeout_remaining_time.tv_nsec < 1e9 / TICK_TIMER_HZ) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * \brief Blocks the thread
 */
void UC_thread_class::block() {
 	set_next_state(SUPER_USER, BLOCKED);
	stp_yield();
}

/**
 * \brief Blocks the thread until it is awaked or a timeout
 *
 * \param abs_timeout The maximum time to be blocked
 * \return If the time has expired returns true
 */
bool UC_thread_class::block(const struct timespec *abs_timeout) {
	struct timespec abs_time;
	if (abs_timeout == NULL) {
		block();
		return false;
	}

	set_next_state(SUPER_USER, BLOCKED);

	qt_parent_rtos->m_tic_timer->clock_gettime(CLOCK_MONOTONIC, &abs_time);

	m_timeout_remaining_time = *abs_timeout - abs_time;

	m_timeout_blocked = true;
	qt_parent_rtos->POSIX->uc_clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, abs_timeout, NULL);
	m_timeout_blocked = false;

	if (m_timeout_remaining_time.tv_sec == 0 && m_timeout_remaining_time.tv_nsec < 1e9 / TICK_TIMER_HZ) {
		return true;
	}
	else {
		return false;
	}
}


/**
 * \brief Unblocks the thread from the knowned list
 */
void UC_thread_class::unblock() {
	if (m_timeout_blocked == true) {
		m_timeout_remaining_time.tv_sec = 0;
		m_timeout_remaining_time.tv_nsec = 0;
		vector<struct uc_nanosleep_data*>* sleep_vector = &qt_parent_rtos->m_tic_timer->get_clock(CLOCK_REALTIME)->list_c;
		for (int i = 0; i < sleep_vector->size(); i++) {
			if ((*sleep_vector)[i]->thread == this) {
				m_timeout_remaining_time = UC_tic_timer_class::tics_2_timespec((*sleep_vector)[i]->counter);
				(*sleep_vector)[i]->counter = 0;
				break;
			}
		}
	}

	if (m_blocked_in_list.size() == 0) {
		if (m_state == BLOCKED) {
			set_next_state(BLOCKED, READY);
			update_state();
		}
		return;
	}

	if(m_state == SUPER_USER ) {
		set_next_state(SUPER_USER, READY);
		// FIXME: If the thread is in other processor and in wait_time the preempt_state must be changed. Is there a better place to do that? Not a safe place.
		m_preempt_state = READY;
	}
	else if( m_state == BLOCKED){
		set_next_state(BLOCKED, READY);
		update_state();
	}
	else{
		printf("Error: Trying to unblock a thread that is not in super_user or blocked state\n");
		exit(-1);
	}

	while (m_blocked_in_list.size() != 0) {
		vector<UC_thread_class *>::iterator it_thread;
		for (it_thread = m_blocked_in_list[0]->begin(); it_thread != m_blocked_in_list[0]->end(); it_thread++) {
			if ((*it_thread) == this) {
				m_blocked_in_list[0]->erase(it_thread);
				break;
			}
		}

		m_blocked_in_list.erase(m_blocked_in_list.begin());
	}
}

/**
 * \brief Creates a gui packet
 *
 * Creates a new gui packet with the actual thread state
 * and enqueues it in the RTOS buffer
 */
void UC_thread_class::create_gui_packet()
{
	uc_gui_packet packet;
	packet.rtos_id = m_parent_task->m_parent_process->m_parent_rtos->get_id();
	packet.tid = m_tid;
	packet.time = sc_time_stamp().value()/1000000;
	packet.state = m_state;
	strncpy(packet.name, m_func_name, 20);

	UC_rtos_class::push_gui_packet(packet);
}

/**
 * \brief Identifies the multievent unblocker for MicroCOS
 * For multievent unblocking it is needed the identifier of the real unblocker
 * to know later who awaked the thread
 *
 * \param unblocker The unblocker type
 * \param id The identifier of the unblocker
 */
void UC_thread_class::set_microcos_unblocker(microcos_unblocker unblocker, int id)
{
	m_microcos_unblocker = unblocker;
	m_microcos_unblocker_id = id;
}

/**
 * \brief Return the pointer to the current thread
 * \return Current thread pointer
 */
UC_thread_class* uc_get_current_thread(){
	return qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
}


unsigned long uc_get_curr_clock_period(){
	long long freq = qt_parent_rtos->get_current_cpu()->m_current_clock;

	return ((long long)1e9)/freq;
}

unsigned long uc_get_base_clock_period(){
	long long freq = qt_parent_rtos->get_current_cpu()->m_parsed_clock;

	return ((long long)1e9)/freq;
}
