/****************************************************************
* FILE Name: HTextInput.h
* Description: Header file HTextInput( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.24.2011) Author: Geng,XuanXuan
****************************************************************/

#ifndef _HTEXTINPUT_H
#define _HTEXTINPUT_H

#include "hwidget.h"

typedef struct _HTextInputOperation HTextInputOperation;
typedef struct _HTextInput HTextInput;

struct _HTextInputOperation {
	/*-----------------------------
	 Function Name: set_text
	 Description: Set text of HTextInput
	 Para(in) p_textinput: Pointer of HTextInput self
	 Para(in) pc_text: Text of HTextInput
	 ------------------------------*/
	void (*set_text)(HTextInput *p_textinput, char *pc_text);

	/*-----------------------------
	 Function Name: get_text
	 Description: Get text of HTextInput
	 Para(in) p_htextinput: Pointer of HTextInput self
	 Return: Text of HTextInput
	  -----------------------------*/
	char* (*get_text)(HTextInput *p_textinput);

	/*-----------------------------
	Function Name: get_len
	Description: Get length of HTextInput
	Para(in) p_htextinput: Pointer of HTextInput self
	Return: Text of HTextInput
	-----------------------------*/
	int (*get_len)(HTextInput *p_textinput);

	/*-----------------------------
	 Function Name: set_input_method
	 Description: Set default input method of HTextInput
	 Para(in) p_htextinput: Pointer of HTextInput self
	 Para(in) i_input_method: Default input method of HTextInput
	-----------------------------*/
	void (*set_input_method)(HTextInput *p_textinput, int i_input_method);

	/*-----------------------------
	Function Name: set_is_password
	Description: Set password flag of HTextInput
	Para(in) p_htextinput: Pointer of HTextInput self
	Para(in) i_password: password flag of HTextInput 
			0 false;  1 true
	-----------------------------*/
	void (*set_is_password)(HTextInput *p_textinput, int i_password);
};

struct _HTextInput {
	/************************* public member **********************/
	/* Base class */
	HWidget base;

	/* HTextInput Operations */
	HTextInputOperation *p_textinput_ops;

	/************************* public member **********************/
	/* Text of HTextInput */
	char *pc_text;

	/* Flag of password model */
	char c_is_password;

	/* Default input method */
	char c_input_method;

	/* x axis move offset */
	short s_move_offset;
};

/*-----------------------------
 Function Name: htextinput_new
 Description: Get a new HTextInput
 Return: Pointer of new HTextInput
-----------------------------*/
extern HTextInput * htextinput_new();

/*-----------------------------
 Function Name: htextinput_new
 Description: Get a new HTextInput
 Para(in) p_htextinput: Pointer of HTextInput self
-----------------------------*/
extern void htextinput_delete(HTextInput *p_textinput);

/*------------------------------
 Function: htextinput_ops_delete
 Description: delete HTextInput global operations
--------------------------------*/
extern void htextinput_ops_delete();


#endif

/********************************EOF***********************************/