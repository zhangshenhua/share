/****************************************************************
* FILE Name: HTextInput.c
* Description: Source file HTextInput( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.24.2011) Author: Geng,XuanXuan
****************************************************************/

#include "htextinput.h"

#include <string.h>
#include <stdlib.h>
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmsys.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"

#include "hlookandfeel.h"

#define TEXT_MAX_SIZE 70

static HTextInputOperation *p_textinput_ops;	/* Single instance of HTextInput Operations  */
static HWidgetOperation *p_base_ops;			/* Single instance of HWidget Operations */
static int i_ops;							/* Number of HTextInput instances */

static HTextInput *p_textinput_buf;			/* Template pointer of HTextInput for input method callback */

static void hwidget_destroy(HWidget *p_hwidget);

/* HTextInput Operations */

/* Set text to HTextInput */
static void set_text(HTextInput *p_textinput, char *pc_text)
{
	int i_len;

	if (!pc_text)
		return;

	i_len = strlen(pc_text);

	if (p_textinput)
		vm_free(p_textinput->pc_text);

	p_textinput->pc_text = (char *)vm_malloc(i_len + 1);
	strncpy(p_textinput->pc_text, pc_text, i_len);
	p_textinput->pc_text[i_len] = '\0';
}

/* Get text from HTextInput */
static char* get_text(HTextInput *p_textinput)
{
	return p_textinput->pc_text;
}

/* Get length of HTextInput's text */
static int get_len(HTextInput *p_textinput)
{
	VMWSTR w_str;
	int i_len;
	i_len = strlen(p_textinput->pc_text);

	w_str = (VMWSTR)vm_malloc((i_len + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, (i_len + 1)* sizeof(VMWCHAR) , p_textinput->pc_text);
	i_len = vm_wstrlen(w_str);

	vm_free(w_str);

	return i_len;
}

/* Set input method of HTextInput */
static void set_input_method(HTextInput *p_textinput, int i_input_method)
{
	p_textinput->c_input_method = i_input_method;
}

/* Set password flag of HTextInput */
static void set_is_password(HTextInput *p_textinput, int i_password)
{
	p_textinput->c_is_password = i_password;

	if (1 == i_password)
		p_textinput->c_input_method |= VM_INPUT_METHOD_PASSWORD;

	if (0 == i_password) 
		p_textinput->c_input_method &= (~VM_INPUT_METHOD_PASSWORD);
}

/* Internal functions */
/* Input method callback function */
static void input_method_callback(VMINT state, VMWSTR str)
{
	int i_len;
	char *p_buf;

	if (VM_INPUT_OK != state)
		return;

	p_buf = (char *)vm_malloc((vm_wstrlen(str) + 1) * sizeof(VMWCHAR));
	vm_ucs2_to_gb2312(p_buf, (vm_wstrlen(str) + 1) * sizeof(VMWCHAR), str);

	i_len = strlen(p_buf);
	p_textinput_buf->pc_text = (char *)vm_malloc(i_len + 1);
	memcpy(p_textinput_buf->pc_text, p_buf, i_len);
	p_textinput_buf->pc_text[i_len] = '\0';

	vm_free(p_buf);

	p_textinput_buf = NULL;
}

/* Call input method by HTextInput */
static void call_input_method(HTextInput *p_textinput)
{
	VMWSTR str;
	str = (VMWSTR)vm_malloc((strlen(p_textinput->pc_text) + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(str, (strlen(p_textinput->pc_text) + 1) * sizeof(VMWCHAR), p_textinput->pc_text);

	p_textinput_buf = p_textinput;
	vm_input_text3(str, TEXT_MAX_SIZE, p_textinput->c_input_method, input_method_callback);

	p_textinput->base.p_widget_ops->repaint((HWidget *)p_textinput);

	vm_free(str);

	if (p_textinput->base.action_performed)
		p_textinput->base.action_performed((HWidget *)p_textinput, NULL);
}

/* Base class Operations */

/* Pen press event callback */
static void pen_press(HWidget *p_widget, short s_x, short s_y)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;

	if (!p_textinput->base.p_widget_ops->is_enable(p_widget))
		return;

	if (p_textinput->base.p_widget_ops->is_enable_focus(p_widget))
		p_textinput->base.p_widget_ops->set_focus(p_widget, 1);

	p_textinput->base.p_widget_ops->repaint(p_widget);
}

/* Pen release event callback */
static void pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;

	if (!p_textinput->base.p_widget_ops->is_enable(p_widget))
		return;

	if (p_textinput->base.p_widget_ops->has_focus(p_widget))
		call_input_method(p_textinput);
}

/* Pen move event callback */
static void pen_move(HWidget *p_widget, short s_x, short s_y)
{
	/*****************************************
	Nothing to do for now
	******************************************/
}

/* pen move enter callback*/
static void pen_enter(HWidget *p_widget, short s_x, short s_y)
{

}

/* pen move leave callback*/
static void pen_leave(HWidget *p_widget, short s_x, short s_y)
{

}

/* Keyboard press event callback */
static void key_press(HWidget *p_widget, int keycode)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;

	if (!p_textinput->base.p_widget_ops->is_enable(p_widget))
		return;

	if (VM_KEY_OK == keycode)
		call_input_method(p_textinput);

	p_textinput->base.p_widget_ops->repaint(p_widget);
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

/* Get HTextInput type */
static HClass get_class(HWidget *p_widget)
{
	return CLASS_TEXTINPUT;
}

/* Get the widget preferred width */
static short get_prefered_width(HWidget *p_widget)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;

	if (p_textinput->base.s_prefered_width)
		return p_textinput->base.s_prefered_width;

	p_textinput->base.s_prefered_width = p_textinput->base.p_widget_ops->get_max_width(p_widget);

	return p_textinput->base.s_prefered_width;
}

/* Get the widget preferred height */
static short get_prefered_height(HWidget *p_widget)
{
	int i_prefered_height;
	char c_font;
	HTextInput *p_textinput = (HTextInput *)p_widget;

	if (p_textinput->base.s_prefered_height)
		return p_textinput->base.s_prefered_height;

	c_font = (p_textinput->base.c_font & 7) >> 1;
	vm_graphic_set_font(c_font);

	i_prefered_height = vm_graphic_get_character_height() + 
		p_textinput->base.s_padding_top + p_textinput->base.s_padding_bottom;

	p_textinput->base.s_prefered_width = i_prefered_height;
	
	return i_prefered_height;
}

/* Paint the HCheckBox */
static void paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	look_paint_textinput((HTextInput *)p_widget, i_handle, s_screen_x, s_screen_y);
}

/* Destroy a HWidget */
static void hwidget_delete(HWidget *p_widget)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;

	--i_ops;

#if 0
	if (0 == i_ops) {
		if (p_textinput_ops)
			vm_free(p_textinput_ops);
		if (p_base_ops)
			vm_free(p_base_ops);
	}
#endif
	if (p_textinput->pc_text)
		vm_free(p_textinput->pc_text);

	if (p_textinput)
		vm_free(p_textinput);
}

/* Init a new HTextInput */
extern HTextInput * htextinput_new(int i_is_password, int i_input_method)
{
	HTextInput *p_textinput = (HTextInput *)vm_malloc(sizeof(HTextInput));
	++i_ops;

	hwidget_init(&(p_textinput->base));

	/* Init operations */
	if (!p_textinput_ops) {
		p_textinput_ops = (HTextInputOperation *)vm_malloc(sizeof(HTextInputOperation));

		p_textinput_ops->get_text = get_text;
		p_textinput_ops->set_text = set_text;
		p_textinput_ops->set_input_method = set_input_method;
		p_textinput_ops->get_len = get_len;
		p_textinput_ops->set_is_password = set_is_password;
	}

	if (!p_base_ops) {
		p_base_ops = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		hwidget_operation_init(p_base_ops);

		p_base_ops->pen_press = pen_press;
		p_base_ops->pen_release = pen_release;
		p_base_ops->pen_move = pen_move;
		p_base_ops->pen_enter = pen_enter;
		p_base_ops->pen_leave = pen_leave;
		p_base_ops->key_press = key_press;
		p_base_ops->is_container = is_container;
		p_base_ops->is_plane = is_plane;
		p_base_ops->get_class = get_class;
		p_base_ops->get_prefered_width = get_prefered_width;
		p_base_ops->get_prefered_height = get_prefered_height;
		p_base_ops->paint = paint;
		p_base_ops->destroy = hwidget_delete;
	}

	p_textinput->c_is_password = 0;

	p_textinput->c_input_method = VM_INPUT_METHOD_TEXT;

	p_textinput->p_textinput_ops = p_textinput_ops;
	p_textinput->base.p_widget_ops = p_base_ops;
	p_textinput->base.action_performed = NULL;

	/* init HWiget attr */
	p_textinput->base.s_padding_left = 8;
	p_textinput->base.s_padding_right = 8;
	p_textinput->base.s_padding_top = 8;
	p_textinput->base.s_padding_bottom = 8;
	p_textinput->base.c_font = FONT_SMALL;

	return p_textinput;
}

/* Destroy a HTextInput */
extern void textinput_delete(HTextInput *p_textinput)
{
	--i_ops;
#if 0

	if (0 == i_ops) {
		if (p_textinput_ops)
			vm_free(p_textinput_ops);
		if (p_base_ops)
			vm_free(p_base_ops);
	}
#endif

	if (p_textinput->pc_text)
		vm_free(p_textinput->pc_text);

	if (p_textinput)
		vm_free(p_textinput);
}
/********************************EOF***********************************/