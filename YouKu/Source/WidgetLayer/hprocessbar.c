/************************************************************************
* Name : hprocessbar.c
* Copyright : Hisoft
* Autor : yi.jiang
* Date : 2011.09.06
* Description : 
************************************************************************/

#include				"vmsys.h"
#include				"vmgraph.h"

#include				"string.h"

#include				"hprocessbar.h"

#define		SCREEN_WIDTH		(vm_graphic_get_screen_width())
#define		SCREEN_HEIGHT		(vm_graphic_get_screen_height())

#define		PROCESSBAR_PADDING			5
#define		PROCESSBAR_MIDDLE_INTERVAL	5
#define		PROCESSBAR_WIDTH			(SCREEN_WIDTH * 7 / 10)
#define		PROCESSBAR_HEIGHT			(SCREEN_HEIGHT * 3 / 20)

// #define		PROCESSIMAGE_WIDTH		(PROCESSBAR_HEIGHT - 2 * PROCESSBAR_PADDING)
// #define		PROCESSIMAGE_HEIGHT		(PROCESSBAR_HEIGHT - 2 * PROCESSBAR_PADDING)
// #define		PROCESSTEST_WIDTH			(PROCESSBAR_WIDTH - PROCESSIMAGE_WIDTH - 2 * PROCESSBAR_PADDING - PROCESSBAR_MIDDLE_INTERVAL)
// #define		PROCESSTEST_HEIGHT			PROCESSIMAGE_HEIGHT

static void press_pen(HWidget *p_par, short s_x, short s_y){
	;
}

static void release_pen(HWidget *p_par, short s_x, short s_y){
	
}

static void move_pen(HWidget *p_par, short s_x, short s_y){
	;
}
static void enter_pen(HWidget *p_par, short s_x, short s_y){
	;
}

static void leave_pen(HWidget *p_par, short s_x, short s_y){
	;
}

static void press_key(HWidget *p_par, int keycode){
	;
}

/*********************** function declaration ***************************
* Name : set_process_image_from_resource
* Description : set statusbar image from the mre resource
************************************************************************/
static void process_image_set_resouce(HProcessBar *p_Me, char *pc_image_name){
	if (p_Me->process_image){
		himage_delete(p_Me->process_image);
		p_Me->process_image = NULL;
	}
	p_Me->process_image = himage_new();
	if (NULL == p_Me->process_image){
		p_Me->process_image = NULL;
		return;
	}

	p_Me->process_image->p_oper->load_image(p_Me->process_image, pc_image_name);
}


/*********************** function declaration ***************************
* Name : set_process_image_from_buffer
* Description : set statusbar image from the mre resource
************************************************************************/
static void process_image_set_buffer(HProcessBar *p_Me, VMUINT8 *pi_buf, VMINT i_size) {
	if (p_Me->process_image){
		himage_delete(p_Me->process_image);
		p_Me->process_image = NULL;
	}
	p_Me->process_image = himage_new();
	if (NULL == p_Me->process_image){
		p_Me->process_image = NULL;
		return;
	}
	p_Me->process_image->p_oper->load_image_2(p_Me->process_image, pi_buf, i_size);
}

/*********************** function declaration ***************************
* Name : set_text
* Description : set the status bar text
************************************************************************/
static void text_set(HProcessBar *p_Me, const char *p_process_status_str){
	int			i_len = 0;

	if (p_process_status_str != NULL){
		if ( (i_len = strlen(p_process_status_str)) > 0){
			if (p_Me->p_process_status_str){
				vm_free(p_Me->p_process_status_str);
				p_Me->p_process_status_str = NULL;
			}
 			p_Me->p_process_status_str = (char *)vm_malloc(i_len + 1);
 			if (NULL == p_Me->p_process_status_str){
 				return;
 			}
 			memset(p_Me->p_process_status_str, 0, i_len + 1);
 			strcpy(p_Me->p_process_status_str, p_process_status_str);
		}
	}else{
		if (p_Me->p_process_status_str){
			vm_free(p_Me->p_process_status_str);
			p_Me->p_process_status_str = NULL;
		}
	}
}


/*********************** function declaration ***************************
* Name : hprocessbar_visible
* Description : show the processbar int the popupmenu plane, and repaint
************************************************************************/
void hprocessbar_visible(struct _HProcessBar *p_Me, 
		const char *p_process_status_str, char *process_image_name, VMUINT16 i_color, VMUINT16 bg_color) {
	HWidget			*p_widget = NULL;
	HPlane				*p_plane = NULL;
 
	if (p_Me == NULL) {
		return;
	}
	p_widget = (HWidget*)p_Me;

	p_plane = window->get_popupmenu_plane();
	if (p_plane == NULL) {
		return;
	}
	p_plane->base.base.p_widget_ops->set_bgcolor((HWidget*)p_plane, VM_COLOR_BLUE);
	p_plane->p_plane_ops->add_widget(p_plane, (HWidget *)p_Me);

	if (i_color != bg_color) {
		p_widget->i_color = i_color;
		p_widget->i_bgcolor = bg_color;
	}

	text_set(p_Me, p_process_status_str);

	if (process_image_name != NULL) {
		p_Me->set_process_image_from_resource(p_Me, process_image_name);
	}

	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 1);
//	p_plane->p_content->base.p_widget_ops->invalidate((HWidget *)p_plane->p_content);
//	window->validate();
	window->repaint();
}

/*********************** function declaration ***************************
* Name : hprocessbar_hide
* Description : hide the processbar int the popupmenu plane, and repaint
************************************************************************/
void hprocessbar_hide(HProcessBar *p_Me){
	HPlane *p_plane = window->get_popupmenu_plane();

	p_plane->p_plane_ops->remove_widget(p_plane, (HWidget*)p_Me);

	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 0);
	window->repaint();
}

/*********************** function declaration ***************************
* Name : hprocessbar_paint
* Description : paint the processbar
************************************************************************/
static void hprocessbar_paint(HWidget *p_widget, int i_layer, short s_x, short s_y){
	HProcessBar			*p_Me = (HProcessBar *)p_widget;
	HPoint					p_tp = {0};
	HRect						r_tp = {0};

	vm_graphic_set_clip(s_x, s_y, s_x + p_widget->s_width, s_y + p_widget->s_height);
	draw_rect_by_widget(i_layer,p_widget, s_x, s_y);

	p_tp.s_x = s_x + p_widget->s_padding_left;
	p_tp.s_y = s_y + p_widget->s_padding_top;
	vm_graphic_set_clip(p_tp.s_x, p_tp.s_y, 
		p_tp.s_x+ p_Me->inside_image_width, p_tp.s_y + p_Me->inside_height);
	if (p_Me->process_image){
		p_Me->process_image->p_oper->set_size(p_Me->process_image, p_Me->inside_image_width, p_Me->inside_height);
		p_Me->process_image->p_oper->paint(p_Me->process_image, i_layer, p_tp.s_x, p_tp.s_y);
	}

	p_tp.s_x = s_x + p_widget->s_width - p_widget->s_padding_right -p_Me->inside_test_width ;
	p_tp.s_y = s_y + p_widget->s_padding_top;
	vm_graphic_set_clip(p_tp.s_x, p_tp.s_y, 
		p_tp.s_x + p_Me->inside_test_width, p_tp.s_y + p_Me->inside_height);
	if (p_Me->p_process_status_str){
		r_tp.s_x = p_tp.s_x;
		r_tp.s_y = p_tp.s_y;
		r_tp.s_width = p_Me->inside_test_width;
		r_tp.s_height = p_Me->inside_height;

		draw_gb2312_text_by_hrect_center(i_layer, r_tp, 
			p_Me->p_process_status_str, p_widget->c_font, p_widget->i_color);
	}
	vm_graphic_set_clip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/*********************** function declaration ***************************
* Name : hprocessbar_destroy
* Description : destroy the processbar
************************************************************************/
static void hprocessbar_destroy(HWidget *p_widget)
{
	HProcessBar* p_Me = (HProcessBar*)p_widget;
	if (p_Me){
		if (p_Me->p_process_status_str){
			vm_free(p_Me->p_process_status_str);
		}

		if (p_Me->process_image) {
			himage_delete(p_Me->process_image);
		}

		p_Me->process_image = NULL;
		p_Me->p_process_status_str = NULL;
		p_Me->set_text = NULL;
		p_Me->set_process_image_from_buffer = NULL;
		p_Me->set_process_image_from_resource = NULL;
		p_Me->hprocessbar_visible = NULL;
		p_Me->hprocessbar_hide = NULL;
		vm_free(p_Me);
	}
}

/***************** hstatusbar_new function declaration *******************
* Name : hprocessbar_new
* Description : create a processbar
************************************************************************/
HProcessBar *hprocessbar_new(){
	HProcessBar			*p_Me = NULL;
	HWidget				*p_par = NULL;

	p_Me = (HProcessBar *)vm_malloc(sizeof(HProcessBar));
	if (p_Me == NULL) {
		return NULL;
	}

	memset(p_Me, 0, sizeof(HProcessBar));

	p_par = (HWidget *)p_Me;

	hwidget_init(p_par);
	hwidget_operation_init(&p_Me->parent_ops);

	p_Me->parent_ops.pen_press = press_pen;
	p_Me->parent_ops.pen_release = release_pen;
	p_Me->parent_ops.pen_move = move_pen;
	p_Me->parent_ops.pen_enter = enter_pen;
	p_Me->parent_ops.pen_leave = leave_pen;
	p_Me->parent_ops.key_press = press_key;

	p_Me->parent_ops.paint = hprocessbar_paint;
	p_Me->parent_ops.destroy = hprocessbar_destroy;

	p_par->p_widget_ops = &p_Me->parent_ops;

	p_par->s_top_x = (SCREEN_WIDTH - PROCESSBAR_WIDTH) / 2;
	p_par->s_top_y = SCREEN_HEIGHT * 3 / 5;
	p_par->c_font = FONT_SMALL;
	p_par->s_padding_bottom = PROCESSBAR_PADDING;
	p_par->s_padding_left = PROCESSBAR_PADDING;
	p_par->s_padding_right = PROCESSBAR_PADDING;
	p_par->s_padding_top = PROCESSBAR_PADDING;
	p_par->s_width = PROCESSBAR_WIDTH;
	p_par->s_height = PROCESSBAR_HEIGHT;
	p_par->i_color = VM_COLOR_BLACK;
	p_par->i_bgcolor = VM_COLOR_WHITE;

	p_Me->inside_height =  p_par->s_height - p_par->s_padding_top - p_par->s_padding_bottom;
	p_Me->inside_image_width = p_Me->inside_height;
	p_Me->inside_test_width = p_par->s_width - p_par->s_padding_left - p_par->s_padding_right
		- p_Me->inside_image_width - PROCESSBAR_MIDDLE_INTERVAL;

	p_Me->process_image = NULL;
	p_Me->p_process_status_str = NULL;
	p_Me->set_text = text_set;
	p_Me->set_process_image_from_buffer = process_image_set_buffer;
	p_Me->set_process_image_from_resource = process_image_set_resouce;
	p_Me->hprocessbar_visible = hprocessbar_visible;
	p_Me->hprocessbar_hide = hprocessbar_hide;

	return p_Me;
}

/***************** hstatusbar_new function declaration *******************
* Name : hprocessbar_delete
* Description : delete a processbar
************************************************************************/
void hprocessbar_delete(struct _HProcessBar *p_Me){
	if (p_Me){
		if (p_Me->p_process_status_str){
			vm_free(p_Me->p_process_status_str);
		}

		if (p_Me->process_image) {
			himage_delete(p_Me->process_image);
		}

		p_Me->process_image = NULL;
		p_Me->p_process_status_str = NULL;
		p_Me->set_text = NULL;
		p_Me->set_process_image_from_buffer = NULL;
		p_Me->set_process_image_from_resource = NULL;
			p_Me->hprocessbar_visible = NULL;
			p_Me->hprocessbar_hide = NULL;
		vm_free(p_Me);
	}
}
