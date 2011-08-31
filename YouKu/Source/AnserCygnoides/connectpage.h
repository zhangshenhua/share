/*---------------------------------
* FILE Name:
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: Name
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
	
	HImage   logoimage;
	int x;
	
	int (*connect) (  /* some string abort connect context */ );

};

ConnectPage* ConnectPage_new();		 //construct function
void ConnectPage_delete();				 //destruct function

 
#endif
