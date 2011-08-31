/****************************************************************
* FILE Name: HTextArea.c
* Description: Source file HTextArea( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.24.2011) Author: Geng,XuanXuan
****************************************************************/

#include "htextarea.h"

#include <string.h>
#include <stdlib.h>
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmsys.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"

static HTextAreaOperation *p_textarea_ops;		/* Single instance of HTextArea Operations  */
static HWidgetOperation *p_base_ops;			/* Single instance of HWidget Operations  */
static i_ops;							/* Number of HTextArea instances */

static HTextArea *p_textarea_buf;		/* template pointer of HTextArea for input method callback */

/* HTextArea Operations */

/* Set text to HTextArea */ 
static void set_text(HTextArea *p_textarea, char *pc_text)
{
	int i_len;

	if (!pc_text)
		return;

	i_len = strlen(pc_text);

	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	p_textarea->pc_text = (char *)vm_malloc(i_len + 1);
	memcpy(p_textarea->pc_text, pc_text, i_len);
	p_textarea->pc_text[i_len + 1] = '\0';
}

/* Get text from HTextArea */
static char* get_text(HTextArea *p_textarea)
{
	return p_textarea->pc_text;
}

/* Insert text to HTextArea's current cursor position */
static void insert_text(HTextArea *p_textarea, char *pc_text)
{
	int i_len;
	char *p_buf;
	char *p_index;

	if (!pc_text)
		return;

	i_len = strlen(p_textarea->pc_text) + strlen(pc_text);
	i_len = i_len > p_textarea->i_max_len ? p_textarea->i_max_len : i_len;

	p_buf = (char *)vm_malloc(sizeof(i_len + 1));

	p_index = p_buf;
	memcpy(p_index, p_textarea->pc_text, strlen(p_textarea->pc_text));
	p_index += strlen(p_textarea->pc_text);
	memcpy(p_index, pc_text, strlen(p_textarea->pc_text));
	p_buf[i_len + 1] = '\0';

	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	p_textarea->pc_text = p_buf;	
}

/* Get length HTextArea's text */
static int get_text_len(HTextArea *p_textarea)
{
	VMWSTR w_str;
	int i_len;
	i_len = strlen(p_textarea->pc_text);

	w_str = (VMWSTR)vm_malloc(i_len * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, i_len * sizeof(VMWCHAR), p_textarea->pc_text);
	i_len = vm_wstrlen(w_str);

	vm_free(w_str);

	return i_len;
}

/* Set input method of HTextArea */
static void set_inputmethod(HTextArea *p_textarea, int i_method)
{
	p_textarea->c_input_method = i_method;
}

/* Set line number of HTextArea */
static void set_line_num(HTextArea *p_textarea, int i_line_num)
{
	p_textarea->c_line_num = i_line_num;
}

/* Set split line flag to HTextArea */
static void set_split_line(HTextArea *p_textarea, int i_split_line)
{
	p_textarea->c_is_split_line = i_split_line;
}

/* Set line space of HTextArea */
static void set_line_space(HTextArea *p_textarea, int i_line_space)
{
	p_textarea->c_line_space = i_line_space;
}

/* Internal functions */

/* Input method callback function */
void input_method_callback(VMINT state, VMWSTR str)
{
	int i_len;
	char *p_buf;

	if (VM_INPUT_OK != state)
		return;

	p_buf = (char *)vm_malloc(vm_wstrlen(str) * sizeof(VMWCHAR));
	vm_ucs2_to_gb2312(p_buf, vm_wstrlen(str) * sizeof(VMWCHAR), str);

	i_len = strlen(p_buf);
	p_textarea_buf->pc_text = (char *)vm_malloc(i_len + 1);
	memcpy(p_textarea_buf->pc_text, p_buf, i_len);
	p_textarea_buf->pc_text[i_len + 1] = '\0';

	vm_free(p_buf);
}

/* Call input method */
void call_input_method(HTextArea *p_textarea)
{
	VMWSTR str;
	str = (VMWSTR)vm_malloc(strlen(p_textarea->pc_text) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(str, strlen(p_textarea->pc_text) * sizeof(VMWCHAR), p_textarea->pc_text);

	p_textarea_buf = p_textarea;
	vm_input_text3(str, p_textarea->i_max_len, p_textarea->c_input_method, input_method_callback);

	p_textarea_buf = NULL;
	p_textarea->base.p_widget_ops->repaint((HWidget *)p_textarea);

	vm_free(str);

	p_textarea->base.action_performed((HWidget *)p_textarea, NULL);
}

/* Base class Operations */
/* Pen press event callback */
static void pen_press(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

	if (!p_textarea->base.p_widget_ops->is_enable(p_widget))
		return;

	if (p_textarea->base.p_widget_ops->is_enable_focus(p_widget))
		p_textarea->base.p_widget_ops->set_focus(p_widget, 1);

	/************************************************************************/
	/*Get cursor position by pen event.                                     */
	/************************************************************************/

	p_textarea->base.p_widget_ops->repaint(p_widget);
}

/* Pen release event callback */
static void pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

	if (!p_textarea->base.p_widget_ops->is_enable(p_widget))
		return;

	if (p_textarea->base.p_widget_ops->has_focus(p_widget))
		call_input_method(p_textarea);
}

/* Pen move event callback */
static void pen_move(HWidget *p_widget, short s_x, short s_y)
{
	/************************************************************************/
	/* Nothing to do                                                        */
	/************************************************************************/
}

/* Keyboard press event callback */
static void key_press(HWidget *p_widget, int keycode)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

	if (!p_textarea->base.p_widget_ops->is_enable(p_widget))
		return;

	if (VM_KEY_OK == keycode)
		call_input_method(p_textarea);

	if (VM_KEY_LEFT == keycode && p_textarea->i_current_position > 0)
		p_textarea->i_current_position -= 1;

	if (VM_KEY_RIGHT == keycode && p_textarea->i_current_position < p_textarea->i_max_len)
		p_textarea->i_current_position += 1;

	p_textarea->base.p_widget_ops->repaint(p_widget);
}

/* Whether the HCheckBox is a container */
static int is_container(HWidget *p_widget)
{
	return 0;
}

/* Whether the HCheckBox is a plane */
static int is_plane(HWidget *p_widget)
{
	return 0;
}

/* Get type of HTextArea */
static HClass get_class(HWidget *p_widget)
{
	return CLASS_TEXTAREA;
}

/* Get the widget preferred width */
static short get_prefered_width(HWidget *p_widget)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

	if (p_textarea->base.s_prefered_width)
		return p_textarea->base.s_prefered_width;

	p_textarea->base.s_prefered_width = p_textarea->base.p_widget_ops->get_max_width(p_widget);

	return p_textarea->base.s_prefered_width;
}

/* Get the widget preferred height */
static short get_prefered_height(HWidget *p_widget)
{
	int i_line_height;
	int i_prefered_height;
	char c_font;
	HTextArea *p_textarea = (HTextArea *)p_widget;

	if (p_textarea->base.s_prefered_height)
		return p_textarea->base.s_prefered_height;

	c_font = (p_textarea->base.c_font & 7) >> 1;
	vm_graphic_set_font(c_font);

	i_line_height = vm_graphic_get_character_height(); 

	i_prefered_height = i_line_height * p_textarea->c_line_num + (p_textarea->c_line_num - 1) * p_textarea->c_line_space + 
		p_textarea->base.uc_padding_top + p_textarea->base.uc_padding_bottom;

	p_textarea->base.s_prefered_height = i_prefered_height;

	return i_prefered_height;
}

/* Paint the HCheckBox */
static void paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

	/************************************************************************/
	/* Todo: draw HTextArea                                                 */
	/************************************************************************/
}

/* Destroy a HWidget */ 
static void hwidget_delete(HWidget *p_widget)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

	--i_ops;

	if (0 == i_ops) {
		if (p_textarea_ops)
			vm_free(p_textarea_ops);
		if (p_base_ops)
			vm_free(p_base_ops);
	}

	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	if(p_textarea)
		vm_free(p_textarea);
}

/* Init a new HTextArea */
extern HTextArea * htextarea_new(int i_input_method, int i_max_len)
{
	HTextArea *p_text_area = (HTextArea *)vm_malloc(sizeof(HTextArea));
	++i_ops;

	hwidget_init(&(p_text_area->base));

	if (!p_textarea_ops) {
		p_textarea_ops = (HTextAreaOperation *)vm_malloc(sizeof(HTextAreaOperation));
		
		p_textarea_ops->get_text = get_text;
		p_textarea_ops->set_text = set_text;
		p_textarea_ops->insert_text = insert_text;
		p_textarea_ops->get_text_len = get_text_len;
		p_textarea_ops->set_inputmethod = set_inputmethod;
		p_textarea_ops->set_line_num = set_line_num;
		p_textarea_ops->set_split_line = set_split_line;
		p_textarea_ops->set_line_space = set_line_space;
	}

	if (!p_base_ops) {
		p_base_ops = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		hwidget_operation_init(p_base_ops);
		
		p_base_ops->pen_press = pen_press;
		p_base_ops->pen_release = pen_release;
		p_base_ops->pen_move = pen_move;
		p_base_ops->key_press = key_press;
		p_base_ops->is_container = is_container;
		p_base_ops->is_plane = is_plane;
		p_base_ops->get_class = get_class;
		p_base_ops->get_prefered_width = get_prefered_width;
		p_base_ops->get_prefered_height = get_prefered_height;
		p_base_ops->paint = paint;
		p_base_ops->destroy = hwidget_delete;
	}

	/* Init private members */
	p_text_area->c_line_num = 4;
	p_text_area->c_is_split_line = 0;
	p_text_area->c_line_space = 4;

	if (i_input_method < 0)
		p_text_area->c_input_method = VM_INPUT_METHOD_TEXT;
	else
		p_text_area->c_input_method = i_input_method;

	if (i_max_len < 0)
		p_text_area->i_max_len = 140;
	else
		p_text_area->i_max_len = i_max_len;

	p_text_area->base.p_widget_ops = p_base_ops;
	p_text_area->p_htextarea_ops = p_textarea_ops;

	/* init HWiget attr */
	p_text_area->base.uc_padding_left = 8;
	p_text_area->base.uc_padding_right = 8;
	p_text_area->base.uc_padding_top = 8;
	p_text_area->base.uc_padding_bottom = 8;
	p_text_area->base.c_font = VM_SMALL_FONT;

	return p_text_area;
}

/* Destroy a HTextArea */
extern void htextarea_delete(HTextArea *p_textarea)
{
	--i_ops;

	if (0 == i_ops) {
		if (p_textarea_ops)
			vm_free(p_textarea_ops);
		if (p_base_ops)
			vm_free(p_base_ops);
	}

	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	if(p_textarea)
		vm_free(p_textarea);
}

/********************************EOF***********************************/