/*---------------------------------
* FILE Name: business.c
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: zhang.shenhua
*
* Modified by zsh on $(date)
*---------------------------------------------------------------------*/
#if(1)

#include "http.h"
#include "MainPage.h"
#include "connectpage.h"
#include "Prompt.h"
#include "video_detail.h"
#include "business.h"
#include "hwindow.h"

ConnectPage* p_connectpage;
PromptPage*  p_promptpage;
MainPage*    p_MainPage;


void businss()
{
	
	p_connectpage = ConnectPage_new();
	OpenPage( p_connectpage );
	

}
#endif
