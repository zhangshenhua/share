/**********************************************************************************
*file: hlookandfeel.c
*author: yi.jiang
*date: 2011.08.29
*description: draw specific widget into specify layer by getting widget structure.
***********************************************************************************/
#include "hlookandfeel.h"

/*
* name : look_draw_plane
* description : draw plane
*/
void look_draw_plane(HPlane *p_plane)
{
	char *p_layer_buf;
	int i_gray_handle;
	char *p_gray_buf;
	vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(),vm_graphic_get_screen_height());
	p_layer_buf = vm_graphic_get_layer_buffer(p_plane->i_handle);
	i_gray_handle = vm_graphic_create_canvas(vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
	p_gray_buf = vm_graphic_get_canvas_buffer(i_gray_handle);
	vm_graphic_fill_rect(p_gray_buf, 0, 0, 
	vm_graphic_get_screen_width(), vm_graphic_get_screen_height(),VM_COLOR_BLACK, VM_COLOR_BLACK);

	vm_graphic_blt_ex(p_layer_buf, 0, 0, p_gray_buf, 0, 0, 
	vm_graphic_get_screen_width(), vm_graphic_get_screen_height(), 1,150);
	vm_graphic_release_canvas(i_gray_handle);
}

/*
 * name : look_paint_pushbutton
 * description : draw pushbutton into a layer
 */
void look_paint_pushbutton(HPushButton *p_pushbutton, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	HWidget* p_widget;
	VMUINT16 f_color;

	p_widget = (HWidget *)p_pushbutton;

	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);
	if(p_widget->p_widget_ops->has_focus(p_widget))
		f_color = VM_COLOR_BLACK;
	else
		f_color = p_widget->i_bgcolor;
	draw_rect_by_widget_trans(i_layer_handle,p_widget,i_screen_x,i_screen_y,f_color);

	if(p_pushbutton->text == NULL)
		return;
	draw_gb2312_text_by_widget_center(i_layer_handle, p_widget, p_pushbutton->text, i_screen_x, i_screen_y);
}

/*
* name : look_paint_textinput
* description : draw text input into a layer
*/
void look_paint_textinput(HTextInput *p_textinput, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	int			i;
	HWidget*	p_widget;
	VMINT		i_ucs2_max_len;
	VMWCHAR *	p_ucs2_string;
	VMUINT16	f_color;

	p_widget = (HWidget *)p_textinput;
	
	/* draw rect */
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);

	if(p_widget->p_widget_ops->has_focus(p_widget))
		f_color = VM_COLOR_RED;
	else
		f_color = VM_COLOR_BLACK;
	draw_rect_by_widget_trans(i_layer_handle, p_widget, i_screen_x, i_screen_y, f_color);

	if(p_textinput->pc_text ==NULL)
		return;
	i_ucs2_max_len = strlen(p_textinput->pc_text);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );

	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_textinput->pc_text);
	if (p_textinput->c_is_password){
		for( i = 0;p_ucs2_string[i] != '\0';i++)
			p_ucs2_string[i] = '*';
	}

	draw_ucs2_text_by_widget_oneline(i_layer_handle, p_widget, p_ucs2_string, i_screen_x, i_screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : look_paint_textarea
* description : draw text area into a layer
*/
extern void look_paint_textarea(HTextArea *p_textarea, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	HWidget*	p_widget;
	VMINT		i_ucs2_max_len;
	VMWCHAR *	p_ucs2_string;

	VMUINT16	f_color;
	VMUINT8*	p_layer_buf = NULL;

	if (i_layer_handle < 0){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL){
		return;
	}
	p_widget = (HWidget *)p_textarea;

	/* draw rect */
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);

	if(p_widget->p_widget_ops->has_focus(p_widget))
		f_color = VM_COLOR_RED;
	else
		f_color = VM_COLOR_BLACK;
	draw_rect_by_widget_trans(i_layer_handle, p_widget, i_screen_x, i_screen_y, f_color);

	if(p_textarea->pc_text ==NULL)
		return;
	i_ucs2_max_len = strlen(p_textarea->pc_text);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );

	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_textarea->pc_text);

	draw_ucs2_text_by_widget_more_line(i_layer_handle,p_widget,p_ucs2_string,i_screen_x,i_screen_y,p_textarea->c_line_space);

	vm_free(p_ucs2_string);
}

/*
* name : look_paint_label
* description : draw label into a layer
*/
void look_paint_label(HLabel *p_label, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	HWidget* p_widget;
	VMUINT8* p_layer_buf;

	VMINT i_ucs2_max_len;
	VMWCHAR *p_ucs2_string;
	VMUINT16 f_color;

	p_widget = (HWidget *)p_label;
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);	

	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);
	if(p_widget->p_widget_ops->has_focus(p_widget))
		f_color = VM_COLOR_BLACK;
	else
		f_color = p_widget->i_bgcolor;
	draw_rect_by_widget_trans(i_layer_handle, p_widget, i_screen_x, i_screen_y, f_color);

	if (p_label->text == NULL)
		return;
	i_ucs2_max_len = strlen(p_label->text);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_label->text);

	draw_ucs2_text_by_widget_more_line(i_layer_handle,p_widget,p_ucs2_string,i_screen_x,i_screen_y,p_label->text_row_spacing);

	vm_free(p_ucs2_string);
}

/*
* name : draw_rect_by_widget
* description : draw rect into a layer by widget struction
* description : the filled color is background color
*/
void draw_rect_by_widget(VMINT i_layer_handle,HWidget* p_widget, int i_screen_x, int i_screen_y)
{
	VMUINT8*	p_layer_buf = NULL;
	if (i_layer_handle < 0){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL){
		return;
	}
	vm_graphic_fill_rect(p_layer_buf, i_screen_x, i_screen_y,
		p_widget->s_width, p_widget->s_height, p_widget->i_bgcolor, p_widget->i_bgcolor);
}


/*
* name : draw_rect_by_widget_trans
* description : draw rect into a layer by widget struction
* description : the filled color is background color
*/
void draw_rect_by_widget_trans(VMINT i_layer_handle,HWidget* p_widget, int i_screen_x, int i_screen_y, VMUINT16 f_color)
{
	VMUINT8*	p_layer_buf = NULL;
	if (i_layer_handle < 0){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL){
		return;
	}
	vm_graphic_rect(p_layer_buf, i_screen_x, i_screen_y,
		p_widget->s_width, p_widget->s_height, f_color);
}

/*
* name : draw_rect_by_hrect
* description : draw rect into a layer
*/
void draw_rect_by_hrect(VMINT i_layer_handle, HRect p_rect, VMUINT16 f_color, VMUINT16 bg_color)
{
	VMINT8	*p_layer_buf = NULL;
	if (i_layer_handle < 0){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL){
		return;
	}
	vm_graphic_fill_rect(p_layer_buf, p_rect.s_x, p_rect.s_y, 
		p_rect.s_width, p_rect.s_height, f_color, bg_color);
}

/*
* name : draw_rect_by_hrect_trans
* description : draw the rect frame into a layer
*/
void draw_rect_by_hrect_trans(VMINT i_layer_handle, HRect p_rect, VMUINT16 f_color)
{
	VMINT8	*p_layer_buf  = NULL;
	if (i_layer_handle < 0){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL){
		return;
	}
	vm_graphic_rect(p_layer_buf, p_rect.s_x, p_rect.s_y, p_rect.s_width, p_rect.s_height, f_color);
}



/*
* name : draw_gb2312_text_by_hpoint
* description : draw rect into a layer
*/
void draw_gb2312_text_by_hpoint(VMINT i_layer_handle, char *p_char_str, HPoint p_point, VMUINT16 ui_color)
{
	int						i_ucs2_cur_len;
	VMINT				i_ucs2_max_len;
	VMWCHAR		*p_ucs2_string;
	VMUINT8			*p_layer_buf = NULL;

	if ((NULL == p_char_str) || (i_layer_handle < 0)){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (NULL == p_layer_buf){
		return;
	}
	i_ucs2_max_len = strlen(p_char_str);
	//	printf("char:%s, i_ucs2_max_len:%d\n",p_char_str, i_ucs2_max_len);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	if (p_ucs2_string != NULL)
		//the second para is the size of bytes,include '\0'
		vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_char_str);
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;
	vm_graphic_textout(p_layer_buf, p_point.s_x, p_point.s_y, p_ucs2_string, i_ucs2_cur_len, ui_color);

	vm_free(p_ucs2_string);
}

/*
* name : draw_gb2312_text_by_widget
* description : draw GB2312 string into a layer,
* description : color of the filled is foreground color
 */
void draw_gb2312_text_by_widget(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y)
{
	VMINT		i_ucs2_max_len;
	VMWCHAR		*p_ucs2_string;

	if ((NULL == p_char_str) || (i_layer_handle < 0)){
		return;
	}
	i_ucs2_max_len = strlen(p_char_str);
//	printf("char:%s, i_ucs2_max_len:%d\n",p_char_str, i_ucs2_max_len);

	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	if (p_ucs2_string == NULL){
		return;
	}
	//the second para is the size of bytes,include '\0'
	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_char_str);

	draw_ucs2_text_by_widget(i_layer_handle, p_widget, p_ucs2_string,i_screen_x,i_screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : draw_gb2312_text_by_hpoint_online
* description : draw GB2312 string into a layer
* description : color of the filled is foreground color
* description : the string is one line
*/
void draw_gb2312_text_by_widget_oneline(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y)
{
	VMINT		i_ucs2_max_len;
	VMWCHAR		*p_ucs2_string;

	if ((NULL == p_char_str) || (i_layer_handle < 0)){
		return;
	}
	i_ucs2_max_len = strlen(p_char_str);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	if (p_ucs2_string == NULL){
		return;
	}
	//the second para is the size of bytes,include '\0'
	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_char_str);

	draw_ucs2_text_by_widget_oneline(i_layer_handle, p_widget, p_ucs2_string,i_screen_x,i_screen_y);

	vm_free(p_ucs2_string);
}


/*
* name : draw_gb2312_text_by_hpoint_center
* description : draw GB2312 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
*/
void draw_gb2312_text_by_widget_center(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y)
{
	VMINT		i_ucs2_max_len;
	VMWCHAR		*p_ucs2_string;

	if ((NULL == p_char_str) || (i_layer_handle < 0)){
		return;
	}
	i_ucs2_max_len = strlen(p_char_str);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	if (p_ucs2_string == NULL){
		return;
	}
	//the second para is the size of bytes,include '\0'
	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_char_str);

	draw_ucs2_text_by_widget_center(i_layer_handle, p_widget, p_ucs2_string,i_screen_x,i_screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : draw_ucs2_text_by_hpoint
* description : draw rect into a layer
*/
void draw_ucs2_text_by_hpoint(VMINT i_layer_handle, VMWCHAR *p_ucs2_string, HPoint p_point, VMUINT16 ui_color)
{
	int									i_ucs2_cur_len;
	VMUINT8						*p_layer_buf = NULL;

	if ((NULL == p_ucs2_string) || (i_layer_handle < 0)){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (NULL == p_layer_buf){
		return;
	}
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;

	vm_graphic_textout(p_layer_buf, p_point.s_x, p_point.s_y, p_ucs2_string, i_ucs2_cur_len, ui_color);

}

/*
* name : draw_ucs2_text_by_widget
* description : draw UCS2 string into a layer,
* description : color of the filled is foreground color
 */
void draw_ucs2_text_by_widget(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y)
{
	int i_ucs2_cur_len;
	VMUINT8		*p_layer_buf = NULL;

	int i_padding_x;
	int i_padding_y;
	int i_padding_width;
	int i_padding_height;

	if ((NULL == p_ucs2_string) || (i_layer_handle < 0)){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (NULL == p_layer_buf){
		return;
	}
	i_padding_x = i_screen_x+p_widget->uc_padding_left;
	i_padding_y = i_screen_y+p_widget->uc_padding_top;
	i_padding_width = p_widget->s_width-p_widget->uc_padding_left-p_widget->uc_padding_right;
	i_padding_height = p_widget->s_height-p_widget->uc_padding_bottom-p_widget->uc_padding_top;

	//i_ucs2_cur_len >= i_gb2312_string_len/2 && i_ucs2_cur_len <= i_gb2312_string_len
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;
	vm_graphic_set_font(p_widget->c_font);
//	print_string_state(p_ucs2_string); 
	vm_graphic_set_clip(i_padding_x, i_padding_y, 
		i_padding_x+	i_padding_width, i_padding_y+i_padding_height);
	vm_graphic_textout(p_layer_buf, 
		i_screen_x+p_widget->uc_padding_left, i_screen_y+p_widget->uc_padding_top,
		p_ucs2_string, i_ucs2_cur_len, p_widget->i_color);
	vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

/*
* name : draw_ucs2_text_by_widget_oneline
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is one line
*/
void draw_ucs2_text_by_widget_oneline(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y)
{
	int i_ucs2_cur_len;
	int i_str_top_y;
	int i_padding_x;
	int i_padding_y;
	int i_padding_width;
	int i_padding_height;
	VMUINT8		*p_layer_buf = NULL;

	if ((NULL == p_ucs2_string) || (i_layer_handle < 0)){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (NULL == p_layer_buf){
		return;
	}

	i_padding_x = i_screen_x+p_widget->uc_padding_left;
	i_padding_y = i_screen_y+p_widget->uc_padding_top;
	i_padding_width = p_widget->s_width-p_widget->uc_padding_left-p_widget->uc_padding_right;
	i_padding_height = p_widget->s_height-p_widget->uc_padding_bottom-p_widget->uc_padding_top;
	i_str_top_y = i_padding_y + (i_padding_height - vm_graphic_get_string_height(p_ucs2_string))/2;

	//i_ucs2_cur_len >= i_gb2312_string_len/2 && i_ucs2_cur_len <= i_gb2312_string_len
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;
	vm_graphic_set_font(p_widget->c_font);
	//	print_string_state(p_ucs2_string); 
	vm_graphic_set_clip(i_padding_x, i_padding_y, 
		i_padding_x+	i_padding_width, i_padding_y+i_padding_height);
	vm_graphic_textout(p_layer_buf, i_padding_y, i_str_top_y,
		p_ucs2_string, i_ucs2_cur_len, p_widget->i_color);
	vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

/*
* name : draw_ucs2_text_by_widget_more_line
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is more than one line
*/
void draw_ucs2_text_by_widget_more_line(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y, int line_space)
{
	VMWCHAR *p_ucs2_line_string;
	VMWCHAR *p_ucs2_remain_string;

	int line_num  = 0;
	int i_padding_x;
	int i_padding_y;
	int i_padding_width;
	int i_padding_height;
	int i_current_y;

	//	int i_ucs2_cur_len;
	VMUINT8		*p_layer_buf = NULL;

	if ((NULL == p_ucs2_string) || (i_layer_handle < 0)){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (NULL == p_layer_buf){
		return;
	}

	i_padding_x = i_screen_x+p_widget->uc_padding_left;
	i_padding_y = i_screen_y+p_widget->uc_padding_top;
	i_padding_width = p_widget->s_width-p_widget->uc_padding_left-p_widget->uc_padding_right;
	i_padding_height = p_widget->s_height-p_widget->uc_padding_bottom-p_widget->uc_padding_top;

	p_ucs2_line_string = p_ucs2_string;
	p_ucs2_remain_string = p_ucs2_string;
	line_num = 0; 
	vm_graphic_set_clip(i_padding_x, i_padding_y, 
		i_padding_x+	i_padding_width, i_padding_y+i_padding_height);
	while(1)
	{
		if(vm_graphic_get_string_width(p_ucs2_remain_string)-vm_graphic_get_string_width(p_ucs2_line_string) > i_padding_width)
		{
			i_current_y = (line_num==0)?i_padding_y:
				i_padding_y+line_num*(vm_graphic_get_string_height(p_ucs2_string)+line_space);

		p_ucs2_line_string--;
		vm_graphic_textout(p_layer_buf, i_padding_x, i_current_y, 
			p_ucs2_remain_string, p_ucs2_line_string-p_ucs2_remain_string, p_widget->i_color);
		p_ucs2_remain_string = p_ucs2_line_string;
		line_num++;
		}
		if(*(p_ucs2_line_string) =='\n')
		{
			i_current_y = (line_num==0)?i_padding_y:
				i_padding_y+line_num*(vm_graphic_get_string_height(p_ucs2_string)+line_space);

		vm_graphic_textout(p_layer_buf, i_padding_x, i_current_y, 
			p_ucs2_remain_string, p_ucs2_line_string-p_ucs2_remain_string, p_widget->i_color);
		p_ucs2_remain_string = p_ucs2_line_string+1;
		line_num++;
		}
		if(*(p_ucs2_line_string) =='\0')
		{
			i_current_y = (line_num==0)?i_padding_y:
				i_padding_y+line_num*(vm_graphic_get_string_height(p_ucs2_string)+line_space);

		vm_graphic_textout(p_layer_buf, 	i_padding_x, i_current_y, 
			p_ucs2_remain_string, p_ucs2_line_string-p_ucs2_remain_string, p_widget->i_color);
		break;
		}
		p_ucs2_line_string++;
	}
	vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

/*
* name : draw_ucs2_text_by_widget_center
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
 */
void draw_ucs2_text_by_widget_center(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y)
{
	int i_ucs2_cur_len;
	int i_str_top_x;
	int i_str_top_y;
	int i_padding_x;
	int i_padding_y;
	int i_padding_width;
	int i_padding_height;
	VMUINT8		*p_layer_buf = NULL;

	if ((NULL == p_ucs2_string) || (i_layer_handle < 0)){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (NULL == p_layer_buf){
		return;
	}
	i_padding_x = i_screen_x+p_widget->uc_padding_left;
	i_padding_y = i_screen_y+p_widget->uc_padding_top;
	i_padding_width = p_widget->s_width-p_widget->uc_padding_left-p_widget->uc_padding_right;
	i_padding_height = p_widget->s_height-p_widget->uc_padding_bottom-p_widget->uc_padding_top;

	i_str_top_x = i_padding_x + (i_padding_width - vm_graphic_get_string_width(p_ucs2_string))/2;
	i_str_top_y = i_padding_y + (i_padding_height - vm_graphic_get_string_height(p_ucs2_string))/2;

	//i_ucs2_cur_len >= i_gb2312_string_len/2 && i_ucs2_cur_len <= i_gb2312_string_len
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;
	vm_graphic_set_font(p_widget->c_font);
//	print_string_state(p_ucs2_string); 
	vm_graphic_set_clip(i_padding_x, i_padding_y, 
		i_padding_x+	i_padding_width, i_padding_y+i_padding_height);
	vm_graphic_textout(p_layer_buf, i_str_top_x, i_str_top_y,
		p_ucs2_string, i_ucs2_cur_len, p_widget->i_color);
	vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

void print_string_state(VMWCHAR* p_ucs2_string)
{
	int i;
	printf("bit/pixel:%d\n",vm_graphic_get_bits_per_pixel());
	printf("c_height:%d, c_width:%d\n",
		vm_graphic_get_character_height(), 
		vm_graphic_get_character_width(p_ucs2_string[0]));
	printf(" s_height:%d, s_width:%d, s_basline:%d\n",
		vm_graphic_get_string_height(p_ucs2_string),
		vm_graphic_get_string_width(p_ucs2_string),
		vm_graphic_get_string_baseline(p_ucs2_string));
	for( i = 0;p_ucs2_string[i] != '\0';i++)
		printf("%x, ",p_ucs2_string[i]);
	printf("\n");
}


