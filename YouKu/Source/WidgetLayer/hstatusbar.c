#include "hstatusbar.h"


/***************** Static variables: ***********************/
static HStatusBarOperation s_StatusBarOper;


/***************** Static functions: ***********************/
static void UI_StatusBar_SetLS(HStatusBar *p_this, VMWSTR text, VMINT color, PF_Action pf_action)
{
	return;
}


void UI_StatusBar_SetRS(HStatusBar *p_this, VMWSTR text, VMINT color, PF_Action pf_action)
{
	return;
}


void UI_StatusBar_SetMS(HStatusBar *p_this, VMWSTR text, VMINT color, PF_Action pf_action)
{
	return;
}


static HStatusBarOperation* UI_StatusBar_Get_Oper()
{
	static int i_Flag = FALSE;

	if( FALSE == i_Flag )
	{
		i_Flag = TRUE;

		s_StatusBarOper.set_ls_key = UI_StatusBar_SetLS;
		s_StatusBarOper.set_rs_key = UI_StatusBar_SetRS;
		s_StatusBarOper.set_ms_key = UI_StatusBar_SetMS;
	}

	return &s_StatusBarOper;
}

/***************** External functions: ***********************/
void hstatusbar_init(HStatusBar* p_this)
{
	if ( NULL == p_this )
		return;

	hcontainer_init( &(p_this->base) );			// initialize its base
	p_this->p_oper = UI_StatusBar_Get_Oper();	// get the operation pointer
}