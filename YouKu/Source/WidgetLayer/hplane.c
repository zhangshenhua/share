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
#include "vmio.h"

#include "../WindowLayer/hwindow.h"
#include "vmlog.h"


/*global instance*/
static HWidgetOperation *gp_hwidget_ops;
static HContainerOperation *gp_container_ops;
static HPlaneOperation *gp_plane_ops;
static void (*hcontainer_paint_PF)(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y);

static void hplane_create_hcontainer_ops();
static void hplane_create_hwidget_ops();
static void hplane_create_plane_ops();


HPlane * hplane_new()
{
	HPlane *ph;
	HWidget *pw;
	short s_screen_width = (short)vm_graphic_get_screen_width();
	short s_screen_height = (short)vm_graphic_get_screen_height();
	ph = (HPlane *)vm_malloc(sizeof(HPlane));
	pw = (HWidget *)ph;

	memset(ph, 0, sizeof(HPlane));
	hcontainer_init((HContainer *)ph);
	if (gp_hwidget_ops == NULL)
		hplane_create_hwidget_ops(pw->p_widget_ops);
	if (gp_container_ops == NULL)
		hplane_create_hcontainer_ops(((HContainer *)ph)->p_container_ops);
	if (gp_plane_ops == NULL)
		hplane_create_plane_ops();
	pw->p_widget_ops = gp_hwidget_ops;
	((HContainer *)ph)->p_container_ops = gp_container_ops;
	ph->p_plane_ops = gp_plane_ops;
	//set max size/size for this plane
	pw->p_widget_ops->set_max_width(pw, s_screen_width);
	pw->p_widget_ops->set_max_height(pw, s_screen_height);
	pw->p_widget_ops->set_width(pw, s_screen_width);
	pw->p_widget_ops->set_height(pw, s_screen_height);
	pw->s_prefered_width = s_screen_width;
	pw->s_prefered_height = s_screen_height;
	// new ph->p_content
	ph->p_content = hcontainer_new();
	((HWidget *)ph->p_content)->p_parent = (HContainer *)ph;
	vector_append_data(((HContainer *)ph)->p_children, ph->p_content);
// 	pw = (HWidget *)ph->p_content;
// 	pw->p_widget_ops->set_max_width(pw, s_screen_width);
// 	pw->p_widget_ops->set_max_height(pw, s_screen_height);
	ph->i_handle = -1;
	
#ifdef H_DEBUG
	pw = (HWidget *)ph;
	pw->p_class_name = vm_malloc(20);
	memset(pw->p_class_name, 0, 20);
	strncpy(pw->p_class_name, "HPlane", 20);
#endif
	return ph;
}

static void reset_plane_child_height(HPlane *p_plane)
{
	short s_tmp_h1 = 0;
	short s_tmp_h2 = 0;
	if (p_plane->p_top_widget) {
		p_plane->p_top_widget->p_widget_ops->validate(p_plane->p_top_widget);
		s_tmp_h1 = p_plane->p_top_widget->s_height;
		if (p_plane->p_top_widget->s_width< window->s_screen_width)
			p_plane->p_top_widget->s_width = window->s_screen_width;
	}
	if (p_plane->p_bottom_widget) {
		p_plane->p_bottom_widget->p_widget_ops->validate(p_plane->p_bottom_widget);
		s_tmp_h2 = p_plane->p_bottom_widget->s_height;
		p_plane->p_bottom_widget->s_top_y = window->s_screen_height - s_tmp_h2;
		if (p_plane->p_bottom_widget->s_width < window->s_screen_width)
			p_plane->p_bottom_widget->s_width = window->s_screen_width;
	}
	((HWidget *)p_plane->p_content)->s_top_y = s_tmp_h1;
	((HWidget *)p_plane->p_content)->p_widget_ops->set_max_height((HWidget *)p_plane->p_content, window->s_screen_height - s_tmp_h1 - s_tmp_h2);
}

static void set_top_widget(HPlane *p_plane, HWidget *p_widget)
{
	if (p_plane->p_top_widget == NULL && p_widget == NULL)
		return;
	if (p_plane->p_top_widget == p_widget)
		return;

	if (p_plane->p_top_widget) {
		vector_remove_data(p_plane->base.p_children, p_plane->p_top_widget);
	}
	p_plane->p_top_widget = p_widget;
	if (p_widget) {
		vector_insert_data(((HContainer *)p_plane)->p_children, 0, p_widget);
		p_widget->p_parent = (HContainer *)p_plane;
		p_widget->s_top_x = p_widget->s_top_y = 0;
	}
	reset_plane_child_height(p_plane);
}

static void set_bottom_widget(HPlane *p_plane, HWidget *p_widget)
{
	if (p_plane->p_bottom_widget == NULL && p_widget == NULL)
		return;
	if (p_plane->p_bottom_widget == p_widget)
		return;
	if (p_plane->p_bottom_widget) {
		vector_remove_data(p_plane->base.p_children, p_plane->p_bottom_widget);
	}
	p_plane->p_bottom_widget = p_widget;
	if (p_widget) {
		vector_append_data(((HContainer *)p_plane)->p_children, p_widget);
		p_widget->p_parent = (HContainer *)p_plane;
	}
	reset_plane_child_height(p_plane);
}

static short hplane_get_max_width(HWidget *p_widget)
{
	return p_widget->s_max_width;
}

static short hplane_get_max_height(HWidget *p_widget)
{
	return p_widget->s_max_height;
}

static HClass hplane_get_class(HWidget *p_widget)
{
	return CLASS_PLANE;
}

static int hplane_is_plane(HWidget *p_widget)
{
	return 1;
}

void hplane_delete(HPlane *p_plane)
{
	int i_idx;
	for (i_idx = 0; i_idx < sizeof(window->p_planes) / sizeof(HPlane *); i_idx ++) {
		if (window->p_planes[i_idx] == p_plane) {
			window->p_planes[i_idx] = NULL;
			break;
		}
	}
	vm_graphic_delete_layer(p_plane->i_handle);
	hcontainer_delete((HContainer *)p_plane);
}

static void hplane_add_widget(HPlane *p_plane, HWidget *p_widget)
{
	HContainer *p_cnt = p_plane->p_content;
	p_cnt->p_container_ops->add_widget(p_cnt, p_widget);
}

static void hplane_reset_content(HContainer *p_cnt)
{
	HWidget *p_widget = (HWidget *)p_cnt;
	p_widget->p_widget_ops->set_max_width(p_widget, 0);
	p_widget->p_widget_ops->set_max_height(p_widget, 0);
	p_widget->p_widget_ops->set_width(p_widget, 0);
	p_widget->p_widget_ops->set_height(p_widget, 0);
	p_widget->s_prefered_width = 0;
	p_widget->s_prefered_height = 0;
}

static void hplane_remove_widget(HPlane *p_plane, HWidget *p_widget)
{
	HContainer *p_cnt = p_plane->p_content;
	if (p_widget == p_plane->p_own_focus_widget)
		p_plane->p_own_focus_widget = NULL;
	p_cnt->p_container_ops->remove_widget(p_cnt, p_widget);
	if (p_cnt->p_children->i_size == 0) {
		//there have no child widget any more
		hplane_reset_content(p_cnt);
	}
}


static void hplane_remove_all(HPlane *p_plane)
{
	HContainer *p_cnt = p_plane->p_content;
	p_cnt->p_container_ops->remove_all(p_cnt);
	hplane_reset_content(p_cnt);
	p_plane->p_own_focus_widget = NULL;
	set_top_widget(p_plane, NULL);
	set_bottom_widget(p_plane, NULL);
}

static void hplane_validate(HWidget *p_widget)
{
	HPlane *p_plane = (HPlane *)p_widget;
	short s_mw = ((HWidget *)p_plane->p_content)->p_widget_ops->get_max_width((HWidget *)p_plane->p_content);
	((HWidget *)p_plane->p_content)->p_widget_ops->validate((HWidget *)p_plane->p_content);

	if (p_plane->p_content->base.s_width < s_mw) {
		((HWidget *)p_plane->p_content)->s_width = s_mw;
	}
}

static void hplane_add_key_down(HPlane *p_plane, HWidget *p_from_widget, HWidget *p_to_widget)
{
	KeyFocusNode *pn = (KeyFocusNode *)vm_malloc(sizeof(KeyFocusNode));;
	pn->p_from_widget = p_from_widget;
	pn->p_to_widget = p_to_widget;
	if (p_plane->p_key_down_head == NULL) {
		pn->p_next = NULL;
		p_plane->p_key_down_head = pn;
	} else {
		pn->p_next = p_plane->p_key_down_head->p_next;
		p_plane->p_key_down_head->p_next = pn;
	}
}

static void hplane_add_key_right(HPlane *p_plane, HWidget *p_from_widget, HWidget *p_to_widget)
{
	KeyFocusNode *pn = (KeyFocusNode *)vm_malloc(sizeof(KeyFocusNode));;
	pn->p_from_widget = p_from_widget;
	pn->p_to_widget = p_to_widget;
	pn->p_next = NULL;
	if (p_plane->p_key_right_head == NULL) {
		p_plane->p_key_right_head = pn;
	} else {
		pn->p_next = p_plane->p_key_right_head->p_next;
		p_plane->p_key_right_head->p_next = pn;
	}
}


/************************************************************************
* 
************************************************************************/
static HWidget *p_cur_press_widget;
static HPoint old_widget_pos;
static HPoint old_screen_pos;
static char has_been_moved_out;

#define  _max_(a,b) (a > b ? a : b)
#define  _min_(a,b) (a < b ? a : b)

static HWidget *child_at_recursive(HContainer *p_c, short s_x, short s_y)
{
	HWidget *pw;
	VectorNode *pn;
	short s_mw, s_mh;
	s_x -= ((HWidget *)p_c)->s_padding_left;
	s_y -= ((HWidget *)p_c)->s_padding_top;
	s_x -= p_c->s_translate_x;
	s_y -= p_c->s_translate_y;
//	plist = p_c->p_layout ? p_c->p_layout->p_widget_list : p_c->p_children;
	vector_for_each(pn, p_c->p_children) {
		pw = (HWidget *)pn->pv_data;
		s_mw = pw->p_widget_ops->get_max_width(pw);
		s_mh = pw->p_widget_ops->get_max_height(pw);
		if (pw->s_top_x <= s_x && pw->s_top_y <= s_y && 
			pw->s_top_x + _min_(pw->s_width, s_mw) >= s_x && pw->s_top_y + _min_(pw->s_height, s_mh) >= s_y) {
			if (pw->p_widget_ops->is_container(pw)) {
				return child_at_recursive((HContainer *)pw, s_x - pw->s_top_x, s_y - pw->s_top_y);
			} else {
				return pw;
			}
		}
	}
	return (HWidget *)p_c;
}


static HPoint point_at_widget(HWidget *p_widget, short s_screen_x, short s_screen_y)
{
	HPoint pos;
	HContainer *pc;
	for (pc = p_widget->p_parent; pc; p_widget = (HWidget *)pc, pc = p_widget->p_parent) {
		s_screen_x -= p_widget->s_top_x - ((HWidget *)pc)->s_padding_left;
		s_screen_y -= p_widget->s_top_y - ((HWidget *)pc)->s_padding_top;
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

static void hplane_pen_press(HWidget *p_widget, short s_x, short s_y)
{
	p_cur_press_widget = child_at_recursive((HContainer *)p_widget, s_x, s_y);
	if (p_cur_press_widget->p_widget_ops->is_plane(p_cur_press_widget)) {
		p_cur_press_widget = NULL;
		return;
	}
	old_widget_pos = point_at_widget(p_cur_press_widget, s_x, s_y);
	old_screen_pos.s_x = s_x;
	old_screen_pos.s_y = s_y;
	p_cur_press_widget->p_widget_ops->pen_press(p_cur_press_widget, old_widget_pos.s_x, old_widget_pos.s_y);
}

static void hplane_pen_double_click(HWidget *p_widget, short s_x, short s_y)
{
	HWidget *pw;
	HPoint pos;
	pw = child_at_recursive((HContainer *)p_widget, s_x, s_y);
	if (pw->p_widget_ops->is_plane(pw)) {
		pw = NULL;
		return;
	}
	pos = point_at_widget(pw, s_x, s_y);
	pw->p_widget_ops->pen_double_click(pw, pos.s_x, pos.s_y);
}

static void hplane_pen_long_tap(HWidget *p_widget, short s_x, short s_y)
{
	HWidget *pw;
	HPoint pos;
	pw = child_at_recursive((HContainer *)p_widget, s_x, s_y);
	if (pw->p_widget_ops->is_plane(pw)) {
		pw = NULL;
		return;
	}
	pos = point_at_widget(pw, s_x, s_y);
	pw->p_widget_ops->pen_long_tap(pw, pos.s_x, pos.s_y);
}

static void hplane_pen_release(HWidget *p_widget, short s_x, short s_y)
{
	HWidget *p_wid;
	HWidget *p_old_focus_widget = ((HPlane *)p_widget)->p_own_focus_widget;
	if (p_cur_press_widget == NULL)
		return;
	p_wid = child_at_recursive((HContainer *)p_widget, s_x, s_y);
	if (p_wid == p_cur_press_widget) {
		if (p_old_focus_widget &&
			p_old_focus_widget != p_cur_press_widget &&
			p_old_focus_widget->p_widget_ops->is_enable_focus(p_old_focus_widget) &&
			p_cur_press_widget->p_widget_ops->is_enable_focus(p_cur_press_widget)
			) {
			//lost focus, and repaint it
			p_old_focus_widget->p_widget_ops->set_focus(p_old_focus_widget, 0);
			if (p_old_focus_widget->p_widget_ops->focus_changed)
				p_old_focus_widget->p_widget_ops->focus_changed(p_old_focus_widget, 0);
			p_old_focus_widget->p_widget_ops->repaint(p_old_focus_widget);
		}
		old_widget_pos = point_at_widget(p_cur_press_widget, s_x, s_y);
		if (p_cur_press_widget->p_widget_ops->is_enable_focus(p_cur_press_widget)) {
			((HPlane *)p_widget)->p_own_focus_widget = p_cur_press_widget;
			//gain focus
			p_cur_press_widget->p_widget_ops->set_focus(p_cur_press_widget, 1);
			if (p_cur_press_widget->p_widget_ops->focus_changed)
				p_cur_press_widget->p_widget_ops->focus_changed(p_cur_press_widget, 1);
		}
		p_cur_press_widget->p_widget_ops->pen_release(p_cur_press_widget, old_widget_pos.s_x, old_widget_pos.s_y);
		p_cur_press_widget = NULL;
	}
}

static void hplane_pen_move(HWidget *p_widget, short s_x, short s_y)
{
	short s_dx = s_x - old_screen_pos.s_x;
	short s_dy = s_y - old_screen_pos.s_y;
	old_screen_pos.s_x = s_x;
	old_screen_pos.s_y = s_y;

	if (p_cur_press_widget == NULL)
		return;
	old_widget_pos = point_at_widget(p_cur_press_widget, s_x, s_y);
	if ((!p_cur_press_widget->p_widget_ops->is_enable_focus(p_cur_press_widget) &&
		!p_cur_press_widget->p_widget_ops->is_enable_drag(p_cur_press_widget)) ||
		!p_cur_press_widget->p_widget_ops->is_enable(p_cur_press_widget)
		) {
		try_scroll(s_dx, s_dy);
		return;
	}
	if (0 <= old_widget_pos.s_x &&
		0 <= old_widget_pos.s_y &&
		_min_(p_cur_press_widget->s_width, p_cur_press_widget->s_max_width) >= old_widget_pos.s_x &&
		_min_(p_cur_press_widget->s_height, p_cur_press_widget->s_max_height) >= old_widget_pos.s_y) {
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

static HWidget * find_next(HContainer *p_con)
{
}

static HWidget * find_next_enable_focus_widget(HPlane *p_plane, int i_dir)
{
	KeyFocusNode *p_node = NULL;
	HWidget *pw = NULL, *pw2;
	HContainer *pc;
	VectorNode *pn;
	short s_dx1, s_dy1, s_dx2, s_dy2;
	if (i_dir == VM_KEY_DOWN || i_dir == VM_KEY_UP)
		p_node = p_plane->p_key_down_head;
	else
		p_node = p_plane->p_key_right_head;
	for (; p_node; p_node = p_node->p_next) {
		if (i_dir == VM_KEY_DOWN || i_dir == VM_KEY_RIGHT) {
			if (p_plane->p_own_focus_widget == p_node->p_from_widget) {
				pw = p_node->p_from_widget;
				break;
			}
		} else {
			if (p_plane->p_own_focus_widget == p_node->p_to_widget) {
				pw = p_node->p_to_widget;
				break;
			}
		}
	}
	//find it
	if (pw)
		return pw;
#define _abs_(a) (a>0? a : -a)
	//not find then ,find by relative position int the same container
	pc = p_plane->p_own_focus_widget->p_parent;
//	plist = pc->p_layout ? pc->p_layout->p_widget_list : pc->p_children;
	vector_for_each(pn, pc->p_children) {
		pw2 = (HWidget *)pn->pv_data;
		if (!pw2->p_widget_ops->is_visible(pw2) || !pw2->p_widget_ops->is_enable_focus(pw2))
			continue;
		switch (i_dir) {
			case VM_KEY_DOWN:
				if (pw2->s_top_y - p_plane->p_own_focus_widget->s_top_y > 0) {
					if (pw) {
						s_dx1 = _abs_(pw->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy1 = _abs_(pw->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						s_dx2 = _abs_(pw2->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy2 = _abs_(pw2->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						if (s_dx1 + s_dy1 > s_dx2 + s_dy2)
							pw = pw2;
					} else {
						pw = pw2;
					}
				}
				break;

			case VM_KEY_RIGHT:
				if (pw2->s_top_x - p_plane->p_own_focus_widget->s_top_x > 0) {
					if (pw) {
						s_dx1 = _abs_(pw->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy1 = _abs_(pw->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						s_dx2 = _abs_(pw2->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy2 = _abs_(pw2->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						if (s_dy1 >= s_dy2 && s_dx1 > s_dx2)
							pw = pw2;
					} else {
						pw = pw2;
					}
				}
				break;

			case VM_KEY_UP:
				if (pw2->s_top_y - p_plane->p_own_focus_widget->s_top_y < 0) {
					if (pw) {
						s_dx1 = _abs_(pw->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy1 = _abs_(pw->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						s_dx2 = _abs_(pw2->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy2 = _abs_(pw2->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						if (s_dx1 + s_dy1 > s_dx2 + s_dy2)
							pw = pw2;
					} else {
						pw = pw2;
					}
				}
				break;

			case VM_KEY_LEFT:
				if (pw2->s_top_x - p_plane->p_own_focus_widget->s_top_x < 0) {
					if (pw) {
						s_dx1 = _abs_(pw->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy1 = _abs_(pw->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						s_dx2 = _abs_(pw2->s_top_x - p_plane->p_own_focus_widget->s_top_x);
						s_dy2 = _abs_(pw2->s_top_y - p_plane->p_own_focus_widget->s_top_y);
						if (s_dy1 <= s_dy2 && s_dx1 < s_dx2)
							pw = pw2;
					} else {
						pw = pw2;
					}
				}
				break;
		}
	}
	return pw;
}

static void hplane_key_press(HWidget *p_widget, int i_keycode)
{
	HWidget *pw;
	HPlane *p_plane = (HPlane *)p_widget;
	switch (i_keycode) {
		case VM_KEY_RIGHT:
		case VM_KEY_DOWN:
		case VM_KEY_LEFT:
		case VM_KEY_UP:
			if (p_plane->p_own_focus_widget->p_widget_ops->can_travel(p_plane->p_own_focus_widget, i_keycode)) {
				p_plane->p_own_focus_widget->p_widget_ops->key_press(p_plane->p_own_focus_widget, i_keycode);
			} else {
				pw = find_next_enable_focus_widget(p_plane, i_keycode);
				if (pw) {
					
					p_plane->p_own_focus_widget->p_widget_ops->set_focus(p_plane->p_own_focus_widget, 0);
					if (p_plane->p_own_focus_widget->p_widget_ops->focus_changed) {
						p_plane->p_own_focus_widget->p_widget_ops->focus_changed(p_plane->p_own_focus_widget, 0);
					}
					p_plane->p_own_focus_widget->p_widget_ops->repaint(p_plane->p_own_focus_widget);
					
					p_plane->p_own_focus_widget = pw;
					pw->p_widget_ops->set_focus(pw, 1);
					if (pw->p_widget_ops->focus_changed) {
						pw->p_widget_ops->focus_changed(pw, 1);
					}
					pw->p_widget_ops->repaint(pw);
				}
			}
			break;

		case VM_KEY_LEFT_SOFTKEY:
		case VM_KEY_RIGHT_SOFTKEY:
			if (p_plane->p_bottom_widget)
				p_plane->p_bottom_widget->p_widget_ops->key_press(p_plane->p_bottom_widget, i_keycode);
			break;

		default:
			p_plane->p_own_focus_widget->p_widget_ops->key_press(p_plane->p_own_focus_widget, i_keycode);
			break;
	}
}

static void hplane_key_release(HWidget *p_widget, int i_keycode)
{
	HPlane *p_plane = (HPlane *)p_widget;
	switch (i_keycode) {
		case VM_KEY_LEFT_SOFTKEY:
		case VM_KEY_RIGHT_SOFTKEY:
			if (p_plane->p_bottom_widget)
				p_plane->p_bottom_widget->p_widget_ops->key_release(p_plane->p_bottom_widget, i_keycode);
			break;

		default:
			p_plane->p_own_focus_widget->p_widget_ops->key_release(p_plane->p_own_focus_widget, i_keycode);
			break;
	}
}

static void hplane_key_long_press(HWidget *p_widget, int i_keycode)
{
	HPlane *p_plane = (HPlane *)p_widget;
	switch (i_keycode) {
		case VM_KEY_LEFT_SOFTKEY:
		case VM_KEY_RIGHT_SOFTKEY:
			if (p_plane->p_bottom_widget)
				p_plane->p_bottom_widget->p_widget_ops->key_long_press(p_plane->p_bottom_widget, i_keycode);
			break;

		default:
			p_plane->p_own_focus_widget->p_widget_ops->key_long_press(p_plane->p_own_focus_widget, i_keycode);
			break;
	}
}


static void hplane_key_repeat(HWidget *p_widget, int i_keycode)
{
	HPlane *p_plane = (HPlane *)p_widget;
	switch (i_keycode) {
		case VM_KEY_DOWN:
		case VM_KEY_UP:
		case VM_KEY_LEFT:
		case VM_KEY_RIGHT:
			if (p_plane->p_own_focus_widget->p_widget_ops->can_travel(p_plane->p_own_focus_widget, i_keycode)) {
				p_plane->p_own_focus_widget->p_widget_ops->key_repeat(p_plane->p_own_focus_widget, i_keycode);
			} else {
				hplane_key_press(p_widget, i_keycode);
			}
			break;

		case VM_KEY_LEFT_SOFTKEY:
		case VM_KEY_RIGHT_SOFTKEY:
			//ignore
			break;

		default:
			p_plane->p_own_focus_widget->p_widget_ops->key_repeat(p_plane->p_own_focus_widget, i_keycode);
			break;
	}
}


static void hplane_paint(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y)
{
	HPlane *p_plane = (HPlane *)p_widget;
	int i_idx, j_idx;
	for (i_idx = j_idx = 0; i_idx < sizeof(window->p_planes)/sizeof(HPlane *); i_idx ++) {
		if (p_plane == window->p_planes[i_idx]) {
			for (j_idx = i_idx - 1; j_idx >= 0; j_idx --) {
				if (window->p_planes[j_idx] && window->p_planes[j_idx]->i_enable_transparent_bg) {
					vm_graphic_fill_rect(vm_graphic_get_layer_buffer(i_handle), s_screen_x, s_screen_y,
						vm_graphic_get_screen_width(), vm_graphic_get_screen_height(),
						p_plane->s_transparent_color, p_plane->s_transparent_color);
				}
			}
			break;
		}
	}
	window->cur_clip_rect.s_x = 0;
	window->cur_clip_rect.s_y = 0;
	window->cur_clip_rect.s_width = window->s_screen_width;
	window->cur_clip_rect.s_height = window->s_screen_height;
	hcontainer_paint_PF(p_widget, i_handle, s_screen_x, s_screen_y);
}


static void hplane_set_visible(HWidget *p_widget, int i_visible)
{
	int i_idx;
	if (i_visible) {
		p_widget->s_attr |= (1 << ATTR_VISIBLE);
	} else {
		p_widget->s_attr &= ~(1 << ATTR_VISIBLE);
	}
	for (i_idx = ((HPlane *)p_widget)->s_plane_index - 1; i_idx >= 0; i_idx --) {
		if (window->p_planes[i_idx]) {
			window->p_planes[i_idx]->i_enable_transparent_bg = i_visible;
			break;
		}
	}
}

static void hplane_create_hwidget_ops(HWidgetOperation *p_widget_ops_prototype)
{
	gp_hwidget_ops = vm_malloc(sizeof(HWidgetOperation));
	memcpy(gp_hwidget_ops, p_widget_ops_prototype, sizeof(HWidgetOperation));
	gp_hwidget_ops->get_max_width = hplane_get_max_width;
	gp_hwidget_ops->get_max_height = hplane_get_max_height;
	gp_hwidget_ops->is_plane = hplane_is_plane;
	gp_hwidget_ops->get_class = hplane_get_class;
	gp_hwidget_ops->destroy = (void (*)(HWidget *))hplane_delete;
	gp_hwidget_ops->validate = hplane_validate;
	gp_hwidget_ops->pen_press = hplane_pen_press;
	gp_hwidget_ops->pen_move = hplane_pen_move;
	gp_hwidget_ops->pen_release = hplane_pen_release;
	gp_hwidget_ops->pen_double_click = hplane_pen_double_click;
	gp_hwidget_ops->pen_long_tap = hplane_pen_long_tap;
	gp_hwidget_ops->key_press = hplane_key_press;
	gp_hwidget_ops->key_release = hplane_key_release;
	gp_hwidget_ops->key_long_press = hplane_key_long_press;
	gp_hwidget_ops->key_repeat = hplane_key_repeat;
	gp_hwidget_ops->set_visible = hplane_set_visible;
	hcontainer_paint_PF = p_widget_ops_prototype->paint;
	gp_hwidget_ops->paint = hplane_paint;
}

static void hplane_create_hcontainer_ops(HContainerOperation *p_container_ops_prototype)
{
	gp_container_ops = vm_malloc(sizeof(HContainerOperation));
	memcpy(gp_container_ops, p_container_ops_prototype, sizeof(HContainerOperation));
}

static void hplane_create_plane_ops()
{
	gp_plane_ops = vm_malloc(sizeof(HPlaneOperation));
	gp_plane_ops->set_bottom_widget = set_bottom_widget;
	gp_plane_ops->set_top_widget = set_top_widget;
	gp_plane_ops->add_widget = hplane_add_widget;
	gp_plane_ops->remove_widget = hplane_remove_widget;
	gp_plane_ops->remove_all = hplane_remove_all;
	gp_plane_ops->add_key_down = hplane_add_key_down;
	gp_plane_ops->add_key_right = hplane_add_key_right;
}

void hplane_ops_delete()
{
	if (gp_hwidget_ops)
		vm_free(gp_hwidget_ops);
	if (gp_container_ops)
		vm_free(gp_container_ops);
	if (gp_plane_ops)
		vm_free(gp_plane_ops);
}