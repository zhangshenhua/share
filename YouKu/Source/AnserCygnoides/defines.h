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
#include <assert.h>
#include "hwidget.h"

typedef char* cstr;
typedef HRect	Rect;

#define classdef(typename) typedef struct typename typename; \
						   struct typename



#endif