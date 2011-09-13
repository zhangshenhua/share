/*---------------------------------
* FILE Name: tools.c
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: zhang.shenhua
*
* Modified by zsh on $(date)
*---------------------------------------------------------------------*/
#include "defines.h"
#include "tools.h"
#include "himage.h"



A_Rect Rect(const int left, const int top, const int width, const int height)
{
	A_Rect rect;
	rect.s_x = left;
	rect.s_y = top ;
	rect.s_width = width;
	rect.s_height = height;
	return rect;
}

HRect make_shift_rect (const HRect source_rect, const int dx, const int dy)
{
	HRect dest_rect;
	dest_rect.s_x = source_rect.s_x + dx ;
	dest_rect.s_y = source_rect.s_y + dy ;
	dest_rect.s_width  = source_rect.s_width;
	dest_rect.s_height = source_rect.s_height;
	return dest_rect;
}

HWidget* set_widget_rect(HWidget* p_widget, const HRect* p_rect)
{
	p_widget->p_widget_ops->set_position(p_widget, p_rect->s_x, p_rect->s_y)  ;
	p_widget->p_widget_ops->set_width(p_widget,	p_rect->s_width );
	p_widget->p_widget_ops->set_height(p_widget, p_rect->s_height );
	return p_widget;
}

P_Button Button(P_Rect p_rect, CString cstr_imgname, CString cstr_title, EVENT_CALLBACK_FN callback_fn) {
	P_Button p_button = hpushbutton_new( cstr_title );
	
	set_widget_rect( (HWidget*)p_button, p_rect);
	
	p_button->ops->set_image_from_resource(p_button, cstr_imgname ) ;

	p_button->base.action_performed = callback_fn;

	return p_button;
}


//static void empty_callback(HWidget *p_source, HEvent *p_evt, void *p_param){/*empty procedure*/}
/************************************************************************/
/* 从文件构造出图片                                                                     */
/************************************************************************/
P_Image  Image(P_Rect p_rect, CString cstr_path) {
	return Button( p_rect, cstr_path, "", NULL );
}

/************************************************************************/
/* 从缓冲构造出图片                                                                     */
/************************************************************************/
P_Image  ImageB(VMUINT8 *pi_buf, VMINT i_size) {
	P_Button p_pushbutton  = hpushbutton_new("");
	p_pushbutton->ops->set_image_from_buffer( p_pushbutton, pi_buf, i_size);
	
	return p_pushbutton;
}


P_Button Button2(P_Rect p_rect, P_Image p_image, CString cstr_title, EVENT_CALLBACK_FN callback_fn) {
	P_Button p_button ;

	if( p_image ) {
		p_button = p_image;
		p_button->ops->set_text( p_button, cstr_title );
	}
	else {
		p_button = hpushbutton_new( cstr_title );

	}

	set_widget_rect( (HWidget*)p_button, p_rect);
	p_button->base.action_performed = callback_fn;
	return p_button;
}

