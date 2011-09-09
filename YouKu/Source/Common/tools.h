/*---------------------------------
* FILE Name: tools.h
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: zhang.shenhua
*
* Modified by zsh on $(date)
*---------------------------------------------------------------------*/
#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "defines.h"
#include "hwidget.h"
#include "hpushbutton.h"
#include "himage.h"

typedef HRect			A_Rect;	
typedef HPushButton		*P_Button;
typedef HImage			*P_Image;

typedef void (*EVENT_CALLBACK_FN)(HWidget *p_source, HEvent *p_evt, void *p_param);

extern A_Rect Rect(const int left, const int top, const int width, const int height);

extern A_Rect shift_rect (const A_Rect source_rect, const int dx, const int dy);

extern void set_widget_rect(HWidget* const p_widget, const HRect rect);

//extern P_Image	Image(A_Rect a_rect, );

extern P_Button Button(A_Rect a_rect, CString cstr_imgname , CString cstr_title, EVENT_CALLBACK_FN callback_fn);

extern P_Image  Image(A_Rect a_rect, CString cstr_path);

#endif