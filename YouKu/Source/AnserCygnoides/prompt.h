 /**********************************************************************
* FILE Name: Prompt.h
* Description: 
* COPYRIGHT: Hisoft
* Created on: $(8.31.2011) Author: Yan Guanglei
***********************************************************************/
/***** Class Definition *********/
#include "page.h"
#include "hpushbutton.h"
#define Retry_Button_X
#define Retry_Button_Y
#define Retry_Button_Height 
#define Retry_Button_Width  
#define Exit_Button_X
#define Exit_BUtton_Y
#define Exit_Button_Height  
#define Exit_Button_Width 
#define Prompt_Container_Top_X 
#define Prompt_Container_Top_Y 
#define Prompt_Container_Height 
#define Prompt_Container_Width 


typedef struct _Prompt_Page
{
	Page *Base;
	HImage* Prompt_Image;
	HContainer* Prompt_Container;
	HPushButton* retry_Button;
	HPushButton* exit_Button;
}PromptPage;

extern PromptPage* PromptPage_new();
extern PromptPage* PromptPage_delete();

