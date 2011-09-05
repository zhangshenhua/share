/************************************************************************
* Name : hlabel.h
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.08.19
* Description : label text length can't exceed 256
************************************************************************/
#ifndef				_HLABEL_H
#define				_HLABEL_H

#ifdef __cplusplus
extern "C" {
#endif

/************************* INCLUDE FILES ***********************************/
#include "HWidget.h"
#include "himage.h"
/************************* END INCLUDE FILES *******************************/

/**************** structure *********************/
typedef struct _hLabel{
/********************** public member ***********************************/
	HWidget			base;

/*********************** function declaration ***************************
* Name : set_text
* Description : set label text
* Parameters@ : 
* Return : 
************************************************************************/
	void			(*set_text)(struct _hLabel *p_Me, const char *pc_str);

/*********************** function declaration ***************************
* Name : set_spacing
* Description : set label text spacing
* Parameters@ : 
* Return : 
************************************************************************/
	void			(*set_spacing)(struct _hLabel *p_Me, int i_spacing);

/*********************** function declaration ***************************
* Name : set_image_from_resource
* Description : set label image from the mre resource
* Parameters@ : 
* Return : 
************************************************************************/
	void			(*set_image_from_resource)(struct _hLabel *p_Me, char *pc_image_name);

/*********************** function declaration ***************************
* Name : set_image_from_buffer
* Description : set label image form image's buffer
* Parameters@ : 
* Return : 
************************************************************************/
	void			(*set_image_from_buffer)(struct _hLabel *p_Me, VMUINT8 *pi_buf, VMINT i_size);



/********************* end public member ********************************/

/********************** private member *********************************/

	char			*text;		/*** storage text ***/
	HImage			*bg_image;
	short			text_row_width;
	short			text_row_count;
	short			text_row_spacing;
	unsigned char	uc_ps;
/********************* end private member ********************************/

}HLabel, *HLabel_P;

/**************** end structure *********************/

/*********************** function declaration ***************************
* Name : 
* Description : 
* Parameters@ : 
* Return : 
************************************************************************/

/*********************** function declaration ***************************
* Name : label_create
* Description : the function create a control of label
* Parameters@ : 
* Return : a pointer of label
************************************************************************/
extern HLabel *hlabel_new();

/*********************** function declaration ***************************
* Name : label_destroy
* Description : destroy the HLabel
* Parameters@ : 
* Return : 
************************************************************************/
extern void	hlabel_delete(HLabel *p_Me);



#ifdef __cplusplus
}
#endif

#endif



/******************************* EOF ***********************************/