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

extern HRect make_rect(const int left, const int top, const int width, const int height);

extern HRect make_shift_rect (const HRect source_rect, const int dx, const int dy);

extern void set_widget_rect(HWidget* const p_widget, const HRect rect);

#endif