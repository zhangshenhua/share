/************************************************************************
* Name : hpushbutton.h
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.08.19
* Description : 
************************************************************************/
#ifndef				_HPUSHBUTTON_H
#define				_HPUSHBUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/************************* INCLUDE FILES ***********************************/
#include "hwidget.h"
#include "himage.h"
/************************* END INCLUDE FILES *******************************/

/**************** structure *********************/
typedef struct{
	/*********************** function declaration ***************************
	* Name : set_text
	* Description : set pushbutton text
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*set_text)(struct _hPushButton *p_Me, const char *pc_str);

	/*********************** function declaration ***************************
	* Name : set_text_point
	* Description : set pushbutton text point
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*set_text_point)(struct _hPushButton *p_Me, short s_x, short s_y);

	/*********************** function declaration ***************************
	* Name : set_image_from_buffer
	* Description : set pushbutton image form image's buffer
	* Parameters@ : 0 is't center,1 is center
	* Return : 
	************************************************************************/
	void			(*set_text_mid)(struct _hPushButton *p_Me, int i_ismid);

	/*********************** function declaration ***************************
	* Name : set_button_autosize_image
	* Description : set pushbutton size according to image size
	* Parameters@ : 0 is fixed or according to text length,1 is autosize according to image size
	* Return : 
	************************************************************************/
	void			(*set_button_autosize_image)(struct _hPushButton *p_Me, int i_isautosize);

	/*********************** function declaration ***************************
	* Name : set_image_from_resource
	* Description : set pushbutton image from the mre resource
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*set_image_from_resource)(struct _hPushButton *p_Me, char *pc_image_name);

	/*********************** function declaration ***************************
	* Name : set_image_from_buffer
	* Description : set pushbutton image form image's buffer
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*set_image_from_buffer)(struct _hPushButton *p_Me, VMUINT8 *pi_buf, VMINT i_size);

	/*********************** function declaration ***************************
	* Name : clean_bgimage
	* Description : clean the pushbutton's bgimage
	* Parameters@ : 
	* Return : 
	************************************************************************/
	void			(*clean_bgimage)(struct _hPushButton *p_Me);


}HPUSHBUTTON_OPS;
/**************** end structure *********************/

/**************** structure *********************/
typedef struct _hPushButton{
	HWidget					base;
/********************** public member ***********************************/
	HPUSHBUTTON_OPS			*ops;
/********************* end public member ********************************/


/********************** private member *********************************/
	char					*text;
	HImage					*bg_image;
	short					text_width;
	short					text_height;
	unsigned char			uc_ismid;
	unsigned char			uc_isautosize;
	unsigned char			uc_ps;
/********************* end private member ********************************/

}HPushButton, *HPushButton_P;
/**************** end structure *********************/


/*********************** function declaration ***************************
* Name : 
* Description : 
* Parameters@ : 
* Return : 
************************************************************************/

/*********************** function declaration ***************************
* Name : pushbutton_create
* Description : create a pushbutton
* Parameters@ : 
* Return : 
************************************************************************/
extern HPushButton *hpushbutton_new(const char *pc_str);

/*********************** function declaration ***************************
* Name : pushbutton_destroy
* Description : destroy a pushbutton
* Parameters@ : 
* Return : 
************************************************************************/
extern void hpushbutton_delete(HPushButton *p_Me);


#ifdef __cplusplus
}
#endif

#endif



/******************************* EOF ***********************************/