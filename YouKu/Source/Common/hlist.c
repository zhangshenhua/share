/************************************************************
 * Name : hlist.c
 * Copyright : Hisoft
 * Author : caom gengxx
 * Date : 2011.05.09
 * Description : the code of the hlist functions
 ***********************************************************/

#include "stdlib.h"
#include "vmsys.h"
#include "hlist.h"
#include "herrorcode.h"

/**********************************************************/

hlist_node_t * hlist_new()
{
	hlist_node_t *p_hlist = (hlist_node_t *)vm_malloc(sizeof(hlist_node_t));
	if (!p_hlist)
		return NULL;

	p_hlist->p_next = p_hlist;
	p_hlist->p_prev = p_hlist;
	p_hlist->pv_data = NULL;
	p_hlist->p_key = NULL;

	return p_hlist;
}

/***********************************************************/

int hlist_destroy(hlist_node_t **pp_hlist)
{
	hlist_node_t *p_first, *p_hlist = *pp_hlist;
	if (!pp_hlist || !(*pp_hlist))
		return HERRORCODE_INVALID;

	while ((p_hlist->p_next != p_hlist) && (p_hlist->p_prev != p_hlist)) {
		p_first = p_hlist->p_next;
		p_hlist->p_next = p_first->p_next;
		p_first->p_next->p_prev = p_hlist;

		if (p_first->p_key)
			p_first->p_key->i_is_available = LIST_NODE_UNAVAILABLE;

		if (p_first->pv_data)
			vm_free(p_first->pv_data);

		vm_free(p_first);
	}

	vm_free(p_hlist);
	*pp_hlist = NULL;

	return HERRORCODE_SUCCESS;
}

int hlist_clear(hlist_node_t *p_hlist)
{
	hlist_node_t *p_first;
	if (!p_hlist)
		return HERRORCODE_INVALID;

	while ((p_hlist->p_next != p_hlist) && (p_hlist->p_prev != p_hlist)) {
		p_first = p_hlist->p_next;
		p_hlist->p_next = p_first->p_next;
		p_first->p_next->p_prev = p_hlist;

		if (p_first->p_key)
			p_first->p_key->i_is_available = LIST_NODE_UNAVAILABLE;

		if (p_first->pv_data)
			vm_free(p_first->pv_data);

		vm_free(p_first);
	}

	return HERRORCODE_SUCCESS;
}

/****************************************************************************/

int hlist_append(hlist_node_t *p_hlist, void *pv_data, hlist_key_t *p_key)
{
	hlist_node_t *p_new;
	if (!p_hlist)
		return HERRORCODE_INVALID;

	p_new = (hlist_node_t *)vm_malloc(sizeof(hlist_node_t));
	if (!p_new)
		return HERRORCODE_MALLOC;

	if (p_key) {
		p_key->i_is_available = LIST_NODE_AVAILABLE;
		p_key->p_addr = p_new;
	}

	p_new->pv_data = pv_data;
	p_new->p_key = p_key;
	p_new->p_next = p_hlist;
	p_new->p_prev = p_hlist->p_prev;
	p_hlist->p_prev->p_next = p_new;
	p_hlist->p_prev = p_new;

	return HERRORCODE_SUCCESS;
}

/********************************************************************************/

int hlist_insert(hlist_node_t *p_hlist, void *pv_data, int i_position, hlist_key_t *p_key)
{
	int i_index = 0;
	hlist_node_t *p_index = p_hlist;
	hlist_node_t *p_new;

	if (!p_hlist)
		return HERRORCODE_INVALID;

	if (i_position < 0)
		return HERRORCODE_POSITION;

	do {
		if (i_index == i_position) {
			p_new = (hlist_node_t *)vm_malloc(sizeof(hlist_node_t));
			if (!p_new)
				return HERRORCODE_MALLOC;

			p_new->pv_data = pv_data;
			p_new->p_key = p_key;

			if (p_key) {
				p_key->i_is_available = LIST_NODE_AVAILABLE;
				p_key->p_addr = p_new;
			}

			p_new->p_next = p_index->p_next;
			p_new->p_prev = p_index;
			p_index->p_next->p_prev = p_new;
			p_index->p_next = p_new;

			return HERRORCODE_SUCCESS;
		}

		i_index++;
		p_index = p_index->p_next;
	} while (p_index != p_hlist);

	return HERRORCODE_POSITION;
}

/*************************************************************************/

int hlist_insert_by_key(hlist_node_t *p_hlist, void *pv_data, hlist_key_t *p_known_key, hlist_key_t *p_new_key)
{
	hlist_node_t *p_new;
	if (!p_hlist)
		return HERRORCODE_INVALID;

	if (!p_known_key)
		return HERRORCODE_KEY_INVALID;
	
	if (LIST_NODE_AVAILABLE != p_known_key->i_is_available)
		return HERRORCODE_NODE_UNAVAILABLE;

	p_new = (hlist_node_t *)vm_malloc(sizeof(hlist_node_t));
	if (!p_new)
		return HERRORCODE_MALLOC;

	p_new->pv_data = pv_data;
	p_new->p_key = p_new_key;
	p_new->p_next = p_known_key->p_addr;
	p_new->p_prev = p_known_key->p_addr->p_prev;

	if (p_new_key) {
		p_new_key->i_is_available = LIST_NODE_AVAILABLE;
		p_new_key->p_addr = p_new;
	}

	/*modify the prev->next pointer first, then modify the p_addr->p_prev itself. important!!!*/
	p_known_key->p_addr->p_prev->p_next = p_new;
	p_known_key->p_addr->p_prev = p_new;

	return HERRORCODE_SUCCESS;
}

/****************************************************************************/

int hlist_remove(hlist_node_t *p_hlist, void *pv_data)
{
	hlist_node_t *p_index;
	if (!p_hlist)
		return HERRORCODE_INVALID;

	p_index = p_hlist->p_next;
	while (p_index != p_hlist) {
		if (pv_data == p_index->pv_data) {
			p_index->p_prev->p_next = p_index->p_next;
			p_index->p_next->p_prev = p_index->p_prev;

			if (p_index->p_key)
				p_index->p_key->i_is_available = LIST_NODE_UNAVAILABLE;

			//TODO: at here we can't do this

			if (p_index->pv_data)
				vm_free(p_index->pv_data);

			vm_free(p_index);

			return HERRORCODE_SUCCESS;
		}

		p_index = p_index->p_next;
	}

	return HERRORCODE_NODE_NOT_FOUND;
}

/***************************************************************************/

int hlist_remove_all(hlist_node_t *p_hlist, void *pv_data, hequal_func_t equal_func_PF)
{
	int i_is_node_found = 0;
	hlist_node_t *p_temp, *p_index;
	if (!p_hlist)
		return HERRORCODE_INVALID;
	
	if (!equal_func_PF)
		return HERRORCODE_FUNCTION;

	p_index = p_hlist->p_next;
	while (p_index != p_hlist) {
		if (equal_func_PF(pv_data, p_index->pv_data)) {
			i_is_node_found = 1;
			p_index->p_prev->p_next = p_index->p_next;
			p_index->p_next->p_prev = p_index->p_prev;

			if (p_index->p_key)
				p_index->p_key->i_is_available = LIST_NODE_UNAVAILABLE;

			if (p_index->pv_data)
				vm_free(p_index->pv_data);

			p_temp = p_index->p_next;
			vm_free(p_index);
			p_index = p_temp;
			continue;
		}

		p_index = p_index->p_next;
	}

	if (i_is_node_found)
		return HERRORCODE_SUCCESS;
	else
		return HERRORCODE_NODE_NOT_FOUND;
}

/*****************************************************************************/

int hlist_remove_by_position(hlist_node_t *p_hlist, int i_position)
{
	int i_index = 0;
	hlist_node_t *p_index ;
	if (!p_hlist)
		return HERRORCODE_INVALID;

	if (i_position < 0)
		return HERRORCODE_POSITION;

	p_index = p_hlist->p_next;
	while (p_index != p_hlist) {
		if (i_index == i_position) {
			p_index->p_prev->p_next = p_index->p_next;
			p_index->p_next->p_prev = p_index->p_prev;

			if (p_index->p_key)
				p_index->p_key->i_is_available = LIST_NODE_UNAVAILABLE;

			if (p_index->pv_data)
				vm_free(p_index->pv_data);

			vm_free(p_index);

			return HERRORCODE_SUCCESS;
		}

		i_index++;
		p_index = p_index->p_next;
	}

	return HERRORCODE_POSITION;
}

/*********************************************************************/

int hlist_remove_by_key(hlist_node_t *p_hlist, hlist_key_t *p_key)
{
	if (!p_hlist)
		return HERRORCODE_INVALID;

	if (!p_key)
		return HERRORCODE_KEY_INVALID;

	if (p_key->i_is_available == LIST_NODE_AVAILABLE) {
		p_key->p_addr->p_prev->p_next = p_key->p_addr->p_next;
		p_key->p_addr->p_next->p_prev = p_key->p_addr->p_prev;

		p_key->p_addr->p_key->i_is_available = LIST_NODE_UNAVAILABLE;

		if (p_key->p_addr->pv_data)
			vm_free(p_key->p_addr->pv_data);

		vm_free(p_key->p_addr);

		return HERRORCODE_SUCCESS;
	}

	return HERRORCODE_NODE_UNAVAILABLE;
}

void hlist_remove_by_node(hlist_node_t *p_node)
{
	if (!p_node)
        return;

    p_node->p_prev->p_next = p_node->p_next;
    p_node->p_next->p_prev = p_node->p_prev;

    if (p_node->pv_data)
        vm_free(p_node->pv_data);

    vm_free(p_node);
}

/*****************************************************************************/

int hlist_find(hlist_node_t *p_hlist, void *p_compare_data, hequal_func_t equal_func_PF, void **pv_data)
{
	hlist_node_t *p_index;
	if (!p_hlist)
		return HERRORCODE_INVALID;
	
	if (!equal_func_PF)
		return HERRORCODE_FUNCTION;

	if (!pv_data)
		return HERRORCODE_FIND_DATA;

	p_index = p_hlist->p_next;

	while (p_index != p_hlist) {
		if (equal_func_PF(p_compare_data, p_index->pv_data)) {
			*pv_data = p_index->pv_data;

			return HERRORCODE_SUCCESS;
		}
		else
			p_index = p_index->p_next;
	}

	return HERRORCODE_NODE_NOT_FOUND;
}

/*****************************************************************************/

int hlist_length(hlist_node_t *p_hlist)
{
	int i_length;
	hlist_node_t *p_index ;
	if (!p_hlist)
		return -1;

	p_index = p_hlist->p_next;

	for (i_length = 0; 
		p_index != p_hlist; 
		++i_length, p_index = p_index->p_next) ;

	return i_length;
}

/*****************************************************************************/

int hlist_is_empty(hlist_node_t *p_hlist)
{
	if (!p_hlist)
		return -1;

	return (p_hlist->p_next == p_hlist);
}

/***********************************************************************/

int hlist_foreach(hlist_node_t *p_hlist, hoperate_func_t operate_func_PF, void *p_user_data)
{
	hlist_node_t *p_index;
	if (!p_hlist)
		return HERRORCODE_INVALID;

	if(!operate_func_PF)
		return HERRORCODE_FUNCTION;

	p_index = p_hlist->p_next;
	while (p_index != p_hlist) {
		operate_func_PF(p_user_data, p_index->pv_data);
		p_index = p_index->p_next;
	}

	return HERRORCODE_SUCCESS;
}


/************************************************************************/


/**********************************EOF***********************************/





