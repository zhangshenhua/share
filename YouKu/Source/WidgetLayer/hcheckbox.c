/**********************************************************************
* FILE Name: HCheckBox.h
* Description: Source file HCheckBox( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.19.2011) Author: Geng,XuanXuan
***********************************************************************/
#include "hcheckbox.h"
#include "hlookandfeel.h"

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

#define MAX_LABLE_LEN 140

static HCheckBoxOperation *p_checkbox_ops; /* Single instance of HCheckBox Operations */
static HWidgetOperation *p_base_ops;  /* Single instance of HWidget Operations  */
static int i_checkbox_ops;  /* Number HCheckBox instances */

/****************** HCheckBox Callback Operation ******************/

/* Set label to HCheckBox */
static void hcheckbox_set_label(HCheckBox *p_checkbox, char *pc_label)
{
	int i_len;

#ifdef H_DEBUG
	vm_log_debug("hcheckbox_set_label");
#endif
	
	if (NULL == p_checkbox->pc_label)
		return;

	vm_free(p_checkbox->pc_label);

	i_len = strlen(pc_label);
	p_checkbox->pc_label = (char *)vm_malloc(i_len + 1);
	memset(p_checkbox->pc_label, 0, i_len + 1);
	strncpy(p_checkbox->pc_label, pc_label, i_len);
	p_checkbox->pc_label[i_len] = '\0';

	p_checkbox->base.p_widget_ops->repaint((HWidget *)p_checkbox);
}

/* Get label from HCheckBox */
static char* hcheckbox_get_label(HCheckBox *p_checkbox)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_get_label");
#endif

	return p_checkbox->pc_label;
}

/* Set check state to HCheckBox */
static void hcheckbox_set_check_state(HCheckBox *p_checkbox, char c_state)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_set_check_state");
#endif

	p_checkbox->c_check_state = c_state;
	p_checkbox->base.p_widget_ops->repaint((HWidget *)p_checkbox);
}

/* Get check state from HCheckBox */
static char hcheckbox_get_check_state(HCheckBox *p_checkbox)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_get_check_state");
#endif

	return p_checkbox->c_check_state;
}

/***************** Internal functions *****************/

/* change checkstate of HCheckBox */
static hcheckbox_change_state(HCheckBox *p_checkbox)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_change_state");
#endif

	if (HCHECKBOX_UNSELECTED == p_checkbox->c_check_state)
		p_checkbox->c_check_state = HCHECKBOX_SELECTED;
	else
		p_checkbox->c_check_state = HCHECKBOX_UNSELECTED;

	p_checkbox->base.p_widget_ops->repaint((HWidget *)p_checkbox);
}

/***************** Base class Callback OP *****************/

/*pen press event callback*/
static void hcheckbox_pen_press(HWidget *p_widget, short s_x, short s_y)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;
	HPenEvent pen_event;

#ifdef H_DEBUG
	vm_log_debug("hcheckbox_pen_press");
#endif

	/* call pen press action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_TAP;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_checkbox->base.action_performed)
		p_checkbox->base.action_performed((HWidget *)p_checkbox, (HEvent *)&pen_event, NULL);
}

/*pen release event callback*/
static void hcheckbox_pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;
	HPenEvent pen_event;

#ifdef H_DEBUG
	vm_log_debug("hcheckbox_pen_release");
#endif

	if(!p_checkbox->base.p_widget_ops->is_enable_focus(p_widget))
		return;

	if (p_checkbox->base.p_widget_ops->has_focus(p_widget)) 
		hcheckbox_change_state(p_checkbox);

	/* call pen release action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_RELEASE;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_checkbox->base.action_performed)
		p_checkbox->base.action_performed((HWidget *)p_checkbox, (HEvent *)&pen_event, NULL);
}

/*keyboard press event callback*/
static void hcheckbox_key_press(HWidget *p_widget, int keycode)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;
	HKeyEvent key_event;

#ifdef H_DEBUG
	vm_log_debug("hcheckbox_key_press");
#endif

	if(!p_checkbox->base.p_widget_ops->is_enable(p_widget))
		return;

	if (VM_KEY_OK == keycode && p_checkbox->base.p_widget_ops->has_focus(p_widget))
		hcheckbox_change_state(p_checkbox);

	/* call key press action callback function */
	key_event.base.i_event_type = VM_KEY_DOWN;
	key_event.i_keycode = keycode;
	if (p_checkbox->base.action_performed)
		p_checkbox->base.action_performed((HWidget *)p_checkbox, (HEvent *)&key_event, NULL);
}

/*whether the wiHCheckBoxdget is a container*/
static int hcheckbox_is_container(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_is_container");
#endif

	return 0;
}

/*whether the HCheckBox is a plane*/
static int hcheckbox_is_plane(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_is_plane");
#endif

	return 0;
}

static HClass hcheckbox_get_class(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_get_class");
#endif

	return CLASS_CHECKBOX;
}

/*get the widget preferred width*/
static short hcheckbox_get_prefered_width(HWidget *p_widget)
{
	VMWSTR w_str;
	int i_len = 0;
	int i_prefered_width = 0;
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("hcheckbox_get_prefered_width");
#endif

	if (p_checkbox->base.s_prefered_width)
		return p_checkbox->base.s_prefered_width;

	i_len = strlen(p_checkbox->pc_label);
	w_str = (VMWSTR)vm_malloc((i_len + 1)* sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, (i_len + 1) * sizeof(VMWCHAR), p_checkbox->pc_label);
 
	vm_graphic_set_font(((p_checkbox->base.c_font) & 7) >> 1);
	i_prefered_width = vm_graphic_get_string_width(w_str) + vm_graphic_get_string_height(w_str)
		+ p_checkbox->base.s_padding_left + p_checkbox->base.s_padding_right + 4;

	if (i_prefered_width > p_checkbox->base.p_widget_ops->get_max_width(p_widget))
		i_prefered_width = p_checkbox->base.p_widget_ops->get_max_width(p_widget);

	p_checkbox->base.p_widget_ops->set_width((HWidget *)p_checkbox, i_prefered_width);
	vm_free(w_str);

	return i_prefered_width;
}

/*get the widget preferred height*/
static short hcheckbox_get_prefered_height(HWidget *p_widget)
{
	int i_prefered_height;
	int i_font_height;
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;
	i_prefered_height = 0;

#ifdef H_DEBUG
	vm_log_debug("hcheckbox_get_prefered_height");
#endif

	if (p_checkbox->base.s_prefered_height)
		return p_checkbox->base.s_prefered_height;
 
	vm_graphic_set_font((p_checkbox->base.c_font & 7) >> 1);
	i_font_height = vm_graphic_get_character_height();

	i_prefered_height = i_font_height + p_checkbox->base.s_padding_top + p_checkbox->base.s_padding_bottom;
	p_checkbox->base.s_prefered_height = i_prefered_height;

	return i_prefered_height;
}

/*paint the HCheckBox */
static void hcheckbox_paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_paint");
#endif

	look_paint_checkbox((HCheckBox *)p_widget, i_handle, s_screen_x, s_screen_y);
}

static void hwidget_delete(HWidget *p_widget)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("HCheckbox hwidget_delete");
#endif

	if (p_checkbox->pc_label)
		vm_free(p_checkbox->pc_label);

	vm_free(p_checkbox);
}

/* Create a new HCheckBox */
extern HCheckBox* hcheckbox_new(const char* label)
{
	int i_len;
	HCheckBox *p_checkbox;

#ifdef H_DEBUG
	vm_log_debug("hcheckbox_new");
#endif

	i_len = 0;
	p_checkbox = (HCheckBox *)vm_malloc(sizeof(HCheckBox));

	++i_checkbox_ops;
	if (!p_checkbox_ops) {
		p_checkbox_ops = (HCheckBoxOperation *)vm_malloc(sizeof(HCheckBoxOperation));
		
		p_checkbox_ops->set_label = hcheckbox_set_label;
		p_checkbox_ops->get_label = hcheckbox_get_label;
		p_checkbox_ops->set_check_state = hcheckbox_set_check_state;
		p_checkbox_ops->get_check_state = hcheckbox_get_check_state;
	}

	if (!p_base_ops) {
		p_base_ops = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		hwidget_operation_init(p_base_ops);

		p_base_ops->pen_press = hcheckbox_pen_press;
		p_base_ops->pen_release = hcheckbox_pen_release;
		p_base_ops->key_press = hcheckbox_key_press;
		p_base_ops->is_container = hcheckbox_is_container;
		p_base_ops->is_plane = hcheckbox_is_plane;
		p_base_ops->get_class = hcheckbox_get_class;
		p_base_ops->get_prefered_width = hcheckbox_get_prefered_width;
		p_base_ops->get_prefered_height = hcheckbox_get_prefered_height;
		p_base_ops->paint = hcheckbox_paint;
		p_base_ops->destroy = hwidget_delete;
	}

	hwidget_init(&(p_checkbox->base));

	/* init private member */
	if (NULL != label) {
		i_len = strlen(label);
		p_checkbox->pc_label = (char *)vm_malloc(i_len + 1);
		memcpy(p_checkbox->pc_label, label, i_len);
		p_checkbox->pc_label[i_len] = '\0';
	} 
	else
		p_checkbox->pc_label = NULL;

	p_checkbox->c_check_state = HCHECKBOX_UNSELECTED;

	p_checkbox->p_checkbox_ops = p_checkbox_ops;
	p_checkbox->base.p_widget_ops = p_base_ops;
	p_checkbox->base.action_performed = NULL;

	/* init HWiget attr */
	p_checkbox->base.s_padding_left = 4;
	p_checkbox->base.s_padding_right = 4;
	p_checkbox->base.s_padding_top = 4;
	p_checkbox->base.s_padding_bottom = 4;
	p_checkbox->base.c_font = FONT_SMALL;

	return p_checkbox;
}

/* Destroy a HCheckBox */
extern void hcheckbox_delete(HCheckBox *p_checkbox)
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_delete");
#endif

	if (p_checkbox->pc_label)
		vm_free(p_checkbox->pc_label);

	vm_free(p_checkbox);
}

/* delete HCheckBox global operatons */
extern void hcheckbox_ops_delete()
{
#ifdef H_DEBUG
	vm_log_debug("hcheckbox_ops_delete");
#endif

	if (p_base_ops)
		vm_free(p_base_ops);

	if (p_checkbox_ops)
		vm_free(p_checkbox_ops);
}

/********************************EOF***********************************/