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

#include "hplane.h"
#include "hwidget.h"
#include "hpushbutton.h"
#include "htextinput.h"
#include "htextarea.h"
#include "hlabel.h"
#include "hcheckbox.h"
#include "himage.h"

#define MAX_RESOURCE_SIZE 1024*1024


typedef struct _HResourceImageProperty HResourceImageProperty;
typedef struct _HResource HResource;

struct _HResourceImageProperty {
	int		i_image_byte_size;
	char		*p_image_strparth;
	HImage *p_image;
};

struct _HResource {
/********************** public member ***********************************/

	HImage* (*load_local_image)(HResource *p_resource, char *p_strParth);

//	HImage* (*load_remote_image)(HResource *p_resource, char *p_strParth);

//	HImage* (*load_face_image)(HResource *p_resource, char *p_code);

	void (*free_image_by_hresource)(HResource *p_resource, HImage* p_image); 

/********************* end public member ********************************/

/********************** private member *********************************/
	int i_resource_byte_size;

	HImage *p_max_image;

	hlist_node_t* p_property_list_head;

	void (*append_property_and_strparth)(HResource *p_resource, HImage *p_image, char *p_strparth);

	int (*remove_property_and_strparth)(HResource *p_resource, HImage *p_image);

	void (*release_maximun_image)(HResource *p_resource);

	int (*get_image_size)(HImage* p_image);

/********************* end private member ********************************/
};

HResource *hresource_new();

void hresource_delete(HResource *p_resource);

/*
* name : look_draw_plane
* description : draw plane
* @para(in) p_plane : a plane
*/
void look_draw_plane(HPlane* p_plane);

/*
* name : look_draw_trans_color
* description : draw gray screen into a layer by hrect struction
* @para(in) i_layer_handle : a layer handle
* @para(in) p_rect : the transparent hrect struction
* @para(in) trans_color : the transparent color
* @para(in) i_transparency : the transparency of color
* @para(in) i_transparency : value range [0,255], 0 means completely transparent 
*/
void look_draw_trans_color(VMINT i_layer_handle, HRect p_rect, VMUINT16 trans_color, VMINT i_transparency);

/*
* name : look_draw_highlight_in_trans
* description : draw hight light rect in a trans rect
* @para(in) i_layer_handle : a layer handle
* @para(in) p_hightlight_rect : the hight light hrect struction
* @para(in) p_trans_rect : the transparent hrect struction
* @para(in) trans_color : the transparent color
* @para(in) i_transparency : the transparency of color
* @para(in) i_transparency : value range [0,255], 0 means completely transparent 
*/
void look_draw_highlight_in_trans(VMINT i_layer_handle, HRect p_hightlight_rect, HRect p_trans_rect, VMUINT16 trans_color, VMINT i_transparency);

/*
* name : look_draw_highlight_by_change_saturation
* description : draw hight light rect by change the saturation of each pixel color
* @para(in) i_layer_handle : a layer handle
* @para(in) p_rect : the hight light hrect struction
* @para(in) i_saturation_change : the change value of staturation
* @para(in) i_saturation_change : the staturation value range [0,255]
*/
void look_draw_highlight_by_change_saturation(VMINT i_layer_handle, HRect p_rect, int i_saturation_change);

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
* @para(in) p_label : a label structure
* @para(in) i_layer_handle : a plane handle by creating layer
* @para(in) i_screen_x : the screen x coordinate
* @para(in) i_screen_y : the screen y coordinate
*/
 void look_paint_label(HLabel *p_label, VMINT i_layer_handle, int i_screen_x, int i_screen_y);

 /*
 * name : look_paint_label
 * description : draw check box into a layer
 * @para(in) p_checkbox : a check box structure
 * @para(in) i_layer_handle : a plane handle by creating layer
 * @para(in) i_screen_x : the screen x coordinate
 * @para(in) i_screen_y : the screen y coordinate
 */
 void look_paint_checkbox(HCheckBox* p_checkbox, int i_handle, short s_screen_x, short s_screen_y);

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