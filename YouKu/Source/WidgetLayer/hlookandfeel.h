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
 * @para(in) i_layer : a plane handle by creating layer
 * @para(in) screen_x : the screen x coordinate
 * @para(in) screen_y : the screen y coordinate
 */
extern void look_paint_pushbutton(HPushButton *p_bushbutton, VMINT i_layer, int screen_x, int screen_y);

/*
* name : look_paint_textinput
* description : draw text input into a layer
* @para(in) p_textinput : a text input structure
* @para(in) i_layer : a plane handle by creating layer
* @para(in) screen_x : the screen x coordinate
* @para(in) screen_y : the screen y coordinate
*/
void look_paint_textinput(HTextInput *p_textinput, VMINT i_layer, int screen_x, int screen_y);

/*
* name : look_paint_label
* description : draw label into a layer
* @para(in) p_label : a text input structure
* @para(in) i_layer : a plane handle by creating layer
* @para(in) screen_x : the screen x coordinate
* @para(in) screen_y : the screen y coordinate
*/
void look_paint_label(HLabel *p_label, VMINT i_layer, int screen_x, int screen_y);

/*
 * name : draw_rect_by_bgcolor
 * description : draw rect into a layer,
 * description : color of the filled is background color
 * @para(in) p_widget : a widget structure
 * @para(in) p_layer_buf : a layer's buffer
 */
extern void draw_rect_by_bgcolor(HWidget* p_widget, VMUINT8* p_layer_buf, int screen_x, int screen_y);

/*
 * name : draw_gb2312_text
 * description : draw GB2312 string into a layer,
 * description : color of the filled is foreground color
 * @para(in) p_widget : a widget structure
 * @para(in) p_layer_buf : a layer's buffer
 * @para(in) p_char : the GB2312 string
 */
extern void draw_gb2312_text(HWidget* p_widget, VMUINT8* p_layer_buf, char *p_char, int screen_x, int screen_y);

/*
* name : draw_gb2312_text_mid
* description : draw GB2312 string into a layer,
* description : color of the filled is foreground color
* description : the string is in the center of the widget
* @para(in) p_widget : a widget structure
* @para(in) p_layer_buf : a layer's buffer
* @para(in) p_char : the GB2312 string
*/
void draw_gb2312_text_mid(HWidget* p_widget, VMUINT8* p_layer_buf, char *p_char, int screen_x, int screen_y);

/*
 * name : draw_ucs2_text
 * description : draw UCS2 string into a layer,
 * description : color of the filled is foreground color
 * @para(in) p_widget : a widget structure
 * @para(in) p_layer_buf : a layer's buffer
 * @para(in) p_ucs2_string : the UCS2 string
 */
extern void draw_ucs2_text(HWidget* p_widget, VMUINT8* p_layer_buf, VMWCHAR* p_ucs2_string, int screen_x, int screen_y);

/*
* name : draw_ucs2_text_ymid
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
* @para(in) p_widget : a widget structure
* @para(in) p_layer_buf : a layer's buffer
* @para(in) p_ucs2_string : the UCS2 string
*/
void draw_ucs2_text_ymid(HWidget* p_widget, VMUINT8* p_layer_buf, VMWCHAR* p_ucs2_string, int screen_x, int screen_y);

/*
 * name : draw_ucs2_text_mid
 * description : draw UCS2 string into a layer,
 * description : color of the filled is foreground color
 * description : the string is in the center of the widget
 * @para(in) p_widget : a widget structure
 * @para(in) p_layer_buf : a layer's buffer
 * @para(in) p_ucs2_string : the UCS2 string
 */
extern void draw_ucs2_text_mid(HWidget* p_widget, VMUINT8* p_layer_buf, VMWCHAR* p_ucs2_string, int screen_x, int screen_y);

/*
 * name : print_string_state
 * description : print string state
 * @para(in) p_ucs2_string : the UCS2 string
 */
extern void print_string_state(VMWCHAR* p_ucs2_string);

#endif