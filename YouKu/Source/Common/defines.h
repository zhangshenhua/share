/*---------------------------------
* FILE Name: mainpage.h
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: zhang.shenhua
*
* Modified by zsh on $(date)
*---------------------------------------------------------------------*/
#ifndef DEFINES_H_
#define DEFINES_H_
#include <vmsys.h>
#include <vmvideo.h>


typedef char*	CString;
typedef char	bool;

#define classdef(typename) typedef struct typename typename; \
						   struct typename


#define SCREEN_WIDTH    vm_graphic_get_screen_width()
#define SCREEN_HEIGHT	vm_graphic_get_screen_height()

#endif