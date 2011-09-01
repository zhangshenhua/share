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
#include "hpushbutton.h"
#include "../type_and_macro.h"
#include "vmsys.h"


typedef struct _HStatusBar_				HStatusBar;
typedef struct _HStatusBarOperation_	HStatusBarOperation;


typedef enum _ENUM_SOFTKEY_TYPE 
{
	SOFTKEY_LEFT = 0,
	SOFTKEY_MID,
	SOFTKEY_RIGHT,
	SOFTKEY_MAX

} ENUM_SOFTKEY_TYPE;


struct _HStatusBar_
{
	// Base class
	HContainer base;

	// Its operation pointer
	HStatusBarOperation *p_oper;

	// Private members:
	HPushButton_P p_Btns[ SOFTKEY_MAX ];	// Three buttons for the LEFT / MIDDLE / RIGHT soft keys.
};


struct _HStatusBarOperation_
{
	/*	Function Name		: set_soft_key
		Description			: Set the text(&color) and the callback function of the soft key.
		Para(in) p_this		: Pointer to the HStatusBar object itself
		Para(in) e_type		: which soft key to set?
		Para(in) text		: the string of the key
		Para(in) color		: the color of the text
		Para(in) pf_action	: the action executed when the key is pressed. */	
	void (*set_soft_key)(HStatusBar *p_this, ENUM_SOFTKEY_TYPE e_type, VMWSTR text, VMINT color, void (*pf_action)(HWidget*,void*));

	/*	
		Function Name:
			clean
		Description: 
			The destructor of this class
		Parameters:
			p_this: Pointer of HStatusBar self	*/	
	void (*clean)(HStatusBar *p_this);
};


/*	Function Name	: hstatusbar_init
	Description		: Initialize a existed HStatusBar object
	Para(in) p_Self	: The pointer of the object 
	Note			: This is the first function you must call if you want to use a HStatusBar object !!
	*/	
void hstatusbar_init( HStatusBar* p_this );



/*	A example:
	{
		HStatusBar bar;
		hstatusbar_init( &bar );

		// ...

		bar.clean( &bar );
	}
*/


#endif