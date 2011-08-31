/***** Class Definition *********/
#include "page.h"
#include "hpushbutton.h"

#define Retry_Txt_Height ;
#define Retry_Txt_Width  ;
#define Exit_Txt_Height  ;
#define Exit_Txt_Width ;
#define Prompt_Container_Top_X ;
#define Prompt_Container_Top_Y ;
#define Prompt_Container_Height ;
#define Prompt_Container_Width ;


typedef struct _Prompt_Page
{
	Page Base;
	HImage* Prompt_Image;
	HContainer* Prompt_Container;
	HPushButton* retry_Button;
	HPushButton* exit_Button;
}PromptPage;

extern PromptPage* PromptPage_new();
extern PromptPage* PromptPage_delete();

