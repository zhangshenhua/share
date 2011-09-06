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
#include "himage.h"

/*���캯���Ĺ���ģʽ			 */
/* 1.����������					 */
/* 2.Ϊ�����߷����ڴ�			 */
/* 3.��������					 */
/* 4.������Ϣ��ͼƬ�����֣�... ����ɫ�� */
/* 5.��������					 */
/* 6.����ָ��					 */	


VideoItem* VideoItem_new(Rect rect, char* cstr_image_path, char* cstr_title, int i_starlevel) {
	/* 1.����������					 */
	/* 2.Ϊ�����߷����ڴ�			 */
	VideoItem   *		p_vitem_result		=	hcontainer_new()		;				
	HImage      *    	p_himg_videoimage	=	himage_new()			;
	HImage      *    	p_himg_stars		=	himage_new()			;
	//HImage      *    	p_himg_detail		=	himage_new()			;
	HLabel      *	   	p_hlabel_title		=	hlabel_new()			; 		   
	HPushButton *		p_hpshbtn_detail	=	hpushbutton_new(NULL)	;

	
	/* 3.��������					 */
	p_vitem_result->base.p_widget_ops->set_position	( p_vitem_result, 0, 0 );
	p_vitem_result->base.p_widget_ops->set_width	( p_vitem_result, VIDEO_ITEM_WIDTH  );
	p_vitem_result->base.p_widget_ops->set_height	( p_vitem_result, VIDEO_ITEM_HEIGHT );

	p_himg_videoimage->p_oper->set_position			( p_himg_videoimage, 0, 0 );
	p_himg_videoimage->p_oper->set_size				(p_himg_videoimage, VIDEO_IMAGE_WIDTH, VIDEO_IMAGE_HEIGHT);

	


	/* 4.������Ϣ��ͼƬ�����֣�... , ��ɫ�� */

	/* 5.��������					 */
	p_vitem_result->p_container_ops->add_widget( p_vitem_result, p_himg_videoimage );
	p_vitem_result->p_container_ops->add_widget( p_vitem_result, p_himg_stars	   );
	p_vitem_result->p_container_ops->add_widget( p_vitem_result, p_hlabel_title	   );
	p_vitem_result->p_container_ops->add_widget( p_vitem_result, p_hpshbtn_detail  );
	/* 6.����ָ��					 */	
	return	p_vitem_result;

}

VideoItem* VideoItem1_new( VideoItem* p_vitem, int i_starlevel) {
// make vitem of copyright

}
VideoItem* VideoItem2_new( VideoItem* p_vitem, int i_starlevel) { // make vitem of none copyright

}


VideoItem_delete( VideoItem* p_it ) {

}


ListBox* ListBox_new( Rect rect ) {

} 

void ListBox_delete( ListBox* p_it ) {

}

void add_item_to_ListBox(ListBox* p_listbox, Item* p_item){}


