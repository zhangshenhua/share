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
#include "tools.h"

typedef HWidget		Item;
typedef HContainer*  P_ListBox;


extern P_ListBox ListBox_new( A_Rect* p_rect );
extern void ListBox_delete( P_ListBox p_it );

extern void add_item_to_ListBox(P_ListBox p_listbox, Item* p_item);





#endif