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
#include "hwidget.h"

HRect make_rect(const int left, const int top, const int width, const int height)
{
	HRect rect;
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