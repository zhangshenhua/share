/********************************************************
*file: hwindow.h
*author: yan.gao
*date: 2011.08.23
*description: Globally unique object in our app.
********************************************************/

#include "hwindow.h"
#include "../WidgetLayer/hwidget.h"
#include "../WidgetLayer/htextinput.h"
#include "../WidgetLayer/htextarea.h"
#include "../WidgetLayer/hcheckbox.h"
#include "../WidgetLayer/hlookandfeel.h"

#include "string.h"
#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmlog.h"


HWindow *window;

static void hwindow_paint();
static HPlane * hwindow_get_default_plane();
static void hwindow_destroy();

/*user define the enter function*/
extern void app_main();

static void hwindow_validate()
{
	int i_idx;
	HWidget *pw;
	for (i_idx = sizeof(window->p_planes)/sizeof(HPlane *) -1; i_idx >= 0; i_idx --) {
		pw = (HWidget *)window->p_planes[i_idx];
		if (pw)
			pw->p_widget_ops->validate(pw);
	}
}

static void sys_event_handle(VMINT message, VMINT param)
{
	switch (message) {
		case VM_MSG_CREATE:
			// create default plane
			vm_log_debug("msg create");
			hwindow_get_default_plane();
			app_main();
			break;

		case VM_MSG_ACTIVE:
			vm_log_debug("msg active");
			hwindow_paint();
			break;

		case VM_MSG_PAINT:
			vm_log_debug("msg paint");
			hwindow_paint();
			break;

		case VM_MSG_INACTIVE:
			vm_log_debug("msg inactive");
			break;

		case VM_MSG_QUIT:
			vm_log_debug("msg quit");
			hwindow_destroy();
			break;
	}
}

/************************************************************************
* Get topest plane
************************************************************************/
static HPlane * hwindow_get_topest_plane()
{
	int i_idx;
	HWidget *pw;
	for (i_idx = sizeof(window->p_planes)/sizeof(HPlane *) -1; i_idx >= 0; i_idx --) {
		pw = (HWidget *)window->p_planes[i_idx];
		if (pw && pw->p_widget_ops->is_visible(pw))
			return (HPlane *)pw;
	}
	return NULL;
}

static void key_event_handle(VMINT evt, VMINT keycode)
{
	HWidget *p_top_plane = (HWidget *)hwindow_get_topest_plane();
	switch (evt) {
		case VM_KEY_EVENT_DOWN:
			p_top_plane->p_widget_ops->key_press(p_top_plane, keycode);
			break;
		case VM_KEY_EVENT_UP:
			p_top_plane->p_widget_ops->key_release(p_top_plane, keycode);
			break;
		case VM_KEY_EVENT_LONG_PRESS:
			p_top_plane->p_widget_ops->key_long_press(p_top_plane, keycode);
			break;
		case VM_KEY_EVENT_REPEAT:
			p_top_plane->p_widget_ops->key_repeat(p_top_plane, keycode);
			break;
	}
}

static void pen_event_handle(VMINT evt, VMINT x, VMINT y)
{
	HWidget *p_top_plane = (HWidget *)hwindow_get_topest_plane();
	switch (evt) {
		case VM_PEN_EVENT_DOUBLE_CLICK:
			p_top_plane->p_widget_ops->pen_double_click(p_top_plane, x, y);
			break;
		case VM_PEN_EVENT_LONG_TAP :
			p_top_plane->p_widget_ops->pen_long_tap(p_top_plane, x, y);
			break;
//		case VM_PEN_EVENT_REPEAT:

		case VM_PEN_EVENT_TAP :
			p_top_plane->p_widget_ops->pen_press(p_top_plane, x, y);
			break;
		case VM_PEN_EVENT_RELEASE :
			p_top_plane->p_widget_ops->pen_release(p_top_plane, x, y);
			break;
		case VM_PEN_EVENT_MOVE :
			p_top_plane->p_widget_ops->pen_move(p_top_plane, x, y);
			break;
	}
}

static void hwindow_paint()
{
	int i_idx, i_count;
	HWidget *pw;
	HPlane *p_plane;
	int p_handles[sizeof(window->p_planes)/sizeof(HPlane *)];
	for (i_idx = i_count = 0; i_idx < sizeof(window->p_planes)/sizeof(HPlane *); i_idx ++) {
		p_plane = window->p_planes[i_idx];
		pw = (HWidget *)p_plane;
		if (pw && pw->p_widget_ops->is_visible(pw)) {
			p_handles[i_count ++] = p_plane->i_handle;
			pw->p_widget_ops->paint(pw, p_plane->i_handle, 0, 0);
			if (p_plane->i_enable_transparent_bg) {
				//TODO: paint transparent pw->i_handle
				look_draw_plane(p_plane);
			}
		}
	}
	//TODO: flush the three planes
	vm_graphic_flush_layer(p_handles, i_count);
}

static void hwindow_repaint()
{
	hwindow_paint();
}

static HPlane * hwindow_get_default_plane()
{
	HPlane *p_plane;
	if (window->p_planes[0] == NULL) {
		p_plane = hplane_new();
		((HWidget *)p_plane)->p_widget_ops->set_enable_bgcolor((HWidget *)p_plane, 1);
		((HWidget *)p_plane)->p_widget_ops->set_bgcolor((HWidget *)p_plane, VM_COLOR_WHITE);
		p_plane->i_handle = vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),vm_graphic_get_screen_height(),-1);
		p_plane->s_plane_index = 0;
		window->p_planes[0] = p_plane;
	}
	return window->p_planes[0];
}

static HPlane * hwindow_get_popupmenu_plane()
{
	HPlane *p_plane;
	if (window->p_planes[1] == NULL) {
		p_plane = hplane_new();
		p_plane->s_transparent_color = 0xabcd;
		p_plane->i_handle = vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),vm_graphic_get_screen_height(),p_plane->s_transparent_color);
		p_plane->s_plane_index = 1;
		window->p_planes[1] = p_plane;
	}
	return window->p_planes[1];
}

static HPlane * hwindow_get_glass_plane()
{
	HPlane *p_plane;
	if (window->p_planes[2] == NULL) {
		p_plane = hplane_new();
		p_plane->s_plane_index = 2;
		window->p_planes[2] = p_plane;
	} else {
		p_plane = window->p_planes[2];
	}
	//visible the popup menu plane
	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 1);
	return p_plane;
}

static PageSwitchController * hwindow_get_page_switch_contrller()
{
	if (window->p_page_switch_contrller == NULL) {
		window->p_page_switch_contrller = page_switch_controller_new();
	}
	return window->p_page_switch_contrller;
}

static void hwindow_destroy()
{
	int i_idx;
	HPlane *p_plane;
	//delete all layer handle
	for (i_idx = sizeof(window->p_planes)/sizeof(HPlane *) -1; i_idx >= 0; i_idx --) {
		p_plane = window->p_planes[i_idx];
		if (p_plane) {
			hplane_delete(p_plane);
		}
	}
	//here delete all ops
	hwidget_ops_delete();
	hcontainer_ops_delete();
	hplane_ops_delete();
	hcheckbox_ops_delete();
	htextinput_ops_delete();
	htextarea_ops_delete();
}


void hwindow_init()
{
	window = (HWindow *)vm_malloc(sizeof(HWindow));
	memset(window, 0, sizeof(HWindow));
  	window->sys_event_handle = sys_event_handle;
  	window->key_event_handle = key_event_handle;
  	window->pen_event_handle = pen_event_handle;
	window->get_default_plane = hwindow_get_default_plane;
	window->get_popupmenu_plane = hwindow_get_popupmenu_plane;
	window->get_glass_plane = hwindow_get_glass_plane;
	window->get_page_switch_contrller = hwindow_get_page_switch_contrller;
	window->validate = hwindow_validate;
	window->repaint = hwindow_repaint;
	window->s_screen_width = vm_graphic_get_screen_width();
	window->s_screen_height = vm_graphic_get_screen_height();
	window->s_is_support_touch_screen = vm_is_support_pen_touch();
}