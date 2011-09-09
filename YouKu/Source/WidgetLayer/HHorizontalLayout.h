/*---------------------------------
* FILE Name: HHorizontalLayout.h
*
* Description: This file declares the horizontal layout class
*
* COPYRIGHT
*
* Created on: 2011.8.31 Author: Pian,HaiSheng
*
* Modified by name on $(date)
*---------------------------------------------------------------------*/
#ifndef HHORIZONTAL_LAYOUT_H
#define HHORIZONTAL_LAYOUT_H

/**************Global Variables*************************/

/*****************INCLUDE FILES*************************/
#include "HLayout.h"

/* HHorizontalLayout type declaration */
typedef struct _HHorizontalLayout
{
	HLayout st_layout_base;
}HHorizontalLayout;

HHorizontalLayout* hhorizontal_layout_new();

void hhorizontal_layout_delete(HHorizontalLayout* p_hori_layout);

#endif
/*********************EOF******************************/
