/*----------------------------------------------------------------
 *	File Name	: hpopupmenu.h
 *	Description	: header file of the class HPopupMenu
 *	COPYRIGHT	: hiSoft
 *	Created on	: 2011-8-22
 *	Modified by	: Xie Liang
 ---------------------------------------------------------------*/

#ifndef _HPOPUPMENU_H
#define _HPOPUPMENU_H


#include "hcontainer.h"


typedef struct _HPopupMenu_				HPopupMenu;
typedef struct _HPopupMenuOperation_	HPopupMenuOperation;


struct _HPopupMenu_
{
	/* Base class */
	HContainer base;

	// Its operation pointer
	HPopupMenuOperation *p_oper;
};


struct _HPopupMenuOperation_
{
	/*	Function Name	: show_popupmenu
		Description		: Show the pop up menu in the specified position
		Para(in) this	: Pointer to the object itself
		Para(in) x		: the x coordinate
		Para(in) y		: the y coordinate		*/	
	void (*show_popupmenu)( HPopupMenu* this, short x, short y );

	/*	Function Name	: hide_popupmenu
		Description		: Hide the pop up menu
		Para(in) this	: Pointer to the object itself	*/		
	void (*hide_popupmenu)( HPopupMenu* this );
};


/*	Function Name
		hpopupmenu_init
	Description 
		Initialize a existed object
	Parameters
		p_this : [IN] pointer to a existed HPopupMenu object to be initialized	*/	
extern void hpopupmenu_init( HPopupMenu *p_this );


#endif