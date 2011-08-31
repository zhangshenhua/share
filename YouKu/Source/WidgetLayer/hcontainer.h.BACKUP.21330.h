/*********************************
*file: hcontainer.h
*author: yan.gao
*date: 2011.08.22
*description: a container
***********************************/

#ifndef _HCONTAINER_H_
#define _HCONTAINER_H_

#include "hwidget.h"
#include "../Common/hlist.h"

typedef struct _HContainer HContainer;
typedef struct _HContainerOperation HContainerOperation;

/************************************************************************
* Function: hcontainer_new                                                              
* Description: create a new HContainer
************************************************************************/
HContainer * hcontainer_new();

/************************************************************************
* Function: hcontainer_delete                                                              
* Description: destroy a  HContainer
* Para(in) p_container : it is a HContainer
************************************************************************/
void hcontainer_delete(HWidget *p_container);

/************************************************************************
* Function: hcontainer_init                                                            
* Description: init a  HContainer
************************************************************************/
void hcontainer_init(HContainer *p_container);


/************************************************************************
* Function : hcontainer_operation_init                                                             
* Description : init the container operation
************************************************************************/
void hcontainer_operation_init(HContainerOperation *p_container_ops);


struct _HContainer {

	/*********************** private member ***************************/
	
	HWidget base;

	/*a translate coordinate of x*/
	short s_translate_x;

	/*a translate coordinate of y*/
	short s_translate_y;

	/*the current has focus widget in the container*/
	hlist_node_t *p_has_focus_widget_node;

	/* children nodes */
	hlist_node_t *p_children;

	/********************** public member *************************/
	
	/*container operations*/
	HContainerOperation *p_container_ops;

};

struct _HContainerOperation {

	/************************************************************************
	* Function : add_widget
	* Description: add a widget to a container   
	* Para(in) p_container :
	* Para(in) p_widget : the will added widget
	************************************************************************/
	void (*add_widget)(HContainer *p_container, HWidget *p_widget);


	/************************************************************************
	* Function : remove_widget
	* Description:  remove a widget from a container,and free the widget memory
	* Para(in) p_container :
	* Para(in) p_widget : the will removed widget
	************************************************************************/
	void (*remove_widget)(HContainer *p_container, HWidget *p_widget);

	/************************************************************************
	* Function : child_at
	* Description : search the container's child widgets, to find the widget at point(s_x, s_y)
	* Para(in) s_x : the relative the container's coordinate of x
	* Para(in) s_y : the relative the container's coordinate of y
	* Return: the widget at(s_x, s_y). if not find then return the container itself
	************************************************************************/
	HWidget* (*child_at)(HContainer *p_container, short s_x, short s_y);


	/************************************************************************
	* Function: remove_all
	* Description: remove all widget in the container, and free its children widgets memory
	* Para(in) p_container : the needed clear container 
	************************************************************************/
	void (*remove_all)(HContainer *p_container);


	/************************************************************************
	* Function: scroll
	* Description :  scroll the container
	* Para(in) s_dx : horizontal scroll value
	* Para(in) s_dy : vertical scroll value
	************************************************************************/
	void (*scroll)(HContainer *p_container, short s_dx, short s_dy);
<<<<<<< HEAD

	/************************************************************************
	* whether the container can be scroll, return yes(1), or no(0)
	************************************************************************/
	int (*can_scroll)(HContainer *p_container);
=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
};



#endif