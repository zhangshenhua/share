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

/* Global var for painting, should init after get focus and set them to 0 after losing focus */
static VMWSTR w_str;
static VMUINT8 *p_layer_buf_global;
static short s_move_offset;
static short s_cursor_x1;
static short s_cursor_y1;
static short s_cursor_x2;
static short s_cursor_y2;

static void draw_cursor(HTextArea *p_textarea, VMWSTR w_str, VMUINT8 *p_layer_buf);

/* Call this function after getting focus */
static init_global(HTextArea *p_textarea)
{

}

/* Call this function after losing focus */
static destroy_global(HTextArea *p_textarea)
{
	if (w_str) 
		vm_free(w_str);

	p_layer_buf_global = NULL;
	s_move_offset = 0;
	s_cursor_x1 = 0;
	s_cursor_x2 = 0;
	s_cursor_y1 = 0;
	s_cursor_y2 = 0;
}

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

	p_textarea->i_current_position = p_textarea->p_htextarea_ops->get_text_len(p_textarea);
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

	p_textarea->i_current_position = p_textarea->p_htextarea_ops->get_text_len(p_textarea);
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

/* Convert char to VMWCHAR */
static void get_wstr_from_htextarea(HTextArea *p_textarea)
{
	int i_len;

	if (w_str) {
		vm_free(w_str);
		w_str = NULL;
	}

	i_len = strlen(p_textarea->pc_text);
	w_str = (VMWSTR)vm_malloc((i_len + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, (i_len + 1) * sizeof(VMWCHAR), p_textarea->pc_text);
}

/* Set input method of HTextArea */
static void set_inputmethod(HTextArea *p_textarea, int i_method)
{
	p_textarea->c_input_method = i_method;
}

/* Set max length of HTextArea's text */
static void set_max_len(HTextArea *p_textarea, int i_len)
{
	p_textarea->i_max_len = i_len;
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

	p_buf = (char *)vm_malloc((vm_wstrlen(str) + 1) * sizeof(VMWCHAR));
	vm_ucs2_to_gb2312(p_buf, (vm_wstrlen(str) + 1) * sizeof(VMWCHAR), str);

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
	str = (VMWSTR)vm_malloc((strlen(p_textarea->pc_text) + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(str, (strlen(p_textarea->pc_text) + 1) * sizeof(VMWCHAR), p_textarea->pc_text);

	p_textarea_buf = p_textarea;
	vm_input_text3(str, p_textarea->i_max_len, p_textarea->c_input_method, input_method_callback);

	p_textarea_buf = NULL;
	p_textarea->base.p_widget_ops->repaint((HWidget *)p_textarea);

	vm_free(str);

	if (p_textarea->base.action_performed)
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

/* pen move enter callback*/
static void pen_enter(HWidget *p_widget, short s_x, short s_y)
{

}

/* pen move leave callback*/
static void pen_leave(HWidget *p_widget, short s_x, short s_y)
{

}

/* Pen move event callback */
static void pen_move(HWidget *p_widget, short s_x, short s_y)
{
	
}

/* Keyboard press event callback */
static void key_press(HWidget *p_widget, int keycode)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

	if (!p_textarea->base.p_widget_ops->is_enable(p_widget))
		return;

	if (VM_KEY_OK == keycode)
		call_input_method(p_textarea);

	if (VM_KEY_LEFT == keycode && p_textarea->i_current_position > 0) {
		p_textarea->i_current_position -= 1;
		draw_cursor(p_textarea, w_str, p_layer_buf_global);
	}

	if (VM_KEY_RIGHT == keycode && p_textarea->i_current_position < p_textarea->i_max_len) {
		p_textarea->i_current_position += 1;
		draw_cursor(p_textarea, w_str, p_layer_buf_global);
	}
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

/* Draw text to screen */
static void draw_text(HTextArea *p_textarea, VMWSTR w_str, VMUINT8 *p_layer_buf)
{
	int i_len_total = 0;
	int i_line_height = 0;
	int i_index = 0;
	int i_width_total = 0;
	int i_line = 0;
	int i_line_space = p_textarea->c_line_space;
	int i_x = 0;
	int i_y = 0;
	int i_text_width;
	char c_font;
	VMWSTR w_index = NULL;
	VMWSTR w_tmp = NULL;

	i_x = p_textarea->base.s_top_x + p_textarea->base.uc_padding_left + s_move_offset;
	i_y = p_textarea->base.s_top_y + p_textarea->base.uc_padding_top + s_move_offset;
	i_text_width = p_textarea->base.s_width - p_textarea->base.uc_padding_left - p_textarea->base.uc_padding_right;
 
	/************************************************************************/
	/* Todo: set font of HTextArea                                          */
	/************************************************************************/

	c_font = p_textarea->base.c_font & 7 >> 1;
	vm_graphic_set_font(0);

	i_len_total = vm_wstrlen(w_str);
	w_tmp = w_str;
	w_index = w_str;
	i_line_height = vm_graphic_get_string_height(w_str);

	while (i_index < i_len_total) {
		printf("enter loop\n");
		i_width_total = vm_graphic_get_string_width(w_tmp) - vm_graphic_get_string_width(w_index);

		if (*w_index == '\n') {
			vm_graphic_textout(p_layer_buf, i_x, i_y + i_line * (i_line_height + i_line_space), w_tmp, (vm_wstrlen(w_tmp) - vm_wstrlen(w_index)), VM_COLOR_BLACK);
			w_tmp = ++w_index;
			++i_line;
			++i_index;
			continue;
		}

		if (i_text_width < i_width_total) {
			--w_index;
			--i_index;
			vm_graphic_textout(p_layer_buf, i_x, i_y + i_line * (i_line_height + i_line_space), w_tmp, (vm_wstrlen(w_tmp) - vm_wstrlen(w_index)), VM_COLOR_BLACK);
			w_tmp = w_index;
			++i_line;
			continue;
		}

		++i_index;
		++w_index;

		if (i_line  == p_textarea->c_line_num) {
			break;
		}
	}

	if (i_line != p_textarea->c_line_num) {
		vm_graphic_textout(p_layer_buf, i_x, i_y + i_line * (i_line_height + i_line_space), w_tmp, (vm_wstrlen(w_tmp) - vm_wstrlen(w_index)), VM_COLOR_BLACK);
	}

	printf("++++++++++++++i_index: %d\n", i_index);
}

/* Draw cursor at HTextArea current cursor position */
static void draw_cursor(HTextArea *p_textarea, VMWSTR w_str, VMUINT8 *p_layer_buf)
{
	int i_len_total = 0;
	int i_line_height = 0;
	int i_position = p_textarea->i_current_position;
	int i_index = 0;
	int i_width_total = 0;
	int i_line = 0;
	int i_line_space = p_textarea->c_line_space;
	int i_x = 0;
	int i_y = 0;
	int i_text_width;
	char c_font;
	VMWSTR w_index = NULL;
	VMWSTR w_tmp = NULL;

	i_x = p_textarea->base.s_top_x + p_textarea->base.uc_padding_left + s_move_offset;
	i_y = p_textarea->base.s_top_y + p_textarea->base.uc_padding_top + s_move_offset;
	i_text_width = p_textarea->base.s_width - p_textarea->base.uc_padding_left - p_textarea->base.uc_padding_right;

	i_len_total = vm_wstrlen(w_str);
	p_textarea->i_current_position = i_len_total;

	w_tmp = w_str;
	w_index = w_str;

	c_font = p_textarea->base.c_font & 7 >> 1;
	vm_graphic_set_font(0);
	i_line_height = vm_graphic_get_string_height(w_str);

	printf("i_position: %d\n", i_position);

	while (i_index < (i_len_total + 1)) {
		i_width_total = vm_graphic_get_string_width(w_tmp) - vm_graphic_get_string_width(w_index);

		if (*w_index == '\n') {
			w_tmp = ++w_index;
			++i_line;
			++i_index;
			continue;
		}

		if ((vm_graphic_get_screen_width() - i_x * 2) < i_width_total) {
			--w_index;
			--i_index;
			w_tmp = w_index;
			++i_line;
			continue;
		}

		if (i_index == i_position)
			break;

		if (i_line == p_textarea->c_line_num)
			break;

		++i_index;
		++w_index;
	}
	printf("i_index: %d\n", i_index);

	/* delete cursor */
	vm_graphic_line(p_layer_buf, s_cursor_x1, s_cursor_y1, s_cursor_x2, s_cursor_y2, VM_COLOR_WHITE);

	s_cursor_x1 = i_x + i_width_total - 1;
	s_cursor_y1 = i_y + i_line * (i_line_height + i_line_space);
	s_cursor_x2 = s_cursor_x1;
	s_cursor_y2 = i_y + i_line * (i_line_height + i_line_space) + i_line_height;	

	/* draw cursor */
	vm_graphic_line(p_layer_buf, s_cursor_x1, s_cursor_y1, s_cursor_x2, s_cursor_y2, VM_COLOR_BLACK);

}

/* Paint the HCheckBox */
static void paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	VMUINT8 *p_layer_buf;
	HTextArea *p_textarea = (HTextArea *)p_widget;

	p_layer_buf = vm_graphic_get_layer_buffer(i_handle);
	p_layer_buf_global = p_layer_buf;

	vm_graphic_set_clip(s_screen_x, s_screen_y, s_screen_x + p_textarea->base.s_width, s_screen_y + p_textarea->base.s_height);
	vm_graphic_roundrect(p_layer_buf, s_screen_x, s_screen_y, p_textarea->base.s_width, p_textarea->base.s_height, 4, VM_COLOR_BLACK);
	vm_graphic_fill_rect(p_layer_buf, s_screen_x + p_textarea->base.uc_padding_left, s_screen_y + p_textarea->base.uc_padding_top,
		p_textarea->base.s_width - p_textarea->base.uc_padding_left - p_textarea->base.uc_padding_right, 
		p_textarea->base.s_height - p_textarea->base.uc_padding_top - p_textarea->base.uc_padding_bottom, VM_COLOR_BLACK, VM_COLOR_WHITE);

	s_move_offset = 20;

	/************************************************************************/
	/* Todo: delete this line                                                     */
	/************************************************************************/
	get_wstr_from_htextarea(p_textarea);

	draw_text(p_textarea, w_str, p_layer_buf);
	draw_cursor(p_textarea, w_str, p_layer_buf);
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
		p_textarea_ops->set_max_len = set_max_len;
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

	/* Init private members */
	p_text_area->c_line_num = 4;
	p_text_area->c_is_split_line = 0;
	p_text_area->c_line_space = 4;
	p_text_area->c_input_method = VM_INPUT_METHOD_TEXT;
	p_text_area->i_max_len = 140;

	p_text_area->base.p_widget_ops = p_base_ops;
	p_text_area->p_htextarea_ops = p_textarea_ops;

	/* init HWiget attr */
	p_text_area->base.uc_padding_left = 8;
	p_text_area->base.uc_padding_right = 8;
	p_text_area->base.uc_padding_top = 8;
	p_text_area->base.uc_padding_bottom = 8;
	p_text_area->base.c_font = FONT_SMALL;

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