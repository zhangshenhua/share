/*********************************
*file: hcontainer.c
*author: yan.gao
*date: 2011.08.23
*description: a container
***********************************/

#include "hcontainer.h"
#include "vmsys.h"
#include "string.h"
#include "vmgraph.h"

/*global instance*/
static HWidgetOperation *gp_hwidget_ops;
static HContainerOperation *gp_container_ops;

static void hcontainer_create_hcontainer_ops();

static void hcontainer_create_hwidget_ops();

HContainer * hcontainer_new()
{
	HContainer *pc;
	pc = vm_malloc(sizeof(HContainer));
	hcontainer_init(pc);
#ifdef H_DEBUG
	((HWidget *)pc)->p_class_name = vm_malloc(20);
	memset(((HWidget *)pc)->p_class_name, 0, 20);
	strncpy(((HWidget *)pc)->p_class_name, "HContainer", 20);
#endif
	return pc;
}

void hcontainer_init(HContainer *p_container)
{
	HWidget *p_widget = (HWidget *)p_container;
	memset(p_container, 0, sizeof(HContainer));
	hwidget_init(&p_container->base);
	if (!gp_hwidget_ops)
		hcontainer_create_hwidget_ops();
	p_container->base.p_widget_ops = gp_hwidget_ops;

	if (!gp_container_ops)
		hcontainer_create_hcontainer_ops();
	p_container->p_container_ops = gp_container_ops;
	//container enable drag
//	p_container->base.p_widget_ops->set_enable_drag(&p_container->base, 1);
	if (p_container->p_children == NULL)
		p_container->p_children = hlist_new();
	//by default, container disable focus
	p_widget->p_widget_ops->set_enable_focus(p_widget, 0);
}

void hcontainer_operation_init(HContainerOperation *p_container_ops)
{
	if (!gp_container_ops)
		hcontainer_create_hcontainer_ops();
	if (p_container_ops != gp_container_ops)
		memcpy(p_container_ops, gp_container_ops, sizeof(HContainerOperation));
}



static void hcontainer_add_widget(HContainer *p_container, HWidget *p_widget)
{
	hlist_append(p_container->p_children, p_widget, NULL);
	p_widget->p_parent = p_container;
}

static void hcontainer_remove_widget(HContainer *p_container, HWidget *p_widget)
{
	hlist_node_t *pn;
	HWidget *pw;
	hlist_for_each(pn, p_container->p_children) {
		pw = (HWidget *)pn->pv_data;
		if (pw == p_widget) {
			p_widget->p_widget_ops->destroy(p_widget);
			pn->pv_data = NULL;
			break;
		}
	}
	hlist_remove(p_container->p_children, p_widget);
}

// static void delete_widget(HContainer *p_container, HWidget *p_widget)
// {
// 	hlist_remove(p_container->p_children, p_widget);
// }

static HWidget* hcontainer_child_at(HContainer *p_container, short s_x, short s_y)
{
	HWidget *pw;
	hlist_node_t *pn;
	hlist_for_each(pn, p_container->p_children) {
		pw = (HWidget *)pn->pv_data;
		if (pw->s_top_x <= s_x && pw->s_top_y <= s_y && 
			pw->s_top_x + pw->s_width >= s_x && pw->s_top_y + pw->s_height >= s_y) {
				return pw;
		}
	}
	return (HWidget *)p_container;
}

static void hcontainer_remove_all(HContainer *p_container)
{
	HWidget *pw;
	hlist_node_t *pn;
	hlist_for_each(pn, p_container->p_children) {
		pw = (HWidget *)pn->pv_data;
		pw->p_widget_ops->destroy(pw);
		pn->pv_data = NULL;
	}
	hlist_clear(p_container->p_children);
}

#define _min_(a, b) ((a) < (b) ? (a) : (b))
#define _max_(a, b) ((a) > (b) ? (a) : (b))

static void hcontainer_scroll(HContainer *p_container, short s_dx, short s_dy)
{
	HWidget *pw = (HWidget *)p_container;
	short s_max_mtx = pw->s_width - pw->p_widget_ops->get_max_width(pw);
	short s_max_mty = pw->s_height - pw->p_widget_ops->get_max_height(pw);
	if (s_max_mtx > 0) {
		if (s_dx > 0) {
			s_dx = _min_(s_dx, - p_container->s_translate_x);
		} else {
			s_dx = _max_(s_dx, - s_max_mtx - p_container->s_translate_x);
		}
	} else {
		s_dx = 0;
	}
	if (s_max_mty > 0) {
		if (s_dy > 0) {
			s_dy = _min_(s_dy, - p_container->s_translate_y);
		} else {
			s_dy = _max_(s_dy, -s_max_mty - p_container->s_translate_y);
		}
	} else {
		s_dy = 0;
	}
	if (s_dx != 0 || s_dy != 0) {
		p_container->s_translate_x += s_dx;
		p_container->s_translate_y += s_dy;
		pw->p_widget_ops->repaint(pw);
	}
}



static void calc_preffered_size(HContainer *pc)
{
	HWidget *pw;
	hlist_node_t *pn;
	short s_w = 0;
	short s_h = 0;
	hlist_for_each(pn, pc->p_children) {
		pw = (HWidget *)pn->pv_data;
		if (pw->p_widget_ops->is_visible(pw)) {
			s_w = (s_w > pw->p_widget_ops->get_width(pw) + pw->s_top_x) ? s_w : pw->s_width + pw->s_top_x;
			s_h = (s_h > pw->p_widget_ops->get_height(pw) + pw->s_top_y) ? s_h : pw->s_height + pw->s_top_y;
		}
	}
	pw = (HWidget *)pc;
	s_w += pw->s_padding_left + pw->s_padding_right;
	s_h += pw->s_padding_top + pw->s_padding_bottom;
	pw->s_prefered_width = s_w;
	pw->s_prefered_height = s_h;
}

static short hcontainer_get_prefered_width(HWidget *p_widget)
{
	if (p_widget->s_prefered_width == 0) {
		calc_preffered_size((HContainer *)p_widget);
	}
	return p_widget->s_prefered_width;
}

static short hcontainer_get_prefered_height(HWidget *p_widget)
{
	if (p_widget->s_prefered_height == 0) {
		calc_preffered_size((HContainer *)p_widget);
	}
	return p_widget->s_prefered_height;
}

static void hcontainer_paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	HWidget *pw;
	HContainer *pc;
	hlist_node_t *pn;
	short s_w;
	short s_h;
	pc = (HContainer *)p_widget;
	//paint container self
	s_w = p_widget->p_widget_ops->get_max_width(p_widget) > p_widget->p_widget_ops->get_width(p_widget) ?
		p_widget->s_width : p_widget->s_max_width;
	s_h = p_widget->p_widget_ops->get_max_height(p_widget) > p_widget->p_widget_ops->get_height(p_widget) ?
		p_widget->s_height : p_widget->s_max_height;
	//TODO: clip  the container rect(s_screen_x, s_screen_y, s_w, s_h)
	vm_graphic_set_layer_clip(i_handle, s_screen_x, s_screen_y, s_screen_x + s_w, s_screen_y + s_h);
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget)) {
		//TODO: fill the container rect(s_screen_x, s_screen_y, s_w, s_h) with bgcolor
		vm_graphic_fill_rect(vm_graphic_get_layer_buffer(i_handle), s_screen_x, s_screen_y, s_w, s_h, (VMINT16)p_widget->i_bgcolor, (VMINT16)p_widget->i_bgcolor);
	}
	//paint children
	hlist_for_each(pn, pc->p_children) {
		pw = (HWidget *)pn->pv_data;
		pw->p_widget_ops->paint(pw,
			i_handle,
			s_screen_x + p_widget->s_padding_left + pw->s_top_x + pc->s_translate_x,
			s_screen_y + p_widget->s_padding_top + pw->s_top_y + pc->s_translate_y);
	}
}

static void hcontainer_notify_size_changed(HWidget *p_widget)
{
	
}

/************************************************************************
* 
************************************************************************/

static void hcontainer_pen_press(HWidget *p_widget, short s_x, short s_y)
{
}


static void hcontainer_pen_release(HWidget *p_widget, short s_x, short s_y)
{
	if (p_widget->action_performed)
		p_widget->action_performed(p_widget, NULL);
}


static void hcontainer_pen_move(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}


static void hcontainer_pen_enter(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}


void hcontainer_pen_leave(HWidget *p_widget, short s_x, short s_y)
{
	//nothing
}


static void hcontainer_key_press(HWidget *p_widget, int keycode)
{
	
}

static HClass hcontainer_get_class(HWidget *p_widget)
{
	return CLASS_CONTAINER;
}

static int hcontainer_is_container(HWidget *p_widget)
{
	return 1;
}

static int hcontainer_can_scroll(HContainer *p_container)
{
	HWidget *pw = (HWidget *)p_container;
	return pw->s_max_width < pw->s_width || pw->s_max_height < pw->s_height;
}

void hcontainer_delete(HContainer *p_container)
{
	hcontainer_remove_all(p_container);
	vm_free(p_container->p_children);
	vm_free(p_container);
}


static void hcontainer_create_hwidget_ops()
{
	gp_hwidget_ops = vm_malloc(sizeof(HWidgetOperation));
	hwidget_operation_init(gp_hwidget_ops);
	gp_hwidget_ops->get_prefered_width = hcontainer_get_prefered_width;
	gp_hwidget_ops->get_prefered_height = hcontainer_get_prefered_height;
	gp_hwidget_ops->paint = hcontainer_paint;
	gp_hwidget_ops->notify_size_changed = hcontainer_notify_size_changed;
	gp_hwidget_ops->pen_press = hcontainer_pen_press;
	gp_hwidget_ops->pen_release = hcontainer_pen_release;
	gp_hwidget_ops->pen_move = hcontainer_pen_move;
	gp_hwidget_ops->pen_enter = hcontainer_pen_enter;
	gp_hwidget_ops->pen_leave = hcontainer_pen_leave;
	gp_hwidget_ops->key_press = hcontainer_key_press;
	gp_hwidget_ops->is_container = hcontainer_is_container;
	gp_hwidget_ops->destroy = (void (*)(HWidget *))hcontainer_delete;
	gp_hwidget_ops->get_class = hcontainer_get_class;
}

static void hcontainer_create_hcontainer_ops()
{
	gp_container_ops = vm_malloc(sizeof(HContainerOperation));
	memset(gp_container_ops, 0, sizeof(HContainerOperation));
	gp_container_ops->add_widget = hcontainer_add_widget;
	gp_container_ops->remove_widget = hcontainer_remove_widget;
	gp_container_ops->child_at = hcontainer_child_at;
	gp_container_ops->remove_all = hcontainer_remove_all;
	gp_container_ops->scroll = hcontainer_scroll;
	gp_container_ops->can_scroll = hcontainer_can_scroll;
}