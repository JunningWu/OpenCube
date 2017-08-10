//==========================================================================
//	uc_debug.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié abr 2 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from SCoPE,
//  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 3 of the License, or
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
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

#include <signal.h>
#include "systemc.h"
#include "rtos/utils/uc_simulator_configuration.h"
#include "rtos/utils/uc_debug.h"

/**
 * \brief SIGINT signal handler
 *
 * This function handles the SIGINT signal and stops the simulation instead of
 * exitting immediately.
 */
void uc_sigint_handler(int) {
	uc_simulation_config.m_sigint=1;
	sc_stop();
}

/**
 * \brief Loads the SIGINT signal handler
 *
 * This function creates a sigaction structure to handle de SIGINT signal.\n
 * When the user generates this signal (CONTROL-C), the simulation executes the
 * assigned function.
 */
void uc_load_stop_sim_handler() {
	struct sigaction sigint_action;
	memset (&sigint_action, 0, sizeof(sigint_action));
	sigint_action.sa_handler = uc_sigint_handler;
	sigint_action.sa_restorer = NULL;

	sigaction (SIGINT, &sigint_action, NULL);
}
