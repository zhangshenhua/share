/*****************************************************************
 * Name : herrorcode.h
 * Copyright : Hisoft
 * Author : caom gengxx
 * Date : 2011.05.09
 * Description : define error code for HList
 ******************************************************************/

#ifndef _HEFC_HLIST_HLIST_ERROR_CODE_H
#define _HEFC_HLIST_HLIST_ERROR_CODE_H

#ifdef __cplusplus
extern "C" {
#endif

#define hget_error_message(error_code) (*(ac_hlist_error_message + error_code))

enum {
	HERRORCODE_SUCCESS = 0,			/*no error*/
	HERRORCODE_MALLOC,				/*malloc null*/
	HERRORCODE_INVALID,				/*hlist null*/
	HERRORCODE_POSITION,			/*position smaller than 0 or beyond the length of list*/
	HERRORCODE_NODE_UNAVAILABLE,	/*key->i_is_avaiable is false*/
	HERRORCODE_NODE_NOT_FOUND,		/*can not find the node specified by user in list*/
	HERRORCODE_KEY_INVALID,			/*key null*/
	HERRORCODE_FUNCTION,			/*function null*/
	HERRORCODE_FIND_DATA,			/*in hlist_find function, the last parameter is null*/
	HERRORCODE_IS_EMPTY,			/*in hlist_is_empty function, the last parameter is null*/
	HERRORCODE_LENGTH,				/*in hlist_length function, the last parameter is null*/
};

extern const char *ac_hlist_error_message[];

#ifdef __cplusplus
}
#endif

#endif

/*****************************EOF***********************************/

