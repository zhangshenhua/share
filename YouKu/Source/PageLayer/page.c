/*********************************
*file: page.h
*author: yan.gao
*date: 2011.08.24
*description: 
***********************************/

#include "page.h"
#include "vmsys.h"
#include "string.h"

/* append page into Controller */
static void append_page(PageSwitchController *p_this, Page *p_page)
{
	hlist_node_t *p_index = NULL;
	hlist_node_t *p_tmp = NULL;
	Page *p_page_tmp;

	hlist_append(&p_this->root_page_node, (void *)p_page, NULL);
	p_this->p_cur_page_node->pv_data = p_page;

	/* delete page after current pages */
	p_index = p_this->p_cur_page_node->p_next;

	while (p_index != &(p_this->root_page_node)) {
		p_tmp = p_index->p_next;

		/* free */
		if (p_index->pv_data) {
			p_page_tmp = (Page *)p_index->pv_data;
			p_page_tmp->destroy(p_page_tmp);
			vm_free(p_index);
		}
		
		p_index = p_tmp;
	}
}

/* Delete page from Controller */
static void delete_page(PageSwitchController *p_this, Page *p_page)
{
	hlist_node_t *p_index = NULL;
	hlist_node_t *p_node_tmp = NULL;
	Page *p_page_tmp = NULL;

	if (p_this->root_page_node.p_next)
		p_index = p_this->root_page_node.p_next;

	while (p_index != &(p_this->root_page_node)) {
		if (p_index->pv_data == p_page) {
			p_index->p_prev->p_next = p_index->p_next;
			p_index->p_next->p_prev = p_index->p_prev;

			p_page_tmp = (Page *)p_index->pv_data;
			p_page_tmp->destroy(p_page_tmp);
			vm_free(p_index);
		}

		p_index = p_index->p_next;
	}
}

/*find a page by  page type*/
static Page * find_page_by_page_type(PageSwitchController *p_this, int i_page_type)
{
	hlist_node_t *p_index = NULL;
	hlist_node_t *p_node_tmp = NULL;
	Page *p_page = NULL;

	if (p_this->root_page_node.p_next)
		p_index = p_this->root_page_node.p_next;

	while (p_index != &(p_this->root_page_node)) {
		p_page = (Page *)p_index->pv_data;
		if (i_page_type == p_page->i_page_type)
			break;

		p_index = p_index->p_next;
	}
	

	return p_page;
}

/*find a page by  page id*/
static Page * find_page_by_page_id(PageSwitchController *p_this, int i_page_id)
{
	hlist_node_t *p_index = NULL;
	hlist_node_t *p_node_tmp = NULL;
	Page *p_page = NULL;

	if (p_this->root_page_node.p_next)
		p_index = p_this->root_page_node.p_next;

	while (p_index != &(p_this->root_page_node)) {
		p_page = (Page *)p_index->pv_data;
		if (i_page_id == p_page->i_page_id)
			break;

		p_index = p_index->p_next;
	}


	return p_page;
}

/*show the current page*/
static void show_cur_page(PageSwitchController *p_this)
{
	Page *p_cur_page = (Page *)p_this->p_cur_page_node->pv_data;
	p_cur_page->fire_visible(p_cur_page);
}

/*set current page*/
static void set_cur_page(PageSwitchController *p_this, Page *p_page)
{
	p_this->p_cur_page_node->pv_data = p_page;
}

/*get current page*/
static Page * get_cur_page(PageSwitchController *p_this)
{
	return (Page *)p_this->p_cur_page_node->pv_data;
}

/*show back page*/
static void go_back(PageSwitchController *p_this)
{
	if (p_this->p_cur_page_node->p_prev != &(p_this->root_page_node))
		p_this->p_cur_page_node = p_this->p_cur_page_node->p_prev;
}

/*destroy the page switch controller, free its memory*/
static void destroy(PageSwitchController *p_this)
{
	if (p_this->p_cur_page_node->p_next != &(p_this->root_page_node))
		p_this->p_cur_page_node = p_this->p_cur_page_node->p_next;
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
	p_psc->show_cur_page = show_cur_page;
	p_psc->set_cur_page = set_cur_page;
	p_psc->get_cur_page = get_cur_page;
	p_psc->go_back = go_back;
	p_psc->destroy = destroy;

	p_psc->p_cur_page_node = &p_psc->root_page_node;

	return p_psc;
}