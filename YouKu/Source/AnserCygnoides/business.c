ConnectPage* p_connectpage;
PromptPage*  p_promptpage;
MainPage*    p_MainPage;

void businss()
{
	p_connectpage = ConsConnectPage();

	if(p_connectpage->connect( /* some string abort connect context */  )) {
		DesConnectPage(p_connectpage);
		p_MainPage = ConsMainPage();
	}
	else
	{
		p_promptpage = ConsPromptPage();
	}
}

