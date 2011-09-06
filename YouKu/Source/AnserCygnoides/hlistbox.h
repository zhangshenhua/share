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


typedef HWidget		Item;
typedef HContainer  VideoItem;
typedef VideoItem   VideoItem1;
typedef VideoItem   VideoItem2;

typedef HContainer  ListBox;


enum VideoType {NONE_TYPE,COPYRIGHT,NO_COPYRIGHT};

#if 0
classdef(VideoItem) {
	//base
	HContainer* p_HContainer_base;
	
	//HImage 

	struct  
	{
		void (*add_item)    ( VideoItem* p_this, Item* p_item );
		void (*remove_item)	( VideoItem* p_this, Item* p_item );  
	}public_ops;

};
#endif

extern VideoItem* VideoItem_new(Rect rect, char* cstr_image_path, char* cstr_title, int i_starlevel); 
extern VideoItem* VideoItem1_new( VideoItem* p_vitem, char* csrt_playedtimes); // make vitem of copyright
extern VideoItem* VideoItem2_new( VideoItem* p_vitem, char* cstr_timelong); // make vitem of none copyright

extern VideoItem_delete( VideoItem* p_it );


#if 0
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
#endif

extern ListBox* ListBox_new( Rect rect );
extern void ListBox_delete( ListBox* p_it );

extern void add_item_to_ListBox(ListBox* p_listbox, Item* p_item);





#endif