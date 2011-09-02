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
#include "hplane.h"
#include "hpushbutton.h"

MainPage* MainPage_new()
{
	MainPage* p_mainpage ;
	VM_NEW(p_mainpage, MainPage);
	init(p_mainpage);
	return p_mainpage;

}

static void init(MainPage* p_mainpage)
{

	/************************************************************************/
	/* set container                                                                      */
	/************************************************************************/
	//construct containers
	p_mainpage->p_container_total = hcontainer_new();
	p_mainpage->p_container_boby = hcontainer_new();
	p_mainpage->p_container_buttongroup  = hcontainer_new();

	//construct 5 pushbuttons
	p_mainpage->pbutton1 = hpushbutton_new("");	
	p_mainpage->pbutton2 = hpushbutton_new("");	
	p_mainpage->pbutton3 = hpushbutton_new("");	
	p_mainpage->pbutton4 = hpushbutton_new("");	
	p_mainpage->pbutton5 = hpushbutton_new("");	


	
	//add widgets into containers
	p_mainpage->p_container_total->p_container_ops->add_widget( p_mainpage->p_container_total, p_mainpage->p_container_boby );
	p_mainpage->p_container_total->p_container_ops->add_widget( p_mainpage->p_container_total, p_mainpage->p_container_buttongroup );
	
	p_mainpage->p_container_total.base.s_top_x = 0;
	p_mainpage->p_container_total->base.s_top_y = 0;
	p_mainpage->p_container_total->base.s_width = vm_graphic_get_screen_width();
	p_mainpage->p_container_total->base.s_height = vm_graphic_get_screen_height();

	p_mainpage->pbutton1->base->s_top_x = UNKNOW; 
	p_mainpage->pbutton1->base->s_top_y = UNKNOW;
	p_mainpage->pbutton1->base->s_width = UNKNOW;
	p_mainpage->pbutton1->base->s_height = UNKNOW;

	p_mainpage->pbutton2->base->s_top_x = UNKNOW; 
	p_mainpage->pbutton2->base->s_top_y = UNKNOW;
	p_mainpage->pbutton2->base->s_width = UNKNOW;
	p_mainpage->pbutton2->base->s_height = UNKNOW;

	p_mainpage->pbutton3->base->s_top_x = UNKNOW; 
	p_mainpage->pbutton3->base->s_top_y = UNKNOW;
	p_mainpage->pbutton3->base->s_width = UNKNOW;
	p_mainpage->pbutton3->base->s_height = UNKNOW;

	p_mainpage->pbutton4->base->s_top_x = UNKNOW; 
	p_mainpage->pbutton4->base->s_top_y = UNKNOW;
	p_mainpage->pbutton4->base->s_width = UNKNOW;
	p_mainpage->pbutton4->base->s_height = UNKNOW;

	p_mainpage->pbutton5->base->s_top_x = UNKNOW; 
	p_mainpage->pbutton5->base->s_top_y = UNKNOW;
	p_mainpage->pbutton5->base->s_width = UNKNOW;
	p_mainpage->pbutton5->base->s_height = UNKNOW;


	p_mainpage->p_plane_boby		= hplane_new();			//plane contains video list, etc
	p_mainpage->p_plane_buttongroup->p_plane_ops->set_top_widget(p_mainpage->p_plane_buttongroup, )
	loginpage->container.base.s_top_x = LOGIN_CONTAINER_S_TOP_X;
	loginpage->container->base.s_top_y = LOGIN_CONTAINER_S_TOP_Y;
	loginpage->container->base.s_width = LOGIN_CONTAINER_WIDTH;
	loginpage->container.base.s_height = LOGIN_CONTAINER_HEIGHT;

	/************************************************************************/
	/* set bg_image                                                                     */
	/************************************************************************/
	loginpage->bg_image = himage_init(LOGIN_BG_NAME);

	/************************************************************************/
	/* set login_button                                                                     */
	/************************************************************************/
	loginpage->login_button = hpushbutton_new();

	loginpage->login_button->set_text(loginpage->login_button,"login");
	loginpage->login_button.base.s_top_x = LOGIN_BUTTON_S_TOP_X;
	loginpage->login_button->base.s_top_y = LOGIN_BUTTON_S_TOP_Y;

	/************************************************************************/
	/* set name_textarea and pwd_textarea                                                                    */
	/************************************************************************/

	loginpage->name_textinput = htextarea_new();
	loginpage->name_textinput->base.s_top_x = LOGIN_NAME_TEXTINPUT_S_TOP_X;
	loginpage->name_textinput->base.s_top_y = LOGIN_NAME_HTEXINPUT_S_TOP_Y;

	loginpage->pwd_textinput = htextarea_new();

	loginpage->pwd_textinput->base.s_top_x = LOGIN_PWD_HTEXTINPUT_S_TOP_X;
	loginpage->pwd_textinput->base.s_top_y = LOGIN_PWD_HTEXINPUT_S_TOP_Y;

	/************************************************************************/
	/* add widgets to the container                                                                      */
	/************************************************************************/
	loginpage->container->p_container_ops->add_widget(loginpage->container,loginpage->login_button);
	loginpage->container->p_container_ops->add_widget(loginpage->container, loginpage->bg_image);
	loginpage->container->p_container_ops->add_widget(loginpage->container, loginpage->name_textinput);
	loginpage->container->p_container_ops->add_widget(loginpage->container, loginpage->pwd_textinput);


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