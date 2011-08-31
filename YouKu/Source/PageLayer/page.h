/*********************************
*file: page.h
*author: yan.gao
*date: 2011.08.19
*description: 
***********************************/
#ifndef _PAGE_H_
#define _PAGE_H_

typedef struct _Page Page;
typedef struct _PageModel PageModel;
typedef struct _PageView PageView;
typedef struct _PageSwitchController PageSwitchController;

/*new a PageSwitchController instance*/
PageSwitchController *page_switch_controller_new();

struct _Page {
	PageModel *p_model;
	PageView *p_view;
	Page *p_next;
	Page *p_prev;

	/*user defined a unique id*/
	int i_page_id;

	/*destroy the page , free its memory*/
	void (*destroy)(Page *p_this);
};

struct _PageModel {

	/*the page type. user should define it*/
	int i_page_type;

	/*destroy the page model, free its memory*/
	void (*destroy)(PageModel *p_this);
};

struct _PageView {

	/*the page model*/
	PageModel *p_model;

	/*use the p_model to init the page view*/
	void (*init)(PageView *p_this, PageModel *p_model);

	/*show this page view*/
	void (*fire_visible)(PageView *p_this);

	/*destroy the page view, free its memory*/
	void (*destroy)(PageView *p_this);
};

struct _PageSwitchController {

	/************************  private member *****************************/

	/*a root page , does not belong to a specific page*/
	Page root_page;

	/*current page*/
	Page *p_cur_page;

	/************************  public member *****************************/

	/*append a page after the current page*/
	void (*append_page)(PageSwitchController *p_this, Page *p_page);

	/*delete a page from the pages, and will free its memory*/
	void (*delete_page)(PageSwitchController *p_this, Page *p_page);

	/*find a page by  page type*/
	Page * (*find_page_by_page_type)(PageSwitchController *p_this, int i_page_type);

	/*find a page by  page id*/
	Page * (*find_page_by_page_id)(PageSwitchController *p_this, int i_page_id);

	/*through p_model to create a page view*/
	PageView * (*create_page_view)(PageSwitchController *p_this, PageModel *p_model);

	/*show the current page*/
	void (*show_cur_page)(PageSwitchController *p_this);

	/*set current page*/
	void (*set_cur_page)(PageSwitchController *p_this, Page *p_page);

	/*show back page*/
	void (*go_back)(PageSwitchController *p_this);
	
	/*destroy the page switch controller, free its memory*/
	void (*destroy)(PageSwitchController *p_this);

};



#endif