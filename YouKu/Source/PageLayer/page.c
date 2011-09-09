/*********************************
*file: page.h
*author: yan.gao
*date: 2011.08.24
*description: 
***********************************/

#include "page.h"
#include "vmsys.h"
#include "string.h"

#include "../WindowLayer/hwindow.h"

/* append page into Controller */
static void load_new_page(PageSwitchController *p_this, Page *p_page)
{
	PageNode *p_index = NULL;
	PageNode *p_tmp = NULL;
	PageNode *p_node;

	p_index = p_this->p_cur_page_node->p_next;

	p_this->p_prev_page_node = p_this->p_cur_page_node;

	p_node = (PageNode *)vm_malloc(sizeof(PageNode));
	p_node->p_page = p_page;

	p_node->p_next = &(p_this->root_page_node);
	p_node->p_prev = p_this->p_cur_page_node;
	p_this->p_cur_page_node->p_next = p_node;
	p_this->root_page_node.p_prev = p_node;

	while (p_index != &(p_this->root_page_node)) {
		p_tmp = p_index->p_next;

		/* free */
		if (p_index->p_page) {
			p_index->p_page->destroy(p_index->p_page);
			vm_free(p_index);
		}
		
		p_index = p_tmp;
	}

	if (p_this->p_cur_page_node != &p_this->root_page_node) {
		if (p_this->p_cur_page_node )
			p_this->p_cur_page_node->p_page->fire_hiden(p_this->p_cur_page_node->p_page);
	}
	
	p_page->fire_visible(p_page);
	p_this->p_cur_page_node = p_node;

	window->validate();
	window->repaint();
}

/* Delete page from Controller */
static void delete_page(PageSwitchController *p_this, Page *p_page)
{
	PageNode *p_index = NULL;
	PageNode *p_node_tmp = NULL;
	Page *p_page_tmp = NULL;

	if (p_page == p_this->p_cur_page_node->p_page)
		return;

	if (p_this->root_page_node.p_next)
		p_index = p_this->root_page_node.p_next;

	while (p_index != &(p_this->root_page_node)) {
		if (p_index->p_page == p_page) {
			p_index->p_prev->p_next = p_index->p_next;
			p_index->p_next->p_prev = p_index->p_prev;

			p_page_tmp = (Page *)p_index->p_page;
			p_page_tmp->destroy(p_page_tmp);
			vm_free(p_index);
		}

		p_index = p_index->p_next;
	}
}

/*find a page by  page type*/
static Page * find_page_by_page_type(PageSwitchController *p_this, int i_page_type)
{
	PageNode *p_index = NULL;
	PageNode *p_node_tmp = NULL;

	p_index = p_this->root_page_node.p_next;

	while (p_index != &(p_this->root_page_node)) {
		if (i_page_type == p_index->p_page->i_page_type)
			break;

		p_index = p_index->p_next;
	}
	

	return p_index->p_page;
}

/*find a page by  page id*/
static Page * find_page_by_page_id(PageSwitchController *p_this, int i_page_id)
{
	PageNode *p_index = NULL;
	PageNode *p_node_tmp = NULL;

	p_index = p_this->root_page_node.p_next;

	while (p_index != &(p_this->root_page_node)) {
		if (i_page_id == p_index->p_page->i_page_id)
			break;

		p_index = p_index->p_next;
	}


	return p_index->p_page;
}

/*show the current page*/
static void load_exists_page(PageSwitchController *p_this, Page *p_exist_page)
{
	Page *p_page = p_this->p_cur_page_node->p_page;

	if (p_this->p_cur_page_node->p_page) {
		if (p_page) 
			p_page->fire_hiden(p_page);

		p_this->p_prev_page_node = p_this->p_cur_page_node;
		p_this->p_cur_page_node->p_page = p_exist_page;
		p_exist_page->fire_visible(p_exist_page);

		window->validate();
		window->repaint();
	}
}

/*get current page*/
static Page * get_cur_page(PageSwitchController *p_this)
{
	return p_this->p_cur_page_node->p_page;
}

/*show back page*/
static void go_back(PageSwitchController *p_this)
{
	if (p_this->p_cur_page_node->p_prev != &(p_this->root_page_node)) {
		p_this->load_exists_page(p_this, p_this->p_cur_page_node->p_prev->p_page);
	}
}

/* Get page switch controller */
PageSwitchController *page_switch_controller_new()
{
	PageSwitchController *p_psc;
	p_psc = vm_malloc(sizeof(PageSwitchController));
	memset(p_psc, 0, sizeof(PageSwitchController));
	p_psc->load_new_page = load_new_page;
	p_psc->delete_page = delete_page;
	p_psc->find_page_by_page_id = find_page_by_page_id;
	p_psc->find_page_by_page_type = find_page_by_page_type;
	p_psc->load_exists_page = load_exists_page;
	p_psc->get_cur_page = get_cur_page;
	p_psc->go_back = go_back;

	p_psc->root_page_node.p_prev = &(p_psc->root_page_node);
	p_psc->root_page_node.p_next = &(p_psc->root_page_node);
	p_psc->root_page_node.p_page = NULL;
	p_psc->p_cur_page_node = &(p_psc->root_page_node);
	p_psc->p_prev_page_node = p_psc->p_cur_page_node;

	p_psc->p_cur_page_node = &p_psc->root_page_node;

	return p_psc;
}


void page_switch_controller_delete(PageSwitchController *p_this)
{
	PageNode *p_index;
	PageNode *p_tmp;

	if (!p_this)
		return;

	p_index = p_this->root_page_node.p_next;

	while (p_index != &(p_this->root_page_node)) {
		p_tmp = p_index->p_next;

		/* free */
		if (p_index->p_page) {
			p_index->p_page->destroy(p_index->p_page);
			vm_free(p_index);
		}

		p_index = p_tmp;
	}

	vm_free(p_this);
}