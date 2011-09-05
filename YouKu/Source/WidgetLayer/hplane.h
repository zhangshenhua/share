/********************************************************
*file: hplane.h
*author: yan.gao
*date: 2011.08.22
*description: a plane, the root container
********************************************************/

#ifndef _HPLANE_H_
#define  _HPLANE_H_

#include "hwidget.h"
#include "hcontainer.h"

typedef struct _HPlane HPlane;
typedef struct _HPlaneOperation HPlaneOperation;

/*new a HPlane*/
HPlane * hplane_new();

void hplane_delete(HPlane *p_plane);


struct _HPlane {

	/********************** private member *************************/

	/*base class*/
	HContainer base;

	/* the top widget of the plane */
	HWidget *p_top_widget;

	/* the bottom widget of the plane*/
	HWidget *p_bottom_widget;
	
	/* the content container of the plane*/
	HContainer *p_content;

	/* MRE paint handle */
	int i_handle;

	/*enable/disable transparent the plane's background*/
	short i_enable_transparent_bg;

	/*the plane's layer trans color*/
	short s_transparent_color;

	/*the index in  window*/
	short s_plane_index;

	/*the cur own focus widget*/
	HWidget *p_own_focus_widget;

	/********************** public member **************************/

	/* plane operation */
	HPlaneOperation *p_plane_ops;
};

struct _HPlaneOperation {

	/************************************************************************
	* add widget to plane's content container
	************************************************************************/
	void (*add_widget)(HPlane *p_plane, HWidget *p_widget);

	/************************************************************************
	* remove widget from plane's content container
	************************************************************************/
	void (*remove_widget)(HPlane *p_plane, HWidget *p_widget);

	/************************************************************************
	* remove all widget from plane's content container, and free there's memory
	************************************************************************/
	void (*remove_all)(HPlane *p_plane);

	/************************************************************************
	* Function: set_top_widget
	* Description: set the top widget of the plane
	************************************************************************/
	void (*set_top_widget)(HPlane *p_plane, HWidget *p_widget);

	/************************************************************************
	* Function: set_top_widget
	* Description: set the bottom widget of the plane
	************************************************************************/
	void (*set_bottom_widget)(HPlane *p_plane, HWidget *p_widget);
	
};

#endif