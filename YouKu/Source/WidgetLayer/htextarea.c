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
#include "vmlog.h"

struct _HTextGlobal {
	char c_timer_id;
	char c_cursor_flag;

	short s_move_from_x;
	short s_move_from_y;
	short s_textarea_height;

	VMWSTR w_str;

	HTextArea *p_textarea;
};

static HTextAreaOperation *p_textarea_ops;		/* Single instance of HTextArea Operations  */
static HWidgetOperation *p_base_ops;			/* Single instance of HWidget Operations  */
static int i_ops;							/* Number of HTextArea instances */

static HTextArea *p_textarea_buf;   /* Template pointer for calling input method */

static HTextGlobal *p_text_global;

static void create_global();
static void destroy_global();

static void get_w_str(HTextArea *p_textarea);
static void parser_w_str(HTextArea *p_textarea);
static void locate_position(HTextArea *p_textarea, short s_x, short s_y);

/* Call this function after getting focus */
static void create_global()
{
#ifdef H_DEBUG
	vm_log_debug("htextarea create_global");
#endif

	if (p_text_global)
		vm_free(p_text_global);

	p_text_global = (HTextGlobal *)vm_malloc(sizeof(HTextGlobal));
	p_text_global->c_cursor_flag = 0;
	p_text_global->s_move_from_x = 0;
	p_text_global->s_move_from_y = 0;
	p_text_global->s_textarea_height = 0;
}

/* Call this function after losing focus */
static void destroy_global()
{
#ifdef H_DEBUG
	vm_log_debug("htextarea destroy_global");
#endif

	if (p_text_global)
		vm_free(p_text_global);
	p_text_global = NULL;
}

static void get_w_str(HTextArea *p_textarea)
{
	int i_len_total;

#ifdef H_DEBUG
	vm_log_debug("htextarea get_w_str");
#endif

	i_len_total = strlen(p_textarea->pc_text);
	p_text_global->w_str = (VMWSTR)vm_malloc((i_len_total + 1) * 2);
	vm_gb2312_to_ucs2(p_text_global->w_str,(i_len_total + 1) * 2, p_textarea->pc_text);
}

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

static void clear_list(HTextArea *p_textarea) 
{
	LineNode *p_index = NULL;
	LineNode *p_tmp = NULL;

#ifdef H_DEBUG
	vm_log_debug("htextarea clear_list");
#endif

	p_index = p_textarea->p_line_list->p_next;

	while(p_index) {
		p_tmp = p_index->p_next;
		vm_free(p_index);
		p_index = p_tmp;
	}
}

static void parser_w_str(HTextArea *p_textarea)
{
	VMWSTR w_tmp;
	VMWSTR w_index;
	int i_len_total = 0;
	int i_len = 0;
	int i_width_total = 0;
	int i_index = 0;
	int i_line_height = 0;
	char c_font;

#ifdef H_DEBUG
	vm_log_debug("htextarea parser_w_str");
#endif

	c_font = ((p_textarea->base.c_font) & 7) >> 1;
	vm_graphic_set_font(c_font);

	i_len_total = vm_wstrlen(p_text_global->w_str);

	vm_log_debug("i total length: %d", i_len_total);

	w_tmp = p_text_global->w_str;
	w_index = p_text_global->w_str;

	i_line_height = vm_graphic_get_string_height(p_text_global->w_str);

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
	vm_log_debug("index : %d", i_index);

	p_textarea->c_text_change_flag = 0;
}


/* HTextArea Operations */

/* Set text to HTextArea */ 
static void htextarea_set_text(HTextArea *p_textarea, char *pc_text)
{
	int i_len;

#ifdef H_DEBUG
	vm_log_debug("htextarea set_text");
#endif

	if (!pc_text)
		return;

	i_len = strlen(pc_text);

	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	p_textarea->pc_text = (char *)vm_malloc(i_len + 1);
	strncpy(p_textarea->pc_text, pc_text, i_len);
	p_textarea->pc_text[i_len] = '\0';

	p_textarea->i_current_position = p_textarea->p_htextarea_ops->get_text_len(p_textarea);

	/* set text changing flag */
	p_textarea->c_text_change_flag = 1;
}

/* Get text from HTextArea */
static char* htextarea_get_text(HTextArea *p_textarea)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea get_text");
#endif

	return p_textarea->pc_text;
}

/* Insert text to HTextArea's current cursor position */
static void htextarea_insert_text(HTextArea *p_textarea, char *pc_text)
{
	/************************************************************************/
	/* Fix me                                                                     */
	/************************************************************************/
	int i_len_text = 0;
	int i_len_insert = 0;
	VMWSTR w_buf = NULL;
	VMWSTR w_text_insert = NULL;
	VMWSTR w_index = NULL;

	char *pc_buf;

#ifdef H_DEBUG
	vm_log_debug("htextarea insert_text");
#endif

	if (!pc_text)
		return;

	if (p_text_global->w_str)
		vm_free(p_text_global->w_str);

	i_len_text = strlen(p_textarea->pc_text) + strlen(pc_text);
	i_len_text = i_len_text > p_textarea->i_max_len ? p_textarea->i_max_len : i_len_text;

	/* convert text of HTextArea from gb2312 to ucs2 */
	p_text_global->w_str = (VMWSTR)vm_malloc((i_len_text + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(p_text_global->w_str, (i_len_text + 1) * sizeof(VMWCHAR), p_textarea->pc_text);
	p_text_global->w_str[i_len_text] = '\0';
	i_len_text = vm_wstrlen(p_text_global->w_str);
	
	/* free memery of HTextArea's text */
	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	/* convert insert text from gb2312 to ucs2 */
	i_len_insert = strlen(pc_text);
	w_text_insert = (VMWSTR)vm_malloc((i_len_insert + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_text_insert, (i_len_insert + 1) * sizeof(VMWCHAR), pc_text);
	w_text_insert[i_len_insert] = '\0';
	i_len_insert = vm_wstrlen(w_text_insert);

	w_buf = (VMWSTR)vm_malloc((i_len_text + i_len_insert + 1) * sizeof(VMWCHAR));

	/* insert text to HTextArea */
	w_index = w_buf;
	vm_wstrncpy(w_index, p_text_global->w_str, p_textarea->i_current_position);
	w_index += p_textarea->i_current_position;
	vm_wstrncpy(w_index, w_text_insert, i_len_insert);
	w_index += i_len_insert;
	vm_wstrncpy(w_index, p_text_global->w_str + p_textarea->i_current_position, i_len_text - p_textarea->i_current_position);	
	w_buf[i_len_insert + i_len_text] = '\0';

	/* free VMWSTR temp vars */
	vm_free(w_text_insert);
	vm_free(p_text_global->w_str);
	p_text_global->w_str = NULL;

	/* convert the final text from ucs2 to gb2312 */
	pc_buf = (char *)vm_malloc((i_len_insert + i_len_text + 1) * 2);
	vm_ucs2_to_gb2312(pc_buf, (i_len_insert + i_len_text + 1) * 2, w_buf);

	/* free VMWSTR final text */
	vm_free(w_buf);

	i_len_text = strlen(pc_buf);
	p_textarea->pc_text = (char *)vm_malloc(i_len_text + 1);
	strncpy(p_textarea->pc_text, pc_buf, i_len_text);
	p_textarea->pc_text[i_len_text] = '\0';

	/* free char * temp var */
	vm_free(pc_buf);

	p_textarea->i_current_position = p_textarea->p_htextarea_ops->get_text_len(p_textarea);

	p_textarea->c_text_change_flag = 1;
}

static void htextarea_delete_character(HTextArea *p_textarea)
{
	int i_len;
	VMWSTR w_buf = NULL;
	VMWSTR w_index = NULL;
	char *pc_buf;

	if (p_textarea->i_current_position == 0)
		return;

#ifdef H_DEBUG
	vm_log_debug("htextarea delete_text");
#endif
	i_len = strlen(p_textarea->pc_text);

	/* convert text of HTextArea from gb2312 to ucs2 */
	p_text_global->w_str = (VMWSTR)vm_malloc((i_len + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(p_text_global->w_str, (i_len + 1) * sizeof(VMWCHAR), p_textarea->pc_text);
	p_text_global->w_str[i_len] = '\0';
	i_len = vm_wstrlen(p_text_global->w_str);

	w_index = p_text_global->w_str + p_textarea->i_current_position - 1;
	while(w_index++) {
		w_index = w_index + 1;
	}
	p_text_global->w_str[i_len - 1] = '\0';

	/* convert the final text from ucs2 to gb2312 */
	pc_buf = (char *)vm_malloc(i_len * 2);
	vm_ucs2_to_gb2312(pc_buf, i_len * 2, w_buf);

	/* free VMWSTR final text */
	vm_free(p_text_global->w_str);

	i_len = strlen(pc_buf);
	p_textarea->pc_text = (char *)vm_malloc(i_len + 1);
	strncpy(p_textarea->pc_text, pc_buf, i_len);
	p_textarea->pc_text[i_len] = '\0';

	/* free char * temp var */
	vm_free(pc_buf);

	p_textarea->i_current_position = p_textarea->p_htextarea_ops->get_text_len(p_textarea);

	p_textarea->c_text_change_flag = 1;
}

/* Get length HTextArea's text */
static int htextarea_get_text_len(HTextArea *p_textarea)
{
	VMWSTR w_str;
	int i_len;

#ifdef H_DEBUG
	vm_log_debug("htextarea get_text_len");
#endif

	i_len = strlen(p_textarea->pc_text);

	w_str = (VMWSTR)vm_malloc((i_len + 1) * sizeof(VMWCHAR));
	vm_gb2312_to_ucs2(w_str, (i_len + 1) * sizeof(VMWCHAR), p_textarea->pc_text);
	i_len = vm_wstrlen(w_str);

	vm_free(w_str);

	return i_len;
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

/* Set line number of HTextArea */
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

/* Set line space of HTextArea */
static void htextarea_set_line_space(HTextArea *p_textarea, int i_line_space)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea set_line_space");
#endif

	p_textarea->c_line_space = i_line_space;
}

/* Internal functions */

/* Input method callback function */
static void htextarea_input_method_callback(VMINT state, VMWSTR str)
{
	char *p_buf;

#ifdef H_DEBUG
	vm_log_debug("htextarea input_method_callback");
#endif

	if (VM_INPUT_OK != state)
		return;

	p_buf = (char *)vm_malloc((vm_wstrlen(str) + 1) * sizeof(VMWCHAR));
	vm_ucs2_to_gb2312(p_buf, (vm_wstrlen(str) + 1) * sizeof(VMWCHAR), str);

	printf("get string from input method: %s\n", p_buf);

	p_textarea_buf->p_htextarea_ops->insert_text(p_textarea_buf, p_buf);

	vm_free(p_buf);

	p_textarea_buf->base.p_widget_ops->repaint((HWidget *)p_textarea_buf);
	if (p_textarea_buf->base.action_performed)
		p_textarea_buf->base.action_performed((HWidget *)p_textarea_buf, NULL);

	p_textarea_buf = NULL;

}

/* Call input method */
static void htextarea_call_input_method(HTextArea *p_textarea)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea call_input_method");
#endif

	p_textarea_buf = p_textarea;
	vm_input_text3(NULL, p_textarea->i_max_len, p_textarea->c_input_method, htextarea_input_method_callback);
}

/* Base class Operations */
/* Pen press event callback */
static void htextarea_pen_press(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea pen_press");
	vm_log_debug("pen press  x: %d, y: %d", s_x, s_y);
#endif

	if (!p_textarea->base.p_widget_ops->is_enable(p_widget))
		return;

	if (p_textarea->base.p_widget_ops->is_enable_focus(p_widget) && !p_textarea->base.p_widget_ops->has_focus(p_widget)) {
		p_textarea->base.p_widget_ops->set_focus(p_widget, 1);

#ifdef H_DEBUG
	vm_log_debug("htextarea focus: %d\n", p_textarea->base.c_font);
#endif
	}

	p_text_global->s_move_from_x = s_x;
	p_text_global->s_move_from_y = s_y;

	/* Get cursor position by pen event. */
	locate_position(p_textarea, s_x, s_y);
	p_textarea->base.p_widget_ops->repaint(p_widget);
}

/* Pen release event callback */
static void htextarea_pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea pen_release");
#endif

	if (!p_textarea->base.p_widget_ops->is_enable(p_widget))
		return;

	if (p_textarea->base.p_widget_ops->has_focus(p_widget))
		htextarea_call_input_method(p_textarea);
}

/* pen move enter callback*/
static void htextarea_pen_enter(HWidget *p_widget, short s_x, short s_y)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea pen_enter");
#endif
}

/* pen move leave callback*/
static void htextarea_pen_leave(HWidget *p_widget, short s_x, short s_y)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea pen_leave");
#endif
}

/* Pen move event callback */
static void htextarea_pen_move(HWidget *p_widget, short s_x, short s_y)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea pen_move");
	vm_log_debug("pen move x: %d, y: %d, from_x: %d, from_y: %d. offset: %d", s_x, s_y, 
		p_text_global->s_move_from_x, p_text_global->s_move_from_y, p_textarea->s_move_offset);
#endif

	if ((s_y - p_text_global->s_move_from_y) > 2) {
		vm_log_debug("+++++++++ move down\n");
		p_textarea->s_move_offset += 4;

		p_widget->p_widget_ops->repaint(p_widget);
	}

	if ((s_y - p_text_global->s_move_from_y) < -2) {
		vm_log_debug("+++++++++ move up\n");
		p_textarea->s_move_offset -= 4;

		p_widget->p_widget_ops->repaint(p_widget);
	}

	p_text_global->s_move_from_y = s_y;
}

/* Keyboard press event callback */
static void htextarea_key_press(HWidget *p_widget, int keycode)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea key_press");
#endif

	if (!p_textarea->base.p_widget_ops->is_enable(p_widget))
		return;

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
	int i_line_height;
	int i_prefered_height;
	char c_font;
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea get_prefered_height");
#endif

	if (p_textarea->base.s_prefered_height)
		return p_textarea->base.s_prefered_height;

	c_font = ((p_textarea->base.c_font) & 7) >> 1;
	vm_graphic_set_font(c_font);

	i_line_height = vm_graphic_get_character_height(); 

	i_prefered_height = i_line_height * p_textarea->c_line_num + (p_textarea->c_line_num - 1) * p_textarea->c_line_space + 
		p_textarea->base.s_padding_top + p_textarea->base.s_padding_bottom;

	p_textarea->base.s_prefered_height = i_prefered_height;

	return i_prefered_height;
}

static void locate_position(HTextArea *p_textarea, short s_x, short s_y)
{
	LineNode *p_index_node = NULL;
	VMWSTR w_index_str = NULL;
	int i_index = 0;
	int i_line = 0;
	char c_font;
	int i_line_height = 0;
	int i_len = 0;
	int i_width = 0;

#ifdef H_DEBUG
	vm_log_debug("htextarea locate_position");
#endif
	
	c_font = ((p_textarea->base.c_font) & 7) >> 1;
	vm_graphic_set_font(c_font);
	i_line_height = vm_graphic_get_string_height(p_text_global->w_str);

	/* locate y axis */
	p_index_node = p_textarea->p_line_list->p_next;
	while(p_index_node) {
		vm_log_debug("line %d length: %d", i_index, p_index_node->s_line_len);

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

	vm_log_debug("locate line: %d", i_line);
	vm_log_debug("end line position: %d", i_len);
	i_index = 0;

	/* locate x axis */
	w_index_str = p_index_node->w_line_head;
	while (i_index < p_index_node->s_line_len) {
		i_width = vm_graphic_get_string_width(p_index_node->w_line_head) - vm_graphic_get_string_width(w_index_str);

		vm_log_debug("i_witdh :%d", i_width);
 
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

	vm_log_debug("locate world position: %d", i_index);
	p_textarea->i_current_position = i_len + i_index;
	vm_log_debug("cursor position: %d", i_len);
}

static void draw_text(HTextArea *p_textarea, VMUINT8 *p_layer_buf, short s_screen_x, short s_screen_y)
{
	LineNode *p_node_index;
	int i_line = 0;
	int i_line_height = 0;
	int i_width = 0;
	int i_len = 0;
	char c_font;

#ifdef H_DEBUG
	vm_log_debug("htextarea draw_text");
#endif

	if (p_textarea->c_text_change_flag) {
		clear_list(p_textarea);
		get_w_str(p_textarea);
		parser_w_str(p_textarea);
	}

	c_font = ((p_textarea->base.c_font) & 7) >> 1;
	vm_graphic_set_font(c_font);
	i_line_height = vm_graphic_get_string_height(p_text_global->w_str);

	p_node_index = p_textarea->p_line_list->p_next;

	while(p_node_index) {
		/* draw text */
		vm_graphic_textout(p_layer_buf, s_screen_x + p_textarea->base.s_padding_left, 
			s_screen_y + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + p_textarea->s_move_offset,
			p_node_index->w_line_head, p_node_index->s_line_len, p_textarea->base.p_widget_ops->get_color((HWidget *)p_textarea));

		/* draw split line */ 
		if (p_textarea->c_is_split_line)
			vm_graphic_line(p_layer_buf, s_screen_x + p_textarea->base.s_padding_left, 
				p_textarea->s_move_offset + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + i_line_height, 
				s_screen_x + p_textarea->base.s_width - p_textarea->base.s_padding_right, 
				p_textarea->s_move_offset + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + i_line_height, VM_COLOR_BLUE);	

		/* draw cursor */
		if (p_textarea->i_current_position == 0)
			vm_graphic_line(p_layer_buf, s_screen_x + p_textarea->base.s_padding_left,
			s_screen_y + p_textarea->base.s_padding_top + p_textarea->s_move_offset, 
			s_screen_x + p_textarea->base.s_padding_left,
			s_screen_y + p_textarea->base.s_padding_top + i_line_height + p_textarea->s_move_offset,
			VM_COLOR_BLUE );

		if (p_textarea->i_current_position > i_len && p_textarea->i_current_position <= i_len + p_node_index->s_line_len) {
				i_width = vm_graphic_get_string_width(p_node_index->w_line_head) - vm_graphic_get_string_width(p_node_index->w_line_head + (p_textarea->i_current_position - i_len));;

				vm_graphic_line(p_layer_buf, s_screen_x + i_width + p_textarea->base.s_padding_left,
					s_screen_y + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + p_textarea->s_move_offset, 
					s_screen_x + i_width + p_textarea->base.s_padding_left,
					s_screen_y + p_textarea->base.s_padding_top + i_line * (p_textarea->c_line_space + i_line_height) + i_line_height + p_textarea->s_move_offset,
					VM_COLOR_BLUE );
		}

		++i_line;
		i_len += p_node_index->s_line_len;
		p_node_index = p_node_index->p_next;
	}
}

/* Paint the HCheckBox */
static void htextarea_paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	VMUINT8 *p_layer_buf;
	HTextArea *p_textarea = (HTextArea *)p_widget;
	char c_font;

#ifdef H_DEBUG
	vm_log_debug("htextarea paint");
#endif

	p_layer_buf = vm_graphic_get_layer_buffer(i_handle);
	c_font = ((p_textarea->base.c_font) & 7) >> 1;
	vm_graphic_set_font(c_font);

	vm_graphic_set_clip(s_screen_x, s_screen_y, s_screen_x + p_textarea->base.s_width, s_screen_y + p_textarea->base.s_height);

	if (p_textarea->base.p_widget_ops->has_focus((HWidget *)p_textarea))
		vm_graphic_roundrect(p_layer_buf, s_screen_x, s_screen_y, p_textarea->base.s_width, p_textarea->base.s_height, 4, VM_COLOR_RED);
	else
		vm_graphic_roundrect(p_layer_buf, s_screen_x, s_screen_y, p_textarea->base.s_width, p_textarea->base.s_height, 4, VM_COLOR_BLACK);

	vm_graphic_set_clip(s_screen_x + p_widget->s_padding_left, s_screen_y + p_widget->s_padding_top, 
		s_screen_x + p_textarea->base.s_width - p_widget->s_padding_left, s_screen_y + p_textarea->base.s_height - p_widget->s_padding_top);

	draw_text(p_textarea, p_layer_buf, s_screen_x, s_screen_y);

	/************************************************************************/
	/* Todo: don't forget delete the follow line                                  */
	/************************************************************************/
}

/* Destroy a HWidget */ 
static void hwidget_delete(HWidget *p_widget)
{
	HTextArea *p_textarea = (HTextArea *)p_widget;

#ifdef H_DEBUG
	vm_log_debug("htextarea hwidget_delete");
#endif

	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	if (p_textarea->p_line_list) {
		vm_free(p_textarea->p_line_list);
		clear_list(p_textarea);
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
		p_textarea_ops->delete_character = htextarea_delete_character;
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
		p_base_ops->pen_enter = htextarea_pen_enter;
		p_base_ops->pen_leave = htextarea_pen_leave;
		p_base_ops->key_press = htextarea_key_press;
		p_base_ops->is_container = htextarea_is_container;
		p_base_ops->is_plane = htextarea_is_plane;
		p_base_ops->get_class = htextarea_get_class;
		p_base_ops->get_prefered_width = htextarea_get_prefered_width;
		p_base_ops->get_prefered_height = htextarea_get_prefered_height;
		p_base_ops->paint = htextarea_paint;
		p_base_ops->destroy = hwidget_delete;
	}

	if (!p_text_global)
		create_global();

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
	p_text_area->base.s_padding_left = 8;
	p_text_area->base.s_padding_right = 8;
	p_text_area->base.s_padding_top = 8;
	p_text_area->base.s_padding_bottom = 8;
	p_text_area->base.c_font = FONT_SMALL;

	return p_text_area;
}

/* Destroy a HTextArea */
extern void htextarea_delete(HTextArea *p_textarea)
{
#ifdef H_DEBUG
	vm_log_debug("htextarea htextarea_delete");
#endif

	if (p_textarea->pc_text)
		vm_free(p_textarea->pc_text);

	if (p_textarea->p_line_list) {
		vm_free(p_textarea->p_line_list);
		clear_list(p_textarea);
	}

	if(p_textarea)
		vm_free(p_textarea);
}

/* delete HTextArea global operations */
extern void htextarea_ops_delete()
{
	if (p_textarea_ops)
		vm_free(p_textarea_ops);
	if (p_base_ops)
		vm_free(p_base_ops);

	destroy_global();
}

/********************************EOF***********************************/