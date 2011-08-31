/*********************************
*file: page.h
*author: yan.gao
*date: 2011.08.24
*description: 
***********************************/

#include "page.h"
#include "vmsys.h"
#include "string.h"

static void append_page(PageSwitchController *p_this, Page *p_page)
{

}

static void delete_page(PageSwitchController *p_this, Page *p_page)
{

}

/*find a page by  page type*/
static Page * find_page_by_page_type(PageSwitchController *p_this, int i_page_type)
{
	Page *p_page;
	//TODO:
	return p_page;
}

/*find a page by  page id*/
static Page * find_page_by_page_id(PageSwitchController *p_this, int i_page_id)
{
	Page *p_page;
	//TODO:
	return p_page;
}

/*through p_model to create a page view*/
static PageView * create_page_view(PageSwitchController *p_this, PageModel *p_model)
{
	PageView *p_pageview;
	//TODO:
	return p_pageview;
}

/*show the current page*/
static void show_cur_page(PageSwitchController *p_this)
{

}

/*set current page*/
static void set_cur_page(PageSwitchController *p_this, Page *p_page)
{

}

/*show back page*/
static void go_back(PageSwitchController *p_this)
{

}

/*destroy the page switch controller, free its memory*/
static void destroy(PageSwitchController *p_this)
{

}

PageSwitchController *page_switch_controller_new()
{
	PageSwitchController *p_psc;
	p_psc = vm_malloc(sizeof(PageSwitchController));
	memset(p_psc, 0, sizeof(PageSwitchController));
	p_psc->append_page = append_page;
	p_psc->delete_page = delete_page;
	p_psc->find_page_by_page_id = find_page_by_page_id;
	p_psc->find_page_by_page_type = find_page_by_page_type;
	p_psc->create_page_view = create_page_view;
	p_psc->show_cur_page = show_cur_page;
	p_psc->set_cur_page = set_cur_page;
	p_psc->go_back = go_back;
	p_psc->destroy = destroy;
	return p_psc;
}