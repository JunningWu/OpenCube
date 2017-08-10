#ifndef _UC_SIMULATOR_CONFIGURATION_H_
#define _UC_SIMULATOR_CONFIGURATION_H_

#include <vector>
#include <systemc>

class UC_rtos_class;
using namespace std;
//class sc_core::sc_trace_file;

class uc_simulation_config_t{

public:

	int end_with_sw_destruction;
	int activate_backtrace;
	int check_system_status;
	vector <UC_rtos_class*> *rtos_list;
	sc_core::sc_trace_file *vcd_file;
	long long m_max_interval;
	long long m_wait_period;
	int 	  m_ic_granularity;
	int 	  m_dc_granularity;
	int 	  m_sigint;

	uc_simulation_config_t();
	~uc_simulation_config_t();
};

// Eleminated old activate_backtrace variable
//int activate_backtrace = 0;
// define clause for compatibility reasons
#define SIMULATION_CONFIG_STRUCT 1

extern uc_simulation_config_t uc_simulation_config;

int uc_check_end_with_sw_destruction();
int uc_activate_end_with_sw_destruction(int val=1);

void uc_set_check_system_status_flag(bool activate);
bool uc_get_check_system_status_flag();

void uc_set_max_wait_interval(long long time);
long long uc_get_max_wait_interval();

void uc_set_sw_wait_period(long long time);
long long uc_get_sw_wait_period();

int uc_get_ic_granularity();
void uc_set_ic_granularity(int);

int uc_get_dc_granularity();
void uc_set_dc_granularity(int);

void uc_user_sc_stop();

UC_rtos_class *uc_get_rtos_data(int id);

#endif
