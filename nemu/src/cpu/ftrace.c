#include <cpu/ftrace.h>

Funct **funct_tab;

void recr_ftrace() {
	for (int i=0; i<128; i++) {
		printf("funct%d: %s, %x, %d\n", i, (*funct_tab)[i].name, (*funct_tab)[i].addr, (*funct_tab)[i].size);
	}
}
