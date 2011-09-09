#include "connectpage.h"
#include "new.h"
#include "himage.h"
#include "hplane.h"

#define  FILENAME_LOGOIAMGE   xxxx_file

ConnectPage* ConnectPage_new() {
	ConnectPage* p_connectpage	= checked_malloc(sizeof(ConnectPage));
	p_connectpage->p_logoimage  = Image(Rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT), FILENAME_LOGOIAMGE);

	p_connectpage->base.destroy			=	destroy		;
	p_connectpage->base.fire_hiden		=	fire_hiden	;
	p_connectpage->base.fire_visible	=	fire_visible;

	return p_connectpage;
}


/*show this page */
static void fire_visible(Page *p_this)
{
	HPlane p_plane = window->get_default_plane();
	p_plane->p_plane_ops->add_widget(p_plane,  ((ConnectPage*)p_this)->p_logoimage);
	window->validate();
	
	if(test_connection()) {
		ClosePage(p_this);
		OpenPage( MainPage_new() );
	}
	else {
		OpenPage( PromptPage_new() );
	}

}

/* hide this page */
static void fire_hiden(Page *p_this)
{
	//ConnectPage* p_connect_page = (ConnectPage*)p_this;
	HPlane *p_plane = window->get_default_plane();

	/* Todo: delete widgets from plane */
	p_plane->p_plane_ops->remove_all(p_plane);
}

/*destroy the page , free its memory*/
static void destroy(Page *p_this)
{
	/* 
	free all memery
	*/
	vm_free(p_this);
}
