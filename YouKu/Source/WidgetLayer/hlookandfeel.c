/**********************************************************************************
*file: hlookandfeel.c
*author: yi.jiang
*date: 2011.08.29
*description: draw specific widget into specify layer by getting widget structure.
***********************************************************************************/
#include "hlookandfeel.h"

#define WIDGET_HAS_BOX

/*
* name : look_draw_plane
* description : draw plane
* @para(in) p_plane : a plane
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
 * @para(in) p_bushbutton : a hPushButton structure
 * @para(in) i_layer : a plane handle by creating layer
 * @para(in) screen_x : the screen x coordinate
 * @para(in) screen_y : the screen y coordinate
 */
void look_paint_pushbutton(HPushButton *p_pushbutton, VMINT i_layer, int screen_x, int screen_y)
{
	HWidget* p_widget;
	VMUINT8* p_layer_buf;

	p_widget = (HWidget *)p_pushbutton;
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer);

//	draw_rect_by_bgcolor(p_widget, p_layer_buf,screen_x,screen_y);
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		vm_graphic_fill_rect(p_layer_buf, screen_x, screen_y, 
			p_widget->s_width, p_widget->s_height, p_widget->i_bgcolor,p_widget->i_bgcolor);
	if(p_widget->p_widget_ops->has_focus(p_widget))
		vm_graphic_rect(p_layer_buf, screen_x, screen_y,
			p_widget->s_width, p_widget->s_height, VM_COLOR_BLACK);
	else
		vm_graphic_rect(p_layer_buf, screen_x, screen_y,
		p_widget->s_width, p_widget->s_height, p_widget->i_bgcolor);

	if(p_pushbutton->text != NULL)
		draw_gb2312_text(p_widget, p_layer_buf, p_pushbutton->text, screen_x, screen_y);
}

/*
* name : look_paint_textinput
* description : draw text input into a layer
* @para(in) p_textinput : a text input structure
* @para(in) i_layer : a plane handle by creating layer
* @para(in) screen_x : the screen x coordinate
* @para(in) screen_y : the screen y coordinate
*/
void look_paint_textinput(HTextInput *p_textinput, VMINT i_layer, int screen_x, int screen_y)
{
	HWidget* p_widget;
	VMUINT8* p_layer_buf;

	VMINT i_ucs2_max_len;
	VMWCHAR *p_ucs2_string;
	int i;

	p_widget = (HWidget *)p_textinput;
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer);	
	/* draw rect */
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		draw_rect_by_bgcolor(p_widget, p_layer_buf, screen_x, screen_y);

	if(p_widget->p_widget_ops->has_focus(p_widget))
		vm_graphic_rect(p_layer_buf, screen_x, screen_y,
			p_widget->s_width, p_widget->s_height, VM_COLOR_RED);
	else
		vm_graphic_rect(p_layer_buf, screen_x, screen_y,
			p_widget->s_width, p_widget->s_height, VM_COLOR_BLACK);

	if(p_textinput->pc_text ==NULL)
		return;
	i_ucs2_max_len = strlen(p_textinput->pc_text);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_textinput->pc_text);
	if (p_textinput->c_is_password){
		for( i = 0;p_ucs2_string[i] != '\0';i++)
			p_ucs2_string[i] = '*';
	}

	draw_ucs2_text_ymid(p_widget, p_layer_buf, p_ucs2_string, screen_x, screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : look_paint_label
* description : draw label into a layer
* @para(in) p_label : a text input structure
* @para(in) i_layer : a plane handle by creating layer
* @para(in) screen_x : the screen x coordinate
* @para(in) screen_y : the screen y coordinate
*/
void look_paint_label(HLabel *p_label, VMINT i_layer, int screen_x, int screen_y)
{
	HWidget* p_widget;
	VMUINT8* p_layer_buf;

	VMINT i_ucs2_max_len;
	VMWCHAR *p_ucs2_string;

	p_widget = (HWidget *)p_label;
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer);	

	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		draw_rect_by_bgcolor(p_widget, p_layer_buf, screen_x, screen_y);

	if(p_widget->p_widget_ops->has_focus(p_widget))
		vm_graphic_rect(p_layer_buf, screen_x, screen_y,
		p_widget->s_width, p_widget->s_height, VM_COLOR_BLACK);
	else
		vm_graphic_rect(p_layer_buf, screen_x, screen_y,
		p_widget->s_width, p_widget->s_height, p_widget->i_bgcolor);

	if (p_label->text == NULL)
		return;
	i_ucs2_max_len = strlen(p_label->text);
	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_label->text);

	draw_ucs2_text(p_widget, p_layer_buf, p_ucs2_string, screen_x, screen_y);
 
	vm_free(p_ucs2_string);
}

/*
* name : draw_rect_by_bgcolor
* description : draw rect into a layer,
* description : color of the filled is background color
* @para(in) p_widget : a widget structure
* @para(in) p_layer_buf : a layer's buffer
 */
void draw_rect_by_bgcolor(HWidget* p_widget, VMUINT8* p_layer_buf, int screen_x, int screen_y)
{
	printf("will fill rect: x:%d, y:%d, w:%d, h:%d, col:%x\n", 
		screen_x, screen_y, p_widget->s_width, p_widget->s_height, p_widget->i_bgcolor);

	vm_graphic_fill_rect(p_layer_buf, screen_x, screen_y,
		p_widget->s_width, p_widget->s_height, 
		p_widget->i_bgcolor, p_widget->i_bgcolor );
}

/*
 * name : draw_gb2312_text
 * description : draw GB2312 string into a layer,
 * description : color of the filled is foreground color
 * @para(in) p_widget : a widget structure
 * @para(in) p_layer_buf : a layer's buffer
 * @para(in) p_char : the GB2312 string
 */
void draw_gb2312_text(HWidget* p_widget, VMUINT8* p_layer_buf, char *p_char, int screen_x, int screen_y)
{
	VMINT i_ucs2_max_len;
	VMWCHAR *p_ucs2_string;

	i_ucs2_max_len = strlen(p_char);
//	printf("char:%s, i_ucs2_max_len:%d\n",p_char, i_ucs2_max_len);

	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	if (p_ucs2_string != NULL)
		//the second para is the size of bytes,include '\0'
		vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_char);

	draw_ucs2_text(p_widget, p_layer_buf, p_ucs2_string,screen_x,screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : draw_gb2312_text_mid
* description : draw GB2312 string into a layer,
* description : color of the filled is foreground color
* description : the string is in the center of the widget
* @para(in) p_widget : a widget structure
* @para(in) p_layer_buf : a layer's buffer
* @para(in) p_char : the GB2312 string
*/
void draw_gb2312_text_mid(HWidget* p_widget, VMUINT8* p_layer_buf, char *p_char, int screen_x, int screen_y)
{
	VMINT i_ucs2_max_len;
	VMWCHAR *p_ucs2_string;

	i_ucs2_max_len = strlen(p_char);
	//	printf("char:%s, i_ucs2_max_len:%d\n",p_char, i_ucs2_max_len);

	p_ucs2_string = vm_malloc( (i_ucs2_max_len+1) * sizeof(VMWCHAR) );
	if (p_ucs2_string != NULL)
		//the second para is the size of bytes,include '\0'
		vm_gb2312_to_ucs2(p_ucs2_string, (i_ucs2_max_len+1)*2, p_char);

	draw_ucs2_text_mid(p_widget, p_layer_buf, p_ucs2_string,screen_x,screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : draw_ucs2_text
* description : draw UCS2 string into a layer,
* description : color of the filled is foreground color
* @para(in) p_widget : a widget structure
* @para(in) p_layer_buf : a layer's buffer
* @para(in) p_ucs2_string : the UCS2 string
 */
void draw_ucs2_text(HWidget* p_widget, VMUINT8* p_layer_buf, VMWCHAR* p_ucs2_string, int screen_x, int screen_y)
{
	int i_ucs2_cur_len;
	//i_ucs2_cur_len >= i_gb2312_string_len/2 && i_ucs2_cur_len <= i_gb2312_string_len
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;
	vm_graphic_set_font(p_widget->c_font);
//	print_string_state(p_ucs2_string); 
	vm_graphic_set_clip(screen_x+p_widget->uc_padding_left, 
		screen_y+p_widget->uc_padding_top, 
		screen_x+p_widget->s_width-p_widget->uc_padding_right, 
		screen_y+p_widget->s_height-p_widget->uc_padding_bottom);
	vm_graphic_textout(p_layer_buf, 
		screen_x+p_widget->uc_padding_left, 
		screen_y+p_widget->uc_padding_top,
		p_ucs2_string, i_ucs2_cur_len, p_widget->i_color);
	vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

/*
* name : draw_ucs2_text_ymid
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
* @para(in) p_widget : a widget structure
* @para(in) p_layer_buf : a layer's buffer
* @para(in) p_ucs2_string : the UCS2 string
*/
void draw_ucs2_text_ymid(HWidget* p_widget, VMUINT8* p_layer_buf, VMWCHAR* p_ucs2_string, int screen_x, int screen_y)
{
	int i_ucs2_cur_len;
	int i_str_top_y;
	i_str_top_y = screen_y + p_widget->uc_padding_top 
		+ (p_widget->s_height - p_widget->uc_padding_top - p_widget->uc_padding_bottom 
		- vm_graphic_get_string_height(p_ucs2_string))/2;

	//i_ucs2_cur_len >= i_gb2312_string_len/2 && i_ucs2_cur_len <= i_gb2312_string_len
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;
	vm_graphic_set_font(p_widget->c_font);
	//	print_string_state(p_ucs2_string); 
	vm_graphic_set_clip(screen_x+p_widget->uc_padding_left, 
		screen_y+p_widget->uc_padding_top, 
		screen_x+p_widget->s_width-p_widget->uc_padding_right, 
		screen_y+p_widget->s_height-p_widget->uc_padding_bottom);
	vm_graphic_textout(p_layer_buf, 
		screen_x+p_widget->uc_padding_left, i_str_top_y,
		p_ucs2_string, i_ucs2_cur_len, p_widget->i_color);
	vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
}

/*
* name : draw_ucs2_text_mid
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
* @para(in) p_widget : a widget structure
* @para(in) p_layer_buf : a layer's buffer
* @para(in) p_ucs2_string : the UCS2 string
 */
void draw_ucs2_text_mid(HWidget* p_widget, VMUINT8* p_layer_buf, VMWCHAR* p_ucs2_string, int screen_x, int screen_y)
{
	int i_ucs2_cur_len;
	int i_str_top_x;
	int i_str_top_y;

	i_str_top_x = screen_x + p_widget->uc_padding_left 
		+ (p_widget->s_width - p_widget->uc_padding_left - p_widget->uc_padding_right
		- vm_graphic_get_string_width(p_ucs2_string))/2;
	i_str_top_y = screen_y + p_widget->uc_padding_top 
		+ (p_widget->s_height - p_widget->uc_padding_top - p_widget->uc_padding_bottom 
		- vm_graphic_get_string_height(p_ucs2_string))/2;

	//i_ucs2_cur_len >= i_gb2312_string_len/2 && i_ucs2_cur_len <= i_gb2312_string_len
	for( i_ucs2_cur_len = 0;p_ucs2_string[i_ucs2_cur_len] != '\0';i_ucs2_cur_len++)
		;
	vm_graphic_set_font(p_widget->c_font);
//	print_string_state(p_ucs2_string); 
	vm_graphic_set_clip(screen_x+p_widget->uc_padding_left, 
		screen_y+p_widget->uc_padding_top, 
		screen_x+p_widget->s_width-p_widget->uc_padding_right, 
		screen_y+p_widget->s_height-p_widget->uc_padding_bottom);
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


