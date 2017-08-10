//==========================================================================
//	uc_memory_map.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar feb 5 2008
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

#ifndef _UC_MEMORY_MAP_H_
#define _UC_MEMORY_MAP_H_

#define DMA_ENABLED 1
#define DMA_CHANNELS 16

//MEMORY MAP
#define ROM_START   0x0
#define ROM_SIZE    0x10000000
#define DMA_START   0x20000000
#define DMA_SIZE    0x90
#define IO_START    0x40000000
#define IO_SIZE     0x40
#define SOUND_START 0x50000000
#define SOUND_SIZE  0x40
#define NET_START   0x60000000
#define NET_SIZE    0x40
#define RAM_START   0x80000000
#define RAM_SIZE    0x80000000

//INTERRUPT LIST
//HW
#define RESET_IRQ   OxF
#define BUS_IRQ	    0xE
#define TT_IRQ	    0xD
#define NET_IRQ	    0x5
#define IO_IRQ	    0x4
#define SOUND_IRQ   0x3
#define ROM_IRQ	    0x2
#define RAM_IRQ	    0x1
#define DMA_IRQ	    0x0
//SW
#define SW_RST_EXC  Ox1F	// SW reset exception
#define SW_WD_EXC   0x1E	// SW watch-dog exception (System blocked)
#define SW_BUS_EXC  0x1D	// Invalid BUS answer
#define INV_INS_EXC 0x1C	// Invalid instruction
#define ALIGN_EXC   0x1B	// Alignment exception
#define PERM_EXC    0x1A	// Permision error
#define TRAP_EXC    0x11	// System Trap

//DMA ADDRESSES ORDER
#define DMA_MODE_PORT      0
#define DMA_ADDR_MEM_PORT  (1<<2)
#define DMA_ADDR_DEV_PORT  (2<<2)
#define DMA_COUNT_PORT     (3<<2)
#define DMA_ENABLED_PORT   (4<<2)
#define DMA_RESIDUE_PORT   (5<<2)
#define DMA_CLEAR_PORT     (6<<2)
#define DMA_MASK_PORT      (7<<2)

//DMA MODE
#define DMA_MODE_READ   1
#define DMA_MODE_WRITE  2

#endif
