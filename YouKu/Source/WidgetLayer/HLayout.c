/*---------------------------------
* FILE Name: hlayout.c
*
* Description: This file implements the parrent class of layout manager
*
* COPYRIGHT
*
* Created on: 2011.8.31 Author: Pian,HaiSheng
*
* Modified by name on $(date)
*---------------------------------------------------------------------*

/**************Global Variables*************************/

/*****************INCLUDE FILES*************************/
#include "vmsys.h"
#include "HLayout.h"


/*****************Macro Definition*************************/
#define HLAYOUT_GAP 5


static HLayoutOperation* gp_layout_ops;
static void create_layout_ops();

static void add_widget(HLayout* p_layout, HWidget* p_widget)
{
	hlist_append(p_layout->p_widget_list, p_widget, NULL);
	// ?
	//p_widget->p_parent = p_layout->p_widget_list;
}

static void remove_widget(HLayout* p_layout, HWidget* p_widget)
{
	hlist_remove(p_layout->p_widget_list, p_widget);
}

static void clear_layout(HLayout* p_layout)
{
	hlist_clear(p_layout->p_widget_list);
}

static void set_max_width(HLayout* p_layout, short s_width)
{
	p_layout->s_max_width = s_width;
}

static short get_max_width(HLayout* p_layout)
{
	return p_layout->s_max_width;
}

static void set_max_height(HLayout* p_layout, short s_height)
{
	p_layout->s_max_height = s_height;
}

static short get_max_height(HLayout* p_layout)
{
	return p_layout->s_max_height;
}

static void set_gap(HLayout* p_layout, LayoutGap st_gap)
{
	p_layout->st_gap.s_gap_x = st_gap.s_gap_x;
	p_layout->st_gap.s_gap_y = st_gap.s_gap_y;
}

static LayoutGap get_gap(HLayout* p_layout)
{
	return p_layout->st_gap;
}

static hlist_node_t* get_widget_list(HLayout* p_layout)
{
	return p_layout->p_widget_list;
}


void hlayout_init(HLayout* p_layout)
{
	p_layout->s_max_width = 0;
	p_layout->s_max_height = 0;

	p_layout->st_gap.s_gap_x = HLAYOUT_GAP;
	p_layout->st_gap.s_gap_y = HLAYOUT_GAP;

	if (NULL == p_layout->p_widget_list)
	{
		p_layout->p_widget_list = hlist_new();
	}

	create_layout_ops();

	p_layout->p_ops = gp_layout_ops;
}

void hlayout_delete(HLayout* p_layout)
{
	if (NULL != p_layout)
	{
		hlist_destroy(&p_layout->p_widget_list);
	}

}

void create_layout_ops()
{
	if (NULL == gp_layout_ops)
	{
		gp_layout_ops = (HLayoutOperation*)vm_malloc(sizeof(HLayoutOperation));

		gp_layout_ops->add_widget = add_widget;
		gp_layout_ops->remove_widget = remove_widget;
		gp_layout_ops->clear_layout = clear_layout;
		gp_layout_ops->set_max_width = set_max_width;
		gp_layout_ops->get_max_width = get_max_width;
		gp_layout_ops->set_max_height = set_max_height;
		gp_layout_ops->get_max_height = get_max_height;
		gp_layout_ops->set_gap = set_gap;
		gp_layout_ops->get_gap = get_gap;
		gp_layout_ops->get_widget_list = get_widget_list;
		gp_layout_ops->validate_layout = NULL;
	}
}
/*********************EOF******************************/