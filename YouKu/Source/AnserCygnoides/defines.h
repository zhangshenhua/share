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


typedef char * cstr;
#define classdef(typename) typedef struct typename typename; \
						   struct typename
#define private
#define public


#endif