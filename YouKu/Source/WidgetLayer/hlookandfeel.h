/**********************************************************************************
*file: hlookandfeel.h
*author: yi.jiang
*date: 2011.08.29
*description: draw specific widget into specify layer by getting widget structure.
***********************************************************************************/


#ifndef _HDRAW_H_
#define _HDRAW_H_

#include <string.h>
#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"

#include "hwidget.h"
#include "hpushbutton.h"
#include "hplane.h"
#include "htextinput.h"
#include "htextarea.h"
#include "hlabel.h"


/*
* name : look_draw_plane
* description : draw plane
* @para(in) p_plane : a plane
*/
extern void look_draw_plane(HPlane *p_plane);

/*
 * name : look_panit_pushbutton
 * description : draw pushbutton into a layer
 * @para(in) p_bushbutton : a hPushButton structure
 * @para(in) i_layer_handle : a plane handle by creating layer
 * @para(in) i_screen_x : the screen x coordinate
 * @para(in) i_screen_y : the screen y coordinate
 */
extern void look_paint_pushbutton(HPushButton *p_bushbutton, VMINT i_layer_handle, int i_screen_x, int i_screen_y);

/*
* name : look_paint_textinput
* description : draw text input into a layer
* @para(in) p_textinput : a text input structure
* @para(in) i_layer_handle : a plane handle by creating layer
* @para(in) i_screen_x : the screen x coordinate
* @para(in) i_screen_y : the screen y coordinate
*/
extern void look_paint_textinput(HTextInput *p_textinput, VMINT i_layer_handle, int i_screen_x, int i_screen_y);

/*
* name : look_paint_textarea
* description : draw text area into a layer
* @para(in) p_textinput : a text input structure
* @para(in) i_layer_handle : a plane handle by creating layer
* @para(in) i_screen_x : the screen x coordinate
* @para(in) i_screen_y : the screen y coordinate
*/
extern void look_paint_textarea(HTextArea *p_textarea, VMINT i_layer_handle, int i_screen_x, int i_screen_y);


/*
* name : look_paint_label
* description : draw label into a layer
* @para(in) p_label : a text input structure
* @para(in) i_layer_handle : a plane handle by creating layer
* @para(in) i_screen_x : the screen x coordinate
* @para(in) i_screen_y : the screen y coordinate
*/
extern void look_paint_label(HLabel *p_label, VMINT i_layer_handle, int i_screen_x, int i_screen_y);

/*
* name : draw_rect_by_widget
* description : draw rect into a layer by widget struction
* description : the filled color is background color
* @para(in) i_layer_handle : a layer handle
* @para(in) p_widget : a widget structure
* @para(in) i_screen_x : the x coordinate in the screen
* @para(in) i_screen_y : the y coordinate in the screen
*/
void draw_rect_by_widget(VMINT i_layer_handle,HWidget* p_widget, int i_screen_x, int i_screen_y);

/*
* name : draw_rect_by_widget_trans
* description : draw the rect frame into a layer by widget struction
* @para(in) i_layer_handle : a layer handle
* @para(in) p_widget : a widget structure
* @para(in) i_screen_x : the x coordinate in the screen
* @para(in) i_screen_y : the y coordinate in the screen
*/
void draw_rect_by_widget_trans(VMINT i_layer_handle,HWidget* p_widget, int i_screen_x, int i_screen_y, VMUINT16 f_color);

/*
* name : draw_rect_by_hrect
* description : draw frect into a layer
* @para(in) p_rect : a rect structure
* @para(in) layer_handle : a layer handle
* @para(in) f_color: color of the frame
* @para(in) bg_color : color of filled
*/
void draw_rect_by_hrect(VMINT layer_handle, HRect p_rect, VMUINT16 f_color, VMUINT16 bg_color);

/*
* name : draw_rect_by_hrect_trans
* description : draw the rect frame into a layer
* @para(in) p_rect : a rect structure
* @para(in) i_layer_handle : a layer handle
* @para(in) f_color : color of the frame
*/
void draw_rect_by_hrect_trans(VMINT i_layer_handle, HRect p_rect, VMUINT16 f_color);

/*
* name : draw_gb2312_text_by_hpoint
* description : draw rect into a layer
* @para(in) p_char_str : the GB2312 string
* @para(in) layer_handle : a layer handle
* @para(in) p_point : the point include x,y
* @para(in) ui_color :the color of string
*/
void draw_gb2312_text_by_hpoint(VMINT layer_handle, char *p_char_str, HPoint p_point, VMUINT16 ui_color);

/*
* name : draw_gb2312_text_by_widget
* description : draw GB2312 string into a layer,
* description : color of the filled is foreground color
* @para(in) p_widget : a widget structure
* @para(in) i_layer_handle : a layer handle
* @para(in) p_char_str : the GB2312 string
*/
void draw_gb2312_text_by_widget(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y);

/*
* name : draw_gb2312_text_by_hpoint_online
* description : draw GB2312 string into a layer
* description : color of the filled is foreground color
* description : the string is one line
* @para(in) p_widget : a widget structure
* @para(in) layer_handle : a layer handle
* @para(in) p_char_str : the GB2312 string
*/
void draw_gb2312_text_by_widget_oneline(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y);

/*
* name : draw_gb2312_text_by_widget_center
* description : draw GB2312 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
* @para(in) p_widget : a widget structure
* @para(in) layer_handle : a layer handle
* @para(in) p_char_str : the GB2312 string
*/
void draw_gb2312_text_by_widget_center(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y);

/*
* name : draw_ucs2_text_by_hpoint
* description : draw rect into a layer
* @para(in) p_ucs2_string : the UCS2 string
* @para(in) layer_handle : a layer handle
* @para(in) p_point : the point include x,y
* @para(in) ui_color :the color of string
*/
void draw_ucs2_text_by_hpoint(VMINT i_layer_handle, VMWCHAR *p_ucs2_string, HPoint p_point, VMUINT16 ui_color);


/*
* name : draw_ucs2_text_by_widget
* description : draw UCS2 string into a layer,
* description : color of the filled is foreground color
* @para(in) p_widget : a widget structure
* @para(in) i_layer_handle : a layer handle
* @para(in) p_ucs2_string : the UCS2 string
*/
void draw_ucs2_text_by_widget(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y);

/*
* name : draw_ucs2_text_by_widget_oneline
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is one line
* @para(in) p_widget : a widget structure
* @para(in) i_layer_handle : a layer handle
* @para(in) p_ucs2_string : the UCS2 string
*/
void draw_ucs2_text_by_widget_oneline(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y);


/*
* name : draw_ucs2_text_by_widget_more_line
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is more than one line
* @para(in) p_widget : a widget structure
* @para(in) i_layer_handle : a layer handle
* @para(in) p_ucs2_string : the UCS2 string
* @para(in) line_space : the line space
*/
void draw_ucs2_text_by_widget_more_line(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y, int line_space);

/*
* name : draw_ucs2_text_center_by_widget_center
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
* @para(in) p_widget : a widget structure
* @para(in) i_layer_handle : a layer handle
* @para(in) p_ucs2_string : the UCS2 string
*/
extern void draw_ucs2_text_by_widget_center(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y);

/*
 * name : print_string_state
 * description : print string state
 * @para(in) p_ucs2_string : the UCS2 string
 */
extern void print_string_state(VMWCHAR* p_ucs2_string);

#endif