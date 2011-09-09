/*---------------------------------
* FILE Name: hlistbox.h
*
* Description: like its name, Hisoft ListBox
*
* COPYRIGHT
*
* Created on: 2011.9.5 Author: zhang.shenhua
*
* Modified by zsh on 2011.9.5
*---------------------------------------------------------------------*/

#ifndef _LISTBOX_H_
#define _LISTBOX_H_
#include "defines.h"
#include "hwidget.h"
#include "hcontainer.h"




typedef struct ListBox_  ListBox;

struct ListBox_ {
	//base
	HContainer* p_HContainer;
	
	/*private*/
	Rect		last_rect;
	//public:

};


extern ListBox* ListBox_new( Rect rect );
extern void ListBox_delete( ListBox* p_it );

extern void add_item_to_ListBox(ListBox* p_listbox, Item* p_item);





#endif