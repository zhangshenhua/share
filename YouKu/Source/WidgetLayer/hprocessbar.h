/************************************************************************
* Name : hprocesssbar.h
* Copyright : Hisoft
* Autor : yi.jiang
* Date : 2011.09.06
* Description : 
************************************************************************/
#ifndef				_HPROCESSBAR_H
#define				_HPROCESSBAR_H

#include				"../WindowLayer/hwindow.h"
#include				"hwidget.h"
#include				"himage.h"
#include				"hplane.h"
#include				"hlookandfeel.h"

typedef struct _HProcessBar {
/********************** public member *********************************/
	HWidget				base;

	/*********************** function declaration ***************************
	* Name : set_text
	* Description : set the status bar text
	* @para(in) p_Me : the hprocessbar structure
	* @para(in) p_process_status_str : the status string 
	************************************************************************/
	void (*set_text)(struct _HProcessBar *p_Me, const char *p_process_status_str);

	/*********************** function declaration ***************************
	* Name : set_process_image_from_resource
	* Description : set statusbar image from the mre resource
	* @para(in) p_Me : the hprocessbar structure
	* @para(in) pc_image_name : the image string parth 
	************************************************************************/
	void (*set_process_image_from_resource)(struct _HProcessBar *p_Me, char *pc_image_name);

	/*********************** function declaration ***************************
	* Name : set_process_image_from_buffer
	* Description : set statusbar image from the mre resource
	* @para(in) p_Me : the hprocessbar structure
	* @para(in) pi_buf : the image buffer
	* @para(in) i_size : the image size
	************************************************************************/
	void (*set_process_image_from_buffer)(struct _HProcessBar *p_Me, VMUINT8 *pi_buf, VMINT i_size);

	/*********************** function declaration ***************************
	* Name : hprocessbar_visible
	* Description : show the processbar int the popupmenu plane, and repaint
	* @para(in) p_Me : the hprocessbar structure
	* @para(in) p_process_status_str : the status string
	* @para(in) process_image_name : the image string parth
	* @para(in) i_color : the string color
	* @para(in) bg_color : the bg color
	************************************************************************/
	void (*hprocessbar_visible)(struct _HProcessBar *p_Me, 
		const char *p_process_status_str, char *process_image_name, VMUINT16 i_color, VMUINT16 bg_color);


	/*********************** function declaration ***************************
	* Name : hprocessbar_hide
	* Description : hide the processbar int the popupmenu plane, and repaint
	* @para(in) p_Me : the hprocessbar structure
	************************************************************************/
	void (*hprocessbar_hide)(struct _HProcessBar *p_Me);

/********************* end public member ********************************/

/********************** private member *********************************/
	short				inside_height;
	short				inside_image_width;
	short				inside_test_width;
	char					*p_process_status_str;
	HImage					*process_image;
	HWidgetOperation parent_ops;

/********************* end private member ********************************/
}HProcessBar;

/***************** hstatusbar_new function declaration *******************
* Name : hprocessbar_new
* Description : create a processbar
* Return : 
************************************************************************/
HProcessBar *hprocessbar_new();

/***************** hstatusbar_new function declaration *******************
* Name : hprocessbar_delete
* Description : delete a processbar
* Return : 
************************************************************************/
void hprocessbar_delete(HProcessBar *p_Me);

#endif

/******************************* EOF ***********************************/