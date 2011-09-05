/*---------------------------------
* FILE Name: HHorizontalLayout.c
*
* Description: This file defines the horizontal layout class
*
* COPYRIGHT
*
* Created on: 2011.8.31 Author: Pian,HaiSheng
*
* Modified by name on $(date)
*---------------------------------------------------------------------*/
#include "vmsys.h"

#include "HHorizontalLayout.h"


static void validate_layout(HLayout* p_layout)
{
	hlist_node_t* p_node = NULL;
	HWidget* p_widget;

	short s_sum_x = 0;
	short s_sum_y = 0;
	short s_line_height = 0;
	short s_start_x = 0;

	HHorizontalLayout* p_hori_layout = (HHorizontalLayout*)p_layout;
	hlist_node_t* p_widget_list = p_hori_layout->st_layout_base.get_widget_list(&(p_hori_layout->st_layout_base));

	short s_max_width = p_hori_layout->st_layout_base.get_max_width(&(p_hori_layout->st_layout_base));
	short s_max_height = p_hori_layout->st_layout_base.get_max_height(&(p_hori_layout->st_layout_base));

	LayoutGap st_gap = p_hori_layout->st_layout_base.get_gap(&(p_hori_layout->st_layout_base));


	/* handle first node */
	p_node = p_widget_list->p_next;
	p_widget = (HWidget*)(p_node->pv_data);

	s_start_x = p_widget->s_top_x;
	s_sum_x  = p_widget->s_top_x + p_widget->s_width;
	s_sum_y = p_widget->s_top_y;
	s_line_height = p_widget->s_height;

	/* handle all the nodes to start from second */
	for(p_node = p_widget_list->p_next->p_next; p_node!= p_widget_list; p_node = p_node->p_next)
	{
		p_widget = (HWidget*)(p_node->pv_data);
		s_sum_x = s_sum_x + st_gap.s_gap_x + p_widget->s_width;
		
		/* adjust widgets from the second based the first */
		/* put widgets in one line */
		if (s_sum_x <= s_max_width)
		{
			p_widget->s_top_x = s_sum_x - p_widget->s_width;
			p_widget->s_top_y = s_sum_y;
			
			/* save the max widget height in one line */
			if (p_widget->s_height > s_line_height)
			{
				s_line_height = p_widget->s_height;
			}
		}
		/* put widgets into new line */
		else
		{
			s_sum_x = s_start_x;
			s_sum_y = s_sum_y + s_line_height + st_gap.s_gap_y;
			s_line_height = 0;
			
			if (s_sum_y <= s_max_height)
			{
				p_widget->s_top_x = s_sum_x;
				p_widget->s_top_y = s_sum_y;

				s_sum_x = s_sum_x + p_widget->s_width;
			}
			/* break if layout cannot put all widgets in it*/
			else
			{
				break;
			}
		}
		
	}
}


HHorizontalLayout* hhorizontal_layout_new()
{
	HHorizontalLayout* p_layout = (HHorizontalLayout*)vm_malloc(sizeof(HHorizontalLayout));	

	hlayout_init(&(p_layout->st_layout_base));

	p_layout->st_layout_base.validate_layout = validate_layout;

	return p_layout;

}

void hhorizontal_layout_delete(HHorizontalLayout* p_hori_layout)
{
	if (NULL != p_hori_layout)
	{
		hlayout_delete(&(p_hori_layout->st_layout_base));
	}

	vm_free(p_hori_layout);
}

/*********************EOF******************************/