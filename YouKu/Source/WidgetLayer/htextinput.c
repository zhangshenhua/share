/****************************************************************
* FILE Name: HTextInput.c
* Description: Source file HTextInput( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.24.2011) Author: Geng,XuanXuan
****************************************************************/

#include "htextinput.h"
#include "hcheckbox.h"
#include "hplane.h"
#include "../WindowLayer/hwindow.h"

#include <string.h>
#include <stdlib.h>
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmsys.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmlog.h"

#include "hlookandfeel.h"

#define TEXT_MAX_SIZE 140

static HTextInputOperation *p_textinput_ops;	/* Single instance of HTextInput Operations  */
static HWidgetOperation *p_base_ops;			/* Single instance of HWidget Operations */
static short s_move_from_x;      /* x axis of pointer HTextInput's text move from */

static HTextInput *p_textinput_buf;			/* Template pointer of HTextInput for input method callback */

/* HTextInput Operations */

/* Set text to HTextInput */
static void htextinput_set_text(HTextInput *p_textinput, char *pc_text)
{
	int i_len;

#ifdef H_DEBUG
	vm_log_debug("htextinput_set_text");
#endif // H_DEBU

	if (!pc_text)
		return;

	if (p_textinput)
		vm_free(p_textinput->pc_text);

	i_len = strlen(pc_text);
	p_textinput->pc_text = (char *)vm_malloc(i_len + 1);
	strncpy(p_textinput->pc_text, pc_text, i_len);
	p_textinput->pc_text[i_len] = '\0';
}

/* Get text from HTextInput */
static char* htextinput_get_text(HTextInput *p_textinput)
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_get_text");
#endif // H_DEBU

	return p_textinput->pc_text;
}

/* Get length of HTextInput's text */
static int htextinput_get_len(HTextInput *p_textinput)
{
	int i_len;
	VMWSTR w_str;

#ifdef H_DEBUG
	vm_log_debug("htextinput_get_len");
#endif // H_DEBU
	/* convert gb2312 text to ucs2 */
	i_len = strlen(p_textinput->pc_text);
	w_str = (VMWSTR)vm_malloc((i_len + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, (i_len + 1)* sizeof(VMWCHAR) , p_textinput->pc_text);
	i_len = vm_wstrlen(w_str);

	vm_free(w_str);

	return i_len;
}

/* Set input method of HTextInput */
static void htextinput_set_input_method(HTextInput *p_textinput, int i_input_method)
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_set_input_method");
#endif // H_DEBU

	p_textinput->c_input_method = i_input_method;
}

/* Set password flag of HTextInput */
static void htextinput_set_is_password(HTextInput *p_textinput, int i_password)
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_set_is_password");
#endif // H_DEBU

	p_textinput->c_is_password = i_password;

	/* reset HTextInput's input method */
	p_textinput->c_input_method = (1 == i_password ? VM_INPUT_METHOD_PASSWORD : VM_INPUT_METHOD_ALPHABETIC);
}

/* Internal functions */
/* Input method callback function */
void htextinput_input_method_callback(VMINT state, VMWSTR str)
{
	int i_len;
	char *p_buf;

#ifdef H_DEBUG
	vm_log_debug("HTextInput input_method_callback");
#endif // H_DEBU

	if (VM_INPUT_OK != state)
		return;

	/* convert input method return text from ucs2 to gb2312 */
	p_buf = (char *)vm_malloc((vm_wstrlen(str) + 1) * sizeof(VMWCHAR));
	vm_ucs2_to_gb2312(p_buf, (vm_wstrlen(str) + 1) * sizeof(VMWCHAR), str);

	/* stored text into HTextInput */
	i_len = strlen(p_buf);
	p_textinput_buf->pc_text = (char *)vm_malloc(i_len + 1);
	strncpy(p_textinput_buf->pc_text, p_buf, i_len);
	p_textinput_buf->pc_text[i_len] = '\0';
	vm_free(p_buf);

	p_textinput_buf->base.p_widget_ops->repaint((HWidget *)p_textinput_buf);
	p_textinput_buf = NULL;
}

/* Call input method by HTextInput */
static void htextinput_call_input_method(HTextInput *p_textinput)
{
	VMWSTR str;

#ifdef H_DEBUG
	vm_log_debug("HTextInput call_input_method");
#endif // H_DEBU

	/* convert gb2312 text to ucs2 for input method default text */
	str = (VMWSTR)vm_malloc((strlen(p_textinput->pc_text) + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(str, (strlen(p_textinput->pc_text) + 1) * sizeof(VMWCHAR), p_textinput->pc_text);

	/* pass HTextInput pointer to input method callback function */
	p_textinput_buf = p_textinput;
	vm_input_text3(str, TEXT_MAX_SIZE, p_textinput->c_input_method, htextinput_input_method_callback);

	vm_free(str);
}

/* Base class Operations */

/* Pen press event callback */
static void htextinput_pen_press(HWidget *p_widget, short s_x, short s_y)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;
	HPenEvent pen_event;

#ifdef H_DEBUG
	vm_log_debug("htextinput_pen_press");
#endif // H_DEBU

	/* record x axis of tap pointer as moving start */
	s_move_from_x = s_x;

	/* call pen press action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_TAP;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textinput->base.action_performed)
		p_textinput->base.action_performed((HWidget *)p_textinput, (HEvent *)&pen_event, NULL);
}

/* Pen release event callback */
static void htextinput_pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;
	HPenEvent pen_event;

#ifdef H_DEBUG
	vm_log_debug("htextinput_pen_release");
#endif // H_DEBU
	
	/* repaint focus effect */
	p_textinput->base.p_widget_ops->repaint(p_widget);

	/* call pen release action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_RELEASE;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textinput->base.action_performed)
		p_textinput->base.action_performed((HWidget *)p_textinput, (HEvent *)&pen_event, NULL);
}

/* Pen move event callback */
static void htextinput_pen_move(HWidget *p_widget, short s_x, short s_y)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;
	HPenEvent pen_event;
	int i_move = 0;

#ifdef H_DEBUG
	vm_log_debug("htextinput_pen_move");
#endif // H_DEBU

	/* get HTextInput's text x axis moving offset */
	p_textinput->s_move_offset += s_x - s_move_from_x;
	s_move_from_x = s_x;
	p_textinput->s_move_offset = p_textinput->s_move_offset > 0 ? 0 : p_textinput->s_move_offset;

	p_widget->p_widget_ops->repaint(p_widget);

	/* call pen move action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_MOVE;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textinput->base.action_performed)
		p_textinput->base.action_performed((HWidget *)p_textinput, (HEvent *)&pen_event, NULL);
}

/* pen double click callback*/
static void htextinput_pen_double_click(HWidget *p_widget, short s_x, short s_y)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;
	HPenEvent pen_event;

	htextinput_call_input_method((HTextInput *)p_widget);

	/* call pen double click action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_DOUBLE_CLICK;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textinput->base.action_performed)
		p_textinput->base.action_performed((HWidget *)p_textinput, (HEvent *)&pen_event, NULL);
}

/* select input method callback */
static void select_input_action(HWidget *p_widget, HEvent *p_event, void *para)
{
	HPlane *p_plane = window->get_popupmenu_plane();

	p_plane->p_plane_ops->remove_widget(p_plane, p_widget);

	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 0);
	window->repaint();
}

/* pen long tap callback call popup menu to select input method */
static void htextinput_pen_long_tap(HWidget *p_widget, short s_x, short s_y)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;
	HCheckBox *p_checkbox = hcheckbox_new("select input method");
	HPlane *p_plane = window->get_popupmenu_plane();
	HPenEvent pen_event;

	p_plane->p_plane_ops->add_widget(p_plane, (HWidget *)p_checkbox);
	p_checkbox->base.p_widget_ops->set_enable_bgcolor((HWidget *)p_checkbox, 1);
	p_checkbox->base.p_widget_ops->set_bgcolor((HWidget *)p_checkbox, VM_COLOR_GREEN);

	p_checkbox->base.s_top_x = (vm_graphic_get_screen_width() - p_checkbox->base.p_widget_ops->get_width((HWidget *)p_checkbox)) / 2;
	p_checkbox->base.s_top_y = (vm_graphic_get_screen_height() - p_checkbox->base.p_widget_ops->get_height((HWidget *)p_checkbox)) / 2;

	p_checkbox->base.action_performed = select_input_action;
	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 1);
	p_plane->p_content->base.p_widget_ops->invalidate((HWidget *)p_plane->p_content);
	window->validate();
	window->repaint();

	/* call pen long tap action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_LONG_TAP;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textinput->base.action_performed)
		p_textinput->base.action_performed((HWidget *)p_textinput, (HEvent *)&pen_event, NULL);
}

/* Keyboard press event callback */
static void htextinput_key_press(HWidget *p_widget, int keycode)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;
	HKeyEvent key_event;

#ifdef H_DEBUG
	vm_log_debug("htextinput_key_press");
#endif // H_DEBU

	/* if keycode is ok , call input method */
	if (VM_KEY_OK == keycode)
		htextinput_call_input_method(p_textinput);

	if (VM_KEY_LEFT == keycode) {
		p_textinput->s_move_offset -= 2;
	}

	if (VM_KEY_RIGHT == keycode) {
		p_textinput->s_move_offset += 2;
	}

	/* call key down action callback function */
	key_event.base.i_event_type = VM_KEY_DOWN;
	key_event.i_keycode = keycode;
	if (p_textinput->base.action_performed)
		p_textinput->base.action_performed((HWidget *)p_textinput, (HEvent *)&key_event, NULL);
}

/* Whether the HCheckBox is a container */
static int htextinput_is_container(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_is_container");
#endif // H_DEBU

	return 0;
}

/* Whether the HCheckBox is a plane */
static int htextinput_is_plane(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_is_plane");
#endif // H_DEBUG

	return 0;
}

/* Get HTextInput type */
static HClass htextinput_get_class(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_get_class");
#endif // H_DEBUG

	return CLASS_TEXTINPUT;
}

/* Get the widget preferred width */
static short htextinput_get_prefered_width(HWidget *p_widget)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextinput_get_prefered_width");
#endif // H_DEBUG

	if (p_textinput->base.s_prefered_width)
		return p_textinput->base.s_prefered_width;

	p_textinput->base.s_prefered_width = p_textinput->base.p_widget_ops->get_max_width(p_widget);

	return p_textinput->base.s_prefered_width;
}

/* Get the widget preferred height */
static short htextinput_get_prefered_height(HWidget *p_widget)
{
	int i_prefered_height;
	HTextInput *p_textinput = (HTextInput *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextinput_get_prefered_height");
#endif // H_DEBUG

	if (p_textinput->base.s_prefered_height)
		return p_textinput->base.s_prefered_height;

	vm_graphic_set_font((p_textinput->base.c_font & 7) >> 1);

	i_prefered_height = vm_graphic_get_character_height() + 
		p_textinput->base.s_padding_top + p_textinput->base.s_padding_bottom;

	p_textinput->base.s_prefered_width = i_prefered_height;
	
	return i_prefered_height;
}

/* Paint the HCheckBox */
static void htextinput_paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_paint");
#endif // H_DEBUG

	look_paint_textinput((HTextInput *)p_widget, i_handle, s_screen_x, s_screen_y);
}

/* Destroy a HWidget */
static void hwidget_delete(HWidget *p_widget)
{
	HTextInput *p_textinput = (HTextInput *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("HTextInput hwidget_delete");
#endif // H_DEBUG

	/* free HTextInput memery */
	if (p_textinput->pc_text)
		vm_free(p_textinput->pc_text);

	if (p_textinput)
		vm_free(p_textinput);
}

/* Init a new HTextInput */
extern HTextInput * htextinput_new(int i_is_password, int i_input_method)
{
	HTextInput *p_textinput = (HTextInput *)vm_malloc(sizeof(HTextInput));

#ifdef H_DEBUG
	vm_log_debug("htextinput_new");
#endif // H_DEBUG

	hwidget_init(&(p_textinput->base));

	/* Init HTextInput global operations */
	if (!p_textinput_ops) {
		p_textinput_ops = (HTextInputOperation *)vm_malloc(sizeof(HTextInputOperation));

		p_textinput_ops->get_text = htextinput_get_text;
		p_textinput_ops->set_text = htextinput_set_text;
		p_textinput_ops->set_input_method = htextinput_set_input_method;
		p_textinput_ops->get_len = htextinput_get_len;
		p_textinput_ops->set_is_password = htextinput_set_is_password;
	}

	/* Init HTextInput base class global operations */
	if (!p_base_ops) {
		p_base_ops = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		hwidget_operation_init(p_base_ops);

		p_base_ops->pen_press = htextinput_pen_press;
		p_base_ops->pen_release = htextinput_pen_release;
		p_base_ops->pen_move = htextinput_pen_move;
		p_base_ops->pen_double_click = htextinput_pen_double_click;
		p_base_ops->pen_long_tap = htextinput_pen_long_tap;
		p_base_ops->key_press = htextinput_key_press;
		p_base_ops->is_container = htextinput_is_container;
		p_base_ops->is_plane = htextinput_is_plane;
		p_base_ops->get_class = htextinput_get_class;
		p_base_ops->get_prefered_width = htextinput_get_prefered_width;
		p_base_ops->get_prefered_height = htextinput_get_prefered_height;
		p_base_ops->paint = htextinput_paint;
		p_base_ops->destroy = hwidget_delete;
	}

	/* init HTextInput attr */
	p_textinput->c_is_password = 0;

	p_textinput->c_input_method = VM_INPUT_METHOD_TEXT;

	p_textinput->p_textinput_ops = p_textinput_ops;
	p_textinput->base.p_widget_ops = p_base_ops;
	p_textinput->base.action_performed = NULL;
	p_textinput->s_move_offset = 0;

	/* init HWiget attr */
	p_textinput->base.s_padding_left = 4;
	p_textinput->base.s_padding_right = 4;
	p_textinput->base.s_padding_top = 4;
	p_textinput->base.s_padding_bottom = 4;
	p_textinput->base.c_font = FONT_SMALL;

	return p_textinput;
}

/* Destroy a HTextInput */
extern void textinput_delete(HTextInput *p_textinput)
{
#ifdef H_DEBUG
	vm_log_debug("textinput_delete");
#endif // H_DEBUG

	if (p_textinput->pc_text)
		vm_free(p_textinput->pc_text);

	if (p_textinput)
		vm_free(p_textinput);
}

/* delete HTextInput global operations */
extern void htextinput_ops_delete()
{
#ifdef H_DEBUG
	vm_log_debug("htextinput_ops_delete");
#endif // H_DEBUG

	if (p_textinput_ops)
		vm_free(p_textinput_ops);
	if (p_base_ops)
		vm_free(p_base_ops);
}

/********************************EOF***********************************/