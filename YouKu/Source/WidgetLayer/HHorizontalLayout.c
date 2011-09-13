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


static void validate_layout(HLayout* p_layout, HRect* ret_p_rect)
{
	hlist_node_t* p_node = NULL;
	HWidget* p_widget;

	short s_sum_x = 0;
	short s_sum_y = 0;
	short s_line_height = 0;
	short s_start_x = 0;

	short s_temp_width = 0;      // save width to return
	short s_temp_height = 0;	 // save height to return

	HHorizontalLayout* p_hori_layout = (HHorizontalLayout*)p_layout;
	hlist_node_t* p_widget_list = p_hori_layout->st_layout_base.p_ops->get_widget_list(&(p_hori_layout->st_layout_base));

	short s_max_width = p_hori_layout->st_layout_base.p_ops->get_max_width(&(p_hori_layout->st_layout_base));
	short s_max_height = p_hori_layout->st_layout_base.p_ops->get_max_height(&(p_hori_layout->st_layout_base));

	LayoutGap st_gap = p_hori_layout->st_layout_base.p_ops->get_gap(&(p_hori_layout->st_layout_base));


	/* handle first node */
	p_node = p_widget_list->p_next;
	p_widget = (HWidget*)(p_node->pv_data);

	s_start_x = p_widget->s_top_x;
	s_sum_x  = p_widget->s_top_x + p_widget->p_widget_ops->get_width(p_widget);
	s_sum_y = p_widget->s_top_y;
	s_line_height = p_widget->p_widget_ops->get_height(p_widget);

	// save returning value
	if (NULL != ret_p_rect)
	{
		ret_p_rect->s_x = p_widget->s_top_x;
		ret_p_rect->s_y = p_widget->s_top_y;
	}

	/* handle all the nodes to start from second */
	for(p_node = p_widget_list->p_next->p_next; p_node!= p_widget_list; p_node = p_node->p_next)
	{
		p_widget = (HWidget*)(p_node->pv_data);
		s_sum_x = s_sum_x + st_gap.s_gap_x + p_widget->p_widget_ops->get_width(p_widget);
		
		/* adjust widgets from the second based the first */
		/* put widgets in one line */
		if (s_sum_x <= s_max_width)
		{
			p_widget->s_top_x = s_sum_x - p_widget->s_width;
			p_widget->s_top_y = s_sum_y;
			
			/* save the max widget height in one line */
			if (p_widget->p_widget_ops->get_height(p_widget) > s_line_height)
			{
				s_line_height = p_widget->s_height;
			}

			// save height to return 
			if (p_widget->s_top_x > s_temp_width)
			{
				s_temp_width = p_widget->s_top_x;
			}
		}
		/* put widgets into new line */
		else
		{
			s_sum_x = s_start_x;
			s_sum_y = s_sum_y + s_line_height + st_gap.s_gap_y;
			s_line_height = 0;
			
			// save height to return 
			if (s_sum_y > s_temp_height)
			{
				s_temp_height = s_sum_y;
			}

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

	if (NULL != ret_p_rect)
	{
		ret_p_rect->s_width = s_temp_width;
		ret_p_rect->s_height = s_temp_height;
	}
}


HHorizontalLayout* hhorizontal_layout_new()
{
	HHorizontalLayout* p_layout = (HHorizontalLayout*)vm_malloc(sizeof(HHorizontalLayout));	

	hlayout_init(&(p_layout->st_layout_base));

	p_layout->st_layout_base.p_ops->validate_layout = validate_layout;
	p_layout->st_layout_base.p_ops->delete_layout = hhorizontal_layout_delete;

	return p_layout;

}

void hhorizontal_layout_delete(HLayout* p_hori_layout)
{
	HHorizontalLayout* p_hori = (HHorizontalLayout*)p_hori_layout;

	if (NULL != p_hori)
	{
		hlayout_delete(&(p_hori->st_layout_base));
	}

	vm_free(p_hori);
}

/*********************EOF******************************/