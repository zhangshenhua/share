/*---------------------------------
* FILE Name: HVerticalLayout.h
*
* Description: This file declares the vertical layout class
*
* COPYRIGHT
*
* Created on: 2011.8.31 Author: Pian,HaiSheng
*
* Modified by name on $(date)
*---------------------------------------------------------------------*/
#ifndef HVERTICAL_LAYOUT_H
#define HVERTICAL_LAYOUT_H

/**************Global Variables*************************/

/*****************INCLUDE FILES*************************/
#include "HLayout.h"


/* HHorizontalLayout type declaration */
typedef struct _HVerticalLayout
{
	HLayout st_layout_base;
}HVerticalLayout;

HVerticalLayout* hvertical_layout_new();

void hvertical_layout_delete(HLayout* p_vert_layout);

#endif
/*********************EOF******************************/