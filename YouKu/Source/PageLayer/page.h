/*********************************
*file: page.h
*author: yan.gao
*date: 2011.08.19
*description: 
***********************************/
#ifndef _PAGE_H_
#define _PAGE_H_

typedef struct _Page Page;
typedef struct _PageSwitchController PageSwitchController;
typedef struct _PageNode PageNode;

/*new a PageSwitchController instance*/
PageSwitchController * page_switch_controller_new();
void page_switch_controller_delete(PageSwitchController *p_this);

struct _PageNode {
	Page *p_page;
	PageNode *p_prev;
	PageNode *p_next;
};

struct _Page {
	/*user defined a unique id*/
	int i_page_id;

	/*the page type. user should define it*/
	int i_page_type;

	/*show this page */
	void (*fire_visible)(Page *p_this);

	/* hide this page */
	void (*fire_hiden)(Page *);

	/*destroy the page , free its memory*/
	void (*destroy)(Page *p_this);
};

struct _PageSwitchController {

	/************************  private member *****************************/

	/*a root page , does not belong to a specific page*/
	PageNode root_page_node;

	/*current page*/
	PageNode *p_cur_page_node;

	/* prev page node */
	PageNode *p_prev_page_node;

	/************************  public member *****************************/

	/* load a new page */
	void (*load_new_page)(PageSwitchController *p_this, Page *p_new_page);

	/*show page find from switch controller*/
	void (*load_exists_page)(PageSwitchController *p_this, Page *p_exist_page);

	/*load a page from the pages, and will free its memory*/
	void (*delete_page)(PageSwitchController *p_this, Page *p_page);

	/*find a page by  page type*/
	Page * (*find_page_by_page_type)(PageSwitchController *p_this, int i_page_type);

	/*find a page by  page id*/
	Page * (*find_page_by_page_id)(PageSwitchController *p_this, int i_page_id);

	/* get current page */
	Page * (*get_cur_page)(PageSwitchController *p_this);

	/*show back page*/
	void (*go_back)(PageSwitchController *p_this);

};



#endif