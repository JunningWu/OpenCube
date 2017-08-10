//==========================================================================
//
// Author(s): Hector Posadas & Gerardo de Miguel González
// Description:   Creates the output xml file
//
//
//==========================================================================
//
//  The following code is derived, directly or indirectly, from XML plug-in for SCoPE,
//  released June 30, 2008. Copyright (C) 2008 Univeristy of Cantabria(UC)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
//
//  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCoPE and the XML plug-in for SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================
#include "xml_if.h"
#include "xml_input.h"
#include "xml_list.h"

#include <algorithm>

#include "xml_obtain_metrics.h"
#include "scuc_noc_sim_if.h"


long double xml_get_tlm_power_consumption(UC_tlm_statistics *statistics, struct hw_instance *instance){
	long double power = 0,  energy = 0;

	power = GET_INSTANCE_PARAMETER(instance, static_power);
	energy = GET_INSTANCE_PARAMETER(instance, read_energy) * statistics->number_of_read_acceses;
	energy += GET_INSTANCE_PARAMETER(instance, write_energy) * statistics->number_of_write_acceses;
	energy += GET_INSTANCE_PARAMETER(instance, read_size_energy) * statistics->size_of_read_acceses/4;
	energy += GET_INSTANCE_PARAMETER(instance, write_size_energy) * statistics->size_of_write_acceses/4;
	power += energy*1e12 / (sc_time_stamp().value());

	return power;
}


//::GMG::(03)::Latency::[L]
long double calculate_latency () {
  long double start_time = sc_time_stamp().value();
  long double end_time = 0;

  for(int kk = 0; kk < rtos_list.size(); kk++) {
      if(rtos_list[kk]->m_statistics->get_min_process_start_time() < start_time){
         start_time = rtos_list[kk]->m_statistics->get_min_process_start_time();
      }
      if(rtos_list[kk]->m_statistics->get_max_process_end_time() > end_time){
         end_time = rtos_list[kk]->m_statistics->get_max_process_end_time();
      }
  }
  return (end_time - start_time) * 1e-12;
}

//::GMG::(04)::Execution_Cycles::[EC]
long double calculate_ec () {
  //::GMG::NOTE:: We use the total latency and the assumption that all
  //       the processors have the same working frequency
  long double clock = static_cast<long double>
                      (((rtos_list[0]) -> get_processor(0)) -> m_current_clock);

  //::GMG::NOTE:: There may be a cpu clock or a parsed clock
  if (!clock)
      clock = static_cast<long double>
              (((rtos_list[0]) -> get_processor(0)) -> m_parsed_clock);
  return static_cast<unsigned long int>(calculate_latency() * clock);
}

//::GMG::(05)::Instruction_Count::[IC]
long double calculate_ic () {
  long double ic = 0;

  for(int kk = 0; kk < rtos_list.size(); kk++) {
      for(int jj=0; jj< (rtos_list[kk]) -> get_num_cpus(); jj++){
          ic += (((rtos_list[kk]) -> get_processor(jj))-> get_statistics()).instructions;
      }
  }
  return ic;
}

//::GMG::(08)::Million of Instructions Per Second::[MIPS]
long double calculate_mips () {
  //::GMG::NOTE:: We use the total latency and the assumption that all
  //       the processors have the same working frequency
  long double clock = static_cast<long double>
                      (((rtos_list[0]) -> get_processor(0)) -> m_current_clock);

  //::GMG::NOTE:: There may be a cpu clock or a parsed clock
  if (!clock)
      clock = static_cast<long double>
              (((rtos_list[0]) -> get_processor(0)) -> m_parsed_clock);
  return static_cast<unsigned long int>(calculate_ic() * clock * 1e-6/calculate_ec());
}

//GMG::(09)::Hit_Rate::[HR]
long double calculate_hr () {
  //unsigned long int im = 0;
  //unsigned long int dm = 0;
  long double misses = 0;
  long double ic = calculate_ic();

  for(int kk = 0; kk < rtos_list.size(); kk++) {
      for(int jj=0; jj< (rtos_list[kk]) -> get_num_cpus(); jj++){
          //im += (((rtos_list[kk]) -> get_processor(jj))-> get_statistics()).instruction_misses;
          //dm += (((rtos_list[kk]) -> get_processor(jj))-> get_statistics()).data_misses;
          if(((rtos_list[kk]) -> get_processor(jj))-> m_icache != NULL) {
            misses +=((rtos_list[kk]) -> get_processor(jj))-> m_icache -> m_total_misses;
          }
      }
  }
  return (1.0 - (static_cast<long double>(misses)/ic));
}

//::GMG::(10)::Memory_Stall_Cycles::[MSC]
long double calculate_msc () {
  long double msc;

  for(int kk = 0; kk < rtos_list.size(); kk++) {
      for(int jj=0; jj< (rtos_list[kk]) -> get_num_cpus(); jj++){
          if(((rtos_list[kk]) -> get_processor(jj))-> m_icache != NULL) {
            msc +=((rtos_list[kk]) -> get_processor(jj))-> m_icache -> m_stall_cycles;
          }
      }
  }
  return msc;
}

//::GMG::(11)::Average Memory Access Time::[AMAT]
long double calculate_amat () {
  long double misses = 0;
  long double msc = calculate_msc();
  long double hr = calculate_hr();

  for(int kk = 0; kk < rtos_list.size(); kk++) {
      for(int jj=0; jj< (rtos_list[kk]) -> get_num_cpus(); jj++){
          if(((rtos_list[kk]) -> get_processor(jj))-> m_icache != NULL) {
            misses +=((rtos_list[kk]) -> get_processor(jj))-> m_icache -> m_total_misses;
          }
      }
  }
  //::GMG::NOTE::Hit Time is supposed to be 1 cycle clock time
  return (1.0 + (1.0 - hr) * static_cast<long double>(msc)/misses);
}

int obtain_xml_metrics(MetricReader &mr){
  //::GMG::Objects for dealing with Metrics and Parameters
  Metric m;

  long double ic = 0;
  long double power = 0;
  long double cpi = 0;         // (06)::Clock_Per_Instruction::[CPI]
  long double ipc = 0;         // (07)::Instructions_Per_Clock::[IPC]
  long double nab = 0;         // (13)::Network_Aggregate Bandwidth::[NAB]
  long double tl = 0;          // (14)::Transport Latency ::[TL]
  int size = 0;

  if(uc_simulation_config.m_sigint>0){ return -1;}

  //::GMG::MetricReader(Writer) writes the metric results to
  //       a M3 output interface file
  size = mr.size();
  for (int ii = 0; ii < size; ii++) {
   m = mr.getMetric();
   m.nameToLower();
   power = 0;
   if (m.getName() == "instruction_count") {
        for(int kk = 0; kk < rtos_list.size(); kk++) {
		for(int jj=0; jj< (rtos_list[kk]) -> get_num_cpus(); jj++){
 			ic += ((rtos_list[kk]) -> get_processor(jj)) ->m_statistics->m_num_instr;
		}
        }
        m.setValue(ic * m.multiplier());
        mr.setMetric(m);
    } else if (m.getName() == "latency") {
	long double start_time = sc_time_stamp().value();
	long double end_time = 0;
        for(int kk = 0; kk < rtos_list.size(); kk++) {
		if(rtos_list[kk]->m_statistics->get_min_process_start_time() < start_time){
			start_time = rtos_list[kk]->m_statistics->get_min_process_start_time();
		}
		if(rtos_list[kk]->m_statistics->get_max_process_end_time() > end_time){
			end_time = rtos_list[kk]->m_statistics->get_max_process_end_time();
		}
        }
        m.setValue((end_time - start_time) * (m.multiplier())/1e12);
        mr.setMetric(m);
    } else if (m.getName() == "power_consumption") {

	struct list_elem * elem=get_hw_instance_header();
	struct hw_instance *instance;
	long double energy = 0;
        while(elem != NULL) {
		long double comp_power=0;
		energy = 0;
		instance = (struct hw_instance *) elem->elem;


		if(strcasecmp(instance->component->category, "network") == 0 ){
			// FIXME: To be completed
		}
		else if(strcasecmp(instance->component->category, "bus") == 0  ){
			UC_TLM_bus_class *bus=(UC_TLM_bus_class *)instance->scope_data;
			comp_power += xml_get_tlm_power_consumption(bus,instance);
		}
		else if(strcasecmp(instance->component->category, "net_if") == 0  ){
			UC_hw_NoC *hw=(UC_hw_NoC *)instance->scope_data;
			comp_power += xml_get_tlm_power_consumption(hw,instance);
		}
		else if(strcasecmp(instance->component->category, "memory") == 0  ){
			UC_hw_memory *hw=(UC_hw_memory *)instance->scope_data;
			comp_power += xml_get_tlm_power_consumption(hw,instance);
		}
		else if(strcasecmp(instance->component->category, "dma") == 0  ){
			UC_hw_dma *hw=(UC_hw_dma *)instance->scope_data;
			comp_power += xml_get_tlm_power_consumption(hw,instance);
		}
		else if(strcasecmp(instance->component->category, "processor") == 0  ){
			comp_power = GET_INSTANCE_PARAMETER(instance, static_power);
			UC_cpu_class *cpu=(UC_cpu_class *)instance->scope_data;
			energy = cpu->m_statistics->m_num_instr;
			energy *= GET_INSTANCE_PARAMETER(instance, active_energy);
			comp_power += 1e12 * energy / sc_time_stamp().value();
		}
		else if(strcasecmp(instance->component->category, "icache") == 0  ){
			power = GET_INSTANCE_PARAMETER(instance, static_power);
			UC_icache_class *cache=(UC_icache_class *)instance->scope_data;
			energy = cache->m_cpu->m_statistics->m_num_instr;
			energy *= GET_INSTANCE_PARAMETER(instance, active_energy);
			comp_power += 1e12 * energy / sc_time_stamp().value();
			energy = cache->m_total_misses;
			energy *= GET_INSTANCE_PARAMETER(instance, read_energy);
			comp_power += 1e12 * energy / sc_time_stamp().value();
		}
		else if(strcasecmp(instance->component->category, "bridge") == 0  ){
//			comp_power += xml_get_tlm_power_consumption(dynamic_cast<UC_tlm_statistics*>(instance->scope_data),instance);
		}
		else if(strcasecmp(instance->component->category, "dcache") == 0  ){
//			comp_power += xml_get_tlm_power_consumption(dynamic_cast<UC_tlm_statistics*>(instance->scope_data),instance);
		}
		else if(strcasecmp(instance->component->category, "as_hw") == 0  ){
			comp_power += xml_get_tlm_power_consumption((UC_master_slave_base*)(instance->scope_data),instance);
		}
		power += comp_power;

		elem = elem->next;
       }
	
        m.setValue(power * m.multiplier());
        mr.setMetric(m);
    } else if (m.getName() == "area") {
	struct list_elem * elem=get_hw_instance_header();
        long double area = 0;
	struct hw_instance *instance;
        while(elem != NULL) {
		instance = (struct hw_instance *) elem->elem;
		if(instance->area){
			area += atoi(instance->area);
		}
 		else if(instance->component->area){
			area += atoi(instance->component->area);
		}
		elem = elem->next;
       }
        m.setValue(area * m.multiplier());
        mr.setMetric(m);
    }
    else if (m.getName() == "execution_cycles") {
        //::GMG::NOTE:: We use the total latency and the assumption that all
        //       the processors have the same working frequency
        m.setValue(calculate_ec() * m.multiplier());
        mr.setMetric(m);
    } else if (m.getName() == "clock_per_instruction") {
        //::GMG::FIXME::case ic = 0::try/catch?
        cpi = static_cast<long double>(calculate_ec())/calculate_ic();
        m.setValue(cpi * m.multiplier());
        mr.setMetric(m);
    } else if (m.getName() == "instructions_per_clock") {
        //::GMG::FIXME::case ec = 0::try/catch?
        ipc = static_cast<long double>(calculate_ic())/calculate_ec();
        m.setValue(ipc * m.multiplier());
        mr.setMetric(m);
    } else if (m.getName() == "mips") {
        m.setValue(calculate_mips() );
        mr.setMetric(m);
    } else if (m.getName() == "hit_rate") {
        m.setValue(calculate_hr());
        mr.setMetric(m);
    } else if (m.getName() == "memory_stall_cycles") {
        m.setValue( calculate_msc () );
        mr.setMetric(m);
    } else if (m.getName() == "amat") {
        m.setValue(calculate_amat());
        mr.setMetric(m);
    } else if (m.getName() == "bus_bandwidth") {
	struct list_elem * elem=get_hw_instance_header();
	struct hw_instance *instance;
	long double transfer = 0;
        while(elem != NULL) {
		instance = (struct hw_instance *) elem->elem;
		if(strcmp(instance->component->category, "bus") == 0  ){
			UC_TLM_bus_class *bus=(UC_TLM_bus_class *)instance->scope_data;
			transfer = bus->size_of_read_acceses + bus->size_of_write_acceses;
			transfer *= 8; // byts -> bits
			transfer = transfer / (sc_time_stamp().value() / 1e12);
		}
		elem = elem->next;
       }
	
        m.setValue(transfer * m.multiplier());
        mr.setMetric(m);
    } else if (m.getName() == "network_aggregate_bandwidth") {
        //::GMG::NOTE::We take the simulation time as the time
        //       to get the bandwidth
        nab = static_cast<long double>(((simulator_list[0]) ->  get_statistics()).bytes)/xml_scope_simulation_info.m_simulation_time;
        m.setValue(nab);
        mr.setMetric(m);
    } else if (m.getName() == "transport_latency") {
        //::GMG::FIXME::We must take PRZSimulation *m_sim; into public in
        //       uc_noc_interface.h
        tl = (simulator_list[0]) -> get_transport_lattency();
        m.setValue(tl * m.multiplier());
        mr.setMetric(m);
    }
  }

  xml_scope_simulation_info.m_report_generated = 1;
}
