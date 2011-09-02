/**********************************************************************
* FILE Name: vedio_detail.h
* Description: 
* COPYRIGHT: Hisoft
* Created on: $(9.2.2011) Author: Yan Guanglei
***********************************************************************/
/***** Class Definition *********/
#include "page.h"
#include "hpushbutton.h"

#define RelevanceVideo_Height 
#define RelevanceVideo_Width  
#define RelevanceVideo_Height  
#define RelevanceVideo_Width 
#define Menu_Container_Top_X 
#define Menu_Container_Top_Y 
#define Menu_Container_Height 
#define Menu_Container_Width 
#define Vedio_Container_Top_X 
#define Vedio_Container_Top_Y 
#define Vedio_Container_Height 
#define Vedio_Container_Width 
//menu button coordinate
#define Type_Button_X   
#define Type_Button_Y  
#define Type_Button_Height 
#define Type_Button_Width
#define Recommand_Button_X  
#define Recommand_Button_Y  
#define Recommand_Button_Height
#define Recommand_Button_Width
#define Search_Button_X   
#define Search_Button_Y  
#define Search_Button_Height
#define Search_Button_Width
#define Top_Button_X   
#define Top_Button_Y  
#define Top_Button_Height
#define Top_Button_Width
#define My_Button_X   
#define My_Button_Y  
#define My_Button_Height
#define My_Button_Width
/*video list macro */
#define Video_List_Title_X
#define Video_List_Title_Y
#define Video_List_Title_Height
#define Video_List_Title_Width
#define Video_List_Top_X 
#define Video_List_Top_Y 
#define Video_Height 
#define Video_Width 
#define Video_List_Label_Height
#define Video_List_Label_Width
#define Video_List_Label2_Height
#define Video_List_Label2_Width
#define Video_List_Star_Height
#define Video_List_Star_Width
#define Video_List_Button_X
#define Video_List_Button_Y
#define Video_List_Button_Height
#define Video_List_Button_Width

/************************************************************************/
/* Relevance Video Container init                                                                      */
/************************************************************************/
#define  Relevance_Video_Container_X 
#define  Relevance_Video_Container_Y
#define  Relevance_Video_Container_Height
#define  Relevance_Video_Container_Width
/************************************************************************/
/* detail video message                                                                      */
/************************************************************************/
#define Video_Detail_Container_X
#define Video_Detail_Container_Y
#define Video_Detail_Container_Height
#define Video_Detail_Container_Width
#define Play_Button_X
#define Play_Button_Y
#define Play_Button_Height
#define Play_Button_Width
#define HD_Button_X
#define HD_Button_Y
#define HD_Button_Height
#define HD_Button_Width
#define Share_Button_X
#define Share_Button_Y
#define Share_Button_Height
#define Share_Button_Width
#define Collect_Button_X
#define Collect_Button_Y
#define Collect_Button_Height
#define Collect_Button_Width
#define UP_Button_X
#define UP_Button_Y
#define UP_Button_Height
#define UP_Button_Width
#define Down_Button_X
#define Down_Button_Y
#define Down_Button_Height
#define Down_Button_Width
#define Down_Button_X
#define Detail_Label_0_X
#define Detail_Label_0_Y
#define Detail_Label_0_Height
#define Detail_Label_0_Width
#define Detail_Label_1_X
#define Detail_Label_1_Y
#define Detail_Label_1_Height
#define Detail_Label_1_Width
#define Detail_Label_2_X
#define Detail_Label_2_Y
#define Detail_Label_2_Height
#define Detail_Label_2_Width
#define Detail_Label_3_X
#define Detail_Label_3_Y
#define Detail_Label_3_Height
#define Detail_Label_3_Width










typedef struct _VideoDetail_Page
{
	Page video_detail;
	//HImage* Prompt_Image;
	HContainer* Menu_Container;
	HContainer* RelevanceVideo_Container;
	HContainer* Video_Detail_Container;
	/*menu button define*/	
	HPushButton* precommand_Button;
	HPushButton* ptype_Button;
	HPushButton* psearch_Button;
	HPushButton* ptop_Button;
	HPushButton* pmy_Button;

	/* detail button define*/
	HPushButton* play_Button;
	HPushButton* HD_Button;
	HPushButton* share_Button;
	HPushButton* collect_Button;
	HPushButton* up_Button;
	HPushButton*down_Button;
	HLabel* detail_label[4];
	
	/*video list */
	HLabel* video_list_title; 
	HContainer* videolist[30];
	HPushButton* video_list_image[30];
	HPushButton video_list_star_image[30];
	HPushButton video_list_button[30];
	HLabel * video_list_label[30];
	HLabel* video_list_label2[30];



}VideoDetailPage;

extern VideoDetailPage* vedio_detail_page_new();
extern VideoDetailPage* vedio_detail_page_delete();

