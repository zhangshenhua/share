/*********************************
*file: hwidget.c
*author: yan.gao
*date: 2011.08.22
*description:  base class widget
***********************************/

#include "stdlib.h"
#include "vmsys.h"
#include "string.h"
<<<<<<< HEAD
#include "vmgraph.h"
=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc

#include "hwidget.h"
#include "hcontainer.h"
#include "hplane.h"

static HWidgetOperation *gp_widget_ops;

static void create_widget_ops();

void hwidget_init(HWidget *p_widget)
{
	if (!gp_widget_ops)
		create_widget_ops();
	memset(p_widget, 0, sizeof(HWidget));
	p_widget->p_widget_ops = gp_widget_ops;
	gp_widget_ops->set_visible(p_widget, 1);
	gp_widget_ops->set_enable_focus(p_widget, 1);
	gp_widget_ops->set_enable(p_widget, 1);
}

void hwidget_operation_init(HWidgetOperation *p_widget_ops)
{
	if (p_widget_ops && p_widget_ops != gp_widget_ops) {
		if (gp_widget_ops == NULL) {
			create_widget_ops();
		}
		memcpy(p_widget_ops, gp_widget_ops, sizeof(HWidgetOperation));
	}
}

enum {
	ATTR_VISIBLE,
	ATTR_ENABLE_FOCUS,
	ATTR_HAS_FOCUS,
	ATTR_ENABLE,
	ATTR_BGCOLOR_ENABLE,
	ATTR_ENABLE_DRAG,

	ATTR_WIDTH_FLAG,
	ATTR_HEIGHT_FLAG,
	ATTR_MAX_WIDTH_FLAG,
	ATTR_MAX_HEIGHT_FLAG,
};

/*whether the widget visible, return 0 or 1*/
static int is_visible(HWidget *p_widget)
{
	return (p_widget->s_attr & (1 << ATTR_VISIBLE) ) != 0;
}

/*set the widget visible, its 0 or 1*/
static void set_visible(HWidget *p_widget, int i_visible)
{
	if (i_visible)
		p_widget->s_attr |= (1 << ATTR_VISIBLE);
	else
		p_widget->s_attr &= ~(1 << ATTR_VISIBLE);
}

/*whether the widget enable focus, return 0 or 1*/
static int is_enable_focus(HWidget *p_widget)
{
	return (p_widget->s_attr & (1 << ATTR_ENABLE_FOCUS) ) != 0;
}

/*set the widget enable focus, its 0 or 1*/
static void set_enable_focus(HWidget *p_widget, int i_enable_focus)
{
	if (i_enable_focus)
		p_widget->s_attr |= (1 << ATTR_ENABLE_FOCUS);
	else
		p_widget->s_attr &= ~(1 << ATTR_ENABLE_FOCUS);
}

/*whether the widget has focus, return 0 or 1*/
static int has_focus(HWidget *p_widget)
{
	return (p_widget->s_attr & (1 << ATTR_HAS_FOCUS) ) != 0;
}

/*set the widget whether has focus, i_focus is 0 or 1*/
static void set_focus(HWidget *p_widget, int i_focus)
{
	if (i_focus)
		p_widget->s_attr |= (1 << ATTR_HAS_FOCUS);
	else
		p_widget->s_attr &= ~(1 << ATTR_HAS_FOCUS);
}

/*whether the widget is enable, return 0 or 1*/
static int is_enable(HWidget *p_widget)
{
	return (p_widget->s_attr & (1 << ATTR_ENABLE) ) != 0;
}

/*set the widget whether  enable, i_enable is 0 or 1*/
static void set_enable(HWidget *p_widget, int i_enable)
{
	if (i_enable)
		p_widget->s_attr |= (1 << ATTR_ENABLE);
	else
		p_widget->s_attr &= ~(1 << ATTR_ENABLE);
}

/*whether the widget bgcolor enable, return 0 or 1. default is 0*/
static int is_enable_bgcolor(HWidget *p_widget)
{
	return (p_widget->s_attr & (1 << ATTR_BGCOLOR_ENABLE) ) != 0;
}

/*set the widget whether  enable, i_bgcolor_enable is 0 or 1*/
static void set_enable_bgcolor(HWidget *p_widget, int i_bgcolor_enable)
{
	if (i_bgcolor_enable)
		p_widget->s_attr |= (1 << ATTR_BGCOLOR_ENABLE);
	else
		p_widget->s_attr &= ~(1 << ATTR_BGCOLOR_ENABLE);
}

/*whether the widget drag enable, return 0 or 1. default is 0*/
int is_enable_drag(HWidget *p_widget)
{
	return (p_widget->s_attr & (1 << ATTR_ENABLE_DRAG)) != 0;
}

/*set the widget whether drag enable, i_enable_drag is 0 or 1*/
void set_enable_drag(HWidget *p_widget, int i_enable_drag)
{
	if (i_enable_drag)
		p_widget->s_attr |= (1 << ATTR_ENABLE_DRAG);
	else
		p_widget->s_attr &= ~(1 << ATTR_ENABLE_DRAG);
}

/*get the widget color*/
static int get_color(HWidget *p_widget)
{
	return p_widget->i_color;
}

/*set the widget color*/
static void set_color(HWidget *p_widget, int i_rgb)
{
	p_widget->i_color = i_rgb;
}

/*get the widget bgcolor*/
static int get_bgcolor(HWidget *p_widget)
{
	return p_widget->i_bgcolor;
}

/*set the widget bgcolor*/
static void set_bgcolor(HWidget *p_widget, int i_rgb)
{
	p_widget->i_bgcolor = i_rgb;
}

/*get the widget default bgcolor, inherit from its ancestor*/
static int get_default_bgcolor(HWidget *p_widget)
{
	HWidget *p;
	for (p = (HWidget *)p_widget->p_parent; p; p = (HWidget *)p->p_parent) {
		if (p->p_widget_ops->is_enable_bgcolor(p))
			return p->i_bgcolor;
	}
	return p_widget->i_bgcolor;
}

/*get the widget font*/
static HFont get_font(HWidget *p_widget)
{
	return p_widget->c_font;
}

/*set the widget font*/
static void set_font(HWidget *p_widget, HFont font)
{
	p_widget->c_font = font;
}

/*get the widget width*/
static short get_width(HWidget *p_widget)
{
	if (p_widget->s_width == 0) {
		p_widget->s_width = p_widget->p_widget_ops->get_prefered_width(p_widget);
	}
	return p_widget->s_width;
}

/*set the widget width . if s_width is 0, then will generate a best width for you*/
static void set_width(HWidget *p_widget, short s_width)
{
	if (s_width)
		p_widget->s_attr |= (1 << ATTR_WIDTH_FLAG);
	 else 
		p_widget->s_attr &= ~(1 << ATTR_WIDTH_FLAG);
	
	p_widget->s_width = s_width;
}

/*get the widget height*/
static short get_height(HWidget *p_widget)
{
	if (p_widget->s_height == 0) {
		p_widget->s_height = p_widget->p_widget_ops->get_prefered_height(p_widget);
	}
	return p_widget->s_height;
}

/*set the widget height*/
static void set_height(HWidget *p_widget, short s_height)
{
	if (s_height)
		p_widget->s_attr |= (1 << ATTR_HEIGHT_FLAG);
	else
		p_widget->s_attr &= ~(1 << ATTR_HEIGHT_FLAG);

	p_widget->s_height = s_height;
}

/*get the widget preferred width*/
//static short get_prefered_width(HWidget *p_widget);

/*get the widget preferred height*/
//static short get_prefered_height(HWidget *p_widget);

/*get the widget max width*/
static short get_max_width(HWidget *p_widget)
{
	HWidget *p;
	if (p_widget->s_max_width == 0) {
		if (p_widget->p_parent) {
			p = (HWidget *)p_widget->p_parent;
			p_widget->s_max_width = p->p_widget_ops->get_max_width(p) - 
				p->uc_padding_left - p->uc_padding_right - p_widget->s_top_x;
		}
	}
	return p_widget->s_max_width;
}

/*set the widget max width*/
static void set_max_width(HWidget *p_widget, short s_max_width)
{
	if (s_max_width)
		p_widget->s_attr |= (1 << ATTR_MAX_WIDTH_FLAG);
	else 
		p_widget->s_attr &= ~(1 << ATTR_MAX_WIDTH_FLAG);

	p_widget->s_max_width = s_max_width;
}

/*get the widget max height*/
static short get_max_height(HWidget *p_widget)
{
	HWidget *p;
	if (p_widget->s_max_height == 0) {
		if (p_widget->p_parent) {
			p = (HWidget *)p_widget->p_parent;
			p_widget->s_max_height = p->p_widget_ops->get_max_height(p) - 
				p->uc_padding_top - p->uc_padding_bottom - p_widget->s_top_y;
		}
	}
	return p_widget->s_max_height;
}

/*set the widget max height*/
static void set_max_height(HWidget *p_widget, short s_max_height)
{
	if (s_max_height)
		p_widget->s_attr |= (1 << ATTR_MAX_HEIGHT_FLAG);
	else 
		p_widget->s_attr &= ~(1 << ATTR_MAX_HEIGHT_FLAG);

	p_widget->s_max_height = s_max_height;
}

/*set the widget padding left*/
static void set_padding_left(HWidget *p_widget, unsigned char s_padding_left)
{
	p_widget->uc_padding_left = s_padding_left;
}

/*set the widget padding right*/
static void set_padding_right(HWidget *p_widget, unsigned char s_padding_right)
{
	p_widget->uc_padding_right = s_padding_right;
}

/*set the widget padding top*/
static void set_padding_top(HWidget *p_widget, unsigned char s_padding_top)
{
	p_widget->uc_padding_top = s_padding_top;
}

/*set the widget padding bottom*/
static void set_padding_bottom(HWidget *p_widget, unsigned char s_padding_bottom)
{
	p_widget->uc_padding_bottom = s_padding_bottom;
}

/*paint the widget*/
//static void paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y);

/*only repaint the widget itself*/
static void repaint(HWidget *p_widget)
{
	HPoint p;
	int i_handle;
	int i_bgcolor_enable;
	int i_bgcolor;
	p = p_widget->p_widget_ops->get_screen_point(p_widget, 0, 0);
	i_handle = p_widget->p_widget_ops->get_paint_handle(p_widget);
	i_bgcolor_enable = p_widget->p_widget_ops->is_enable_bgcolor(p_widget);
	if (!i_bgcolor_enable) {
		i_bgcolor = p_widget->p_widget_ops->get_default_bgcolor(p_widget);
		//TODO: use i_bgcolor to fill a rect(p_widget->s_width,p_widget->s_height) at position p
<<<<<<< HEAD
		vm_graphic_fill_rect(vm_graphic_get_layer_buffer(i_handle), p.s_x, p.s_y, p_widget->s_width, p_widget->s_height, i_bgcolor, i_bgcolor);
	}
	p_widget->p_widget_ops->paint(p_widget, i_handle, p.s_x, p.s_y);
	//TODO: flush the screen
	vm_graphic_flush_layer(&i_handle, 1);
=======
	}
	p_widget->p_widget_ops->paint(p_widget, i_handle, p.s_x, p.s_y);
	//TODO: flush the screen
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
}

/*validate the widget preferred size, size , and max size*/
static void validate(HWidget *p_widget)
{
	p_widget->p_widget_ops->get_width(p_widget);
	p_widget->p_widget_ops->get_height(p_widget);
	p_widget->p_widget_ops->get_max_width(p_widget);
	p_widget->p_widget_ops->get_max_height(p_widget);
}

/*invalidate the widget preferred size, size , and max size*/
static void invalidate(HWidget *p_widget)
{
	if (!(p_widget->s_attr & (1 << ATTR_WIDTH_FLAG)))
		p_widget->s_width = 0;
	if (!(p_widget->s_attr & (1 << ATTR_HEIGHT_FLAG)))
		p_widget->s_height = 0;
	if (!(p_widget->s_attr & (1 << ATTR_MAX_WIDTH_FLAG)))
		p_widget->s_max_width = 0;
	if (!(p_widget->s_attr & (1 << ATTR_MAX_HEIGHT_FLAG)))
		p_widget->s_max_height = 0;
	p_widget->s_prefered_width = 0;
	p_widget->s_prefered_height = 0;
}

/*translate the widget point to screen point, if the p_widget is not in a HPlane, then return Point{-1,-1}*/
static HPoint get_screen_point(HWidget *p_widget, short s_x, short s_y)
{
	HPoint point;
<<<<<<< HEAD
	point.s_x = s_x + p_widget->s_top_x;
	point.s_y = s_y + p_widget->s_top_y;
	if (p_widget->p_widget_ops->is_plane(p_widget)) {
		return point;
	}
	for (p_widget = (HWidget *)p_widget->p_parent; p_widget && p_widget->p_parent; p_widget = (HWidget *)p_widget->p_parent) {
		point.s_x += p_widget->s_top_x + ((HContainer *)p_widget)->s_translate_x + p_widget->uc_padding_left;
		point.s_y += p_widget->s_top_y + ((HContainer *)p_widget)->s_translate_y + p_widget->uc_padding_top;
	}
	if (p_widget->p_widget_ops->is_plane(p_widget)) {
=======
	struct _HContainer *pc;
	point.s_x = s_x + p_widget->s_top_x;
	point.s_y = s_y + p_widget->s_top_y;
	for (pc = p_widget->p_parent; pc && !((HWidget *)pc)->p_parent; pc = ((HWidget *)pc)->p_parent) {
		point.s_x += ((HWidget *)pc)->s_top_x + pc->s_translate_x + ((HWidget *)pc)->uc_padding_left;
		point.s_y += ((HWidget *)pc)->s_top_y + pc->s_translate_y + ((HWidget *)pc)->uc_padding_top;
	}
	if (((HWidget *)pc)->p_widget_ops->is_plane(((HWidget *)pc))) {
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
		return point;
	}
	//the p_widet is not in a HPlane
	point.s_x = -1;
	point.s_y = -1;
	return point;
}

/*when the widget size(width/height) change call it to notify parent recalculate its size*/
static void notify_size_changed(HWidget *p_widget)
{
	if (!p_widget->p_parent)
		return;
	p_widget->p_widget_ops->invalidate(p_widget);
	p_widget->p_widget_ops->validate(p_widget);
	((HWidget *)p_widget->p_parent)->p_widget_ops->notify_size_changed((HWidget *)p_widget->p_parent);
}

/*notify the widget parent its visible changed*/
static void notify_visible_changed(HWidget *p_widget)
{
	notify_size_changed(p_widget);
}

/*notify the widget parent its coordinate changed*/
static void notify_coordinate_changed(HWidget *p_widget)
{
	notify_size_changed(p_widget);
}

/*pen press event callback*/
static void pen_press(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}

/*pen release event callback*/
static void pen_release(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}

/*pen move event callback*/
static void pen_move(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}

static void pen_enter(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}

static void pen_leave(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}

/*keyboard press event callback*/
static void key_press(HWidget *p_widget, int keycode)
{
	//nothing
}

/*whether the widget is a container*/
static int is_container(HWidget *p_widget)
{
	return 0;
}

/*whether the widget is a plane*/
static int is_plane(HWidget *p_widget)
{
	return 0;
}

<<<<<<< HEAD
static int can_travel(HWidget *p_widget, int keycode)
{
	return 0;
}

#if 0
static void focus_changed(HWidget *p_widget, int i_gained_focus)
{
	p_widget->p_widget_ops->set_focus(p_widget, i_gained_focus);
	p_widget->p_widget_ops->repaint(p_widget);
}
#endif

/*get the widget root widget(its a HPlane), if the widget not in a plane, NULL will be returned*/
static HPlane* get_root(HWidget *p_widget)
{
	if (p_widget->p_widget_ops->is_plane(p_widget))
		return (HPlane*)p_widget;
	for (p_widget = (HWidget *)p_widget->p_parent; p_widget && p_widget->p_parent; p_widget = (HWidget *)p_widget->p_parent);
	if (p_widget->p_widget_ops->is_plane(p_widget))
		return (HPlane *)p_widget;
=======
/*get the widget root widget(its a HPlane), if the widget not in a plane, NULL will be returned*/
static HPlane* get_root(HWidget *p_widget)
{
	HContainer *pc;
	for (pc = p_widget->p_parent; pc && !((HWidget *)pc)->p_parent; pc = ((HWidget *)pc)->p_parent);
	if (((HWidget *)pc)->p_widget_ops->is_plane((HWidget *)pc))
		return (HPlane *)pc;
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	return NULL;
}

/*get the paint handle. if the p_widget is not in a HPlane, then will return -1*/
int get_paint_handle(HWidget *p_widget)
{
	HPlane *p_root;
	p_root = get_root(p_widget);
	if (p_root)
		return p_root->i_handle;
	return -1;
}

/*clone the operation structure*/
static HWidgetOperation * clone(HWidget *p_widget)
{
	HWidgetOperation *p_ops;
	p_ops = vm_malloc(sizeof(HWidgetOperation));
	memcpy(p_ops, p_widget->p_widget_ops, sizeof(HWidgetOperation));
	return p_ops;
}

/*destroy a widget*/
static void destroy(HWidget *p_widget)
{
	vm_free(p_widget);
}

static void create_widget_ops()
{
	gp_widget_ops = vm_malloc(sizeof(HWidgetOperation));

	gp_widget_ops->is_visible = is_visible;
	gp_widget_ops->set_visible = set_visible;
	gp_widget_ops->is_enable_focus = is_enable_focus;
	gp_widget_ops->set_enable_focus = set_enable_focus;
	gp_widget_ops->has_focus = has_focus;
	gp_widget_ops->set_focus = set_focus;
	gp_widget_ops->is_enable = is_enable;
	gp_widget_ops->set_enable = set_enable;
	gp_widget_ops->is_enable_bgcolor = is_enable_bgcolor;
	gp_widget_ops->set_enable_bgcolor = set_enable_bgcolor;
	gp_widget_ops->is_enable_drag = is_enable_drag;
	gp_widget_ops->set_enable_drag = set_enable_drag;

	gp_widget_ops->get_color = get_color;
	gp_widget_ops->set_color = set_color;
	gp_widget_ops->get_bgcolor = get_bgcolor;
	gp_widget_ops->set_bgcolor = set_bgcolor;
	gp_widget_ops->get_default_bgcolor = get_default_bgcolor;
	gp_widget_ops->get_font = get_font;
	gp_widget_ops->set_font = set_font;
	gp_widget_ops->get_width = get_width;
	gp_widget_ops->set_width = set_width;
	gp_widget_ops->get_height = get_height;
	gp_widget_ops->set_height = set_height;
	gp_widget_ops->get_max_width = get_max_width;
	gp_widget_ops->set_max_width = set_max_width;
	gp_widget_ops->get_max_height = get_max_height;
	gp_widget_ops->set_max_height = set_max_height;
	gp_widget_ops->set_padding_bottom = set_padding_bottom;
	gp_widget_ops->set_padding_left  = set_padding_left;
	gp_widget_ops->set_padding_right = set_padding_right;
	gp_widget_ops->set_padding_top = set_padding_top;
	gp_widget_ops->repaint = repaint;
	gp_widget_ops->validate = validate;
	gp_widget_ops->invalidate = invalidate;
	gp_widget_ops->get_screen_point = get_screen_point;
	gp_widget_ops->notify_size_changed = notify_size_changed;
	gp_widget_ops->notify_visible_changed = notify_visible_changed;
	gp_widget_ops->notify_coordinate_changed = notify_coordinate_changed;
 	gp_widget_ops->pen_press = pen_press;
 	gp_widget_ops->pen_release = pen_release;
 	gp_widget_ops->pen_move = pen_move;
	gp_widget_ops->pen_enter = pen_enter;
	gp_widget_ops->pen_leave = pen_leave;

 	gp_widget_ops->key_press = key_press;
	gp_widget_ops->is_container = is_container;
	gp_widget_ops->is_plane = is_plane;
<<<<<<< HEAD
	gp_widget_ops->can_travel = can_travel;
//	gp_widget_ops->focus_changed = focus_changed;
=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	gp_widget_ops->get_root = get_root;
	gp_widget_ops->get_paint_handle = get_paint_handle;
	gp_widget_ops->clone = clone;
	gp_widget_ops->destroy = destroy;
}