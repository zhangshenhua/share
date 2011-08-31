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
#ifndef _MAINPAGE_H_
#define _MAINPAGE_H_
#include "defines.h"
#include "page.h"
#include "hplane.h"

classdef(MainPage) 
{
	Page base;
	
	HPlane* p_plane_boby;
	HPlane* p_plane_buttongroup;       //area for recommend,search,.....
	
};

extern MainPage* MainPage_new();
extern void MainPage_delete();


#endif