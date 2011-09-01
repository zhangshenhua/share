/*********************************
*file: page.h
*author: yan.gao
*date: 2011.08.19
*description: 
***********************************/
#ifndef _PAGE_H_
#define _PAGE_H_

#include "../Common/herrorcode.h"
#include "../Common/hlist.h"

typedef struct _Page Page;
typedef struct _PageSwitchController PageSwitchController;

/*new a PageSwitchController instance*/
PageSwitchController * page_switch_controller_new();

struct _Page {
	/*user defined a unique id*/
	int i_page_id;

	/*the page type. user should define it*/
	int i_page_type;

	/* Init page view, create widgets after page creating or resuming complete */
	void (*init)(Page *p_page);

	/*show this page */
	void (*fire_visible)(Page *p_this);

	/* hide this page */
	void (*fire_hide)(Page *);

	/*destroy the page , free its memory*/
	void (*destroy)(Page *p_this);
};

struct _PageSwitchController {

	/************************  private member *****************************/

	/*a root page , does not belong to a specific page*/
	hlist_node_t root_page_node;

	/*current page*/
	hlist_node_t *p_cur_page_node;

	/************************  public member *****************************/

	/*append a page after the current page*/
	void (*append_page)(PageSwitchController *p_this, Page *p_page);

	/*delete a page from the pages, and will free its memory*/
	void (*delete_page)(PageSwitchController *p_this, Page *p_page);

	/*find a page by  page type*/
	Page * (*find_page_by_page_type)(PageSwitchController *p_this, int i_page_type);

	/*find a page by  page id*/
	Page * (*find_page_by_page_id)(PageSwitchController *p_this, int i_page_id);

	/*show the current page*/
	void (*show_cur_page)(PageSwitchController *p_this);

	/*set current page*/
	void (*set_cur_page)(PageSwitchController *p_this, Page *p_page);

	/* get current page */
	Page * (*get_cur_page)(PageSwitchController *p_this);

	/*show back page*/
	void (*go_back)(PageSwitchController *p_this);
	
	/*destroy the page switch controller, free its memory*/
	void (*destroy)(PageSwitchController *p_this);

};



#endif