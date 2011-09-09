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

void set_widget_rect(HWidget* const p_widget, const HRect rect)
{
	p_widget->s_top_x = rect.s_x;
	p_widget->s_top_y = rect.s_y;
	p_widget->s_width = rect.s_width;
	p_widget->s_height = rect.s_height;
}

P_Button Button(A_Rect a_rect, CString cstr_imgname, CString cstr_title, EVENT_CALLBACK_FN callback_fn) {
	P_Button p_button = hpushbutton_new( cstr_title );
	
	p_button->base.p_widget_ops->set_position((HWidget*)p_button, a_rect.s_x, a_rect.s_y );
	p_button->base.p_widget_ops->set_width((HWidget*)p_button, a_rect.s_width );
	p_button->base.p_widget_ops->set_height((HWidget*)p_button, a_rect.s_height );
	
	p_button->ops.set_image_from_resource( cstr_imgname ) ;

	p_button->base.action_performed = callback_fn;

	return p_button;
}

P_Image  Image(A_Rect a_rect, CString cstr_path) {
	P_Image p_image = himage_new();
	p_image->p_oper->set_position( p_image, a_rect.s_x, a_rect.s_y );
	p_image->p_oper->set_size( p_image, a_rect.s_width, a_rect.s_height );
	p_image->p_oper->load_image( p_image, cstr_path );
	return p_image;
}