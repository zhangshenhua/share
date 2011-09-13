/********************************************************
*file: hwindow.h
*author: yan.gao
*date: 2011.08.22
*description: Globally unique object in our app.
********************************************************/

#ifndef  _HWINDOW_H_
#define  _HWINDOW_H_

#include "../WidgetLayer/hplane.h"
#include "../PageLayer/page.h"
#include "vmsys.h"

typedef struct _HWindow HWindow;

/*global window instance*/
extern HWindow *window;

void hwindow_init();

void hwindow_exit();

struct _HWindow {

	/******************* private member **********************/

	/*By default, in a window has three planes. We define these names: Default plane, Popup menu plane, and Glass plane*/
	struct _HPlane *p_planes[3];

	/* a instance of PageSwitchController*/
	PageSwitchController *p_page_switch_contrller;

	void (*sys_event_handle)(VMINT message, VMINT param);

	void (*key_event_handle)(VMINT evt, VMINT keycode);

	 void (*pen_event_handle)(VMINT evt, VMINT x, VMINT y);
	/******************* public member **********************/


	/************************************************************************
	* Function: get_default_plane
	* Description: get the default plane. It real is planes[0]
	************************************************************************/
	struct _HPlane * (*get_default_plane)();

	/************************************************************************
	* Function: get_popupmenu_plane
	* Description: get the popup menu plane. It real is planes[1]
	************************************************************************/
	struct _HPlane * (*get_popupmenu_plane)();

	/************************************************************************
	* Function: get_glass_plane
	* Description: get the glass plane. It real is planes[2]
	************************************************************************/
	struct _HPlane * (*get_glass_plane)();

	/************************************************************************
	* Function : get_page_switch_contrller
	* Description: get the PageSwitchController instance
	************************************************************************/
	PageSwitchController * (*get_page_switch_contrller)();

	/************************************************************************
	* validate the window's all planes
	************************************************************************/
	void (*validate)();

	/*repaint all planes*/
	void (*repaint)();

	/************************************************************************
	* the screen width 
	************************************************************************/
	short s_screen_width;

	/************************************************************************
	* the screen height 
	************************************************************************/
	short s_screen_height;

	/************************************************************************
	* the device is support touch screen, yes:1 no:0
	************************************************************************/
	short s_is_support_touch_screen;

	/************************************************************************
	/* current clip
	************************************************************************/
	HRect cur_clip_rect;
};





#endif