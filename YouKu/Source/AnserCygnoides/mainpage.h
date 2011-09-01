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
#include "hcontainer.h"

classdef(MainPage) 
{
	Page base;
	HContainer* p_container_total;
	HContainer* p_container_boby;
	HContainer* p_container_buttongroup;       //area for recommend,search,.....
	
	HPushButton pbutton1;				//point to recommend button 
	HPushButton pbutton2;				//point to category button	
	HPushButton pbutton3;				//point to search button
	HPushButton pbutton4;				//point to top(play) button
	HPushButton pbutton5;				//point to my-youku button
	
};

extern MainPage* MainPage_new();
extern void MainPage_delete();


#endif