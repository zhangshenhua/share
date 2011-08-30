#ifndef _CONNECT_PAGE_H
#define _CONNECT_PAGE_H

//#include "hpage.h"
#include "defines.h"

#include "himage.h"
classdef(ConnectPage)
{
	//HPage    base;        //
	
	HImage   logoimage;
	int x;
	
	int (*connect) (  /* some string abort connect context */ );

};

ConnectPage* ConsConnectPage();		 //construct function
void DesConnectPage();				 //destruct function

 
#endif
