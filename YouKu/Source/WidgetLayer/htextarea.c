/****************************************************************
* FILE Name: HTextArea.c
* Description: Source file HTextArea( a HWidget)
* COPYRIGHT: Hisoft
* Created on: $(8.24.2011) Author: Geng,XuanXuan
****************************************************************/

#include "htextarea.h"
#include "../WindowLayer/hwindow.h"
#include "hplane.h"
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

static char c_timer_id;             /* timer of cursor twinkling effect */
static char c_cursor_flag;		     /* cursor visable or hiden */
static short s_move_from_y;		 /* text move offset of y axis */
static short s_textarea_height;     /* text area height */

static HTextAreaOperation *p_textarea_ops;		/* Single instance of HTextArea Operations  */
static HWidgetOperation *p_base_ops;			/* Single instance of HWidget Operations  */
static int i_ops;							/* Number of HTextArea instances */

static HTextArea *p_textarea_buf;   /* Template pointer for calling input method */


static void get_linenode_list(HTextArea *p_textarea);
static void locate_position(HTextArea *p_textarea, short s_x, short s_y);
static void clear_linenode_list(HTextArea *p_textarea);
static void draw_text(HTextArea *p_textarea, VMUINT8 *p_layer_buf, short s_screen_x, short s_screen_y);

/* HTextArea Operations */

/* Set text to HTextArea */ 
static void htextarea_set_text(HTextArea *p_textarea, char *pc_text)
{
	int i_len = 0;

#ifdef H_DEBUG
	vm_log_debug("htextarea set_text");

#endif

	if (!pc_text)
		return;

	if (p_textarea->w_text)
		vm_free(p_textarea->w_text);

	/* convert gb2312 text to ucs2 */
	i_len = strlen(pc_text);
	p_textarea->w_text = (VMWSTR)vm_malloc((i_len + 1) * 2);
	vm_gb2312_to_ucs2(p_textarea->w_text, (i_len + 1) * 2, pc_text);

	/* resize memory by HTextArea text length */
	i_len = vm_wstrlen(p_textarea->w_text);
	i_len = i_len > p_textarea->i_max_len ? p_textarea->i_max_len : i_len;
	p_textarea->w_text = vm_realloc(p_textarea->w_text, (i_len + 1) * 2);
	p_textarea->w_text[i_len] = '\0';

	/* set text changing flag */
	p_textarea->c_text_change_flag = 1;
	p_textarea->i_current_position = p_textarea->p_htextarea_ops->get_text_len(p_textarea);
}

/* Get text from HTextArea */
static char* htextarea_get_text(HTextArea *p_textarea)
{
	int i_len = 0;
	char *pc_text;

#ifdef H_DEBUG
	vm_log_debug("htextarea get_text");
#endif

	/* convert ucs2 text to gb2312 */
	i_len = vm_wstrlen(p_textarea->w_text);
	pc_text = (char *)vm_malloc((i_len + 1) * 2);
	vm_ucs2_to_gb2312(pc_text, (i_len + 1) * 2, p_textarea->w_text);

	i_len = strlen(pc_text);
	vm_realloc(pc_text, i_len + 1);
	pc_text[i_len] = '\0';

	return pc_text;
}

/* Insert text to HTextArea's current cursor position */
static void htextarea_insert_text(HTextArea *p_textarea, char *pc_text)
{
	int i_len_text = 0;
	int i_len_insert = 0;
	VMWSTR w_buf = NULL;
	VMWSTR w_text_insert = NULL;
	VMWSTR w_index = NULL;
	char *pc_buf = NULL;

#ifdef H_DEBUG
	vm_log_debug("htextarea insert_text");
#endif

	if (!pc_text)
		return;

	i_len_text = vm_wstrlen(p_textarea->w_text);
	
	/* convert insert text from gb2312 to ucs2 */
	i_len_insert = strlen(pc_text);
	w_text_insert = (VMWSTR)vm_malloc((i_len_insert + 1) * 2);
	vm_gb2312_to_ucs2(w_text_insert, (i_len_insert + 1) * 2, pc_text);
	i_len_insert = vm_wstrlen(w_text_insert);

	/* insert text to HTextArea */
	w_buf = (VMWSTR)vm_malloc((i_len_text + i_len_insert + 1) * 2);
	w_index = w_buf;
	vm_wstrncpy(w_index, p_textarea->w_text, p_textarea->i_current_position);
	w_index += p_textarea->i_current_position;
	vm_wstrncpy(w_index, w_text_insert, i_len_insert);
	w_index += i_len_insert;
	vm_wstrncpy(w_index, p_textarea->w_text + p_textarea->i_current_position, i_len_text - p_textarea->i_current_position);	
	w_buf[i_len_insert + i_len_text] = '\0';
	vm_free(w_text_insert);

	/* resize memory by HTextArea text length */
	i_len_text = vm_wstrlen(w_buf);
	i_len_text = i_len_text > p_textarea->i_max_len ? p_textarea->i_max_len : i_len_text;
	p_textarea->w_text = (VMWSTR)vm_malloc((i_len_text + 1) * 2);
	vm_wstrncpy(p_textarea->w_text, w_buf, i_len_text);
	p_textarea->w_text[i_len_text] = '\0';
	vm_free(w_buf);

	p_textarea->c_text_change_flag = 1;
	p_textarea->i_current_position += i_len_insert;
}

#if 0
/* delete character by current cursor position */
static void htextarea_delete_character(HTextArea *p_textarea)
{
	int i_len = 0;
	VMWSTR w_index = NULL;

	if (p_textarea->i_current_position == 0)
		return;

#ifdef H_DEBUG
	vm_log_debug("htextarea delete_text");
#endif

	/* delete character by cursor position */
	w_index = p_textarea->w_text + p_textarea->i_current_position;
	i_len = vm_wstrlen(p_textarea->w_text);

	while(*(w_index)) {
		*w_index = *(w_index + 1);
		++w_index;
	}

	w_index = vm_realloc(p_textarea->w_text, i_len * 2);
	p_textarea->w_text = w_index;
	p_textarea->w_text[i_len - 1] = '\0';

	p_textarea->i_current_position--;
	p_textarea->c_text_change_flag = 1;
}
#endif

/* Get length HTextArea's text */
static int htextarea_get_text_len(HTextArea *p_textarea)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea get_text_len");
#endif

	return vm_wstrlen(p_textarea->w_text);
}

/* Set input method of HTextArea */
static void htextarea_set_inputmethod(HTextArea *p_textarea, int i_method)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea set_inputmethod");
#endif

	p_textarea->c_input_method = i_method;
}

/* Set max length of HTextArea's text */
static void htextarea_set_max_len(HTextArea *p_textarea, int i_len)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea set_max_len");
#endif

	p_textarea->i_max_len = i_len;
}

/* Set line number showing of HTextArea */
static void htextarea_set_line_num(HTextArea *p_textarea, int i_line_num)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea set_line_num");
#endif

	p_textarea->c_line_num = i_line_num;
}

/* Set split line flag to HTextArea */
static void htextarea_set_split_line(HTextArea *p_textarea, int i_split_line)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea set_split_line");
#endif

	p_textarea->c_is_split_line = i_split_line;
}

/* Set rows space of HTextArea */
static void htextarea_set_line_space(HTextArea *p_textarea, int i_line_space)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea set_line_space");
#endif

	p_textarea->c_line_space = i_line_space;
}

/******************** functions for input method *********************/
/* Input method callback function */
static void htextarea_input_method_callback(VMINT state, VMWSTR str)
{
	int i_len;

#ifdef H_DEBUG
	vm_log_debug("htextarea input_method_callback");
#endif

	if (VM_INPUT_OK != state)
		return;
	
	i_len = vm_wstrlen(str);
	p_textarea_buf->w_text = (VMWSTR)vm_malloc((i_len + 1) * 2);
	vm_wstrncpy(p_textarea_buf->w_text, str, i_len);
	p_textarea_buf->w_text[i_len] = '\0';

	p_textarea_buf->c_text_change_flag = 1;
	p_textarea_buf->base.p_widget_ops->repaint((HWidget *)p_textarea_buf);
	p_textarea_buf = NULL;
}

/* Call input method */
static void htextarea_call_input_method(HTextArea *p_textarea)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea call_input_method");
#endif

	p_textarea_buf = p_textarea;
	vm_input_text3(p_textarea->w_text, p_textarea->i_max_len, p_textarea->c_input_method, htextarea_input_method_callback);
}

/******************** Base class Operations *******************/
/* Pen press event callback */
static void htextarea_pen_press(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;
	HPenEvent pen_event;

#ifdef H_DEBUG
	vm_log_debug("htextarea pen_press");
	vm_log_debug("pen press  x: %d, y: %d", s_x, s_y);
#endif

	s_move_from_y = s_y;

	/* call pen tap action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_TAP;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textarea->base.action_performed)
		p_textarea->base.action_performed((HWidget *)p_textarea, (HEvent *)&pen_event, NULL);
}

/* Pen release event callback */
static void htextarea_pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;
	HPenEvent pen_event;

#ifdef H_DEBUG
	vm_log_debug("htextarea pen_release");
#endif

	/* Get cursor position by pen event. */
	locate_position(p_textarea, s_x, s_y);

	/* repaint focus effect */
	p_textarea->base.p_widget_ops->repaint(p_widget);

	/* call pen release action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_RELEASE;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textarea->base.action_performed)
		p_textarea->base.action_performed((HWidget *)p_textarea, (HEvent *)&pen_event, NULL);
}

/* Pen move event callback */
static void htextarea_pen_move(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;
	HPenEvent pen_event;

#ifdef H_DEBUG
	vm_log_debug("htextarea pen_move");
	vm_log_debug("pen move x: %d, y: %d, from_y: %d. offset: %d", s_x, s_y, s_move_from_y, p_textarea->s_move_offset);
#endif

	p_textarea->s_move_offset += s_y - s_move_from_y;
	p_textarea->s_move_offset = p_textarea->s_move_offset > 0 ? 0 : p_textarea->s_move_offset;
	s_move_from_y = s_y;

	p_widget->p_widget_ops->repaint(p_widget);

	/* call pen move action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_MOVE;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textarea->base.action_performed)
		p_textarea->base.action_performed((HWidget *)p_textarea, (HEvent *)&pen_event, NULL);
}

/* pen double click callback*/
static void htextarea_pen_double_click(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;
	HPenEvent pen_event;

	htextarea_call_input_method(p_textarea);

	/* call pen double click action callback function */
	pen_event.base.i_event_type = VM_PEN_EVENT_DOUBLE_CLICK;
	pen_event.s_x = s_x;
	pen_event.s_y = s_y;
	if (p_textarea->base.action_performed)
		p_textarea->base.action_performed((HWidget *)p_textarea, (HEvent *)&pen_event, NULL);
}

/* select input method callback */
static void select_input_action(HWidget *p_widget, HEvent *p_event, void *para)
{
	HPlane *p_plane = window->get_popupmenu_plane();
		p_plane->p_plane_ops->remove_widget(p_plane, p_widget);

	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 0);
	window->repaint();
}

/* pen long tap callback, popup input method select menu */
static void htextarea_pen_long_tap(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;
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
	if (p_textarea->base.action_performed)
		p_textarea->base.action_performed((HWidget *)p_textarea, (HEvent *)&pen_event, NULL);
}

/* Keyboard press event callback */
static void htextarea_key_press(HWidget *p_widget, int keycode)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;
	HKeyEvent key_event;

#ifdef H_DEBUG
	vm_log_debug("htextarea key_press, keycode: %d", keycode);
#endif

	if (VM_KEY_OK == keycode)
		htextarea_call_input_method(p_textarea);

	if (VM_KEY_LEFT == keycode && (p_textarea->i_current_position > 0)) {
		(p_textarea->i_current_position)--;
		p_textarea->base.p_widget_ops->repaint((HWidget *)p_textarea);
	}

	if (VM_KEY_RIGHT == keycode && (p_textarea->i_current_position < p_textarea->p_htextarea_ops->get_text_len(p_textarea))) {
		(p_textarea->i_current_position)++;
		p_textarea->base.p_widget_ops->repaint((HWidget *)p_textarea);
	}

	/* call key down action callback function */
	key_event.base.i_event_type = VM_KEY_DOWN;
	key_event.i_keycode = keycode;
	if (p_textarea->base.action_performed)
		p_textarea->base.action_performed((HWidget *)p_textarea, (HEvent *)&key_event, NULL);
}

/* Whether the HCheckBox is a container */
static int htextarea_is_container(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea is_container");
#endif

	return 0;
}

/* Whether the HCheckBox is a plane */
static int htextarea_is_plane(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea is_plane");
#endif

	return 0;
}

/* Get type of HTextArea */
static HClass htextarea_get_class(HWidget *p_widget)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea get_class");
#endif

	return CLASS_TEXTAREA;
}

/* Get the widget preferred width */
static short htextarea_get_prefered_width(HWidget *p_widget)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea get_prefered_width");
#endif

	if (p_textarea->base.s_prefered_width)
		return p_textarea->base.s_prefered_width;

	p_textarea->base.s_prefered_width = p_textarea->base.p_widget_ops->get_max_width(p_widget);

	return p_textarea->base.s_prefered_width;
}

/* Get the widget preferred height */
static short htextarea_get_prefered_height(HWidget *p_widget)
{
	int i_line_height = 0;
	int i_prefered_height = 0;
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea get_prefered_height");
#endif

	if (p_textarea->base.s_prefered_height)
		return p_textarea->base.s_prefered_height;

	vm_graphic_set_font(((p_textarea->base.c_font) & 7) >> 1);
	i_line_height = vm_graphic_get_character_height(); 

	i_prefered_height = i_line_height * p_textarea->c_line_num + (p_textarea->c_line_num - 1) * p_textarea->c_line_space + 
		p_textarea->base.s_padding_top + p_textarea->base.s_padding_bottom;

	p_textarea->base.s_prefered_height = i_prefered_height;

	return i_prefered_height;
}

/* focus changing action callback function */
static void htextarea_focus_changed(HWidget *p_widget, int i_gain_focus)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;
#ifdef H_DEBUG
	vm_log_debug("htextarea_focus_changed");
#endif // H_DEBUG
	if (i_gain_focus)
		return;

	s_move_from_y = 0;
}

/* if widget need direction keys, return 1 */
static int htextarea_can_travel(HWidget *p_widget, int keycode)
{
	if (VM_KEY_UP == keycode && VM_KEY_DOWN == keycode)
		return 0;

	return 1;
}

/* Paint the HCheckBox */
static void htextarea_paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	VMUINT8 *p_layer_buf;
	HTextArea *p_textarea = (HTextArea *)p_widget;
	p_layer_buf = vm_graphic_get_layer_buffer(i_handle);

#ifdef H_DEBUG
	vm_log_debug("htextarea paint");
#endif

#if 0
	vm_graphic_set_font(((p_textarea->base.c_font) & 7) >> 1);

	vm_graphic_set_clip(s_screen_x, s_screen_y, s_screen_x + p_textarea->base.s_width, s_screen_y + p_textarea->base.s_height);

	vm_graphic_fill_rect(p_layer_buf, s_screen_x, s_screen_y, p_textarea->base.s_width, p_textarea->base.s_height, VM_COLOR_BLACK, VM_COLOR_WHITE);

	if (p_textarea->base.p_widget_ops->has_focus((HWidget *)p_textarea))
		vm_graphic_roundrect(p_layer_buf, s_screen_x, s_screen_y, p_textarea->base.s_width, p_textarea->base.s_height, 4, VM_COLOR_RED);
	else
		vm_graphic_roundrect(p_layer_buf, s_screen_x, s_screen_y, p_textarea->base.s_width, p_textarea->base.s_height, 4, VM_COLOR_BLACK);

	vm_graphic_set_clip(s_screen_x + p_widget->s_padding_left, s_screen_y + p_widget->s_padding_top, 
		s_screen_x + p_textarea->base.s_width - p_widget->s_padding_right, s_screen_y + p_textarea->base.s_height - p_widget->s_padding_bottom);
#endif
	
	look_paint_textarea(p_textarea, i_handle, s_screen_x, s_screen_y);
	draw_text(p_textarea, p_layer_buf, s_screen_x, s_screen_y);
}


/************** for paint functions ***************/
static void append_node(HTextArea *p_textarea, VMWSTR w_str, int i_len)
{
	LineNode *p_line_node;

#ifdef H_DEBUG
	vm_log_debug("htextarea append_node");
#endif

	p_line_node = (LineNode *)vm_malloc(sizeof(LineNode));
	p_line_node->w_line_head = w_str;
	p_line_node->s_line_len = i_len;
	p_line_node->p_next = NULL;
	p_textarea->p_line_list_tail->p_next = p_line_node;
	p_textarea->p_line_list_tail = p_line_node;
}

static void clear_linenode_list(HTextArea *p_textarea) 
{
	LineNode *p_index = NULL;
	LineNode *p_tmp = NULL;

#ifdef H_DEBUG
	vm_log_debug("htextarea clear_linenode_list");
#endif

	p_index = p_textarea->p_line_list->p_next;

	while(p_index) {
		p_tmp = p_index->p_next;
		vm_free(p_index);
		p_index = p_tmp;
	}
	p_textarea->p_line_list->p_next = NULL;
	p_textarea->p_line_list_tail = p_textarea->p_line_list;
}

static void get_linenode_list(HTextArea *p_textarea)
{
	VMWSTR w_tmp;
	VMWSTR w_index;
	int i_len_total = 0;
	int i_len = 0;
	int i_width_total = 0;
	int i_index = 0;
	int i_line_height = 0;

#ifdef H_DEBUG
	vm_log_debug("htextarea get_linenode_list");
#endif
	
	i_len_total = vm_wstrlen(p_textarea->w_text);

#ifdef H_DEBUG
	vm_log_debug("i total length: %d", i_len_total);
#endif // H_DEBUG

	w_tmp = p_textarea->w_text;
	w_index = p_textarea->w_text;

	vm_graphic_set_font(((p_textarea->base.c_font) & 7) >> 1);
	i_line_height = vm_graphic_get_string_height(p_textarea->w_text);

	while (i_index < i_len_total + 1) {
		i_width_total = vm_graphic_get_string_width(w_tmp) - vm_graphic_get_string_width(w_index);
		i_len = vm_wstrlen(w_tmp) - vm_wstrlen(w_index);

		if (*w_index == '\n') {
			append_node(p_textarea, w_tmp, i_len);

			w_tmp = w_index++;
			++i_index;

			continue;
		}

		if ((p_textarea->base.s_width - p_textarea->base.s_padding_left - p_textarea->base.s_padding_right) < i_width_total) {
			append_node(p_textarea, w_tmp, i_len - 1);

			--w_index;
			--i_index;
			w_tmp = w_index;
			continue;
		}

		if (*w_index == '\0') {
			append_node(p_textarea, w_tmp, i_len);

			break;
		}

		++i_index;
		++w_index;
	}

#ifdef H_DEBUG
	vm_log_debug("i total length: %d", i_len_total);
#endif // H_DEBUGX
}

/* locate position by pen touch */
static void locate_position(HTextArea *p_textarea, short s_x, short s_y)
{
	LineNode *p_index_node = NULL;
	VMWSTR w_index_str = NULL;
	VMWSTR w_index_str_next = NULL;
	int i_index = 0;
	int i_line = 0;
	int i_line_height = 0;
	int i_len = 0;
	int i_width = 0;

#ifdef H_DEBUG
	vm_log_debug("htextarea locate_position");
#endif

	vm_graphic_set_font(((p_textarea->base.c_font) & 7) >> 1);
	i_line_height = vm_graphic_get_string_height(p_textarea->w_text);

	/* locate y axis */
	p_index_node = p_textarea->p_line_list->p_next;
	while(p_index_node) {
#ifdef H_DEBUG
		vm_log_debug("line %d length: %d", i_index, p_index_node->s_line_len);
		if (*(p_index_node->w_line_head) == '\n')
			vm_log_debug("line head is \n");
#endif // H_DEBUG

		if (s_y > (p_textarea->s_move_offset + p_textarea->base.s_padding_top + i_index *(i_line_height + p_textarea->c_line_space) - p_textarea->c_line_space / 2)
			&& s_y < p_textarea->s_move_offset + p_textarea->base.s_padding_top + i_index *(i_line_height + p_textarea->c_line_space)
			+ i_line_height + p_textarea->c_line_space / 2) {
				i_line = i_index;
				break;
		}

		++i_index;
		i_len += p_index_node->s_line_len;
		p_index_node = p_index_node->p_next;
	}

	/* click out at text */
	if (!p_index_node)
		return;

#ifdef H_DEBUG
	vm_log_debug("locate line: %d", i_line);
	vm_log_debug("end line position: %d", i_len);
#endif // H_DEBUG
#if 0
while (i_index < p_index_node->s_line_len) {
		i_width = vm_graphic_get_string_width(p_index_node->w_line_head) - vm_graphic_get_string_width(w_index_str);

		vm_log_debug("w_index_str: %s, w_index width: %d, i_index: %d, i_witdh :%d", w_index_str, vm_graphic_get_character_width(*w_index_str), i_index ,i_width);

		if (s_x < p_textarea->base.s_padding_left) {
			++i_index;
			break;
		}

		if (s_x > (p_textarea->base.s_padding_left + i_width - vm_graphic_get_character_width(*w_index_str) / 2)
			&& s_x < (p_textarea->base.s_padding_left + i_width + vm_graphic_get_character_width(*w_index_str) / 2)) {		
				break;
		}

		++i_index;
		++w_index_str;
	}
#endif

/* locate x axis */
i_index = 0;
w_index_str = p_index_node->w_line_head;
	while (i_index < p_index_node->s_line_len) {
		w_index_str_next = w_index_str + 1;
		if (s_x < p_textarea->base.s_padding_left) {
			if (0 != i_line)
				++i_index;
			break;
		}

		if (s_x > (vm_graphic_get_string_width(p_index_node->w_line_head) - vm_graphic_get_string_width(w_index_str))
			&& s_x < (vm_graphic_get_string_width(p_index_node->w_line_head) - vm_graphic_get_string_width(w_index_str_next)))
			break;

		++i_index;
		++w_index_str;
	}
	p_textarea->i_current_position = i_len + i_index;

#ifdef H_DEBUG
	vm_log_debug("locate world position: %d,  current position: %d", i_index, p_textarea->i_current_position);
#endif // H_DEBUG
}

/* draw text, line and cursor */
static void draw_text(HTextArea *p_textarea, VMUINT8 *p_layer_buf, short s_screen_x, short s_screen_y)
{
	LineNode *p_node_index;
	int i_line = 0;
	int i_line_height = 0;
	int i_width = 0;
	int i_len = 0;

#ifdef H_DEBUG
	vm_log_debug("htextarea draw_text");
#endif

	if (p_textarea->c_text_change_flag) {
		clear_linenode_list(p_textarea);
		get_linenode_list(p_textarea);
		p_textarea->c_text_change_flag = 0;
	}

	vm_graphic_set_font(((p_textarea->base.c_font) & 7) >> 1);
	i_line_height = vm_graphic_get_string_height(p_textarea->w_text);

#ifdef H_DEBUG
	vm_log_debug("draw text line height: %d", i_line_height);
#endif // H_DEBUG

	p_node_index = p_textarea->p_line_list->p_next;

	set_actual_clip(s_screen_x + p_textarea->base.s_padding_left, s_screen_y + p_textarea->base.s_padding_top,
		s_screen_x + p_textarea->base.s_width - p_textarea->base.s_padding_right, s_screen_y + p_textarea->base.s_height - p_textarea->base.s_padding_bottom);

	while(p_node_index) {
		/* draw cursor */
		if (p_textarea->i_current_position == 0)
			vm_graphic_line(p_layer_buf, s_screen_x + p_textarea->base.s_padding_left,
			s_screen_y + p_textarea->base.s_padding_top + p_textarea->s_move_offset, 
			s_screen_x + p_textarea->base.s_padding_left,
			s_screen_y + p_textarea->base.s_padding_top + i_line_height + p_textarea->s_move_offset,
			VM_COLOR_BLACK );

		if (p_textarea->i_current_position > i_len && p_textarea->i_current_position <= i_len + p_node_index->s_line_len) {
			vm_graphic_set_font(((p_textarea->base.c_font) & 7) >> 1);
			i_width = vm_graphic_get_string_width(p_node_index->w_line_head) - vm_graphic_get_string_width(p_node_index->w_line_head + (p_textarea->i_current_position - i_len));;

			vm_graphic_line(p_layer_buf, s_screen_x + i_width + p_textarea->base.s_padding_left,
				s_screen_y + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + p_textarea->s_move_offset, 
				s_screen_x + i_width + p_textarea->base.s_padding_left,
				s_screen_y + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + i_line_height + p_textarea->s_move_offset,
				VM_COLOR_BLACK );
		}

		/* draw split line */ 
		if (p_textarea->c_is_split_line) {
#ifdef H_DEBUG
	vm_log_debug("draw splite line");
#endif // H_DEBUG

			vm_graphic_line(p_layer_buf, s_screen_x + p_textarea->base.s_padding_left, 
				p_textarea->s_move_offset + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + i_line_height, 
				s_screen_x + p_textarea->base.s_width - p_textarea->base.s_padding_right, 
				p_textarea->s_move_offset + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + i_line_height, VM_COLOR_BLACK);	
		}

		/* draw text */
		vm_graphic_textout(p_layer_buf, s_screen_x + p_textarea->base.s_padding_left, 
			s_screen_y + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + p_textarea->s_move_offset,
			p_node_index->w_line_head, p_node_index->s_line_len, p_textarea->base.p_widget_ops->get_color((HWidget *)p_textarea));

		++i_line;
		i_len += p_node_index->s_line_len;
		p_node_index = p_node_index->p_next;
	}

	reset_parent_clip();
}

/*********** init and destroy **********/
/* Destroy a HWidget */ 
static void hwidget_delete(HWidget *p_widget)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea hwidget_delete");
#endif

	if (p_textarea->w_text)
		vm_free(p_textarea->w_text);

	if (p_textarea->p_line_list) {
		vm_free(p_textarea->p_line_list);
		clear_linenode_list(p_textarea);
	}

	if(p_textarea)
		vm_free(p_textarea);
}

/* Init a new HTextArea */
extern HTextArea * htextarea_new(int i_input_method, int i_max_len)
{
	HTextArea *p_text_area = (HTextArea *)vm_malloc(sizeof(HTextArea));

#ifdef H_DEBUG
	vm_log_debug("htextarea htextarea_new");
#endif

	hwidget_init(&(p_text_area->base));

	if (!p_textarea_ops) {
		p_textarea_ops = (HTextAreaOperation *)vm_malloc(sizeof(HTextAreaOperation));
		
		p_textarea_ops->get_text = htextarea_get_text;
		p_textarea_ops->set_text = htextarea_set_text;
		p_textarea_ops->insert_text = htextarea_insert_text;
/*		p_textarea_ops->delete_character = htextarea_delete_character;  */
		p_textarea_ops->get_text_len = htextarea_get_text_len;
		p_textarea_ops->set_inputmethod = htextarea_set_inputmethod;
		p_textarea_ops->set_line_num = htextarea_set_line_num;
		p_textarea_ops->set_split_line = htextarea_set_split_line;
		p_textarea_ops->set_line_space = htextarea_set_line_space;
		p_textarea_ops->set_max_len = htextarea_set_max_len;
	}

	if (!p_base_ops) {
		p_base_ops = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		hwidget_operation_init(p_base_ops);
		
		p_base_ops->pen_press = htextarea_pen_press;
		p_base_ops->pen_release = htextarea_pen_release;
		p_base_ops->pen_move = htextarea_pen_move;
		p_base_ops->pen_double_click = htextarea_pen_double_click;
		p_base_ops->pen_long_tap = htextarea_pen_long_tap;
		p_base_ops->key_press = htextarea_key_press;
		p_base_ops->is_container = htextarea_is_container;
		p_base_ops->is_plane = htextarea_is_plane;
		p_base_ops->get_class = htextarea_get_class;
		p_base_ops->get_prefered_width = htextarea_get_prefered_width;
		p_base_ops->get_prefered_height = htextarea_get_prefered_height;
		p_base_ops->focus_changed = htextarea_focus_changed;
		p_base_ops->can_travel = htextarea_can_travel;
		p_base_ops->paint = htextarea_paint;
		p_base_ops->destroy = hwidget_delete;
	}

	/* Init private members */
	p_text_area->c_line_num = 4;
	p_text_area->c_is_split_line = 0;
	p_text_area->c_line_space = 4;
	p_text_area->c_input_method = VM_INPUT_METHOD_TEXT;
	p_text_area->i_max_len = 140;
	p_text_area->s_move_offset = 0;
	p_text_area->c_text_change_flag = 0;

	p_text_area->base.p_widget_ops = p_base_ops;
	p_text_area->p_htextarea_ops = p_textarea_ops;
	p_text_area->base.action_performed = NULL;

	p_text_area->p_line_list = (LineNode *)vm_malloc(sizeof(LineNode));
	p_text_area->p_line_list->p_next = NULL;
	p_text_area->p_line_list_tail = p_text_area->p_line_list;

	/* init HWiget attr */
	p_text_area->base.s_padding_left = 4;
	p_text_area->base.s_padding_right = 4;
	p_text_area->base.s_padding_top = 4;
	p_text_area->base.s_padding_bottom = 4;
	p_text_area->base.c_font = FONT_SMALL;

	return p_text_area;
}

/* Destroy a HTextArea */
extern void htextarea_delete(HTextArea *p_textarea)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea htextarea_delete");
#endif

	if (p_textarea->w_text)
		vm_free(p_textarea->w_text);

	if (p_textarea->p_line_list) {
		vm_free(p_textarea->p_line_list);
		clear_linenode_list(p_textarea);
	}

	if(p_textarea)
		vm_free(p_textarea);
}

/* delete HTextArea global operations */
extern void htextarea_ops_delete()
{
#ifdef H_DEBUG
	vm_log_debug("htextarea_ops_delete");
#endif

	if (p_textarea_ops)
		vm_free(p_textarea_ops);
	if (p_base_ops)
		vm_free(p_base_ops);
}

/********************************EOF***********************************/