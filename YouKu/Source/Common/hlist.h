/*********************************************************************
 * Name : hlist.h
 * Copyright : Hisoft
 * Author : caom gengxx
 * Date : 2011.05.09
 * Descrption : A foundation library of list for Hisoft
 *********************************************************************/

#ifndef _HEFC_HLIST_HLIST_H
#define _HEFC_HLIST_HLIST_H

#include"herrorcode.h"

#ifdef __cplusplus
extern "C" {
#endif

/*value for hlist_key->i_is_available*/
#define LIST_NODE_AVAILABLE 	0x01010101
#define LIST_NODE_UNAVAILABLE	0x02020202

/***************************data stucture*****************************/

typedef struct _hlist_node_t hlist_node_t;

/*Key of node store node's available flag and the address of node*/
typedef struct _hlist_key_t{
	int i_is_available;		/*mark the node p_addr pointed to is LIST_NODE_AVAILABLE or LIST_NODE_UNAVAILABLE*/
	hlist_node_t *p_addr;		/*point to a list node so that user can find the node by O(1)*/
}hlist_key_t;

/*List node struct*/
struct _hlist_node_t{
	void *pv_data;			/*store data*/
	hlist_node_t *p_next;		/*point to next node*/
	hlist_node_t *p_prev;		/*point to previous node*/
	hlist_key_t *p_key;		/*point to key, can be NULL if user don't want to known the address of this node*/
};

/*Function pointer that defined by user when calling the hlist_foreach function*/
typedef void (*hoperate_func_t) (void *pv_user_data, void *pv_node_data);

/*Function pointer defined by user that judge two datas are the same*/
typedef int (*hequal_func_t) (void *pv_user_data, void *pv_node_data);

/**************************function declaration***********************/

/*
 * name : hlist_new
 * description : create a new list
 * return : address of the new list's head address
 */
hlist_node_t * hlist_new();

/*
 * name : hlist_destroy
 * description : destroy a list
 * @para(in/out) p_hlist : will set to NULL if destroy succeed
 * return : herrorcode
 */
int hlist_destroy(hlist_node_t **p_hlist);

/************************************************************************/
/* clear all nodes in the hlist                                                                     */
/************************************************************************/
int hlist_clear(hlist_node_t *p_hlist);

/*
 * name : hlist_append
 * description : append a new node at the end of the list
 * @para(in/out) p_hlist : pointer of a list
 * @para(in) p_data : data that would be appended at the the list
 * @para(in/out) p_key : key pointer of the appended node, if user don't want the node's address, set it to NULL.
 * return : herror_code
 */
int hlist_append(hlist_node_t *p_hlist, void *pv_data, hlist_key_t *p_key);

/*
 * name : hlist_insert
 * description : insert a new node before a specified node
 * @para(in/out) p_hlist : pointer of a list
 * @para(in) p_data : data that would insert to the list
 * @para(in) i_position : position that the data would insert before (the first node position is 0.)
 * @para(in/out) p_key : key pointer of the inserted node, if user don't want the node's address, set it to NULL.
 * return : herror_code
 */
int hlist_insert(hlist_node_t *p_hlist, void *pv_data, int i_position, hlist_key_t *p_key);


/*
 * name: hlist_remove_by_node
 * description: remove the node from list
 * @para(in) p_node: the needed to bo remove node
 */
void hlist_remove_by_node(hlist_node_t *p_node);

/*
 * name : hlist_insert_by_key
 * description : insert a new node before a specified node confirmed by a known key
 * @para(in/out) p_hlist : pointer of a list
 * @para(in) p_data : data that would insert to the list
 * @para(in) p_known_key : known key specified the node which the data would be inserted before
 * @para(in/out) p_new_key : key pointer of the inserted node, if user don't want the node's address, set it to NULL.
 * return : herror_code
 */
int hlist_insert_by_key(hlist_node_t *p_hlist, void *pv_data, hlist_key_t *p_known_key, hlist_key_t *p_new_key);

/*
 * name : hlist_remove
 * description : remove first node from list according to the condition of the user-defined function
 * @para(in/out) p_hlist : pointer of a list
 * @para(in) p_data : compare data for the user-defined function
 * @para(in) p_compare_func : a function defined by user which judge the datas in the list
 * 								and the compare data are the same
 * return : herror_code
 */
int hlist_remove(hlist_node_t *p_hlist, void *pv_data);

/*
 * name : hlist_remove_all
 * descrition : remove all nodes from list according to the condition of the user-defined function
 * @para(in/out) p_hlist : pointer of a list
 * @para(in) p_data : compare data
 * @para(in) p_equal_func : a function defined by user which judge the datas in the list
 * 								and the compare data are the same
 * return : herror_code
 */
int hlist_remove_all(hlist_node_t *p_hlist, void *pv_data, hequal_func_t equal_func_PF);

/*
 * name : hlist_remove_by_position
 * description : remove a node from the list at the specified position
 * @para(in/out) p_hlist : pointer of a list
 * @para(in) i_position : position that specified which node would be removed
 * return : herror_code
 */
int hlist_remove_by_position(hlist_node_t *p_hlist, int i_position);

/*
 * name : hlist_remove_by_key
 * description : remove a specified node that confirmed by a known key
 * @para(in/out) p_hlist : pointer of a list
 * @para(in) p_key : key that specified the address of the node to be removed
 * return : herror_code
 */
int hlist_remove_by_key(hlist_node_t *p_hlist, hlist_key_t *p_key);

/*
 * name : hlist_find
 * description : find the first node of list according to the condition of the user-defined function
 * @para(in) p_hlist : pointer of a list
 * @para(in) p_compare_data : compare data
 * @para(in) compare_func : a function defined by user which judge the datas in the list
 * 								and the compare data are the same
 * @para(in/out) p_data : data of found node
 * return : herror_code
 */
int hlist_find(hlist_node_t *p_hlist, void *pv_compare_data, hequal_func_t equal_func_PF, void **pv_data);

/*
 * name : hlist_length
 * description : get the length of the list
 * @para(in) p_hlist : pointer of a list
 * return : the length of the list, -1 if null list
 */
int hlist_length(hlist_node_t *p_hlist);

/*
 * name : hlist_is_empty
 * description : judge if the list is empty
 * @para(in) p_hlist : pointer of a list
 * return : 1 if the list is empty, else 0
 */
int hlist_is_empty(hlist_node_t *p_hlist);

/*
 * name : hlist_foreach
 * description : operate every node of the list by the user-defined function
 * @para(in) p_hlist : pointer of a list
 * @para(in) operate_func_PF : a function defined by user which operate every data in the node of list
 * @para(in/out) p_user_data : a data for the user-defined function to operate with data in node of the list
 * return : herror_code
 */
int hlist_foreach(hlist_node_t *p_hlist, hoperate_func_t operate_func_PF, void *pv_user_data);

/***************************functin declaration end***********************/

#define hlist_for_each(p_node, p_hlist) \
        for (p_node = p_hlist->p_next; p_node != p_hlist; p_node = p_node->p_next)

#ifdef __cplusplus
}
#endif

#endif

/********************************EOF**************************************/


