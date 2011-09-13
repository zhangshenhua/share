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
#include "../WindowLayer/hwindow.h"
#include "hlookandfeel.h"


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
		p_container->p_children = hwidget_new_vector();
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
	vector_append_data(p_container->p_children, p_widget);
	p_widget->p_parent = p_container;
}

#define pw_get_attr(pw, name) (pw->s_attr & (1 << name))
#define pw_set_attr(pw, name) (pw->s_attr |= 1 << name)
#define pw_clear_attr(pw, name) (pw->s_attr &= ~(1 << name))

static void hcontainer_remove_widget(HContainer *p_container, HWidget *p_widget)
{
	vector_remove_data(p_container->p_children, p_widget);
}


static HWidget* hcontainer_child_at(HContainer *p_container, short s_x, short s_y)
{
	HWidget *pw;
	VectorNode *pn;
	//plist = p_container->p_layout ? p_container->p_layout->p_widget_list : p_container->p_children;
	vector_for_each(pn, p_container->p_children) {
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
	vector_clear(p_container->p_children);
	pw = (HWidget *)p_container;
	pw_clear_attr(pw, ATTR_CONTAINER_CALC_FLAG);
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

static HRect calc_children_preffered_size(HContainer *pc)
{
	HWidget *pw;
	VectorNode *pn;
	HRect rect = {0};
	if (pc->p_layout) {
		pw = (HWidget *)pc;
		pc->p_layout->s_max_width = pw->p_widget_ops->get_max_width(pw);
		pc->p_layout->s_max_height = pw->p_widget_ops->get_max_height(pw);
		pc->p_layout->p_ops->validate_layout(pc->p_layout, &rect);
	} else {
		vector_for_each(pn, pc->p_children) {
			pw = (HWidget *)pn->pv_data;
			pw->p_widget_ops->get_width(pw);
			pw->p_widget_ops->get_height(pw);
			if (pw->p_widget_ops->is_visible(pw)) {
				rect.s_width = (rect.s_width > pw->s_width + pw->s_top_x) ? rect.s_width : pw->s_width + pw->s_top_x;
				rect.s_height = (rect.s_height > pw->s_height + pw->s_top_y) ? rect.s_height : pw->s_height + pw->s_top_y;
			}
		}
	}
	return rect;
}

static void calc_preffered_size(HContainer *pc)
{
	HWidget *pw = (HWidget *)pc;
	HRect rect = calc_children_preffered_size(pc);
	rect.s_width += pw->s_padding_left + pw->s_padding_right;
	rect.s_height += pw->s_padding_top + pw->s_padding_bottom;
	pw->s_prefered_width = rect.s_width;
	pw->s_prefered_height = rect.s_height;
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

static short hcontainer_get_width(HWidget *p_widget)
{
	if (p_widget->s_width == 0) {
		p_widget->s_width = p_widget->p_widget_ops->get_prefered_width(p_widget);
	} else if (pw_get_attr(p_widget, ATTR_WIDTH_FLAG) && !pw_get_attr(p_widget, ATTR_CONTAINER_CALC_FLAG)) {
		pw_set_attr(p_widget, ATTR_CONTAINER_CALC_FLAG);
		calc_children_preffered_size((HContainer *)p_widget);
	}
	return p_widget->s_width;
}

static short hcontainer_get_height(HWidget *p_widget)
{
	if (p_widget->s_height == 0) {
		p_widget->s_height = p_widget->p_widget_ops->get_prefered_height(p_widget);
	} else if (pw_get_attr(p_widget, ATTR_HEIGHT_FLAG) && !pw_get_attr(p_widget, ATTR_CONTAINER_CALC_FLAG)) {
		pw_set_attr(p_widget, ATTR_CONTAINER_CALC_FLAG);
		calc_children_preffered_size((HContainer *)p_widget);
	}
	return p_widget->s_height;
}

static void hcontainer_set_bgimg(HContainer *p_container, HImage *p_img)
{
	p_container->p_bgimg = p_img;
}

static void hcontainer_set_layout(HContainer *p_container, HLayout *p_layout)
{
	VectorNode *p_pos;
	HWidget *p_widget;
	if (p_layout) {
		if (!p_container->p_layout)
			vector_delete(p_container->p_children);
		//TODO: set layout's widgets parent
		p_container->p_children = p_layout->p_ops->get_widget_list(p_layout);
		vector_for_each(p_pos, p_container->p_children) {
			p_widget = (HWidget *)p_pos->pv_data;
			p_widget->p_parent = p_container;
		}
	} else {
		if (p_container->p_layout)
			p_container->p_children = hwidget_new_vector();
	}
	p_container->p_layout = p_layout;
}

static void hcontainer_paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	HWidget *pw;
	HContainer *pc;
	VectorNode *pn;
	short s_w;
	short s_h;
	short s_tmp_x;
	short s_tmp_y;
	HRect old_clip = window->cur_clip_rect;
	HRect cur_clip;
	pc = (HContainer *)p_widget;

	//paint container self
	s_w = p_widget->p_widget_ops->get_max_width(p_widget) > p_widget->p_widget_ops->get_width(p_widget) ?
		p_widget->s_width : p_widget->s_max_width;
	s_h = p_widget->p_widget_ops->get_max_height(p_widget) > p_widget->p_widget_ops->get_height(p_widget) ?
		p_widget->s_height : p_widget->s_max_height;
	//TODO: clip  the container rect(s_screen_x, s_screen_y, s_w, s_h)
	cur_clip.s_x = s_screen_x;
	cur_clip.s_y = s_screen_y;
	cur_clip.s_width = s_w;
	cur_clip.s_height = s_h;
	cur_clip = calc_intersect_clip(&window->cur_clip_rect, &cur_clip);
	vm_graphic_set_layer_clip(i_handle, cur_clip.s_x, cur_clip.s_y, cur_clip.s_x + cur_clip.s_width, cur_clip.s_y + cur_clip.s_height);
	//restore the container's clip
	window->cur_clip_rect = cur_clip;

	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget)) {
		//TODO: fill the container rect(s_screen_x, s_screen_y, s_w, s_h) with bgcolor
		vm_graphic_fill_rect(vm_graphic_get_layer_buffer(i_handle), s_screen_x, s_screen_y, s_w, s_h, (VMINT16)p_widget->i_bgcolor, (VMINT16)p_widget->i_bgcolor);
	}
	//paint the background image
	if (pc->p_bgimg)
		pc->p_bgimg->p_oper->paint(pc->p_bgimg, i_handle, s_screen_x, s_screen_y);
	//paint children
//	plist = pc->p_layout ? pc->p_layout->p_widget_list : pc->p_children;
	vector_for_each(pn, pc->p_children) {
		pw = (HWidget *)pn->pv_data;
		if (!pw->p_widget_ops->is_visible(pw))
			continue;
		s_tmp_x = s_screen_x + p_widget->s_padding_left + pw->s_top_x + pc->s_translate_x;
		s_tmp_y = s_screen_y + p_widget->s_padding_top + pw->s_top_y + pc->s_translate_y;
		if (s_tmp_y + pw->s_height >= _max_(s_screen_y, 0) && s_tmp_y <= _min_(s_screen_y + s_h, window->s_screen_height)) {
			pw->p_widget_ops->paint(pw, i_handle, s_tmp_x, s_tmp_y);
		}
	}
	window->cur_clip_rect = old_clip;
}

static void hcontainer_notify_size_changed(HWidget *p_widget)
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
	//TODO: here delete layout
	if (p_container->p_layout) {
		p_container->p_layout->p_ops->delete_layout(p_container->p_layout);
	} else {
		vector_delete(p_container->p_children);
	}
}


static void hcontainer_create_hwidget_ops()
{
	gp_hwidget_ops = vm_malloc(sizeof(HWidgetOperation));
	hwidget_operation_init(gp_hwidget_ops);
	gp_hwidget_ops->get_prefered_width = hcontainer_get_prefered_width;
	gp_hwidget_ops->get_prefered_height = hcontainer_get_prefered_height;
	gp_hwidget_ops->get_width = hcontainer_get_width;
	gp_hwidget_ops->get_height = hcontainer_get_height;
	gp_hwidget_ops->paint = hcontainer_paint;
	gp_hwidget_ops->notify_size_changed = hcontainer_notify_size_changed;
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
	gp_container_ops->set_bgimg = hcontainer_set_bgimg;
	gp_container_ops->set_layout = hcontainer_set_layout;
}

void hcontainer_ops_delete()
{
	if (gp_hwidget_ops)
		vm_free(gp_hwidget_ops);
	if (gp_container_ops)
		vm_free(gp_container_ops);
}