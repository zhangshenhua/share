/*---------------------------------
* FILE Name: new.c
*
* Description: malloc with check
*
* COPYRIGHT
*
* Created on: $(date) Author: zhang.shenhua
*
* Modified by zsh on $(date)
*---------------------------------------------------------------------*/
#include "new.h"
#include <vmsys.h>

void* checked_malloc(int size) {
		void* p  = vm_malloc(size);											
		if(!p) {													
			vm_log_debug("memory out of range");					
			vm_exit_app();											
		}															
		return p;
}
