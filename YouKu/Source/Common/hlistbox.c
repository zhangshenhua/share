/*---------------------------------
* FILE Name: hlistbox.c
*
* Description: like its name, Hisoft ListBox
*
* COPYRIGHT
*
* Created on: 2011.9.5 Author: zhang.shenhua
*
* Modified by zsh on 2011.9.5
*---------------------------------------------------------------------*/
#include "hlistbox.h"
#include "new.h"
#include "hwidget.h"
#include "hpushbutton.h"
#include "hcontainer.h"
#include "himage.h"
#include "tools.h"
#include "HVerticalLayout.h"
#include "hwindow.h"

P_ListBox ListBox_new( A_Rect* p_rect ) {
	P_ListBox p_listbox = hcontainer_new();
	HVerticalLayout* p_vertical_layout =  hvertical_layout_new();
	LayoutGap	gap = {0,0};

	set_widget_rect((HWidget*)p_listbox, p_rect );

	p_vertical_layout->st_layout_base.p_ops->set_gap(
		(HLayout*)p_vertical_layout, 
		gap	);

	p_listbox->p_container_ops->set_layout(p_listbox, (HLayout *)p_vertical_layout);
	
	return p_listbox;
} 

void ListBox_delete( P_ListBox p_it ) {
	hcontainer_delete( p_it );
}

void add_item_to_ListBox(P_ListBox p_listbox, Item* p_item){
	p_listbox->p_container_ops->add_widget( p_listbox, p_item );
	window->validate();

}


