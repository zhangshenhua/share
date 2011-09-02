/*---------------------------------
* FILE Name: new.h
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: zhang.shenhua
*
* Modified by zsh on $(date)
*---------------------------------------------------------------------*/
#ifndef _NEW_H_
#define _NEW_H_
#include <vmlog.h>
#include <vmsys.h>
extern void* vm_check_malloc(int);

#define VM_CHECK_MALLOC(p,n)									\
	do{																\
		p = vm_malloc(n);											\
		if(!(p)) {													\
			vm_log_debug("memory out of range");					\
			vm_exit_app();											\
		}															\
	}while(0);

#define VM_NEW(p,typename)	do{VM_CHECK_MALLOC(p,sizeof(typename))}while(0);
	
#endif