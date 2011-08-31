/*----------------------------------------------------------------
 *	File Name	: himage.h
 *	Description	: header file of the class HImage
 *	COPYRIGHT	: hiSoft
 *	Created on	: 2011-8-22
 *	Modified by	: Xie Liang
 ---------------------------------------------------------------*/

#ifndef _HIMAGE_H
#define _HIMAGE_H


#include "vmsys.h"
#include "hwidget.h"


typedef struct _HImage_			 HImage;
typedef struct _HImageOperation_ HImageOperation;


struct _HImage_
{
	HImageOperation *p_oper;	// Its operation pointer
	VMINT	i_canvas;			// canvas
	HRect	st_pos_size;		// Image's relative position & the restricting size
	short	s_width;
	short	s_height;
};


struct _HImageOperation_
{
	/*	
	Function Name:
		paint
	Description:
		Paint the image
	Parameters:
		(in) p_this			: Pointer of HImage self
		(in) i_LayerHandle	: the layer handle
		(in) i_OffsetX		: the X offset of the Image's container relative to the layer
		(in) i_OffsetY		: the Y offset of the Image's container relative to the layer		
	Note:
		If the image is a GIF, then the first frame will be painted.
	*/
	void (*paint)(HImage *p_this, VMINT i_LayerHandle, VMINT i_OffsetX, VMINT i_OffsetY);

	/*	
	Function Name:
		paint_frame
	Description:
		Paint the specified frame of this image
	Parameters:
		(in) p_this			: Pointer of HImage self
		(in) i_LayerHandle	: the layer handle
		(in) i_OffsetX		: the X offset of the Image's container relative to the layer
		(in) i_OffsetY		: the Y offset of the Image's container relative to the layer		
		(in) i_frame_index	: the frame index, start from 1
	Note:
		If 'i_frame_index' < 1 or greater than the max frame number of this image, nothing will be painted.
	*/
	void (*paint_frame)( HImage *p_this, VMINT i_LayerHandle, VMINT i_OffsetX, VMINT i_OffsetY, int i_frame_index );

	/*	Function Name		: load_image
		Description			: Load the image from a file
		Para(in) p_this		: Pointer of HImage self
		Para(in) wstrPath	: File name		
		*/	
	void (*load_image)(HImage *p_this, char *strPath);

	/*	
	Function Name: 
		load_image_2
	Description: 
		Load the image from a buffer
	Parameters:
		p_this	: [IN] pointer of HImage self
		p_buf	: [IN] pointer of source image data
		i_size	: [IN] image length
	Note: 
		This function will NOT free the memory that 'p_buf' point to, you must free it yourself!	*/	
	void (*load_image_2)(HImage *p_this, VMUINT8 *p_buf, VMINT i_size);

	/*	
	Function Name:
		set_position
	Description: 
		set the position of the image
	Parameters:
		(in) p_this	: Pointer of HImage self	
		(in) x		: the x coordinate relative to the image's container
		(in) y		: the y coordinate relative to the image's container	*/	
	void (*set_position)(HImage *p_this, short x, short y);
	
	/*	
	Function Name:
		set_size
	Description: 
		set the size of the image's painting area
	Parameters:
		(in) p_this	  : Pointer of HImage self	
		(in) s_width  : the width;	If <= 0, the real width of the iamge will be set.
		(in) s_height : the height; If <= 0, the real height of the iamge will be set.
	Note:
		If the given width or height value is less than the real size of image, then
		the image will be cut;
		If the given width or height value is larger than the real size of image, then
		the iamge will be painted to its real size.
		*/	
	void (*set_size)(HImage *p_this, short s_width, short s_height);

	/*	
	Function Name:
		get_position
	Description: 
		get the relative position of the image
	Parameters:
		p_this : [IN] Pointer of HImage self	
	Return:
		the position	*/
	HPoint (*get_position)(HImage *p_this);

	/*	
	Function Name:
	get_image_real_width
	Description: 
	get the real width of image
	Parameters:
	p_this : [IN] Pointer of HImage self	
	Return:
	the width	*/
	short (*get_image_real_width)(HImage *p_this);

	/*	
	Function Name:
	get_image_real_height
	Description: 
	get the real height of image
	Parameters:
	p_this : [IN] Pointer of HImage self	
	Return:
	the position	*/
	short (*get_image_real_height)(HImage *p_this);

	/*	
	Function Name:
		get_frame_num
	Description: 
		get the frame number of the image
	Parameters:
		p_this : [IN] Pointer of HImage self
	*/
	int (*get_frame_num)(HImage *p_this);

};


/*	Function Name: 
		himage_init
	Description: 
		Initialize a existed object
	Parameters:
		p_img : [IN] pointer to a existed HImage object to be initialized
	Note:
		This is the first function you must call if you want to use a HImage object!
*/	
/*extern void himage_init( HImage *p_img );*/
extern HImage *himage_new();

/*	
Function Name:
himage_delete
Description: 
Free the resource the image object used.
Parameters:
p_this: Pointer of HImage self	*/	
extern void himage_delete(HImage *p_this);


/*	A simple example:

	{
		HImage img;
		himage_init( &img );
		img.p_oper->load_image( &img, "abc.gif" );

		// ...

		img.p_oper->clean( &img );
	}
*/


#endif