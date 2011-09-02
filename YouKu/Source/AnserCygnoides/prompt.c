/**********************************************************************
* FILE Name: Prompt.c
* Description: 
* COPYRIGHT: Hisoft
* Created on: $(8.31.2011) Author: Yan Guanglei
***********************************************************************/
#include "../WidgetLayer/hpushbutton.h"
#include "Prompt.h"
/*page visible*/
static void Base_FireVisible(Page *p_page) 
{
	
}
/*page init */
static void Baes_init(Page *p_page) 
{
	
}
/*define reconnect the you ku server */
static void RetryConnect()
{

}
//define exit the youku client.
static void exit_App()
{

}
static void PromptPageInit( PromptPage* pPromptPage ) 
{
	
	pPromptPage->Base.i_page_type=0;
	pPromptPage->Base.i_page_id=1;
	pPromptPage->Base.fire_visible=Base_FireVisible(pPromptPage->Base);
	pPromptPage->Base.init=Baes_init(pPromptPage->Base);

	//init the container
	pPromptPage->Prompt_Container=hcontainer_new();
	pPromptPage->Prompt_Container->base.s_top_x=Prompt_Container_Top_X ;
	pPromptPage->Prompt_Container->base.s_top_y=Prompt_Container_Top_Y;
	pPromptPage->Prompt_Container->base.s_height=Prompt_Container_Height;
	pPromptPage->Prompt_Container->base.s_width=Prompt_Container_Width;
	pPromptPage->Prompt_Container->p_container_ops->add_widget(pPromptPage->Prompt_Container,(HWidget*)pPromptPage->retry_Button);
	pPromptPage->Prompt_Container->p_container_ops->add_widget(pPromptPage->Prompt_Container,(HWidget*)pPromptPage->exit_Button);
	
	//init the retry button
	pPromptPage->retry_Button=hpushbutton_new();/*hpushbutton_new*/
	if (!pPromptPage->retry_Button)
	{
		return NULL;
	}
	pPromptPage->retry_Button->base.s_top_x=Retry_Button_X;
	pPromptPage->retry_Button->base.s_top_y=Retry_Button_Y;
	pPromptPage->retry_Button->base.s_height=Retry_Button_Height;
	pPromptPage->retry_Button->base.s_width =Retry_Button_Width;
	pPromptPage->retry_Button->ops->set_image_from_resource(pPromptPage->retry_Button,"Retry button file name");
    pPromptPage->retry_Button->base.action_performed=RetryConnect();
	//init the exit button
	pPromptPage->exit_Button=hpushbutton_new();/*hpushbutton_new()*/
	if (!pPromptPage->retry_Button)
	{
		return NULL;
	}
	pPromptPage->exit_Button->base.s_top_x=Exit_Button_X;
	pPromptPage->exit_Button->base.s_top_y =Exit_BUtton_Y;
	pPromptPage->exit_Button->base.s_height=Exit_Button_Height;
	pPromptPage->exit_Button->base.s_width=Exit_Button_Width;
	pPromptPage->exit_Button->ops->set_image_from_buffer(pPromptPage->exit_Button,"exit button file name")
	pPromptPage->exit_Button->base.action_performed=exit_App();
}
PromptPage* PromptPage_new() 
{
	PromptPage* pPromptPage=vm_malloc(sizeof(PromptPage));
	if (!pPromptPage)
	{
		return NULL;
	}
	PromptPage->Base->init= PromptPageInit;
	//PromptPageInit(pPromptPage);
	return pPromptPage;

}


PromptPage* PromptPage_delete()
{

}