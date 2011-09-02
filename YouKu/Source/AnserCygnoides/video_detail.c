/**********************************************************************
* FILE Name: vedio_detail.c
* Description: 
* COPYRIGHT: Hisoft
* Created on: $(9.2.2011) Author: Yan Guanglei
***********************************************************************/
/***** Class Definition *********/
#include "video_detail.h"

void Video_Detail_Visible() 
{
	
}
static void down() 
{
	
}
void up() 
{
	
}
void collect() 
{
	
}
void share() 
{
	
}
void HD_play_video() 
{
	
}
void ChangeToVideoDetailPage() 
{
	
}
void play_video() 
{
	
}
void ChangeToMyPage() 
{
	
}
void ChangeToTopPage() 
{
	
}
void ChangeToSearchPage() 
{
	
}
void ChangeToTypePage() 
{
	
}
void ChangeToRecommandPage() 
{
	
}
/*video detail page init*/
void video_detail_page_init( VideoDetailPage* pvideo_detail_page ) 
{
	
	pvideo_detail_page->video_detail.i_page_id=    ;
	pvideo_detail_page->video_detail.i_page_type   ;
	pvideo_detail_page->video_detail.fire_visible=Video_Detail_Visible();

	pvideo_detail_page->precommand_Button=hpushbutton_new();
	if (!pvideo_detail_page->precommand_Button)
	{
		return NULL;
	}
	pvideo_detail_page->precommand_Button->base.s_top_x=Recommand_Button_X;
	pvideo_detail_page->precommand_Button->base.s_top_y=Recommand_Button_Y;
	pvideo_detail_page->precommand_Button->base.s_height=Recommand_Button_Height;
	pvideo_detail_page->precommand_Button->base.s_width=Recommand_Button_Width;
	pvideo_detail_page->precommand_Button.ops->set_image_from_resource(pvideo_detail_page->precommand_Button,"recommand file");
	pvideo_detail_page->precommand_Button->base.action_performed=ChangeToRecommandPage();
	
	pvideo_detail_page->ptype_Button=hpushbutton_new();
	if (!pvideo_detail_page->ptype_Button)
	{
		return NULL;
	}
	pvideo_detail_page->ptype_Button->base.s_top_x=Type_Button_X;
	pvideo_detail_page->ptype_Button->base.s_top_y=Type_Button_Y;
	pvideo_detail_page->ptype_Button->base.s_height=Type_Button_Height;
	pvideo_detail_page->ptype_Button->base.s_width=Type_Button_Width;
	pvideo_detail_page->ptype_Button->ops->set_image_from_resource(pvideo_detail_page->ptype_Button,"type file");
	pvideo_detail_page->ptype_Button->base.action_performed=ChangeToTypePage();

	pvideo_detail_page->psearch_Button=hpushbutton_new();
	if (!pvideo_detail_page->psearch_Button)
	{
		return NULL;
	}
	pvideo_detail_page->psearch_Button->base.s_top_x=Search_Button_X;
	pvideo_detail_page->psearch_Button->base.s_top_y=Search_Button_Y;
	pvideo_detail_page->psearch_Button->base.s_height=Search_Button_Height;
	pvideo_detail_page->psearch_Button->base.s_width=Search_Button_Width;
	pvideo_detail_page->psearch_Button->ops->set_image_from_resource(pvideo_detail_page->psearch_Button,"search file");
	pvideo_detail_page->psearch_Button->base.action_performed=ChangeToSearchPage();

	pvideo_detail_page->ptop_Button=hpushbutton_new();
	if (!pvideo_detail_page->ptop_Button)
	{
		return NULL;
	}
	pvideo_detail_page->ptop_Button->base.s_top_x=Top_Button_X;
	pvideo_detail_page->ptop_Button->base.s_top_y=Top_Button_Y;
	pvideo_detail_page->ptop_Button->base.s_height=Top_Button_Height;
	pvideo_detail_page->ptop_Button->base.s_width=Top_Button_Width;
	pvideo_detail_page->ptop_Button->ops->set_image_from_resource(pvideo_detail_page->ptop_Button,"TOP file");
	pvideo_detail_page->ptop_Button->base.action_performed=ChangeToTopPage();

	pvideo_detail_page->pmy_Button=hpushbutton_new();
	if (!pvideo_detail_page->pmy_Button)
	{
		return NULL;
	}
	pvideo_detail_page->pmy_Button->base.s_top_x=My_Button_X;
	pvideo_detail_page->pmy_Button->base.s_top_y=My_Button_Y;
	pvideo_detail_page->pmy_Button->base.s_height=My_Button_Height;
	pvideo_detail_page->pmy_Button->base.s_width=My_Button_Width;
	pvideo_detail_page->pmy_Button->ops->set_image_from_resource(pvideo_detail_page->pmy_Button,"my you ku file");
	pvideo_detail_page->pmy_Button->base.action_performed=ChangeToMyPage();

	pvideo_detail_page->Menu_Container=hcontainer_new();
	if (!pvideo_detail_page->Menu_Container)
	{
		return NULL;
	}
	pvideo_detail_page->Menu_Container->base.s_top_x=Menu_Container_Top_X;
	pvideo_detail_page->Menu_Container->base.s_top_y=Menu_Container_Top_Y;
	pvideo_detail_page->Menu_Container->base.s_height=Menu_Container_Height;
	pvideo_detail_page->Menu_Container->base.s_width=Menu_Container_Width;
	pvideo_detail_page->Menu_Container->p_container_ops->add_widget(pvideo_detail_page->Menu_Container,(HWidget*)pvideo_detail_page->precommand_Button);
	pvideo_detail_page->Menu_Container->p_container_ops->add_widget(pvideo_detail_page->Menu_Container,(HWidget*)pvideo_detail_page->ptype_Button);
	pvideo_detail_page->Menu_Container->p_container_ops->add_widget(pvideo_detail_page->Menu_Container,(HWidget*)pvideo_detail_page->ptop_Button);
	pvideo_detail_page->Menu_Container->p_container_ops->add_widget(pvideo_detail_page->Menu_Container,(HWidget*)pvideo_detail_page->psearch_Button);
	pvideo_detail_page->Menu_Container->p_container_ops->add_widget(pvideo_detail_page->Menu_Container,(HWidget*)pvideo_detail_page->pmy_Button);
	
	/*  video list */
	pvideo_detail_page->video_list_title=hlabel_new();
	if (!pvideo_detail_page->video_list_title)
	{
		return NULL;
	}
    pvideo_detail_page->video_list_title->base.s_top_x=Video_List_Title_X;
	pvideo_detail_page->video_list_title->base.s_top_y=Video_List_Title_Y;
	pvideo_detail_page->video_list_title->base.s_height=Video_List_Title_Height;
	pvideo_detail_page->video_list_title->base.s_width=Video_List_Title_Width;
	pvideo_detail_page->video_list_title->set_image_from_resource(pvideo_detail_page->video_list_title,"list title file");
	
	for (int i=0;i<30;i++)
	{
		pvideo_detail_page->video_list_image[i]=hpushbutton_new();
		if (!pvideo_detail_page->video_list_image[i])
		{
			return NULL;
		}
		pvideo_detail_page->video_list_image[i]->base.s_top_x=Video_List_Top_X;
		pvideo_detail_page->video_list_image[i]->base.s_top_y=Video_List_Top_Y+(i*Video_Height);
		pvideo_detail_page->video_list_image[i]->base.s_height=Video_Height;
		pvideo_detail_page->video_list_image[i]->base.s_width=Video_Width;
		pvideo_detail_page->video_list_image[i]->ops->set_image_from_buffer();
		pvideo_detail_page->video_list_image[i]->base.action_performed=play_video();
		
		pvideo_detail_page->video_list_label[i]=hlabel_new();
		if (!pvideo_detail_page->video_list_label[i])
		{
			return NULL;
		}
		pvideo_detail_page->video_list_label[i]->base.s_top_x=Video_List_Top_X+Video_Width;
		pvideo_detail_page->video_list_label[i]->base.s_top_y=Video_List_Top_Y+(i*Video_Height);
		pvideo_detail_page->video_list_label[i]->base.s_height=Video_List_Label_Height;
		pvideo_detail_page->video_list_label[i]->base.s_width=Video_List_Label_Width;
		pvideo_detail_page->video_list_label[i]->set_text(pvideo_detail_page->video_list_label[i],"point text buffer");
		pvideo_detail_page->video_list_label->base.action_performed=play_video();

		pvideo_detail_page->video_list_label2[i]=hlabel_new();
		if (!pvideo_detail_page->video_list_label2[i])
		{
			return NULL;
		}
		pvideo_detail_page->video_list_label2[i]->base.s_top_x=Video_List_Top_X+Video_Width;
		pvideo_detail_page->video_list_label2[i]->base.s_top_y=Video_List_Top_Y+Video_List_Label_Height+(i*Video_Height);
		pvideo_detail_page->video_list_label2[i]->base.s_height=Video_List_Label2_Height;
		pvideo_detail_page->video_list_label2[i]->base.s_width=Video_List_Label2_Width;
		pvideo_detail_page->video_list_label2[i]->set_text(pvideo_detail_page->video_list_label2[i],"point text buffer");
		pvideo_detail_page->video_list_label2->base.action_performed=play_video();

		pvideo_detail_page->video_list_star_image[i]=hpushbutton_new();
		if (!pvideo_detail_page->video_list_star_image[i])
		{
			return NULL;
		}
		pvideo_detail_page->video_list_star_image[i]->base.s_top_x=Video_List_Top_X+Video_Width;
		pvideo_detail_page->video_list_star_image[i]->base.s_top_y=Video_List_Top_Y+Video_List_Label_Height+Video_List_Label2_Height+(i*Video_Height);
		pvideo_detail_page->video_list_star_image[i]->base.s_height=Video_List_Star_Height;
		pvideo_detail_page->video_list_star_image[i]->base.s_width=Video_List_Star_Width;
		pvideo_detail_page->video_list_star_image[i]->ops->set_image_from_buffer(pvideo_detail_page->video_list_star_image,"point to star buffer");

		pvideo_detail_page->video_list_button[i]=hpushbutton_new();
		if (!pvideo_detail_page->video_list_button[i])
		{
			return NULL;
		}
		pvideo_detail_page->video_list_button[i]->base.s_top_x=Video_List_Button_X;
		pvideo_detail_page->video_list_button[i]->base.s_top_y=Video_List_Button_Y+(i*Video_Height);
		pvideo_detail_page->video_list_button[i]->base.s_height=Video_List_Button_Height;
		pvideo_detail_page->video_list_button[i]->base.s_width=Video_List_Button_Width;
		pvideo_detail_page->video_list_button[i]->ops->set_image_from_buffer(pvideo_detail_page->video_list_button,"point to button buffer");
		pvideo_detail_page->video_list_button[i]->base.action_performed=ChangeToVideoDetailPage();
		/*container init */
		pvideo_detail_page->videolist[i]=hcontainer_new();
		if (!pvideo_detail_page->videolist[i])
		{
			return NULL;
		}
		pvideo_detail_page->videolist[i]->base.s_top_x=Video_List_Top_X;
		pvideo_detail_page->videolist[i]->base.s_top_y=Video_List_Top_Y+(i*Video_Height);
		pvideo_detail_page->videolist[i]->base.s_height=Video_Height;
		pvideo_detail_page->videolist[i]->base.s_width=Video_Width;
		pvideo_detail_page->videolist[i]->p_container_ops->add_widget(pvideo_detail_page->videolist[i],(HWidget*)pvideo_detail_page->video_list_image[i]);
		pvideo_detail_page->videolist[i]->p_container_ops->add_widget(pvideo_detail_page->videolist[i],(HWidget*)pvideo_detail_page->video_list_label[i]);
		pvideo_detail_page->videolist[i]->p_container_ops->add_widget(pvideo_detail_page->videolist[i],(HWidget*)pvideo_detail_page->video_list_star_image[i]);
		pvideo_detail_page->videolist[i]->p_container_ops->add_widget(pvideo_detail_page->videolist[i],(HWidget*)pvideo_detail_page->video_list_label2[i]);
		pvideo_detail_page->videolist[i]->p_container_ops->add_widget(pvideo_detail_page->videolist[i],(HWidget*)pvideo_detail_page->video_list_button[i]);


	}

	pvideo_detail_page->RelevanceVideo_Container=hcontainer_new();
	if (!pvideo_detail_page->RelevanceVideo_Container)
	{
		return NULL;
	}
	pvideo_detail_page->RelevanceVideo_Container->base.s_top_x=Relevance_Video_Container_X;
	pvideo_detail_page->RelevanceVideo_Container->base.s_top_y=Relevance_Video_Container_Y;
	pvideo_detail_page->RelevanceVideo_Container->base.s_height=Relevance_Video_Container_Height;
	pvideo_detail_page->RelevanceVideo_Container->base.s_width=Relevance_Video_Container_Width;
	pvideo_detail_page->RelevanceVideo_Container->p_container_ops->add_widget(pvideo_detail_page->RelevanceVideo_Container,(HWidget*)pvideo_detail_page->video_list_title);
	for (int i=0;i<30;i++)
	{
		pvideo_detail_page->RelevanceVideo_Container->p_container_ops->add_widget(pvideo_detail_page->RelevanceVideo_Container,(HWidget*)pvideo_detail_page->videolist[i]);
	}
/************************************************************************/
/*video  detail message                                                                     */
/************************************************************************/
	pvideo_detail_page->play_Button=hpushbutton_new();
	if (!pvideo_detail_page->play_Button)
	{
		return NULL;
	}
	pvideo_detail_page->play_Button->base.s_top_x=Play_Button_X;
	pvideo_detail_page->play_Button->base.s_top_y=Play_Button_Y;
	pvideo_detail_page->play_Button->base.s_height=Play_Button_Height;
	pvideo_detail_page->play_Button->base.s_width=Play_Button_Width;
	pvideo_detail_page->play_Button->ops->set_image_from_buffer(pvideo_detail_page->play_Button,"point to capture");
	pvideo_detail_page->play_Button->base.action_performed=play_video();
	pvideo_detail_page->HD_Button=hpushbutton_new();
	if (!pvideo_detail_page->HD_Button)
	{
		return NULL;
	}
	pvideo_detail_page->HD_Button->base.s_top_x=HD_Button_X;
	pvideo_detail_page->HD_Button->base.s_top_y=HD_Button_Y;
	pvideo_detail_page->HD_Button->base.s_height=HD_Button_Height;
	pvideo_detail_page->HD_Button->base.s_width=HD_Button_Width;
	pvideo_detail_page->HD_Button->ops->set_image_from_buffer(pvideo_detail_page->HD_Button,"point to capture");
	pvideo_detail_page->HD_Button->base.action_performed=HD_play_video();
	
	pvideo_detail_page->share_Button=hpushbutton_new();
	if (!pvideo_detail_page->share_Button)
	{
		return NULL;
	}
	pvideo_detail_page->share_Button->base.s_top_x=Share_Button_X;
	pvideo_detail_page->share_Button->base.s_top_y=Share_Button_Y;
	pvideo_detail_page->share_Button->base.s_height=Share_Button_Height;
	pvideo_detail_page->share_Button->base.s_width=Share_Button_Width;
	pvideo_detail_page->share_Button->ops->set_image_from_buffer(pvideo_detail_page->share_Button,"point to capture");
	pvideo_detail_page->share_Button->base.action_performed=share();

	
	pvideo_detail_page->collect_Button=hpushbutton_new();
	if (!pvideo_detail_page->collect_Button)
	{
		return NULL;
	}
	pvideo_detail_page->collect_Button->base.s_top_x=Collect_Button_X;
	pvideo_detail_page->collect_Button->base.s_top_y=Collect_Button_Y;
	pvideo_detail_page->collect_Button->base.s_height=Collect_Button_Height;
	pvideo_detail_page->collect_Button->base.s_width=Collect_Button_Width;
	pvideo_detail_page->collect_Button->ops->set_image_from_buffer(pvideo_detail_page->collect_Button,"point to capture");
	pvideo_detail_page->collect_Button->base.action_performed=collect();

	pvideo_detail_page->up_Button=hpushbutton_new();
	if (!pvideo_detail_page->up_Button)
	{
		return NULL;
	}
	pvideo_detail_page->up_Button->base.s_top_x=UP_Button_X;
	pvideo_detail_page->up_Button->base.s_top_y=UP_Button_Y;
	pvideo_detail_page->up_Button->base.s_height=UP_Button_Height;
	pvideo_detail_page->up_Button->base.s_width=UP_Button_Width;
	pvideo_detail_page->up_Button->ops->set_image_from_buffer(pvideo_detail_page->up_Button,"point to capture");
	pvideo_detail_page->up_Button->base.action_performed=up();

	pvideo_detail_page->down_Button=hpushbutton_new();
	if (!pvideo_detail_page->down_Button)
	{
		return NULL;
	}
	pvideo_detail_page->down_Button->base.s_top_x=Down_Button_X;
	pvideo_detail_page->down_Button->base.s_top_y=Down_Button_Y;
	pvideo_detail_page->down_Button->base.s_height=Down_Button_Height;
	pvideo_detail_page->down_Button->base.s_width=Down_Button_Width;
	pvideo_detail_page->down_Button->ops->set_image_from_buffer(pvideo_detail_page->down_Button,"point to capture");
	pvideo_detail_page->down_Button->base.action_performed=down();

	pvideo_detail_page->detail_label[0]=hlabel_new();
	if (!pvideo_detail_page->detail_label[0])
	{
		return NULL;
	}
	pvideo_detail_page->detail_label[0]->base.s_top_x=Detail_Label_0_X;
	pvideo_detail_page->detail_label[0]->base.s_top_y=Detail_Label_0_Y;
	pvideo_detail_page->detail_label[0]->base.s_height=Detail_Label_0_Height;
	pvideo_detail_page->detail_label[0]->base.s_width=Detail_Label_0_Width;
	pvideo_detail_page->detail_label[0]->set_image_from_buffer(pvideo_detail_page->detail_label[0],,);
	pvideo_detail_page->detail_label[0]->set_text(pvideo_detail_page->detail_label[0],"point to char");

	pvideo_detail_page->detail_label[1]=hlabel_new();
	if (!pvideo_detail_page->detail_label[1])
	{
		return NULL;
	}
	pvideo_detail_page->detail_label[1]->base.s_top_x=Detail_Label_1_X;
	pvideo_detail_page->detail_label[1]->base.s_top_y=Detail_Label_1_Y;
	pvideo_detail_page->detail_label[1]->base.s_height=Detail_Label_1_Height;
	pvideo_detail_page->detail_label[1]->base.s_width=Detail_Label_1_Width;
	pvideo_detail_page->detail_label[1]->set_text(pvideo_detail_page->detail_label[1],"point to char");

	pvideo_detail_page->detail_label[2]=hlabel_new();
	if (!pvideo_detail_page->detail_label[2])
	{
		return NULL;
	}
	pvideo_detail_page->detail_label[2]->base.s_top_x=Detail_Label_2_X;
	pvideo_detail_page->detail_label[2]->base.s_top_y=Detail_Label_2_Y;
	pvideo_detail_page->detail_label[2]->base.s_height=Detail_Label_2_Height;
	pvideo_detail_page->detail_label[2]->base.s_width=Detail_Label_2_Width;
	pvideo_detail_page->detail_label[2].set_image_from_buffer(pvideo_detail_page->detail_label[2],,);
	
	pvideo_detail_page->detail_label[3]=hlabel_new();
	if (!pvideo_detail_page->detail_label[3])
	{
		return NULL;
	}
	pvideo_detail_page->detail_label[3]->base.s_top_x=Detail_Label_3_X;
	pvideo_detail_page->detail_label[3]->base.s_top_y=Detail_Label_3_Y;
	pvideo_detail_page->detail_label[3]->base.s_height=Detail_Label_3_Height;
	pvideo_detail_page->detail_label[3]->base.s_width=Detail_Label_3_Width;
	pvideo_detail_page->detail_label[3]->set_text(pvideo_detail_page->detail_label[3],"point to char");

	pvideo_detail_page->Video_Detail_Container=hcontainer_new();
	if (!pvideo_detail_page->Video_Detail_Container)
	{
		return NULL;
	}
	pvideo_detail_page->Video_Detail_Container->base.s_top_x=Video_Detail_Container_X;
	pvideo_detail_page->Video_Detail_Container->base.s_top_y=Video_Detail_Container_Y;
	pvideo_detail_page->Video_Detail_Container->base.s_height=Video_Detail_Container_Height;
	pvideo_detail_page->Video_Detail_Container->base.s_width=Video_Detail_Container_Width;
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,(HWidget*)pvideo_detail_page->detail_label[0]);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,(HWidget*)pvideo_detail_page->detail_label[1]);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,(HWidget*)pvideo_detail_page->detail_label[2]);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,(HWidget*)pvideo_detail_page->detail_label[3]);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,pvideo_detail_page->play_Button);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,pvideo_detail_page->HD_Button);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,pvideo_detail_page->collect_Button);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,pvideo_detail_page->share_Button);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,pvideo_detail_page->up_Button);
	pvideo_detail_page->Video_Detail_Container->p_container_ops->add_widget(pvideo_detail_page->Video_Detail_Container,pvideo_detail_page->down_Button);









}
VideoDetailPage* vedio_detail_page_new() 
{
	VideoDetailPage* pvideo_detail_page=vm_malloc(sizeof(VideoDetailPage));
	if (!pvideo_detail_page)
	{
		return NULL;
	}
	video_detail_page_init(pvideo_detail_page);
	return pvideo_detail_page;

}


VideoDetailPage* vedio_detail_page_delete()
{

}