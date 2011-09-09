/************************************************************************
* Name : hstatusbar.h
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.08.31
* Description : 
************************************************************************/
#ifndef				_HSTATUSBAR_H
#define				_HSTATUSBAR_H

#ifdef __cplusplus
extern "C" {
#endif

/************************* INCLUDE FILES ***********************************/
#include				"hwidget.h"
#include				"himage.h"
/************************* END INCLUDE FILES *******************************/
#define							STATUSBAR_LEFT			1
#define							STATUSBAR_RIGHT			2
/**************** structure *********************/

/**************** end structure *********************/

/**************** HStatusBar structure *********************/
typedef struct _HStatusBar{
/********************** public member *********************************/
	HWidget				base;
/*********************** function declaration ***************************
* Name : set_text
* Description : set the status bar text
* Parameters@ : 
* Return : 
************************************************************************/
	void				(*set_text)(struct _HStatusBar *p_Me, const char *pc_left_str, const char *pc_right_str);

	/*********************** function declaration ***************************
	* Name : set_image_from_resource
	* Description : set statusbar image from the mre resource
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*set_image_from_resource)(struct _HStatusBar *p_Me, char *pc_image_name);

	/*********************** function declaration ***************************
	* Name : set_image_from_buffer
	* Description : set the statusbar image form image's buffer
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*set_image_from_buffer)(struct _HStatusBar *p_Me, VMUINT8 *pi_buf, VMINT i_size);

	/*********************** function declaration ***************************
	* Name : clean_bgimage
	* Description : clean the statusbar's's bgimage
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*clean_bgimage)(struct _HStatusBar *p_Me);

/********************* end public member ********************************/

/********************** private member *********************************/
	char					*pc_left_str;
	char					*pc_right_str;
	HImage					*bg_image;

	short					s_left_len;
	short					s_right_len;
	HWidgetOperation		parent_ops;

/********************* end private member ********************************/
}HStatusBar;

/**************** end structure *********************/

/***************** hstatusbar_new function declaration *******************
* Name : hstatusbar_new
* Description : create a statusbar
* Parameters@ : 
* Return : 
************************************************************************/

extern HStatusBar *hstatusbar_new(const char *pc_left_str, const char *pc_right_str);

/*********************** function declaration ***************************
* Name : hstatusbar_delete
* Description : destroy the statusbar
* Parameters@ : 
* Return : 
************************************************************************/

extern void hstatusbar_delete(HStatusBar *p_Me);

/********************** public member *********************************/

/********************* end public member ********************************/



/********************** private member *********************************/

/********************* end private member ********************************/



/*********************** function declaration ***************************
* Name : 
* Description : 
* Parameters@ : 
* Return : 
************************************************************************/

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