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
#if(1)
#include "MainPage.h"
#include "connectpage.h"


ConnectPage* p_connectpage;
PromptPage*  p_promptpage;
MainPage*    p_MainPage;

void businss()
{

	p_connectpage = ConnectPage_new();

	if(p_connectpage->connect( /* some string abort connect context */  )) {
		ConnectPage_delete(p_connectpage);
		p_MainPage = MainPage_new();
	}
	else
	{
		p_promptpage = PromptPage_new();
	}

}
#endif
