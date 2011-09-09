/*---------------------------------
* FILE Name: connectpage.h
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: shenhua.zhang
*
* Modified by name on $(date)
*---------------------------------------------------------------------*/
#ifndef _CONNECT_PAGE_H
#define _CONNECT_PAGE_H


#include "defines.h"
#include "page.h"
#include "himage.h"


classdef(ConnectPage)
{
	Page    base;        //
	
	HImage*	p_logoimage;

};

ConnectPage* ConnectPage_new();		 //construct function
extern ConnectPage* g_p_connectpage;

 
#endif
