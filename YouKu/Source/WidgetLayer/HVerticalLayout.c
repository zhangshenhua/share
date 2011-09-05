/*---------------------------------
* FILE Name: HVerticalLayout.c
*
* Description: This file defines the vertical layout class
*
* COPYRIGHT
*
* Created on: 2011.8.31 Author: Pian,HaiSheng
*
* Modified by name on $(date)
*---------------------------------------------------------------------*/
#include "vmsys.h"
#include "HVerticalLayout.h"

//extern void hlayout_init();

static void validate_layout(HLayout* p_layout)
{
	hlist_node_t* p_node = NULL;
	HWidget* p_widget;

	short s_sum_x = 0;
	short s_sum_y = 0;
	short s_column_width = 0;
	short s_start_y = 0;

	HVerticalLayout* p_vert_layout = (HVerticalLayout*)p_layout;
	hlist_node_t* p_widget_list = p_vert_layout->st_layout_base.get_widget_list(&(p_vert_layout->st_layout_base));

	short s_max_width = p_vert_layout->st_layout_base.get_max_width(&(p_vert_layout->st_layout_base));
	short s_max_height = p_vert_layout->st_layout_base.get_max_height(&(p_vert_layout->st_layout_base));

	LayoutGap st_gap = p_vert_layout->st_layout_base.get_gap(&(p_vert_layout->st_layout_base));


	/* handle first node */
	p_node = p_widget_list->p_next;
	p_widget = (HWidget*)(p_node->pv_data);

	s_start_y = p_widget->s_top_y;
	s_sum_x = p_widget->s_top_x;
	s_sum_y  = p_widget->s_top_y + p_widget->s_height;
	
	s_column_width = p_widget->s_width;

	/* handle all the nodes to start from second */
	for(p_node = p_widget_list->p_next->p_next; p_node!= p_widget_list; p_node = p_node->p_next)
	{
		p_widget = (HWidget*)(p_node->pv_data);
		s_sum_y = s_sum_y + st_gap.s_gap_y + p_widget->s_height;

		/* adjust widgets from the second based the first */
		/* put widgets in one column */
		if (s_sum_y <= s_max_height)
		{
			p_widget->s_top_x = s_sum_x;
			p_widget->s_top_y = s_sum_y - p_widget->s_max_height;

			/* save the max widget height in one line */
			if (p_widget->s_width > s_column_width)
			{
				s_column_width = p_widget->s_width;
			}
		}
		/* put widgets into new column */
		else
		{
			s_sum_y = s_start_y;
			s_sum_x = s_sum_x + s_column_width + st_gap.s_gap_x;
			s_column_width = 0;

			if (s_sum_x <= s_max_width)
			{
				p_widget->s_top_x = s_sum_x;
				p_widget->s_top_y = s_sum_y;

				s_sum_y = s_sum_y + p_widget->s_width;
			}
			/* break if layout cannot put all widgets in it*/
			else
			{
				break;
			}
		}

	}
}

HVerticalLayout* hvertical_layout_new()
{
	HVerticalLayout* p_layout = (HVerticalLayout*)vm_malloc(sizeof(HVerticalLayout));	

	hlayout_init(&(p_layout->st_layout_base));

	p_layout->st_layout_base.validate_layout = validate_layout;

	return p_layout;

}

void hvertical_layout_delete(HVerticalLayout* p_vert_layout)
{
	if (NULL != p_vert_layout)
	{
		hlayout_delete(&(p_vert_layout->st_layout_base));
	}

	vm_free(p_vert_layout);
}
/*********************EOF******************************/