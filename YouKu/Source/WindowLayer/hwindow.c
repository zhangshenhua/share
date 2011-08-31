/********************************************************
*file: hwindow.h
*author: yan.gao
*date: 2011.08.23
*description: Globally unique object in our app.
********************************************************/

#include "hwindow.h"
#include "../WidgetLayer/hwidget.h"
#include "../WidgetLayer/hlookandfeel.h"

#include "string.h"
#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmlog.h"


HWindow *window;

static void paint();
static HPlane * get_default_plane();

/*user define the enter function*/
extern void app_enter();

static void validate()
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
	int i_idx;
	switch (message) {
		case VM_MSG_CREATE:
			vm_log_debug("vm msg create");
			// create default plane
			get_default_plane();
			app_enter();
			break;

		case VM_MSG_ACTIVE:
			vm_log_debug("vm msg active");
			break;

		case VM_MSG_PAINT:
			vm_log_debug("vm msg paint");
			paint();
			break;

		case VM_MSG_INACTIVE:
			vm_log_debug("vm msg inactive");
			//delete all layer handle
			for (i_idx = sizeof(window->p_planes)/sizeof(HPlane *) -1; i_idx >= 0; i_idx --) {
				if (window->p_planes[i_idx] && window->p_planes[i_idx]->i_handle != -1)
					vm_graphic_delete_layer(window->p_planes[i_idx] ->i_handle);
			}
			break;

		case VM_MSG_QUIT:
			break;
	}
}

/************************************************************************/
/* Get topest plane
/************************************************************************/
static HPlane * get_topest_plane()
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
}

static void pen_event_handle(VMINT evt, VMINT x, VMINT y)
{
	HWidget *p_top_plane = (HWidget *)get_topest_plane();
	switch (evt) {
//		case VM_PEN_EVENT_DOUBLE_CLICK:
//		case VM_PEN_EVENT_LONG_TAP :
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

static void paint()
{
	int i_idx;
	HWidget *pw;
	HPlane *p_plane;
	int p_handles[sizeof(window->p_planes)/sizeof(HPlane *)];
	int i_handle_idx = 0;
	for (i_idx = 0; i_idx < sizeof(window->p_planes)/sizeof(HPlane *); i_idx ++) {
		p_plane = window->p_planes[i_idx];
		pw = (HWidget *)p_plane;
		if (pw && pw->p_widget_ops->is_visible(pw)) {
			p_handles[i_handle_idx ++] = p_plane->i_handle;
			pw->p_widget_ops->paint(pw, p_plane->i_handle, 0, 0);
			if (p_plane && p_plane->i_enable_transparent_bg && pw->p_widget_ops->is_visible(pw)) {
				//TODO: paint transparent pw->i_handle
				look_draw_plane(p_plane);
			}
		}
	}
	//TODO: flush the three planes
	vm_graphic_flush_layer(p_handles, i_handle_idx);
}

<<<<<<< HEAD
static void repaint()
{
	paint();
}

=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
static HPlane * get_default_plane()
{
	HPlane *p_plane;
	if (window->p_planes[0] == NULL) {
		p_plane = hplane_new();
		//default is visible
		//((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 1);
		((HWidget *)p_plane)->p_widget_ops->set_enable_bgcolor((HWidget *)p_plane, 1);
		((HWidget *)p_plane)->p_widget_ops->set_bgcolor((HWidget *)p_plane, VM_COLOR_WHITE);
		window->p_planes[0] = p_plane;
	}
	return window->p_planes[0];
}

static HPlane * get_popupmenu_plane()
{
	HPlane *p_plane;
	if (window->p_planes[1] == NULL) {
		p_plane = hplane_new();
		//default is enable transparent background
		p_plane->p_plane_ops->set_enable_transparent_bg(p_plane, 1);
		window->p_planes[1] = p_plane;
	} else {
		p_plane = window->p_planes[1];
	}
	//visible the popup menu plane
	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 1);
	return p_plane;
}

static HPlane * get_glass_plane()
{
	HPlane *p_plane;
	if (window->p_planes[2] == NULL) {
		p_plane = hplane_new();
		//default is enable transparent background
		p_plane->p_plane_ops->set_enable_transparent_bg(p_plane, 1);
		window->p_planes[2] = p_plane;
	} else {
		p_plane = window->p_planes[2];
	}
	//visible the popup menu plane
	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 1);
	return p_plane;
}

static PageSwitchController * get_page_switch_contrller()
{
	if (window->p_page_switch_contrller == NULL) {
		window->p_page_switch_contrller = page_switch_controller_new();
	}
	return window->p_page_switch_contrller;
}

static void update_new_page(Page *p_page)
{
	PageSwitchController *p_ctrl = window->get_page_switch_contrller();
	p_ctrl->append_page(p_ctrl, p_page);
	p_ctrl->show_cur_page(p_ctrl);
}

void hwindow_init()
{
	window = (HWindow *)vm_malloc(sizeof(HWindow));
	memset(window, 0, sizeof(HWindow));
  	window->sys_event_handle = sys_event_handle;
  	window->key_event_handle = key_event_handle;
  	window->pen_event_handle = pen_event_handle;
	window->get_default_plane = get_default_plane;
	window->get_popupmenu_plane = get_popupmenu_plane;
	window->get_glass_plane = get_glass_plane;
	window->get_page_switch_contrller = get_page_switch_contrller;
	window->update_new_page = update_new_page;
	window->validate = validate;
<<<<<<< HEAD
	window->repaint = repaint;
=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
}