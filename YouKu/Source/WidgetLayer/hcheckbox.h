/**********************************************************************
* FILE Name: HCheckBox.h
* Description: Header file HCheckBox( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.19.2011) Author: Geng,XuanXuan
***********************************************************************/

#ifndef _HCHECKBOX_H
#define _HCHECKBOX_H

#include "hwidget.h"

#define HCHECKBOX_SELECTED 'S'           /* Selected state of HCheckBox */
#define HCHECKBOX_UNSELECTED 'U'		 /* UnSelected state of HCheckBox */

typedef struct HCheckBox_t HCheckBox;


/* HCheckBox Operations */
typedef struct HCheckBoxOperation_t {
	/* 
	Function Name: set_label
	Description: Callback function that set label in HCheckBox
	Para(in) p_checkbox: Pointer of HCheckBox self
	Para(in) pc_label: Label of HCheckBox
	*/
	void (*set_label)(HCheckBox *p_checkbox, char *pc_label);

	/*
	Function Name: get_label
	Description: Callback function that get label if the HCheckBox
	Para(in) p_checkbox: Pointer of HCheckBox self
	Return: Label of HCheckBox
	*/
	char* (*get_label)(HCheckBox *p_checkbox);

	/*
	Function Name: set_check_state
	Description: Callback function that set check state to HCheckBox
	Para(in) p_checkbox: Pointer of HCheckBox self
	Para(in) c_check_state: check state of HCheckBox
	HCHECKBOX_SELECTED,			Selected state of HCheckBox
	HCHECKBOX_UNSELECTED,		UnSelected state of HCheckBox
	*/
	void (*set_check_state)(HCheckBox *p_checkbox, char c_check_state);

	/*
	Function Name: get_check_state
	Description: Callback function that get check state of the HCheckBox
	Para(in) p_checkbox: Pointer of HCheckBox self
	Return: Check state of the HCheckBox
	*/
	char (*get_check_state)(HCheckBox *p_checkbox);

} HCheckBoxOperation;

typedef struct HCheckBox_t {
	/************************* public member ***********************/
	/* Base class */
	HWidget base;

	/* Pointer of HCheckBoxOperation */
	HCheckBoxOperation *p_checkbox_ops;

	/************************* private member **********************/
	/* Label in HCheckBox */
	char *pc_label;

	/* State of HCheckBox */
	char c_check_state;
	
} HCheckBox;

/*
 Function Name: hcheckbox_new
 Description: Get a New HCheckBox
 Para(in) pc_label: Label in HCheckBox
 Return: A pointer of HCheckBox
*/
extern HCheckBox* hcheckbox_new(const char* pc_label);

/*
 Function Name: hcheckbox_destroy
 Description: Destroy a HCheckBox
 Para(in) p_checkbox: A pointer of HCheckBox
*/
extern void hcheckbox_delete(HCheckBox *p_checkbox);

/*
 Function Name: hcheckbox_ops_delete
 Description: delete HCheckBox global operatons 
*/
extern void hcheckbox_ops_delete();

#endif
/********************************EOF***********************************/