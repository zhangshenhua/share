/********************************************************
*file: hplane.c
*author: yan.gao
*date: 2011.08.23
*description: a plane, the root container
********************************************************/

#include "hplane.h"
#include "hcontainer.h"
#include "string.h"
#include "vmsys.h"
#include "vmgraph.h"

#include "../WindowLayer/hwindow.h"


/*global instance*/
static HWidgetOperation *gp_hwidget_ops;
static HContainerOperation *gp_container_ops;
static HPlaneOperation *gp_plane_ops;

static void create_hcontainer_ops();
static void create_hwidget_ops();
static void create_plane_ops();
static void set_enable_transparent_bg(HPlane *p_plane, int i_enable);


HPlane * hplane_new()
{
	HPlane *ph;
	ph = (HPlane *)vm_malloc(sizeof(HPlane));
	memset(ph, 0, sizeof(HPlane));
	hcontainer_init((HContainer *)ph);
	if (gp_hwidget_ops == NULL)
		create_hwidget_ops(((HWidget *)ph)->p_widget_ops);
	if (gp_container_ops == NULL)
		create_hcontainer_ops(((HContainer *)ph)->p_container_ops);
	if (gp_plane_ops == NULL)
		create_plane_ops();
	((HWidget *)ph)->p_widget_ops = gp_hwidget_ops;
	((HContainer *)ph)->p_container_ops = gp_container_ops;
	ph->p_plane_ops = gp_plane_ops;
	//set max size for this plane
	((HWidget *)ph)->p_widget_ops->set_max_width((HWidget *)ph, vm_graphic_get_screen_width());
	((HWidget *)ph)->p_widget_ops->set_max_height((HWidget *)ph, vm_graphic_get_screen_height());
	// new ph->p_content
	ph->p_content = hcontainer_new();
<<<<<<< HEAD
	((HWidget *)ph->p_content)->p_parent = (HContainer *)ph;
=======
	((HWidget *)ph->p_content)->p_parent = ph;
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	hlist_append(((HContainer *)ph)->p_children, ph->p_content, NULL);
	((HWidget *)ph->p_content)->p_widget_ops->set_max_width((HWidget *)ph, vm_graphic_get_screen_width());
	((HWidget *)ph->p_content)->p_widget_ops->set_max_height((HWidget *)ph, vm_graphic_get_screen_height());
	ph->i_handle = vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),vm_graphic_get_screen_height(),-1);
#ifdef H_DEBUG
	((HWidget *)ph)->p_class_name = vm_malloc(20);
	memset(((HWidget *)ph)->p_class_name, 0, 20);
	strncpy(((HWidget *)ph)->p_class_name, "HPlane", 20);
#endif
	return ph;
}

static void reset_plane_child_height(HPlane *p_plane)
{
	short s_h = ((HWidget *)p_plane)->p_widget_ops->get_max_height((HWidget *)p_plane);
	if (p_plane->p_top_widget)
		s_h -= p_plane->p_top_widget->p_widget_ops->get_prefered_height(p_plane->p_top_widget);
	if (p_plane->p_bottom_widget)
		s_h -= p_plane->p_bottom_widget->p_widget_ops->get_prefered_height(p_plane->p_bottom_widget);
	((HWidget *)p_plane->p_content)->p_widget_ops->set_max_height((HWidget *)p_plane->p_content, s_h);
}

static void set_top_widget(HPlane *p_plane, HWidget *p_widget)
{
	if (p_plane->p_top_widget == NULL && p_widget == NULL)
		return;
	if (p_plane->p_top_widget == p_widget)
		return;

	if (p_plane->p_top_widget) {
		hlist_remove(((HContainer *)p_plane)->p_children, p_plane->p_top_widget);
		p_plane->p_top_widget->p_widget_ops->destroy(p_plane->p_top_widget);
	}
	p_plane->p_top_widget = p_widget;
	if (p_widget)
		hlist_insert(((HContainer *)p_plane)->p_children, p_widget, 0, NULL);
	reset_plane_child_height(p_plane);
}

static void set_bottom_widget(HPlane *p_plane, HWidget *p_widget)
{
	if (p_plane->p_bottom_widget == NULL && p_widget == NULL)
		return;
	if (p_plane->p_bottom_widget == p_widget)
		return;
	if (p_plane->p_bottom_widget) {
		hlist_remove(((HContainer *)p_plane)->p_children, p_plane->p_bottom_widget);
		p_plane->p_top_widget->p_widget_ops->destroy(p_plane->p_top_widget);
	}
	p_plane->p_bottom_widget = p_widget;
	if (p_widget)
		hlist_append(((HContainer *)p_plane)->p_children, p_widget, NULL);
	reset_plane_child_height(p_plane);
}

static short get_max_width(HWidget *p_widget)
{
	return p_widget->s_max_width - p_widget->uc_padding_left - p_widget->uc_padding_right;
}

static short get_max_height(HWidget *p_widget)
{
	return p_widget->s_max_height - p_widget->uc_padding_top - p_widget->uc_padding_bottom;
}

static HClass get_class(HWidget *p_widget)
{
	return CLASS_PLANE;
}

static int is_plane(HWidget *p_widget)
{
	return 1;
}

void hplane_delete(HWidget *p_widget)
{
	int i_idx;
	for (i_idx = 0; i_idx < sizeof(window->p_planes) / sizeof(HPlane *); i_idx ++) {
		if (window->p_planes[i_idx] == (HPlane *)p_widget) {
			window->p_planes[i_idx] = NULL;
			break;
		}
	}
	vm_graphic_delete_layer(((HPlane *)p_widget)->i_handle);
	hcontainer_delete(p_widget);
}

static void add_widget(HPlane *p_plane, HWidget *p_widget)
{
	HContainer *p_cnt = p_plane->p_content;
	p_cnt->p_container_ops->add_widget(p_cnt, p_widget);
}

static void plane_validate(HWidget *p_widget)
{
	HPlane *p_plane = (HPlane *)p_widget;
	((HWidget *)p_plane->p_content)->p_widget_ops->validate((HWidget *)p_plane->p_content);
}

static void set_enable_transparent_bg(HPlane *p_plane, int i_enable)
{
	p_plane->i_enable_transparent_bg = i_enable;
}

<<<<<<< HEAD
/************************************************************************
* 
************************************************************************/
static HWidget *p_cur_press_widget;
static HPoint old_widget_pos;
static HPoint old_screen_pos;
static char has_been_moved_out;

static HWidget *child_at_recursive(HContainer *p_c, short s_x, short s_y)
{
	HWidget *pw;
	hlist_node_t *pn;
	s_x -= ((HWidget *)p_c)->uc_padding_left;
	s_x -= ((HWidget *)p_c)->uc_padding_top;
	s_x += p_c->s_translate_x;
	s_y += p_c->s_translate_y;
	hlist_for_each(pn, p_c->p_children) {
		pw = (HWidget *)pn->pv_data;
		if (pw->s_top_x <= s_x && pw->s_top_y <= s_y && 
			pw->s_top_x + pw->s_width >= s_x && pw->s_top_y + pw->s_height >= s_y) {
			if (pw->p_widget_ops->is_container(pw)) {
				return child_at_recursive((HContainer *)pw, s_x - pw->s_top_x, s_y - pw->s_top_y);
			} else {
				return pw;
			}
		}
	}
	return (HWidget *)p_c;
}


static HPoint get_widget_point(HWidget *p_widget, short s_screen_x, short s_screen_y)
{
	HPoint pos;
	HContainer *pc;
	for (pc = p_widget->p_parent; pc; p_widget = (HWidget *)pc, pc = p_widget->p_parent) {
		s_screen_x -= p_widget->s_top_x - ((HWidget *)pc)->uc_padding_left;
		s_screen_y -= p_widget->s_top_y - ((HWidget *)pc)->uc_padding_top;
		s_screen_x -= pc->s_translate_x;
		s_screen_y -= pc->s_translate_y;
	}
	pos.s_x = s_screen_x;
	pos.s_y = s_screen_y;
	return pos;
}

static void try_scroll(short s_dx, short s_dy)
{
	HContainer *pc;
	if (p_cur_press_widget->p_widget_ops->is_container(p_cur_press_widget))
		pc = (HContainer *)p_cur_press_widget;
	else 
		pc = p_cur_press_widget->p_parent;
	for (; pc; pc = ((HWidget *)pc)->p_parent) {
		if (pc->p_container_ops->can_scroll(pc)) {
			pc->p_container_ops->scroll(pc, s_dx, s_dy);
			break;
		}
	}
}

static void hanle_pen_press(HWidget *p_widget, short s_x, short s_y)
{
	p_cur_press_widget = child_at_recursive((HContainer *)p_widget, s_x, s_y);
	if (p_cur_press_widget->p_widget_ops->is_plane(p_cur_press_widget)) {
		p_cur_press_widget = NULL;
		return;
	}
	old_widget_pos = get_widget_point(p_cur_press_widget, s_x, s_y);
	old_screen_pos.s_x = s_x;
	old_screen_pos.s_y = s_y;
	p_cur_press_widget->p_widget_ops->pen_press(p_cur_press_widget, old_widget_pos.s_x, old_widget_pos.s_y);
}

static void hanle_pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HWidget *p_wid;
	HWidget *p_old_focus_widget = ((HPlane *)p_widget)->p_own_focus_widget;
	if (p_cur_press_widget == NULL)
		return;
	p_wid = child_at_recursive((HContainer *)p_widget, s_x, s_y);
	if (p_wid == p_cur_press_widget) {
		old_widget_pos = get_widget_point(p_cur_press_widget, s_x, s_y);
		p_cur_press_widget->p_widget_ops->pen_release(p_cur_press_widget, old_widget_pos.s_x, old_widget_pos.s_y);
		if (p_old_focus_widget && p_old_focus_widget != p_cur_press_widget && p_old_focus_widget->p_widget_ops->is_enable_focus(p_old_focus_widget)) {
			//lost focus, and repaint it
			p_old_focus_widget->p_widget_ops->set_focus(p_old_focus_widget, 0);
			p_old_focus_widget->p_widget_ops->repaint(p_old_focus_widget);
		}
		((HPlane *)p_widget)->p_own_focus_widget = p_cur_press_widget;
		p_cur_press_widget = NULL;
	}
}

static void hanle_pen_move(HWidget *p_widget, short s_x, short s_y)
{
	short s_dx = s_x - old_screen_pos.s_x;
	short s_dy = s_y - old_screen_pos.s_y;
	old_screen_pos.s_x = s_x;
	old_widget_pos.s_y = s_y;
	if (p_cur_press_widget == NULL)
		return;
	old_widget_pos = get_widget_point(p_cur_press_widget, s_x, s_y);
	if (!p_cur_press_widget->p_widget_ops->is_enable_focus(p_cur_press_widget) && !p_cur_press_widget->p_widget_ops->is_enable_drag(p_cur_press_widget)) {
		try_scroll(s_dx, s_dy);
		return;
	}
	if (p_cur_press_widget->s_top_x <= s_x &&
		p_cur_press_widget->s_top_y <= s_y &&
		p_cur_press_widget->s_width >= old_widget_pos.s_x &&
		p_cur_press_widget->s_height >= old_widget_pos.s_y) {
		if (has_been_moved_out) {
			has_been_moved_out = 0;
			p_cur_press_widget->p_widget_ops->pen_enter(p_cur_press_widget, old_widget_pos.s_x, old_widget_pos.s_y);
		} else {
			p_cur_press_widget->p_widget_ops->pen_move(p_cur_press_widget, old_widget_pos.s_x, old_widget_pos.s_y);
		}
	} else {
		if (has_been_moved_out) {
			//ignore
		} else {
			has_been_moved_out = 1;
			p_cur_press_widget->p_widget_ops->pen_leave(p_cur_press_widget, old_widget_pos.s_x, old_widget_pos.s_y);
		}
	}
}

=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
static void create_hwidget_ops(HWidgetOperation *p_widget_ops_prototype)
{
	gp_hwidget_ops = vm_malloc(sizeof(HWidgetOperation));
	memcpy(gp_hwidget_ops, p_widget_ops_prototype, sizeof(HWidgetOperation));
	gp_hwidget_ops->get_max_width = get_max_width;
	gp_hwidget_ops->get_max_height = get_max_height;
	gp_hwidget_ops->is_plane = is_plane;
	gp_hwidget_ops->get_class = get_class;
	gp_hwidget_ops->destroy = hplane_delete;
	gp_hwidget_ops->validate = plane_validate;
<<<<<<< HEAD
	gp_hwidget_ops->pen_press = hanle_pen_press;
	gp_hwidget_ops->pen_move = hanle_pen_move;
	gp_hwidget_ops->pen_release = hanle_pen_release;
=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
}

static void create_hcontainer_ops(HContainerOperation *p_container_ops_prototype)
{
	gp_container_ops = vm_malloc(sizeof(HContainerOperation));
	memcpy(gp_container_ops, p_container_ops_prototype, sizeof(HContainerOperation));
}

static void create_plane_ops()
{
	gp_plane_ops = vm_malloc(sizeof(HPlaneOperation));
	gp_plane_ops->set_bottom_widget = set_bottom_widget;
	gp_plane_ops->set_top_widget = set_top_widget;
	gp_plane_ops->set_enable_transparent_bg = set_enable_transparent_bg;
	gp_plane_ops->add_widget = add_widget;
}