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

typedef HRect Rect;
typedef HWidget Item;

classdef(VideoItem) {
	//base
	HContainer* p_HContainer_base;
	
	Rect rect_image ;

	struct  
	{
		void (*add_item)    ( VideoItem* p_this, Item* p_item );
		void (*remove_item)	( VideoItem* p_this, Item* p_item );  
	}public_ops;

};

extern VideoItem_new();
extern VideoItem_delete( VideoItem* p_it );


classdef(ListBox) {
	//base
	HContainer* p_HContainer_base;


	//public:
	struct  
	{
		void (*add_item)    ( ListBox* p_this, Item* p_item );
		void (*remove_item)	( ListBox* p_this, Item* p_item );  
	}public_ops;
	
	
};

extern ListBox* ListBox_new( Rect rect );
extern void ListBox_delete( ListBox* p_it );






#endif