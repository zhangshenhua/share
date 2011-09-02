#if(0)

#include "../WidgetLayer/hpushbutton.h"
#include "Prompt.h"
static void FireVisible() 
{
	g_pWindow->DefaultPlane->AddWidget(pThis->m_pButton);;
}
static void init() 
{
	throw std::exception("The method or operation is not implemented.");
}
/*define reconnect the youku server */
static void RetryConnect()
{

}
//define exit the youku client.
static void exit_App()
{

}
void PromptPageInit( PromptPage* pPromptPage ) 
{
	pPromptPage->Base.fire_visible=FireVisible();
	pPromptPage->Base.init=init();
	//init the container
	pPromptPage->Prompt_Container=hcontainer_new();
	pPromptPage->Prompt_Container->base.s_top_x=Prompt_Container_Top_X ;
	pPromptPage->Prompt_Container->base.s_top_y=Prompt_Container_Top_Y;
	pPromptPage->Prompt_Container->base.s_height=Prompt_Container_Height;
	pPromptPage->Prompt_Container->base.s_width=Prompt_Container_Width;
	pPromptPage->Prompt_Container->p_container_ops->add_widget(pPromptPage->Prompt_Container,pPromptPage->retry_Button);
	pPromptPage->Prompt_Container->p_container_ops->add_widget(pPromptPage->Prompt_Container,pPromptPage->exit_Button);
	
	//init the retry button
	pPromptPage->retry_Button=hpushbutton_new();/*hpushbutton_new*/
	if (!pPromptPage->retry_Button)
	{
		return NULL;
	}
	pPromptPage->retry_Button->text_height=Retry_Txt_Height;
	pPromptPage->retry_Button->text_width =Retry_Txt_Width;
	pPromptPage->retry_Button->set_image_from_resource(pPromptPage->retry_Button,"Retry button file name");
    pPromptPage->retry_Button->base.action_performed=RetryConnect();
	//init the exit button
	pPromptPage->exit_Button=hpushbutton_new();/*hpushbutton_new()*/
	if (!pPromptPage->retry_Button)
	{
		return NULL;
	}
	pPromptPage->retry_Button->text_height=Exit_Txt_Height;
	pPromptPage->retry_Button->text_width =Exit_Txt_Width;
	pPromptPage->retry_Button->set_image_from_buffer(pPromptPage->exit_Button,"exit button file name")
	pPromptPage->retry_Button->base.action_performed=exit_App();
}
PromptPage* PromptPage_new() 
{
	PromptPage* pPromptPage=vm_malloc(sizeof(PromptPage));
	if (!pPromptPage)
	{
		return NULL;
	}
	PromptPageInit(pPromptPage);
	return pPromptPage;

}


PromptPage* PromptPage_delete()
{

}

#endif