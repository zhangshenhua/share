/************************************************************************
* Name : hstatusbar.c
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.08.31
* Description : 
************************************************************************/

/************************* INCLUDE FILES ***********************************/
#include						"vmsys.h"
#include						"vmgraph.h"

#include						"string.h"

#include						"hlookandfeel.h"
#include						"hstatusbar.h"
/************************* END INCLUDE FILES *******************************/
#define							STATUSBAR_BASE_HEIGHT	8
#define							STATUSBAR_LEN_BORDER	6
#define							STATUSBAR_LEN_MAX		32
#define							CHARACTER_HEIGHT		(vm_graphic_get_character_height())
#define							STATUSBAR_HEIGHT		(CHARACTER_HEIGHT + STATUSBAR_BASE_HEIGHT)
#define							SCREEN_WIDTH			(vm_graphic_get_screen_width())
#define							SCREEN_HEIGHT			(vm_graphic_get_screen_height())


/********************** public member *********************************/

/********************* end public member ********************************/



/********************** private member *********************************/

/********************* end private member ********************************/


/*** ***/

/*** end  ***/

/*** press_pen***/
static void press_pen(HWidget *p_par, short s_x, short s_y){
	;
}
/*** end press_pen***/

/*** release_pen***/
static void release_pen(HWidget *p_par, short s_x, short s_y){
	HStatusBar			*p_Me = (HStatusBar *)p_par;
	HEvent				h_event = {0};
	if (p_par->action_performed){
		h_event.i_event_type = VM_PEN_EVENT_RELEASE;
		if (p_Me->pc_left_str){
			if (p_par->s_padding_left + p_Me->s_left_len > s_x && (0 < s_x)){
				p_par->action_performed(p_par, (HEvent *)&h_event, (void *)STATUSBAR_LEFT);
				return;
			}
		}
		if (p_Me->pc_right_str){
			if ((SCREEN_WIDTH - p_par->s_padding_right - p_Me->s_right_len < s_x) && (s_x < SCREEN_WIDTH)){
				p_par->action_performed(p_par, (HEvent *)&h_event, (void *)STATUSBAR_RIGHT);
				return;
			}
		}
	}
}
/*** end release pen***/

/*** move_pen***/
static void move_pen(HWidget *p_par, short s_x, short s_y){
	;
}
/*** end move_pen***/

/*** enter_pen***/
static void enter_pen(HWidget *p_par, short s_x, short s_y){
	;
}
/*** end enter_pen***/

/*** leave_pen***/
static void leave_pen(HWidget *p_par, short s_x, short s_y){
	;
}
/*** end leave_pen***/

/*** press_key***/
static void press_key(HWidget *p_par, int keycode){
	;
}
/*** end press_key***/

/*** image_clean***/
static void image_clean(HStatusBar *p_Me){
	himage_delete(p_Me->bg_image);
	p_Me->bg_image = NULL;
}
/*** end image_clean ***/

/*** loading image for the pushbutton ***/
static void image_set_resouce(HStatusBar *p_Me, char *pc_image_name){
	if (p_Me->bg_image){
		himage_delete(p_Me->bg_image);
		p_Me->bg_image = NULL;
	}
	p_Me->bg_image = himage_new();
	if (NULL == p_Me->bg_image){
		p_Me->bg_image = NULL;
		return;
	}

	p_Me->bg_image->p_oper->load_image(p_Me->bg_image, pc_image_name);
}
/*** end image_set_resource ***/

/*** loading image for the pushbutton ***/
static void image_set_buffer(HStatusBar *p_Me, VMUINT8 *pi_buf, VMINT i_size){
	if (p_Me->bg_image){
		himage_delete(p_Me->bg_image);
		p_Me->bg_image = NULL;
	}
	p_Me->bg_image = himage_new();
	if (NULL == p_Me->bg_image){
		p_Me->bg_image = NULL;
		return;
	}

	p_Me->bg_image->p_oper->load_image_2(p_Me->bg_image, pi_buf, i_size);

}
/*** end image_set_buffer ***/

/*** preferred_width ***/
static short preferred_width(HWidget *p_Me){
	return (SCREEN_WIDTH);
}
/*** end preferred ***/

/*** return preferred label height ***/
static short preferred_height(HWidget *p_Me){
	return (STATUSBAR_HEIGHT);
}
/*** end preferred***/

/*** get_len_wstr ***/
static short get_len_wstr(char *pc_str){
	VMWCHAR			ac_wstr[STATUSBAR_LEN_MAX] = {0};

	if (NULL == pc_str){
		return 0;
	}
	vm_gb2312_to_ucs2(ac_wstr, STATUSBAR_LEN_MAX, pc_str);
	return (vm_graphic_get_string_width(ac_wstr));
}
/*** end get_len_wstr ***/

/*** text_status_set ***/
static char *text_status_set(const char *pc_str){
	int			i_len = 0;
	char		*pc_sstr = NULL;

	if (!pc_str){
		return pc_sstr;
	}

	i_len = strlen(pc_str);
	if (0 < i_len){
		pc_sstr = (char *)vm_malloc(i_len + 1);
		if (NULL == pc_sstr){
			return pc_sstr;
		}

		memset(pc_sstr, 0, i_len + 1);
		strcpy(pc_sstr, pc_str);
	}
	return pc_sstr;
}
/*** end text_status_set ***/

/*** text_set***/
static void text_set(HStatusBar *p_Me, const char *pc_left_str, const char *pc_right_str){
	if (!(pc_left_str || pc_right_str)){
		return;
	}

	if (pc_left_str){
		if (0 < strlen(pc_left_str)){
			if (p_Me->pc_left_str){
				vm_free(p_Me->pc_left_str);
			}
			p_Me->pc_left_str = text_status_set(pc_left_str);
			p_Me->s_left_len = get_len_wstr(p_Me->pc_left_str);
		}
	}else{
		if (p_Me->pc_left_str){
			vm_free(p_Me->pc_left_str);
			p_Me->pc_left_str = NULL;
			p_Me->s_left_len = 0;
		}
	}

	if (pc_right_str){
		if (0 < strlen(pc_right_str)){
			if (p_Me->pc_right_str){
				vm_free(p_Me->pc_right_str);
			}
			p_Me->pc_right_str = text_status_set(pc_right_str);
			p_Me->s_right_len = get_len_wstr(p_Me->pc_right_str);
		}
	}else{
		if (p_Me->pc_right_str){
			vm_free(p_Me->pc_right_str);
			p_Me->pc_right_str = NULL;
			p_Me->s_right_len = 0;
		}
	}
}
/*** end text_set ***/

/*** draw ***/
static void draw(HWidget *p_widget, int i_layer, short s_x, short s_y){
	HStatusBar				*p_Me = (HStatusBar *)p_widget;
	HPoint					p_tp = {0};
	HRect					r_tp = {0};

	if (-1 != p_widget->i_bgcolor){
		r_tp.s_x = s_x;
		r_tp.s_y = s_y;
		r_tp.s_height = STATUSBAR_HEIGHT;
		r_tp.s_width = SCREEN_WIDTH;

		draw_rect_by_hrect(i_layer, r_tp, p_widget->i_bgcolor, p_widget->i_bgcolor);
	}

	if (p_Me->bg_image){
		p_Me->bg_image->p_oper->set_size(p_Me->bg_image, SCREEN_WIDTH, STATUSBAR_HEIGHT);
		p_Me->bg_image->p_oper->paint(p_Me->bg_image, i_layer, s_x, s_y);
	}

	p_tp.s_y = SCREEN_HEIGHT - p_widget->s_padding_bottom - CHARACTER_HEIGHT;
	if (p_Me->pc_left_str){
		p_tp.s_x = p_widget->s_padding_left;
		draw_gb2312_text_by_hpoint(i_layer, p_Me->pc_left_str, p_tp, p_widget->i_color);
	}
	if (p_Me->pc_right_str){
		p_tp.s_x = SCREEN_WIDTH - p_widget->s_padding_right - p_Me->s_right_len;
		draw_gb2312_text_by_hpoint(i_layer, p_Me->pc_right_str, p_tp, p_widget->i_color);
	}
}
/*** end draw ***/

/*** initialize***/
static void initialize(HStatusBar *p_Me){
	p_Me->set_text = text_set;
	p_Me->set_image_from_buffer = image_set_buffer;
	p_Me->set_image_from_resource = image_set_resouce;
	p_Me->clean_bgimage = image_clean;

	p_Me->pc_right_str = NULL;
	p_Me->pc_left_str = NULL;
	p_Me->s_left_len = 0;
	p_Me->s_right_len = 0;
}
/*** end initialize ***/

/*** hstatusbar_new ***/
HStatusBar *hstatusbar_new(const char *pc_left_str, const char *pc_right_str){
	HStatusBar				*p_Me = (HStatusBar *)vm_malloc(sizeof(HStatusBar));
	HWidget					*p_par = NULL;

	if (NULL == p_Me){
		return NULL;
	}
	if (!(pc_left_str || pc_right_str)){
		vm_free(p_Me);
		return NULL;
	}
	memset(p_Me, 0, sizeof(HStatusBar));

	p_par = (HWidget *)p_Me;

	hwidget_init(p_par);
	hwidget_operation_init(&p_Me->parent_ops);
	p_Me->parent_ops.paint = draw;
	p_Me->parent_ops.get_prefered_height = preferred_height;
	p_Me->parent_ops.get_prefered_width = preferred_width;
	p_Me->parent_ops.pen_press = press_pen;
	p_Me->parent_ops.pen_release = release_pen;
	p_Me->parent_ops.pen_move = move_pen;
	p_Me->parent_ops.pen_enter = enter_pen;
	p_Me->parent_ops.pen_leave = leave_pen;
	p_Me->parent_ops.key_press = press_key;

	p_par->p_widget_ops = &p_Me->parent_ops;

	p_par->s_top_x = 0;
	p_par->s_top_y = SCREEN_HEIGHT - STATUSBAR_HEIGHT;
	p_par->c_font = FONT_SMALL;
	p_par->s_padding_bottom = STATUSBAR_BASE_HEIGHT/2;
	p_par->s_padding_left = STATUSBAR_LEN_BORDER;
	p_par->s_padding_right = STATUSBAR_LEN_BORDER;
	p_par->s_padding_top = STATUSBAR_BASE_HEIGHT/2;
	p_par->s_width = SCREEN_WIDTH;
	p_par->s_height = STATUSBAR_HEIGHT;
	p_par->i_color = 0X0000;
	p_par->i_bgcolor = -1;

	initialize(p_Me);
	text_set(p_Me, pc_left_str, pc_right_str);

	return p_Me;
}
/*** end hstatusbar_new ***/

/*** hstatusbar_delete ***/
void hstatusbar_delete(HStatusBar *p_Me){
	if (p_Me){
		if (p_Me->pc_left_str){
			vm_free(p_Me->pc_left_str);
		}
		if (p_Me->pc_right_str){
			vm_free(p_Me->pc_right_str);
		}
		if (p_Me->bg_image){
			himage_delete(p_Me->bg_image);
		}
		p_Me->set_text = NULL;
		p_Me->set_image_from_buffer = NULL;
		p_Me->set_image_from_resource = NULL;
		p_Me->clean_bgimage = NULL;
		vm_free(p_Me);
	}
}
/*** end hstatusbar_delete ***/


/******************************* EOF ***********************************/