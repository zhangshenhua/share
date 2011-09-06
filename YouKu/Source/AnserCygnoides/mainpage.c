/*---------------------------------
* FILE Name: mainpage.c
*
* Description:
*
* COPYRIGHT
*
* Created on: $(date) Author: zhang.shenhua
*
* Modified by name on $(date)
*---------------------------------------------------------------------*/
#include <vmvideo.h>
#include "mainpage.h"
#include "new.h"
#include ".\..\WindowLayer\hwindow.h"

static void init(MainPage*);
static void main_firevisible(MainPage* mainpage);

MainPage* MainPage_new()
{
	MainPage* p_mainpage ;
	VM_NEW(p_mainpage, MainPage);
	init(p_mainpage);
	return p_mainpage;

}

void MainPage_delete(MainPage* p_mainpage)
{
	p_mainpage->base.destroy(p_mainpage);

}

static void init(MainPage* p_mainpage)
{
	const int screen_width  = vm_graphic_get_screen_width();
	const int screen_height = vm_graphic_get_screen_height();

	
	/************************************************************************/
	/* set container                                                                      */
	/************************************************************************/
	//construct containers
	p_mainpage->p_container_total = hcontainer_new();
	p_mainpage->p_container_boby = hcontainer_new();
	p_mainpage->p_container_buttongroup  = hcontainer_new();

	//construct 5 pushbuttons
	p_mainpage->pbutton1 = hpushbutton_new("推荐");	
	p_mainpage->pbutton2 = hpushbutton_new("分\n类");	
	p_mainpage->pbutton3 = hpushbutton_new("搜索");	
	p_mainpage->pbutton4 = hpushbutton_new("最热");	
	p_mainpage->pbutton5 = hpushbutton_new("我的\n优酷");	
	
	
	//add widgets into containers
	p_mainpage->p_container_total->p_container_ops->add_widget( p_mainpage->p_container_total, (HWidget*)p_mainpage->p_container_boby );
	p_mainpage->p_container_total->p_container_ops->add_widget( p_mainpage->p_container_total, (HWidget*)p_mainpage->p_container_buttongroup );
	
	p_mainpage->p_container_total->base.s_top_x = 0;
	p_mainpage->p_container_total->base.s_top_y = 0;
	p_mainpage->p_container_total->base.s_width  = screen_width;
	p_mainpage->p_container_total->base.s_height = screen_height;
	
#if 1
	p_mainpage->p_container_buttongroup->base.s_top_x = 0;	
	p_mainpage->p_container_buttongroup->base.s_top_y = screen_height - screen_width / 5 ;
	p_mainpage->p_container_buttongroup->base.s_width  = p_mainpage->p_container_total->base.s_width;
	p_mainpage->p_container_buttongroup->base.s_height = p_mainpage->p_container_total->base.s_height;
#else

	p_mainpage->p_container_buttongroup->base.p_widget_ops = 0;	
	p_mainpage->p_container_buttongroup->base.s_top_y = screen_height - screen_width / 5 ;
	p_mainpage->p_container_buttongroup->base.s_width  = p_mainpage->p_container_total->base.s_width;
	p_mainpage->p_container_buttongroup->base.s_height = p_mainpage->p_container_total->base.s_height;
#endif

	p_mainpage->pbutton1->base.s_top_x = 64*0; 
	p_mainpage->pbutton1->base.s_top_y = 0;
	p_mainpage->pbutton1->base.s_width = 64;
	p_mainpage->pbutton1->base.s_height = 64;

	p_mainpage->pbutton2->base.s_top_x = 64*1; 
	p_mainpage->pbutton2->base.s_top_y = 0;
	p_mainpage->pbutton2->base.s_width  = 64;
	p_mainpage->pbutton2->base.s_height = 64;

	p_mainpage->pbutton3->base.s_top_x  = 64*2; 
	p_mainpage->pbutton3->base.s_top_y  = 0;
	p_mainpage->pbutton3->base.s_width  = 64;
	p_mainpage->pbutton3->base.s_height = 64;

	p_mainpage->pbutton4->base.s_top_x = 64*3; 
	p_mainpage->pbutton4->base.s_top_y = 0;
	p_mainpage->pbutton4->base.s_width = 64;
	p_mainpage->pbutton4->base.s_height = 64;

	p_mainpage->pbutton5->base.s_top_x = 64*4; 
	p_mainpage->pbutton5->base.s_top_y = 0;
	p_mainpage->pbutton5->base.s_width = 64;
	p_mainpage->pbutton5->base.s_height = 64;


//p_mainpage->p_container_boby = hcontainer_new();			//plane contains video list, etc

#if 0 
	p_mainpage->p_container_buttongroup->p_plane_ops->set_top_widget(p_mainpage->p_plane_buttongroup, )
	loginpage->container.base.s_top_x = LOGIN_CONTAINER_S_TOP_X;
	loginpage->container->base.s_top_y = LOGIN_CONTAINER_S_TOP_Y;
	loginpage->container->base.s_width = LOGIN_CONTAINER_WIDTH;
	loginpage->container.base.s_height = LOGIN_CONTAINER_HEIGHT;
#endif
	

	/************************************************************************/
	/* add widgets to the container                                                                      */
	/************************************************************************/
	p_mainpage->p_container_buttongroup->p_container_ops->add_widget(p_mainpage->p_container_buttongroup, (HWidget*)p_mainpage->pbutton1);
	p_mainpage->p_container_buttongroup->p_container_ops->add_widget(p_mainpage->p_container_buttongroup, (HWidget*)p_mainpage->pbutton2);
	p_mainpage->p_container_buttongroup->p_container_ops->add_widget(p_mainpage->p_container_buttongroup, (HWidget*)p_mainpage->pbutton3);
	p_mainpage->p_container_buttongroup->p_container_ops->add_widget(p_mainpage->p_container_buttongroup, (HWidget*)p_mainpage->pbutton4);
	p_mainpage->p_container_buttongroup->p_container_ops->add_widget(p_mainpage->p_container_buttongroup, (HWidget*)p_mainpage->pbutton5);


	/************************************************************************/
	/*                        init functions                                */
	/************************************************************************/
	p_mainpage->show = main_firevisible;
	

}

/************************************************************************/
/* add plane                                                                      */
/************************************************************************/

static void main_firevisible(MainPage* mainpage)
{
	window->get_default_plane()->p_plane_ops->add_widget(window->get_default_plane(), (HWidget*)mainpage->p_container_total);
	window->validate();
	
}


void  main_switchtonewpage()
{
	// to do something 
}



static void show_recommend_interface(MainPage* p_mainpage)
{

}

static void show_category_interface(MainPage* p_mainpage)
{

}

static void show_search_interface(MainPage* p_mainpage)
{

}

static void show_top_interface(MainPage* p_mainpage)
{

}

static void show_myyouku_interface(MainPage* p_mainpage)
{

}