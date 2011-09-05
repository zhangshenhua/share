#include "himage.h"
#include "vmgraph.h"
#include "vmres.h"


/***************** Static variables: ***********************/
static HImageOperation s_ImageOperation;


/***************** Static functions below: ***********************/

static short get_height_image(HImage *p_Me){
	if (p_Me->i_canvas >= VM_GDI_SUCCEED){
		return (p_Me->s_height);
	}else{
		return 0;
	}
}

static short get_width_image(HImage *p_Me){
	if (p_Me->i_canvas >= VM_GDI_SUCCEED){
		return (p_Me->s_width);
	}else{
		return 0;
	}
}


static void UI_Image_PaintFrame( HImage *p_this, VMINT i_LayerHandle, VMINT i_OffsetX, VMINT i_OffsetY, int i_frame_index )
{
	VMINT x, y;
	int i_width, i_height;
	
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

	i_width = ( p_this->st_pos_size.s_width <= 0 ) ? ( p_Frame->width ) : ( p_this->st_pos_size.s_width );
	i_height= ( p_this->st_pos_size.s_height<= 0 ) ? ( p_Frame->height) : ( p_this->st_pos_size.s_height);

	vm_graphic_blt(
		vm_graphic_get_layer_buffer(i_LayerHandle), 
		x, 
		y,
		vm_graphic_get_canvas_buffer(p_this->i_canvas), 
		p_Frame->left, 
		p_Frame->top,
		i_width,
		i_height,
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
	struct frame_prop *p_Frame = NULL;

	if( NULL == p_this )	
		return;
	if( NULL == strPath )	
		return;

	p_ImgData = vm_load_resource(strPath, &i_ImgSize);

	if( p_ImgData != NULL )
	{
		// Put the resource date into MRE graphic canvas.
		p_this->i_canvas = vm_graphic_load_image(p_ImgData, i_ImgSize);
		if (p_this->i_canvas < VM_GDI_SUCCEED){
			p_this->i_canvas = VM_GDI_FAILED;
			vm_free( p_ImgData );
			p_ImgData = NULL;
			return;
		}
		p_Frame = vm_graphic_get_img_property( p_this->i_canvas, 1 );
		p_this->s_height = p_Frame->height;
		p_this->s_width = p_Frame->width;
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

	p_this->st_pos_size.s_x = x;
	p_this->st_pos_size.s_y = y;
}


static void UI_Image_SetSize(HImage *p_this, short s_width, short s_height)
{
	if( NULL == p_this )	
		return;

	p_this->st_pos_size.s_width  = s_width;
	p_this->st_pos_size.s_height = s_height;
}


static HPoint UI_Image_GetPosition( HImage *p_this )
{
	HPoint pos_zero = {0, 0};

	if( p_this )	
	{
		pos_zero.s_x = p_this->st_pos_size.s_x;
		pos_zero.s_y = p_this->st_pos_size.s_y;
	}

	return pos_zero;
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


static HImageOperation* UI_Image_Get_Oper()
{
	static int i_Flag = FALSE;

	if( FALSE == i_Flag )
	{
		i_Flag = TRUE;

		s_ImageOperation.paint					= UI_Image_Paint;
		s_ImageOperation.paint_frame			= UI_Image_PaintFrame;
		s_ImageOperation.load_image				= UI_Image_LoadImage;
		s_ImageOperation.load_image_2			= UI_Image_LoadImage_2;
		s_ImageOperation.set_position			= UI_Image_SetPosition;
		s_ImageOperation.set_size				= UI_Image_SetSize;
		s_ImageOperation.get_position			= UI_Image_GetPosition;
		s_ImageOperation.get_frame_num			= UI_Image_GetFrameNum;
		s_ImageOperation.get_image_real_height	= get_height_image;
		s_ImageOperation.get_image_real_width	= get_width_image;
	}

	return &s_ImageOperation;
}


/***************** External functions below: ***********************/

HImage *himage_new()
{
	HImage			*p_img = NULL;

	p_img = (HImage *)vm_malloc(sizeof(HImage));
	if (NULL == p_img){
		return NULL;
	}
	p_img->p_oper = UI_Image_Get_Oper();		// Initialize the operation pointer
	p_img->i_canvas = VM_GDI_FAILED;
/*	p_img->p_oper->set_position( p_img, 0, 0 );	*/ // The default position is (0, 0)
/*	p_img->p_oper->set_size( p_img, 0, 0 );		*/ // Set to the real size of the image
	p_img->st_pos_size.s_x = 0;
	p_img->st_pos_size.s_y = 0;
	p_img->st_pos_size.s_height = 0;
	p_img->st_pos_size.s_width = 0;
	p_img->s_height = 0;
	p_img->s_width = 0;

	return p_img;
}

void himage_delete( HImage *p_this )
{
	if( NULL == p_this ){
		return;
	}

	if ( p_this->i_canvas >= VM_GDI_SUCCEED )
	{
		vm_graphic_release_canvas( p_this->i_canvas );
		p_this->i_canvas = VM_GDI_FAILED;
	}
	vm_free(p_this);
}


