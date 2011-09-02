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
#include "../PageLayer/page.h"
#include "../WidgetLayer/hplane.h"
#include "../WidgetLayer/hcontainer.h"
#include "../WidgetLayer/hpushbutton.h"


#define MAIN_BUTTON1_S_TOP_X   10
#define MAIN_BUTTON1_S_TOP_Y   10
#define MAIN_BUTTON2_S_TOP_X   10
#define MAIN_BUTTON2_S_TOP_Y   10


classdef(MainPage) 
{
	Page base;
	HContainer* p_container_total;
	HContainer* p_container_boby;
	HContainer* p_container_buttongroup;       //area for recommend,search,.....
	
	HPushButton* pbutton1;				//point to recommend button 
	HPushButton* pbutton2;				//point to category button	
	HPushButton* pbutton3;				//point to search button
	HPushButton* pbutton4;				//point to top(play) button
	HPushButton* pbutton5;				//point to my-youku button
	
	void (*show)(MainPage* this);
};

extern MainPage* MainPage_new();
extern void MainPage_delete(MainPage*);


#endif