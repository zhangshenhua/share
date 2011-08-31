#include "himage.h"
#include "vmgraph.h"
#include "vmres.h"


/***************** Static variables: ***********************/
static HImageOperation s_ImageOperation;


/***************** Static functions below: ***********************/

static void UI_Image_PaintFrame( HImage *p_this, VMINT i_LayerHandle, VMINT i_OffsetX, VMINT i_OffsetY, int i_frame_index )
{
	VMINT x, y;
	struct frame_prop *p_Frame = NULL;

	if( NULL == p_this )
		return;

	if( p_this->i_canvas < VM_GDI_SUCCEED )	// canvas is invalid
		return;

	if( i_frame_index < 1 || 
		i_frame_index > p_this->p_oper->get_frame_num(p_this) )
		return;


	p_Frame = vm_graphic_get_img_property( p_this->i_canvas, i_frame_index );

	// calculate the x & y coordinates of the image relative to the layer
	x = p_this->p_oper->get_position(p_this).s_x + i_OffsetX;
	y = p_this->p_oper->get_position(p_this).s_y + i_OffsetY;

	vm_graphic_blt(
		vm_graphic_get_layer_buffer(i_LayerHandle), 
		x, 
		y,
		vm_graphic_get_canvas_buffer(p_this->i_canvas), 
		p_Frame->left, 
		p_Frame->top,
		p_Frame->width, 
		p_Frame->height, 
		i_frame_index );
}


static void UI_Image_Paint( HImage *p_this, VMINT i_LayerHandle, VMINT i_OffsetX, VMINT i_OffsetY )
{
	UI_Image_PaintFrame( p_this, i_LayerHandle, i_OffsetX, i_OffsetY, 1 );
}


static void UI_Image_LoadImage(HImage* p_this, char *strPath)
{
	VMUINT8 *p_ImgData = NULL;
	VMINT i_ImgSize = 0;

	if( NULL == p_this )	
		return;
	if( NULL == strPath )	
		return;

	p_ImgData = vm_load_resource(strPath, &i_ImgSize);

	if( p_ImgData != NULL )
	{
		// Put the resource date into MRE graphic canvas.
		p_this->i_canvas = vm_graphic_load_image(p_ImgData, i_ImgSize);

		// Free the data.
		vm_free( p_ImgData );
		p_ImgData = NULL;
	}
}


static void UI_Image_LoadImage_2( HImage *p_this, VMUINT8 *p_buf, VMINT i_size )
{
	if( NULL == p_this || NULL == p_buf )
		return;

	// Put the resource date into MRE graphic canvas.
	p_this->i_canvas = vm_graphic_load_image( p_buf, i_size );
}


static void UI_Image_SetPosition( HImage *p_this, short x, short y )
{
	if( NULL == p_this )	
		return;

	p_this->st_pos.s_x = x;
	p_this->st_pos.s_y = y;
}


static HPoint UI_Image_GetPosition( HImage *p_this )
{
	HPoint pos_zero = {0, 0};

	if( NULL == p_this )	
		return pos_zero;
	else
		return p_this->st_pos;
}


static int UI_Image_GetFrameNum( HImage *p_this )
{
	if( NULL == p_this )	
		return 0;
	
	if( p_this->i_canvas >= VM_GDI_SUCCEED )
		return vm_graphic_get_frame_number( p_this->i_canvas );
	else
		return 0;
}


static void UI_Image_Clean( HImage *p_this )
{
	if( NULL == p_this )	
		return;

	if ( p_this->i_canvas >= VM_GDI_SUCCEED )
	{
		vm_graphic_release_canvas( p_this->i_canvas );
		p_this->i_canvas = VM_GDI_FAILED;
	}
}


static HImageOperation* UI_Image_Get_Oper()
{
	static int i_Flag = FALSE;

	if( FALSE == i_Flag )
	{
		i_Flag = TRUE;

		s_ImageOperation.paint			= UI_Image_Paint;
		s_ImageOperation.paint_frame	= UI_Image_PaintFrame;
		s_ImageOperation.load_image		= UI_Image_LoadImage;
		s_ImageOperation.load_image_2	= UI_Image_LoadImage_2;
		s_ImageOperation.set_position	= UI_Image_SetPosition;
		s_ImageOperation.get_position	= UI_Image_GetPosition;
		s_ImageOperation.get_frame_num	= UI_Image_GetFrameNum;
		s_ImageOperation.clean			= UI_Image_Clean;
	}

	return &s_ImageOperation;
}


/***************** External functions below: ***********************/

void himage_init( HImage *p_img )
{
	if ( NULL == p_img )
		return;

	p_img->p_oper = UI_Image_Get_Oper();		// Initialize the operation pointer
	p_img->i_canvas = VM_GDI_FAILED;
	p_img->p_oper->set_position( p_img, 0, 0 );	// The default position is (0, 0)
}
