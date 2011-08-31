/**********************************************************************
* FILE Name: HCheckBox.h
* Description: Source file HCheckBox( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.19.2011) Author: Geng,XuanXuan
***********************************************************************/
#include "hcheckbox.h"

#include <string.h>
#include <stdlib.h>
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmsys.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"

#define MAX_LABLE_LEN 140

static HCheckBoxOperation *p_checkbox_ops; /* Single instance of HCheckBox Operations */
static HWidgetOperation *p_base_ops;  /* Single instance of HWidget Operations  */
static int i_checkbox_ops;  /* Number HCheckBox instances */

static void widget_destroy(HWidget *p_widget);

/****************** HCheckBox Callback Operation ******************/

/* Set label to HCheckBox */
static void set_label(HCheckBox *p_checkbox, const char *pc_label)
{
	int i_len;
	
	if (NULL == p_checkbox->pc_label)
		return;

	vm_free(p_checkbox->pc_label);

	i_len = strlen(pc_label);
	p_checkbox->pc_label = (char *)vm_malloc(i_len + 1);
	memcpy(p_checkbox->pc_label, pc_label, i_len);
	p_checkbox->pc_label[i_len + 1] = '\0';

	p_checkbox->base.p_widget_ops->repaint((HWidget *)p_checkbox);
}

/* Get label from HCheckBox */
static char* get_label(HCheckBox *p_checkbox)
{
	return p_checkbox->pc_label;
}

/* Set check state to HCheckBox */
static void set_check_state(HCheckBox *p_checkbox, char c_state)
{
	p_checkbox->c_check_state = c_state;

	p_checkbox->base.p_widget_ops->repaint((HWidget *)p_checkbox);
}

/* Get check state from HCheckBox */
static char get_check_state(HCheckBox *p_checkbox)
{
	return p_checkbox->c_check_state;
}

/***************** Internal functions *****************/

/* change checkstate of HCheckBox */
static change_state(HCheckBox *p_checkbox)
{
	if (HCHECKBOX_UNSELECTED == p_checkbox->c_check_state)
		p_checkbox->c_check_state = HCHECKBOX_SELECTED;
	else
		p_checkbox->c_check_state = HCHECKBOX_UNSELECTED;

	p_checkbox->base.p_widget_ops->repaint((HWidget *)p_checkbox);

	if (p_checkbox->base.action_performed)
		p_checkbox->base.action_performed((HWidget *)p_checkbox, NULL);
}

/***************** Base class Callback OP *****************/

/*pen press event callback*/
static void pen_press(HWidget *p_widget, short s_x, short s_y)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

	if(!p_checkbox->base.p_widget_ops->is_enable_focus(p_widget))
		return;

	p_checkbox->base.p_widget_ops->set_focus(p_widget, 1);
}

/*pen release event callback*/
static void pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

	if(!p_checkbox->base.p_widget_ops->is_enable_focus(p_widget))
		return;

	if (p_checkbox->base.p_widget_ops->has_focus(p_widget)) 
		change_state(p_checkbox);
}

/*pen move event callback*/
static void pen_move(HWidget *p_widget, short s_x, short s_y)
{
	/*********************************
		Nothing to do for now
	**********************************/
}

/* pen move enter callback*/
static void pen_enter(HWidget *p_widget, short s_x, short s_y)
{

}

/* pen move leave callback*/
static void pen_leave(HWidget *p_widget, short s_x, short s_y)
{

}

/*keyboard press event callback*/
static void key_press(HWidget *p_widget, int keycode)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

	if(!p_checkbox->base.p_widget_ops->is_enable(p_widget))
		return;

	if (VM_KEY_OK == keycode && p_checkbox->base.p_widget_ops->has_focus(p_widget))
		change_state(p_checkbox);
}

/*whether the wiHCheckBoxdget is a container*/
static int is_container(HWidget *p_widget)
{
	return 0;
}

/*whether the HCheckBox is a plane*/
static int is_plane(HWidget *p_widget)
{
	return 0;
}

static HClass get_class(HWidget *p_widget)
{
	return CLASS_CHECKBOX;
}

/*get the widget preferred width*/
static short get_prefered_width(HWidget *p_widget)
{
	VMWSTR w_str;
	int i_len = 0;
	char c_font;
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

	if (p_checkbox->base.s_prefered_width)
		return p_checkbox->base.s_prefered_width;

	i_len = strlen(p_checkbox->pc_label);
	w_str = (VMWSTR)vm_malloc((i_len + 1)* sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, (i_len + 1) * sizeof(VMWCHAR), p_checkbox->pc_label);

	c_font = (p_checkbox->base.c_font & 7) >> 1; 
	vm_graphic_set_font(c_font);
	p_checkbox->base.s_prefered_width = vm_graphic_get_string_width(w_str) + 
		vm_graphic_get_string_height(w_str)
		+ p_checkbox->base.uc_padding_left + p_checkbox->base.uc_padding_right + 4;

	vm_free(w_str);

	return p_checkbox->base.s_prefered_width;
}

/*get the widget preferred height*/
static short get_prefered_height(HWidget *p_widget)
{
	int i_prefered_height;
	int i_font_height;
	char c_font;
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;
	i_prefered_height = 0;

	if (p_checkbox->base.s_prefered_height)
		return p_checkbox->base.s_prefered_height;

	c_font = (p_checkbox->base.c_font & 7) >> 1; 
	vm_graphic_set_font(c_font);
	i_font_height = vm_graphic_get_character_height();

	i_prefered_height = i_font_height + p_checkbox->base.uc_padding_top + p_checkbox->base.uc_padding_bottom;

	p_checkbox->base.s_prefered_height = i_prefered_height;

	return i_prefered_height;
}

/*paint the HCheckBox */
static void paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	/***********************************
		Fix me
	***********************************/


	VMUINT8 *p_handle_buf;
	short s_rect_x;
	short s_rect_y;
	short s_rect_width;
	short s_rect_height;
	short s_text_x;
	short s_text_y;
	VMWSTR w_str;
	int i_len;
	char c_font;

	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

	s_rect_x = 0;
	s_rect_y = 0;
	s_rect_width = 0;
	s_rect_height = 0;
	s_text_x = 0;
	s_text_y = 0;

	p_handle_buf = vm_graphic_get_layer_buffer(i_handle);

	c_font = (p_checkbox->base.c_font & 7) >> 1; 
	vm_graphic_set_font(c_font);

	i_len = strlen(p_checkbox->pc_label);
	w_str = (VMWSTR)vm_malloc((i_len + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, (i_len + 1)* sizeof(VMWCHAR), p_checkbox->pc_label);

	s_rect_x = s_screen_x + p_checkbox->base.uc_padding_left;
	s_rect_y = s_screen_y + (p_checkbox->base.s_height - vm_graphic_get_string_height(w_str)) / 2;
	s_rect_height = vm_graphic_get_string_height(w_str);
	s_rect_width = s_rect_height;

#ifdef H_DEBUG	
	vm_graphic_rect(p_handle_buf, s_screen_x, s_screen_y, p_checkbox->base.s_width, p_checkbox->base.s_height, VM_COLOR_BLUE);
#endif

	if (p_checkbox->base.p_widget_ops->is_enable_bgcolor(p_widget))
		vm_graphic_fill_roundrect(p_handle_buf, s_screen_x, s_screen_y, p_checkbox->base.s_width, p_checkbox->base.s_height, 
		p_checkbox->base.uc_padding_top/2, p_checkbox->base.p_widget_ops->get_bgcolor(p_widget));
/*
	vm_graphic_set_clip(s_screen_x, s_screen_y, 
		p_checkbox->base.s_width - p_checkbox->base.uc_padding_left - p_checkbox->base.uc_padding_right,
		p_checkbox->base.s_height- p_checkbox->base.uc_padding_top - p_checkbox->base.uc_padding_bottom);
*/

	if (p_checkbox->base.p_widget_ops->has_focus(p_widget))
		vm_graphic_rect(p_handle_buf, s_rect_x, s_rect_y, s_rect_width, s_rect_height, VM_COLOR_RED);
	else
		vm_graphic_rect(p_handle_buf, s_rect_x, s_rect_y, s_rect_width, s_rect_height, VM_COLOR_BLACK);


	if (p_checkbox->p_checkbox_ops->get_check_state(p_checkbox) == HCHECKBOX_SELECTED)
		vm_graphic_fill_rect(p_handle_buf, s_rect_x + 2, s_rect_y + 2, s_rect_width - 4, s_rect_height - 4 ,VM_COLOR_BLACK, VM_COLOR_RED);    /* Todo draw check */

	s_text_x = s_rect_x + s_rect_width + 4;
	s_text_y = s_screen_y + (p_checkbox->base.s_height - vm_graphic_get_string_height(w_str)) / 2;

	vm_graphic_set_font(c_font);
	vm_graphic_textout(p_handle_buf, s_text_x, s_text_y, w_str, vm_wstrlen(w_str), p_checkbox->base.p_widget_ops->get_color(p_widget));

	vm_free(w_str);
}

static void hwidget_delete(HWidget *p_widget)
{
	HCheckBox *p_checkbox = (HCheckBox *)p_widget;

	--i_checkbox_ops;
	if (0 == i_checkbox_ops) {
		vm_free(p_base_ops);
		vm_free(p_checkbox_ops);
	}

	if (NULL != p_checkbox->pc_label)
		vm_free(p_checkbox->pc_label);

	vm_free(p_checkbox);
}

/* Create a new HCheckBox */
extern HCheckBox* hcheckbox_new(const char* label)
{
	int i_len;
	HCheckBox *p_checkbox;

	i_len = 0;
	p_checkbox = (HCheckBox *)vm_malloc(sizeof(HCheckBox));

	++i_checkbox_ops;
	if (!p_checkbox_ops) {
		p_checkbox_ops = (HCheckBoxOperation *)vm_malloc(sizeof(HCheckBoxOperation));
		
		p_checkbox_ops->set_label = set_label;
		p_checkbox_ops->get_label = get_label;
		p_checkbox_ops->set_check_state = set_check_state;
		p_checkbox_ops->get_check_state = get_check_state;
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

	hwidget_init(&(p_checkbox->base));

	/* init private member */
	if (NULL != label) {
		i_len = strlen(label);
		p_checkbox->pc_label = (char *)vm_malloc(i_len + 1);
		memcpy(p_checkbox->pc_label, label, i_len);
		p_checkbox->pc_label[i_len + 1] = '\0';
	} 
	else
		p_checkbox->pc_label = NULL;

	p_checkbox->c_check_state = HCHECKBOX_UNSELECTED;

	p_checkbox->p_checkbox_ops = p_checkbox_ops;
	p_checkbox->base.p_widget_ops = p_base_ops;

	/* init HWiget attr */
	p_checkbox->base.uc_padding_left = 8;
	p_checkbox->base.uc_padding_right = 8;
	p_checkbox->base.uc_padding_top = 8;
	p_checkbox->base.uc_padding_bottom = 8;
	p_checkbox->base.c_font = FONT_SMALL;

	return p_checkbox;
}

/* Destroy a HCheckBox */
extern void hcheckbox_delete(HCheckBox *p_checkbox)
{
	--i_checkbox_ops;
	if (0 == i_checkbox_ops) {
		vm_free(p_base_ops);
		vm_free(p_checkbox_ops);
	}

	if (NULL != p_checkbox->pc_label)
		vm_free(p_checkbox->pc_label);

	vm_free(p_checkbox);
}

/********************************EOF***********************************/