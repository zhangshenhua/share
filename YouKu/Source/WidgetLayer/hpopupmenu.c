#include "hpopupmenu.h"
#include "vmsys.h"


/***************** Static variables: ***********************/
static HPopupMenuOperation s_PopupMenuOper;


/***************** Static functions below: ***********************/
static void UI_PopupMenu_Show( HPopupMenu* p_Self, VMSHORT x, VMSHORT y )
{
	return;
}

static void UI_PopupMenu_Hide( HPopupMenu* p_Self )
{
	return;
}

static HPopupMenuOperation* UI_PopupMenu_Get_Oper()
{
	static int i_Flag = FALSE;

	if( FALSE == i_Flag )
	{
		i_Flag = TRUE;

		s_PopupMenuOper.show_popupmenu	= UI_PopupMenu_Show;
		s_PopupMenuOper.hide_popupmenu	= UI_PopupMenu_Hide;
	}

	return &s_PopupMenuOper;
}


/***************** External functions below: ***********************/
void hpopupmenu_init( HPopupMenu *p_this )
{
	if ( NULL == p_this )
		return;

	hcontainer_init( &(p_this->base) );			// initialize its base
	p_this->p_oper = UI_PopupMenu_Get_Oper();	// get the operation point
}