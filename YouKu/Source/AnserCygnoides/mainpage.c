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
#include "def"
#include "mainpage.h"
#include "new.h"
#include "tools.h"
#include ".\..\WindowLayer\hwindow.h"

static void init(MainPage*);
static void main_firevisible(MainPage* mainpage);


static const CString c_cstr_rcm_btn_img		= "";
static const CString c_cstr_ctg_btn_img		= "";
static const CString c_cstr_sch_btn_img		= "";
static const CString c_cstr_tp_btn_img		= "";
static const CString c_cstr_myk_btn_img		= "";
static const CString c_cstr_qut_btn_img		= "";

MainPage* MainPage_new()
{
	MainPage* p_mainpage = checked_malloc(sizeof( MainPage ));
	p_mainpage->p_container_buttongroup = hcontainer_new();
	HContainer* p_cntn_button_group = p_mainpage->p_container_buttongroup;
	
	p_cntn_button_group->p_container_ops->add_widget (	
		p_cntn_button_group, 
		Button( 
			Rect( RECOMMEND_BUTTON_LEFT, RECOMMEND_BUTTON_TOP, RECOMMEND_BUTTON_WIDTH, RECOMMEND_BUTTON_HEIGHT), 
			c_cstr_rcm_btn_img, 
			"推荐" , 
			fn_recommend ));
	
	p_cntn_button_group->p_container_ops->add_widget (	
		p_cntn_button_group, 
		Button( 
			Rect( CATEGORY_BUTTON_LEFT, CATEGORY_BUTTON_TOP, CATEGORY_BUTTON_WIDTH, CATEGORY_BUTTON_HEIGHT), 
			c_cstr_ctg_btn_img, 
			"分类" , 
			fn_category ));
	
	p_cntn_button_group->p_container_ops->add_widget (	
		p_cntn_button_group, 
		Button( 
			Rect( SEARCH_BUTTON_LEFT, SEARCH_BUTTON_TOP, SEARCH_BUTTON_WIDTH, SEARCH_BUTTON_HEIGHT),
			c_cstr_sch_btn_img, 
			"搜索", 
			fn_search ));
	
	p_cntn_button_group->p_container_ops->add_widget (	
		p_cntn_button_group, 
		Button( 
			Rect( TOP_BUTTON_LEFT, TOP_BUTTON_TOP, TOP_BUTTON_WIDTH, TOP_BUTTON_HEIGHT),
			c_cstr_tp_btn_img, 
			"最热", 
			fn_top ));
	
	p_cntn_button_group->p_container_ops->add_widget (	
		p_cntn_button_group, 
		Button( 
			Rect( MYYOUKU_BUTTON_LEFT, MYYOUKU_BUTTON_TOP, MYYOUKU_BUTTON_WIDTH, MYYOUKU_BUTTON_HEIGHT),
			c_cstr_myk_btn_img, 
			"我的优酷", 
			fn_myyouku ));
	
	p_cntn_button_group->p_container_ops->add_widget (	
		p_cntn_button_group, 
		Button( 
			Rect( QUIT_BUTTON_LEFT, QUIT_BUTTON_TOP, QUIT_BUTTON_WIDTH, QUIT_BUTTON_HEIGHT),
			c_cstr_qut_btn_img, 
			"退出", 
			fn_quit));


	p_mainpage->base.fire_visible = fire_visible;
	p_mainpage->base.fire_hiden   = fire_hiden;
	p_mainpage->base.destroy      = destroy;
	return p_mainpage;

}


/*show this page */
static void fire_visible(Page *p_this)
{
	window->get_default_plane()->p_plane_ops->add_widget(window->get_default_plane(), (HWidget*)mainpage->p_container_total);
	window->validate();
}

/* hide this page */
static void fire_hiden(Page *p_this)
{

}

/*destroy the page , free its memory*/
static void destroy(Page *p_this)
{
	/* 
	free all memery
	*/
	vm_free(p_this);
}

static void recommend(HWidget *p_source, HEvent *p_evt, void *p_param)
{
	OpenPage( recommendpage_new() );

}

static void category(HWidget *p_source, HEvent *p_evt, void *p_param)
{
	OpenPage( recommendpage_new() );
}

static void search(HWidget *p_source, HEvent *p_evt, void *p_param)
{

}

static void top(HWidget *p_source, HEvent *p_evt, void *p_param)
{

}

static void myyouku(HWidget *p_source, HEvent *p_evt, void *p_param)
{

}
