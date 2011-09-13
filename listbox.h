#ifndef _LISTBOX_H_
#define _LISTBOX_H_
#include "defines"

classdef(ListBox)
{

	
//public:
	void additem( ListBox* p_this, VideoItem* p_vitem);
			
};

extern ListBox* ListBox_new( Rect rect );
extern void ListBox_delete( ListBox* p_it );






#endif
