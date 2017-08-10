//==========================================================================
//	uc_gui_interface.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie sep 26 2008
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

#include "rtos/utils/uc_gui_interface.h"
#include "rtos/kernel/uc_rtos_class.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

void *uc_gui_thread(void *) {
	int numbytes;
	char bufr[MAX_GUI_BUFFER_SIZE*sizeof(uc_gui_packet)];
	void *bufw;

	while(true) {
		int sin_size = sizeof(struct sockaddr_in);

		while ((numbytes=read(UC_rtos_class::m_gui_fd, bufr, 10)) == -1){
			printf("Error in recv() %d\n", errno);
			sleep(1);
		}

		// Sending the token
		bufw = UC_rtos_class::pop_gui_packets(numbytes);
		if (numbytes != 0) {
			printf("************sending %d \n", numbytes);
			write(UC_rtos_class::m_gui_fd, bufw, numbytes);
		}
	}

	close(UC_rtos_class::m_gui_fd);
	return NULL;
}

void uc_gui_init() {
	int sockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("ERROR opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 12345;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("ERROR on binding");
	}
	listen(sockfd,1);
	clilen = sizeof(cli_addr);
	UC_rtos_class::m_gui_fd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (UC_rtos_class::m_gui_fd < 0) {
		printf("ERROR on accept");
	}

	pthread_t th1;
	pthread_create(&th1, NULL, uc_gui_thread, NULL);
}
