/*---------------------------------
* FILE Name: videoitem.c
*
* Description: like its name, Hisoft ListBox
*
* COPYRIGHT
*
* Created on: 2011.9.7 Author: zhang.shenhua
*
* Modified by zsh on 2011.9.7
*---------------------------------------------------------------------*/
#include "defines.h"
#include "hwidget.h"
#include "hcontainer.h"
#include "videoitem.h"
#include "new.h"
#include "hlabel.h"
#include "hpushbutton.h"
#include "tools.h"

static void callback_videoitem(HWidget *p_source, HEvent *p_evt, void *p_param);
static void callback_videodetail(HWidget *p_source, HEvent *p_evt, void *p_param);

/*构造函数的工作模式					*/
/* 1.声明参与者							*/
/* 2.为参与者分配内存					*/
/* 3.设置坐标							*/
/* 4.加载信息（图片，文字，... ，颜色） */
/* 5.设置回调入口						*/
/* 6.加入容器							*/
/* 7.返回指针							*/	


VideoItem* VideoItem_new( VID vid, P_Image p_himg_video, char* cstr_title, float f_starlevel) {
#if 0


	
	/* 1.声明参与者					 */
	/* 2.为参与者分配内存			 */


	VideoItem   *		p_vitem_result		=	checked_malloc(sizeof(VideoItem));

	HImage      *    	p_himg_videoimage	=	p_himg_video			;
	HImage      *    	p_himg_stars		=	himage_new()			;
	HLabel      *	   	p_hlabel_title		=	hlabel_new()			; 		   
	HPushButton *		p_hpshbtn_detail	=	hpushbutton_new(NULL)	;
	p_vitem_result->p_base_container		=	hcontainer_new()		;

	/* 3.设置坐标					 */
	p_vitem_result->p_base_container->base.p_widget_ops->set_position	( p_vitem_result,		0, 0 );
	p_vitem_result->p_base_container->base.p_widget_ops->set_width		( p_vitem_result,		VIDEO_ITEM_WIDTH  );
	p_vitem_result->p_base_container->base.p_widget_ops->set_height		( p_vitem_result,		VIDEO_ITEM_HEIGHT );

	p_himg_videoimage->p_oper->set_position				( p_himg_videoimage,	VIDEO_IMAGE_LEFT,  VIDEO_IMAGE_TOP );
	p_himg_videoimage->p_oper->set_size					( p_himg_videoimage,	VIDEO_IMAGE_WIDTH, VIDEO_IMAGE_HEIGHT);

	p_himg_stars->p_oper->set_position					( p_himg_stars,			VIDEO_STARS_LEFT,  VIDEO_STARS_TOP );
	p_himg_stars->p_oper->set_size						( p_himg_stars,			VIDEO_STARS_WIDTH, VIDEO_STARS_HEIGHT);

	p_hlabel_title->base.p_widget_ops->set_position		( p_hlabel_title,		VIDEO_TITLE_LEFT, VIDEO_TITLE_LEFT );
	p_hlabel_title->base.p_widget_ops->set_width		( p_hlabel_title,		VIDEO_TITLE_WIDTH  );
	p_hlabel_title->base.p_widget_ops->set_height		( p_hlabel_title,		VIDEO_TITLE_HEIGHT );

	p_hpshbtn_detail->base.p_widget_ops->set_position	( p_hpshbtn_detail,		VIDEO_DETAIL_LEFT, VIDEO_DETAIL_LEFT );
	p_hpshbtn_detail->base.p_widget_ops->set_width		( p_hpshbtn_detail,		VIDEO_DETAIL_WIDTH  );
	p_hpshbtn_detail->base.p_widget_ops->set_height		( p_hpshbtn_detail,		VIDEO_DETAIL_HEIGHT );

	/* 4.加载信息（图片，文字，... , 颜色） */
	p_himg_stars->p_oper->load_image(p_himg_stars, getStarsImg( f_starlevel ));

	p_hlabel_title->set_text( p_hlabel_title , cstr_title );

	p_hpshbtn_detail->ops->set_image_from_resource( p_hpshbtn_detail, DETAIL_PUSHBUSH_IMG_PATHNAME);

	/* 5.设置回调入口						*/

	p_vitem_result->p_base_container->base.action_performed   = callback_videoitem;
	p_hpshbtn_detail->base.action_performed = callback_videodetail;

	/* 6.加入容器							*/
	p_vitem_result->p_base_container->p_container_ops->add_widget( p_vitem_result, p_himg_videoimage	);
	p_vitem_result->p_base_container->p_container_ops->add_widget( p_vitem_result, p_himg_stars			);
	p_vitem_result->p_base_container->p_container_ops->add_widget( p_vitem_result, p_hlabel_title		);
	p_vitem_result->p_base_container->p_container_ops->add_widget( p_vitem_result, p_hpshbtn_detail		);

	/* 7.返回指针							*/	
#else
	VideoItem   *		p_vitem_result		=	checked_malloc(sizeof(VideoItem));
	p_vitem_result->p_base_container		=	hcontainer_new();
	
	p_vitem_result->p_base_container->p_container_ops->add_widget(
		p_vitem_result->p_base_container,
		set_widget_rect(p_himg_video, 
						&Rect(VIDEO_IMAGE_LEFT, VIDEO_IMAGE_TOP, VIDEO_IMAGE_WIDTH, VIDEO_IMAGE_HEIGHT));
		
		

	p_vitem_result->p_base_container->p_container_ops->add_widget(
		p_vitem_result->p_base_container,
		make_starlevel( f_starlevel ),
		);


	

#endif

	
	return	p_vitem_result;

}

static void callback_videoitem(HWidget *p_source, HEvent *p_evt, void *p_param)
{
	/* 直接播放 */
	play( ((VideoItem*)p_source).vid );
}

static void callback_videodetail(HWidget *p_source, HEvent *p_evt, void *p_param)
{
	/* 进入详情页 */
	show( VideoDetail_new( (VideoItem*)p_source->p_parent).vid ) );


}

VideoItem* VideoItem1_new( VideoItem* p_vitem, char* csrt_playedtimes ) {
	// make vitem of copyright

}
VideoItem* VideoItem2_new( VideoItem* p_vitem, char* cstr_timelong) { // make vitem of none copyright
	// make vitem of none copyright

}
