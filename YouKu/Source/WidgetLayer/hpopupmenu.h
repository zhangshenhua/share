/************************************************************************
* Name : hpopumenu.h
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.09.01
* Description : 
************************************************************************/
#ifndef				_HPOPUPMENU_H
#define				_HPOPUPMENU_H

#ifdef __cplusplus
extern "C" {
#endif

/************************* INCLUDE FILES ***********************************/
#include					"hwidget.h"
#include					"himage.h"
/************************* END INCLUDE FILES *******************************/

/**************** structure *********************/

/**************** end structure *********************/

/**************** structure *********************/
typedef enum{
	MEM_EXIST = -1,
	POINT_EMPTY = 0,
	TEXT = 1,
	IMAGE = 2,
	FG_COLOR = 3,
	BG_COLOR = 4,
	BORDER_COLOR = 5,
	MEM_FULL = 254,
	MEM_MALLOC_ERROR = 255,
}POPUP_TYPE;
/**************** end structure *********************/


/**************** structure *********************/
typedef struct{
	short					s_mem_width;
	short					s_mem_height;

	unsigned char			uc_style;
	unsigned char			uc_mem_distance;

	int						i_color;
	int						i_bg_color;
	short					s_width;
	short					s_height;
	short					s_padding_left;		/*padding left*/
	short					s_padding_right;		/*padding right*/
	short					s_padding_top;		/*padding top*/
	short					s_padding_bottom;		/*padding bottom*/
}HPOPUP_PROPERTY;
/**************** end structure *********************/


/**************** structure *********************/
typedef struct _HPOPUPCMD{
	char					*pc_text;
	HRect					rect_text;		/* text's position */

	HImage					*p_img;
	HRect					rect_img;		/* image's position */
	
	int						i_border_color;
	int						i_fg_color;
	int						i_bg_color;

	unsigned char			uc_id;

	struct _HPOPUPCMD		*prev;
	struct _HPOPUPCMD		*next;
}HPOPUPMEMBER;
/**************** end structure *********************/


/**************** structure *********************/
typedef struct _Popup{
/********************** public member *********************************/
	HWidget					base;
	struct _hPopupOps		*ops;

/********************* end public member ********************************/
	HPOPUPMEMBER			*member;
	HPOPUPMEMBER			*tail;

	short					s_mem_width;
	short					s_mem_height;

	unsigned char			uc_style;
	unsigned char			uc_mem_distance;

	unsigned char			uc_mem_num;
}HPopupMenu;
/**************** end structure *********************/

/**************** structure *********************/
typedef struct _hPopupOps{
	HWidgetOperation		base_ops;
	int						i_num_pop;	/* the number of the popupmenu */

	/*********************** function declaration ***************************
	* Name : add_member
	* Description : add member for popupmenu
	* Parameters@ : 
	* Return : 
	************************************************************************/
	short					(*add_member)(HPopupMenu *p_popup, const unsigned char uc_id);

	/*********************** function declaration ***************************
	* Name : del_member
	* Description : delete member for popupmenu
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*del_member)(HPopupMenu *p_popup, unsigned char uc_id);

	/*********************** function declaration ***************************
	* Name : clean_member
	* Description : delete all member for popupmenu
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*clean_member)(HPopupMenu *p_popup);

	/*********************** function declaration ***************************
	* Name : set_member_text
	* Description : set text for popupmenu's member
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_member_text)(HPopupMenu *p_Me, const unsigned char uc_id, const char *pc_str);

	/*********************** function declaration ***************************
	* Name : set_member_image
	* Description : set image for popupmenu's member
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_member_image_resource)(HPopupMenu *p_Me, const unsigned char uc_id, char *pc_str);

	/*********************** function declaration ***************************
	* Name : set_member_image
	* Description : set image for popupmenu's member
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_member_image_buffer)(HPopupMenu *p_Me, const unsigned char uc_id, VMUINT8 *buffer, VMINT i_size);

#if 0
	/*********************** function declaration ***************************
	* Name : set_member_rect
	* Description : set size for popupmenu's member
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_member_rect)(HPopupMenu *p_Me, const unsigned char uc_id, HRect rect_Me);
#endif

	/*********************** function declaration ***************************
	* Name : set_submember_point
	* Description : set point for popupmenu's submember
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_submember_point)(HPopupMenu *p_Me, const unsigned char uc_id, POPUP_TYPE sub_type, HPoint point_Me);

	/*********************** function declaration ***************************
	* Name : set_submember_color
	* Description : set color for popupmenu's submember
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_submember_color)(HPopupMenu *p_Me, const unsigned char uc_id, POPUP_TYPE sub_type, int i_color);

#if 0
	/*********************** function declaration ***************************
	* Name : set_submember_isborder
	* Description : set border for popupmenu's submember
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_submember_isborder)(HPopupMenu *p_Me, const unsigned char uc_id, const unsigned char uc_border);
#endif

	/*********************** function declaration ***************************
	* Name : set_popup_property
	* Description : set property for popupmenu
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void					(*set_popup_property)(HPopupMenu *p_Me, HPOPUP_PROPERTY property_Me);

}HPopupOps;
/**************** end structure *********************/


/********************** public member *********************************/

/********************* end public member ********************************/

/*********************** function declaration ***************************
* Name : hpopupmenu_new
* Description : create a popupmenu
* Parameters@ : 
* Return : 
************************************************************************/
extern HPopupMenu	*hpopupmenu_new();

/*********************** function declaration ***************************
* Name : hpopupmenu_delete
* Description : delete the popupmenu
* Parameters@ : 
* Return : 
************************************************************************/
extern void hpopupmenu_delete(HPopupMenu *p_Me);

/********************** private member *********************************/

/********************* end private member ********************************/



/*********************** function declaration ***************************
* Name : 
* Description : 
* Parameters@ : 
* Return : 
************************************************************************/



#ifdef __cplusplus
}
#endif

#endif



/******************************* EOF ***********************************/