/*
 * Author: Juan Castillo
 * Date: 5-11-2007
 * Purpouse: 
 */

OUTPUT_ARCH(arm)
boot_processor = 0x00000000;
ENTRY(boot_processor)

SECTIONS
{
	. = 0x00000000;
	.exc_vectors : { *(.exc_vectors) }
	
	/* System sections */
	.interp : { *(.interp) }
	.note.ABI-tag : { *(.note.ABI-tag) }
	.hash : { *(.hash) }
	.dynsym : { *(.dynsym) }
	.dynstr : { *(.dynstr) }
	.gnu.version : { *(.gnu.version) }
	.gnu.version_r : { *(.gnu.version_r) }
	.rel.dyn : { *(.rel.dyn) }
	.rel.plt : { *(.rel.plt) }
	.init : { *(.init) }
	.plt : { *(.plt) }
	.text : { *(.text) }
	.fini : { *(.fini) }
	.rodata : { *(.rodata) }
	.eh_frame : { *(.eh_frame) }
	.ctors : { *(.ctors) }
	.dtors : { *(.dtors) }
	.jcr : { *(.jcr) }
	.dynamic : { *(.dynamic) }
	.got : { *(.got) }

	.data : { *(.data) }
	.bss : { *(.bss) }
	
	. = ALIGN(16384);
	.mmu_table : { *(.mmu_table) }
	
	. = ALIGN(4);
	_ld_heap_start = .;
	
	. = ALIGN(4);
	.stack_area : { *(.stack_area) }
	
	/* Stabs debugging sections.    */
	.stab           0 : { *(.stab) }
	.stabstr        0 : { *(.stabstr) }
	.stab.excl      0 : { *(.stab.excl) }
	.stab.exclstr   0 : { *(.stab.exclstr) }
	.stab.index     0 : { *(.stab.index) }
	.stab.indexstr  0 : { *(.stab.indexstr) }
	.comment        0 : { *(.comment) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_info     0 : { *(.debug_info) }
	.debug_line     0 : { *(.debug_line) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }

}
