/****************************************************************
* FILE Name: HTextArea.h
* Description: Header file HTextArea( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.24.2011) Author: Geng,XuanXuan
****************************************************************/
#ifndef _HTEXTAREA_H
#define _HTEXTAREA_H

#include "hwidget.h"
#include "vmsys.h"

typedef struct _HTextAreaOperation HTextAreaOperation;
typedef struct _HTextArea HTextArea;
typedef struct _LineNode LineNode;
typedef struct _HTextGlobal HTextGlobal;

struct _LineNode {
	VMWSTR w_line_head;
	short s_line_len;

	LineNode *p_next;
};

/* HEditor Operations */
struct _HTextAreaOperation {
	/*
	Function Name: set_text
	Description: Callback function that set text to HTextArea.
	Para(in) p_htextarea: Pointer of HEditor self.
	Para(in) pc_text: Pointer of a string.
	*/
	void (*set_text)(HTextArea *p_textarea, char *pc_text);

	/*
	Function Name: get_text
	Description: Callback that get text of HTextArea.
	Para(in) p_htextarea: Pointer of HTextArea self.
	Return (VMWSTR): Pointer of HTextArea's text.
	*/
	char* (*get_text)(HTextArea *p_textarea);

	/*
	Function Name: insert_text
	Description: Insert text by current cursor position.
	Para(in) p_htextarea: Pointer of HTextArea self.
	Para(in) pc_text: text for inserting.
	*/
	void (*insert_text)(HTextArea *p_textarea, char *pc_text);


	/*
	 Function Name: delete_character
	 Description: delete character of current position
	 Para(in) p_textarea: Pointer of HTextArea self.
	*/
	void (*delete_character)(HTextArea *p_textarea);
	/*
	Function Name: get_text_len
	Description: Get length of HTextArea's text.
	Para(in) p_textarea: Pointer of HTextArea self.
	Return: length of HTextArea's text (word).
	*/
	int (*get_text_len)(HTextArea *p_textarea);

	/*
	Function Name: set_inputmethod
	Description: Set input method of HTextArea.
	Para(in) p_htextarea: Pointer of HTextArea self.
	Para(in) i_method: Input Method of HTextArea.
	*/
	void (*set_inputmethod)(HTextArea *p_textarea, int i_method);

	/*
	Function Name: set_max_len
	Description: Set max length of HTextArea's text.
	Para(in) p_htextarea: Pointer of HTextArea self.
	Para(in) i_method: Max length of HTextArea's text.
	*/
	void (*set_max_len)(HTextArea *p_textarea, int i_len);

	/*
	Function Name: set_line_num
	Description: Set line number of HTextArea.
	Para(in) p_htextarea: Pointer of HTextArea self.
	Para(in) i_line_num: Line number of HTextArea, default number is 4.
	*/
	void (*set_line_num)(HTextArea *p_textarea, int i_line_num);

	/*
	Function Name: set_split_line
	Description: Set split line flag of HEditor.
	Para(in) p_htextarea: Pointer of HEditor self.
	Para(in) i_split_line: Flag of split line  0 or 1
	*/
	void (*set_split_line)(HTextArea *p_textarea, int i_split_line);
	
	/*
	Function Name: set_line_space
	Description: Set line space of HTextArea.
	Para(in) p_htextarea: Pointer of HTextArea self.
	Para(in) i_line_num: Line space of HTextArea, default number is 4.
	*/
	void (*set_line_space)(HTextArea *p_textarea, int i_line_space);
};

struct _HTextArea {
	/************************* public member **********************/
	/* Base class */
	HWidget base;

	/* Pointer of HEditorOperation */
	HTextAreaOperation *p_htextarea_ops;

	/************************* private member **********************/
	/* HEditor text */
	char *pc_text;

	char c_text_change_flag;

	/* Number of showing HEditor's text line, default line number is 4*/
	char c_line_num;

	/* Flag of split line, default split line is non(0) */
	char c_is_split_line;

	/* Space between lines */
	char c_line_space;

	/* Set default input method for MRE input method*/
	char c_input_method;

	/* Offset for pen move event */
	short s_move_offset;

	/* Current position of cursor */
	int i_current_position;

	/* Max length of text in HEditor */
	int i_max_len;


	LineNode *p_line_list;

	LineNode *p_line_list_tail;

};

/*
 Function Name: heditor_new
 Description: Get a new HEditor
	i_method: Default input method for MRE input method
	VM_INPUT_METHOD_TEXT,		 Input text and the default input method is PINYIN 
	VM_INPUT_METHOD_NUMERIC,	 Input number and the default input method is phone number
	VM_INPUT_METHOD_ALPHABETIC,  Input English and the default input method is multitap abc
	VM_INPUT_METHOD_PASSWORD,    Output * install characters
	If i_method = -1, the default method is PINYIN (VM_INPUT_METHOD_TEXT) 
	i_max_len: Set max length of HEditor's text (word)
	If i_max_len = -2, the default length is 140
 Return: Pointer of a new HEditor
*/
extern HTextArea* htextarea_new();

/*
 Function Name: heditor_destroy
 Description: Destroy a HEditor
 Para(in) p_editor: A pointer of HEditor 
*/
extern void htextarea_delete(HTextArea *p_textarea);

/*
 Function Name: htextarea_delete
 Description: delete HTextArea global operations
*/
extern void htextarea_ops_delete();

#endif
/********************************EOF***********************************/