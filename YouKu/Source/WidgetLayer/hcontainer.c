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

static void create_hcontainer_ops();

static void create_hwidget_ops();

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
	memset(p_container, 0, sizeof(HContainer));
	hwidget_init(&p_container->base);
	if (!gp_hwidget_ops)
		create_hwidget_ops();
	p_container->base.p_widget_ops = gp_hwidget_ops;

	if (!gp_container_ops)
		create_hcontainer_ops();
	p_container->p_container_ops = gp_container_ops;
	//container enable drag
	p_container->base.p_widget_ops->set_enable_drag(&p_container->base, 1);
<<<<<<< HEAD
	if (p_container->p_children == NULL)
=======
	//init the p_children
	if (!p_container->p_children)
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
		p_container->p_children = hlist_new();
}

void hcontainer_operation_init(HContainerOperation *p_container_ops)
{
	if (!gp_container_ops)
		create_hcontainer_ops();
<<<<<<< HEAD
	if (p_container_ops != gp_container_ops)
=======
	if (p_container_ops != p_container_ops)
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
		memcpy(p_container_ops, gp_container_ops, sizeof(HContainerOperation));
}



static void add_widget(HContainer *p_container, HWidget *p_widget)
{
	hlist_append(p_container->p_children, p_widget, NULL);
	p_widget->p_parent = p_container;
}

static void remove_widget(HContainer *p_container, HWidget *p_widget)
{
<<<<<<< HEAD

=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	hlist_remove(p_container->p_children, p_widget);
}

// static void delete_widget(HContainer *p_container, HWidget *p_widget)
// {
// 	hlist_remove(p_container->p_children, p_widget);
// }

static HWidget* child_at(HContainer *p_container, short s_x, short s_y)
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

static void clear(HContainer *p_container)
{
	hlist_clear(p_container->p_children);
}

static void scroll(HContainer *p_container, short s_dx, short s_dy)
{
	p_container->s_translate_x += s_dx;
	p_container->s_translate_y += s_dy;
	p_container->base.p_widget_ops->repaint((HWidget *)p_container);
}



static void calc_preffered_size(HContainer *pc)
{
	HWidget *pw;
	hlist_node_t *pn;
	short s_w = 0;
	short s_h = 0;
	hlist_for_each(pn, pc->p_children) {
		pw = (HWidget *)pn->pv_data;
<<<<<<< HEAD
		s_w = (s_w > pw->p_widget_ops->get_width(pw) + pw->s_top_x) ? s_w : pw->s_width + pw->s_top_x;
		s_h = (s_h > pw->p_widget_ops->get_height(pw) + pw->s_top_y) ? s_h : pw->s_height + pw->s_top_y;
=======
		s_w = (s_w > pw->p_widget_ops->get_width(pw) + pw->s_top_x) ? s_w : pw->s_prefered_width + pw->s_top_x;
		s_h = (s_h > pw->p_widget_ops->get_height(pw) + pw->s_top_y) ? s_h : pw->s_prefered_height + pw->s_top_y;
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	}
	pw = (HWidget *)pc;
	s_w += pw->uc_padding_left + pw->uc_padding_right;
	s_h += pw->uc_padding_top + pw->uc_padding_bottom;
	pw->s_prefered_width = s_w;
	pw->s_prefered_height = s_h;
}

static short get_prefered_width(HWidget *p_widget)
{
	if (p_widget->s_prefered_width == 0) {
		calc_preffered_size((HContainer *)p_widget);
	}
	return p_widget->s_prefered_width;
}

static short get_prefered_height(HWidget *p_widget)
{
	if (p_widget->s_prefered_height == 0) {
		calc_preffered_size((HContainer *)p_widget);
	}
	return p_widget->s_prefered_height;
}

static void paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	HWidget *pw;
	HContainer *pc;
	hlist_node_t *pn;
	short s_w;
	short s_h;
	pc = (HContainer *)p_widget;
	//paint container self
	s_w = p_widget->p_widget_ops->get_max_width(p_widget) > p_widget->s_width ? p_widget->p_widget_ops->get_max_width(p_widget) : p_widget->s_width;
	s_h = p_widget->p_widget_ops->get_max_height(p_widget) > p_widget->s_height ? p_widget->p_widget_ops->get_max_height(p_widget) : p_widget->s_height;
	//TODO: clip  the container rect(s_screen_x, s_screen_y, s_w, s_h) 
	vm_graphic_set_layer_clip(i_handle, s_screen_x, s_screen_y, s_screen_x + s_w, s_screen_y + s_h);
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget)) {
		//TODO: fill the container rect(s_screen_x, s_screen_y, s_w, s_h) with bgcolor
		vm_graphic_fill_rect(vm_graphic_get_layer_buffer(i_handle), s_screen_x, s_screen_y, s_w, s_h, (VMINT16)p_widget->i_bgcolor, (VMINT16)p_widget->i_bgcolor);
	}
	//paint children
	hlist_for_each(pn, pc->p_children) {
		pw = (HWidget *)pn->pv_data;
<<<<<<< HEAD
		pw->p_widget_ops->paint(pw,
=======
		pw->p_widget_ops->paint(pw, 
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
			i_handle,
			s_screen_x + p_widget->uc_padding_left + pw->s_top_x + pc->s_translate_x,
			s_screen_y + p_widget->uc_padding_top + pw->s_top_y + pc->s_translate_y);
	}
}

static void notify_size_changed(HWidget *p_widget)
{
	
}

static void pen_press(HWidget *p_widget, short s_x, short s_y)
{
	
}


static void pen_release(HWidget *p_widget, short s_x, short s_y)
{
	
}


static void pen_move(HWidget *p_widget, short s_x, short s_y)
{
	
}


static void pen_enter(HWidget *p_widget, short s_x, short s_y)
{
	
}


void pen_leave(HWidget *p_widget, short s_x, short s_y)
{
	
}


static void key_press(HWidget *p_widget, int keycode)
{
	
}

static HClass get_class(HWidget *p_widget)
{
	return CLASS_CONTAINER;
}

static int is_container(HWidget *p_widget)
{
	return 1;
}

<<<<<<< HEAD
static int can_scroll(HContainer *p_container)
{
	HWidget *pw = (HWidget *)p_container;
	return pw->s_max_width < pw->s_width || pw->s_max_height < pw->s_height;
}

 void hcontainer_delete(HWidget *p_widget)
{
	HContainer *pc;
	hlist_node_t *pn;
	pc = (HContainer *)p_widget;
	hlist_for_each(pn, pc->p_children) {
		((HWidget *)pn->pv_data)->p_widget_ops->destroy((HWidget *)pn->pv_data);
	}
=======
 void hcontainer_delete(HWidget *p_widget)
{
	HContainer *pc;
	pc = (HContainer *)p_widget;
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	hlist_destroy(&pc->p_children);
	vm_free(pc);
}


static void create_hwidget_ops()
{
	gp_hwidget_ops = vm_malloc(sizeof(HWidgetOperation));
	hwidget_operation_init(gp_hwidget_ops);
	gp_hwidget_ops->get_prefered_width = get_prefered_width;
	gp_hwidget_ops->get_prefered_height = get_prefered_height;
	gp_hwidget_ops->paint = paint;
	gp_hwidget_ops->notify_size_changed = notify_size_changed;
	gp_hwidget_ops->pen_press = pen_press;
	gp_hwidget_ops->pen_release = pen_release;
	gp_hwidget_ops->pen_move = pen_move;
	gp_hwidget_ops->pen_enter = pen_enter;
	gp_hwidget_ops->pen_leave = pen_leave;
	gp_hwidget_ops->key_press = key_press;
	gp_hwidget_ops->is_container = is_container;
	gp_hwidget_ops->destroy = hcontainer_delete;
	gp_hwidget_ops->get_class = get_class;
}

static void create_hcontainer_ops()
{
	gp_container_ops = vm_malloc(sizeof(HContainerOperation));
	memset(gp_container_ops, 0, sizeof(HContainerOperation));
	gp_container_ops->add_widget = add_widget;
	gp_container_ops->remove_widget = remove_widget;
//	gp_container_ops->delete_widget = delete_widget;
	gp_container_ops->child_at = child_at;
	gp_container_ops->remove_all = clear;
//	gp_container_ops->delete_all = delete_all;
	gp_container_ops->scroll = scroll;
<<<<<<< HEAD
	gp_container_ops->can_scroll = can_scroll;
=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
}