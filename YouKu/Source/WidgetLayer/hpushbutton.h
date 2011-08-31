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
typedef struct _hPushButton{
	HWidget			base;

/********************** public member ***********************************/

/*********************** function declaration ***************************
* Name : set_text
* Description : set pushbutton text
* Parameters@ : 
* Return : 
************************************************************************/
	void			(*set_text)(struct _hPushButton *p_Me, const char *pc_str);

/*********************** function declaration ***************************
* Name : set_image_from_resource
* Description : set pushbutton image from the mre resource
* Parameters@ : 
* Return : 
************************************************************************/
	void			(*set_image_from_resource)(struct _hPushButton *p_Me, const char *pc_image_name);

/*********************** function declaration ***************************
* Name : set_image_from_buffer
* Description : set pushbutton image form image's buffer
* Parameters@ : 
* Return : 
************************************************************************/
	void			(*set_image_from_buffer)(struct _hPushButton *p_Me, VMUINT8 *pi_buf, VMINT i_size);

/********************* end public member ********************************/


/********************** private member *********************************/
	char			*text;
	HImage			*bg_image;
	short			text_width;
	short			text_height;
	unsigned char	uc_ps;
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
extern HPushButton *hpushbutton_new();

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