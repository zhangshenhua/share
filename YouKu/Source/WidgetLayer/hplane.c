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
	((HWidget *)ph->p_content)->p_parent = ph;
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