/*---------------------------------
 * FILE Name: hlayout.h
 *
 * Description: This file describes the parrent class definition of layout manager
 *
 * COPYRIGHT
 *
 * Created on: 2011.8.31 Author: Pian,HaiSheng
 *
 * Modified by name on $(date)
 *---------------------------------------------------------------------*/
#ifndef HLAYOUT_H
#define HLAYOUT_H

/**************Global Variables*************************/
/*****************INCLUDE FILES*************************/
#include "hwidget.h"

#include "../Common/hlist.h"

typedef struct _HLayout HLayout;
typedef struct _HLayoutOperation HLayoutOperation;
typedef struct _Layout_Gap LayoutGap;

/*---------------------------------
* Function Name:	hlayout_init
*
* Description:		init hlayout object
*
* Parameters:		
*
* Return:			void
*---------------------------------------------------------------------*/
void hlayout_init(HLayout* p_layout);

/*---------------------------------
* Function Name:	hlayout_delete
*
* Description:		delete hlayout object
*
* Parameters:		
*
* Return:			void
*---------------------------------------------------------------------*/
void hlayout_delete(HLayout* p_layout);

struct _Layout_Gap
{
	short s_gap_x;
	short s_gap_y;
};

/* HLayout type declaration */
struct _HLayout
{
	/* max width and height for putting widget area */
	short s_max_width;
	short s_max_height;

	/* gap between widgets */
	LayoutGap st_gap;

	/* list to save all the added widgets */
	hlist_node_t* p_widget_list;

	HLayoutOperation* p_ops;
};

struct _HLayoutOperation
{
	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	void (*add_widget)(HLayout* p_layout, HWidget* p_widget);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	void (*remove_widget)(HLayout* p_layout, HWidget* p_widget);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	void (*clear_layout)(HLayout* p_layout);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	void (*set_max_width)(HLayout* p_layout, short s_width);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	short (*get_max_width)(HLayout* p_layout);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	void (*set_max_height)(HLayout* p_layout, short s_height);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	short (*get_max_height)(HLayout* p_layout);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	void (*set_gap)(HLayout* p_layout, LayoutGap st_gap);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	LayoutGap (*get_gap)(HLayout* p_layout);

	/*---------------------------------
	* Function Name:	
	*
	* Description:		
	*
	* Parameters:		
	*
	* Return:			
	*---------------------------------------------------------------------*/
	hlist_node_t* (*get_widget_list)(HLayout* p_layout);

	/*---------------------------------
	* Function Name:	layout_validate
	*
	* Description:		make layout to be valid
	*
	* Parameters:		
	*
	* Return:			void
	*---------------------------------------------------------------------*/
	void (*validate_layout)(HLayout* p_layout, HRect* ret_p_rect);
};

#endif
 /*********************EOF******************************/