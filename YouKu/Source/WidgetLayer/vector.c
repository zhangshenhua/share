#include "vector.h"
#include "vmstdlib.h"
#include "string.h"

void vector_append_node(Vector *p_vec, VectorNode *p_node)
{
	VectorNode *p_tail;
	p_tail = p_vec->head.p_prev;
	p_tail->p_next = p_node;
	p_node->p_prev = p_tail;
	p_node->p_next = &p_vec->head;
	p_vec->head.p_prev = p_node;
	++ p_vec->i_size;
}

VectorNode * vector_append_data(Vector *p_vec, void *pv_data)
{
	VectorNode *p_node;
	p_node = vector_node_new(pv_data);
	vector_append_node(p_vec, p_node);
	return p_node;
}

void vector_insert_node(Vector *p_vec, VectorNode *p_pos, VectorNode *p_node)
{
	p_pos->p_prev->p_next = p_node;
	p_node->p_prev = p_pos->p_prev;
	p_pos->p_prev = p_node;
	p_node->p_next = p_pos;
	++ p_vec->i_size;
}

VectorNode * vector_insert_data(Vector *p_vec, int i_pos, void *pv_data)
{
	VectorNode *p_node;
	VectorNode *p_pos;
	p_node = vector_node_new(pv_data);
	p_pos = vector_node_at(p_vec, i_pos);
	vector_insert_node(p_vec, p_pos, p_node);
	return p_node;
}

void vector_take_down(Vector *p_vec, VectorNode *p_node)
{
	VectorNode *pp;
	VectorNode *pn;
	pp = p_node->p_prev;
	pn = p_node->p_next;
	pp->p_next = pn;
	pn->p_prev = pp;
	vector_node_init(p_node);
	-- p_vec->i_size;
}

void vector_remove_node(Vector *p_vec, VectorNode *p_node)
{
	vector_take_down(p_vec, p_node);
	if (p_vec->destroy)
		p_vec->destroy(p_node->pv_data);
	else
		vm_free(p_node->pv_data);
	vm_free(p_node);
	-- p_vec->i_size;
}

void vector_remove_data(Vector *p_vec, void *pv_data)
{
	VectorNode *p_node;
	p_node = vector_node_of(p_vec, pv_data);
	vector_remove_node(p_vec, p_node);
}

int vector_index_of(Vector *p_vec, void *pv_data)
{
	VectorNode *p_pos;
	int i_idx = 0;
	vector_for_each(p_pos, p_vec) {
		if (p_pos->pv_data == pv_data)
			return i_idx;
		++ i_idx;
	}
	return -1;
}

VectorNode * vector_node_of(Vector *p_vec, void *pv_data)
{
	VectorNode *p_pos;
	vector_for_each(p_pos, p_vec) {
		if (p_pos->pv_data == pv_data)
			return p_pos;
	}
	return NULL;
}

VectorNode *vector_node_at(Vector *p_vec, int i_pos)
{
	VectorNode *p_pos;
	if (i_pos < 0)
		return NULL;
	vector_for_each(p_pos, p_vec) {
		if (!i_pos)
			return p_pos;
		-- i_pos;
	}
	return NULL;
}

void vector_clear(Vector *p_vec)
{
	VectorNode *p_pos;
	VectorNode *p_n;
	vector_for_each_safe(p_pos, p_n, p_vec) {
		vector_remove_node(p_vec, p_pos);
	}
	vector_node_init(&p_vec->head);
	p_vec->i_size = 0;
}

Vector *vector_new( void (*destroy)(void *) )
{
	Vector *p_vec;
	p_vec = vm_malloc(sizeof(Vector));
	memset(p_vec, 0, sizeof(Vector));
	vector_node_init(&p_vec->head);
	p_vec->destroy = destroy;
	return p_vec;
}

void vector_delete(Vector *p_vec)
{
	vector_clear(p_vec);
	vm_free(p_vec);
}

VectorNode *vector_node_new(void *pv_data)
{
	VectorNode *p_node;
	p_node = vm_malloc(sizeof(VectorNode));
	memset(p_node, 0, sizeof(VectorNode));
	vector_node_init(p_node);
	p_node->pv_data = pv_data;
	return p_node;
}