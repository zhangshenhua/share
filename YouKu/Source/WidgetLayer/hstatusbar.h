/*----------------------------------------------------------------
 *	File Name	: hstatusbar.h
 *	Description	: header file of the class HStatusBar
 *	COPYRIGHT	: hiSoft
 *	Created on	: 2011-8-22
 *	Modified by	: Xie Liang
 ---------------------------------------------------------------*/

#ifndef _HSTATUSBAR_H
#define _HSTATUSBAR_H


#include "hcontainer.h"
#include "../type_and_macro.h"
#include "vmsys.h"


typedef struct _HStatusBar_				HStatusBar;
typedef struct _HStatusBarOperation_	HStatusBarOperation;


struct _HStatusBar_
{
	/* Base class */
	HContainer base;

	// Its operation pointer
	HStatusBarOperation *p_oper;
};


struct _HStatusBarOperation_
{
	/*	Function Name		: set_ls_key
		Description			: Set the text(color) and the callback function of the left soft key.
		Para(in) p_this		: Pointer to the HStatusBar object itself
		Para(in) text		: the string of the LS key
		Para(in) color		: the color of the text
		Para(in) pf_action	: the action executed when the LS key is pressed. */	
	void (*set_ls_key)(HStatusBar *p_this, VMWSTR text, VMINT color, PF_Action pf_action);

	/*	Function Name		: set_rs_key
		Description			: Set the text(color) and the callback function of the right soft key.
		Para(in) p_this		: Pointer to the HStatusBar object itself
		Para(in) text		: the string of the RS key
		Para(in) color		: the color of the text
		Para(in) pf_action	: the action executed when the RS key is pressed. */	
	void (*set_rs_key)(HStatusBar *p_this, VMWSTR text, VMINT color, PF_Action pf_action);

	/*	Function Name		: set_ms_key
		Description			: Set the text(color) and the callback function of the middle soft key.
		Para(in) p_this		: Pointer to the HStatusBar object itself
		Para(in) text		: the string of the middle key
		Para(in) color		: the color of the text
		Para(in) pf_action	: the action executed when the middle key is pressed. */	
	void (*set_ms_key)(HStatusBar *p_this, VMWSTR text, VMINT color, PF_Action pf_action);
};


/*	Function Name	: hstatusbar_init
	Description		: Initialize a existed HStatusBar object
	Para(in) p_Self	: The pointer of the object */	
void hstatusbar_init( HStatusBar* p_this );


#endif