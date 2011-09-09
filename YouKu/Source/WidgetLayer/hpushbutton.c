/************************************************************************
* Name : hpushbutton.c
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.08.19
* Description : 
************************************************************************/
#define			WSTR_LINE_MAX				32

/************************* INCLUDE FILES ***********************************/
#include			"hwidget.h"
#include			"vmsys.h"
#include			"hpushbutton.h"
#include			"vmchset.h"
#include			"vmgraph.h"

#include			"string.h"
#include			"hlookandfeel.h"
#include			"himage.h"
/************************* END INCLUDE FILES *******************************/

static HWidgetOperation		*p_base_ops_button = NULL;
static HPUSHBUTTON_OPS		*p_button_ops = NULL;
static int					i_num_button = 0;

/*** ***/

/*** end  ***/

/*** press_pen***/
static void press_pen(HWidget *p_par, short s_x, short s_y){
	;
}
/*** end press_pen***/

/*** release_pen***/
static void release_pen(HWidget *p_par, short s_x, short s_y){
	if (p_par->action_performed){
		p_par->action_performed(p_par, NULL);
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

/*** set_text_point ***/
static void text_point_set(HPushButton *p_Me, short s_x, short s_y){
	if ((0 > s_x) && (vm_graphic_get_screen_width() < s_x) && (vm_graphic_get_screen_height() < s_y) && (0 > s_y)){
		return;
	}
	p_Me->base.s_padding_left = s_x;
	p_Me->base.s_padding_top = s_y;
}
/*** end set_text_point ***/

/*** test_set_mid ***/
static void text_set_mid(HPushButton *p_Me, int i_ismid){
	if (0 == i_ismid){
		p_Me->uc_ismid = (unsigned char)0;
	}else if (1 == i_ismid){
		p_Me->uc_ismid = (unsigned char)1;
	}
}
/*** end test_set_mid ***/

/*** image_clean***/
static void image_clean(HPushButton *p_Me){
	himage_delete(p_Me->bg_image);
	p_Me->bg_image = NULL;
}
/*** end image_clean ***/

/*** image_clean***/
static void button_autosize_image(HPushButton *p_Me, int i_isautosize){
	if (0 == i_isautosize){
		p_Me->uc_isautosize = (unsigned char)0;
	}else if (1 == i_isautosize){
		p_Me->uc_isautosize = (unsigned char)1;
	}
}
/*** end image_clean ***/


/*** set text width and height ***/
static void text_width_height(HPushButton *p_Me){
	HWidget		*p_par = (HWidget *)p_Me;
	VMWCHAR		ac_wstr[WSTR_LINE_MAX] = {0};
	int			i_ret = 0, i_len = 0;

	i_len = p_par->p_widget_ops->get_max_width(p_par) - p_par->s_padding_left - p_par->s_padding_right;
	if (p_Me->text){
		vm_gb2312_to_ucs2(ac_wstr, WSTR_LINE_MAX, p_Me->text);
		i_ret = vm_graphic_get_string_width(ac_wstr);
		p_Me->text_width = (i_ret <= i_len) ? (i_ret) : (i_len);
		p_Me->text_height = vm_graphic_get_character_height();
	}
	p_Me->uc_ps = (unsigned char)1;
}
/*** end text_width_height ***/

/*** set pushbutton text ***/
static void text_set(HPushButton *p_Me, const char *pc_str){
	HWidget		*p_parent = (HWidget *)p_Me;
	int			i_len = 0;
	char		*pc_sstr;

	if (!pc_str){
		return;
	}
	i_len = strlen(pc_str);
	if (0 >= i_len){
		if (p_Me->text){
			vm_free(p_Me->text);
		}
		p_Me->text = NULL;
	}else{
		if (p_Me->text){
			vm_free(p_Me->text);
		}
		pc_sstr = (char *)vm_malloc(i_len + 1);
		if (NULL == pc_sstr){
			p_Me->text = NULL;		
			return;
		}
		p_Me->text = pc_sstr;
		memset(p_Me->text, 0, i_len + 1);
		strcpy(p_Me->text, pc_str);
	}
	p_Me->uc_ps = (unsigned char)2;
	p_parent->p_widget_ops->notify_size_changed(p_parent);
}
/*** end text_text ***/

/*** loading image for the pushbutton ***/
static void image_set_resouce(HPushButton *p_Me, char *pc_image_name){
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
static void image_set_buffer(HPushButton *p_Me, VMUINT8 *pi_buf, VMINT i_size){
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

/*** return preferred label width ***/
static short preferred_width(HWidget *p_Me){
	HPushButton		*p_this = (HPushButton *)p_Me;
	short			i_tp_one = 0, i_tp_two = 0;
	if ((unsigned char)1 != p_this->uc_ps){
		text_width_height(p_this);
	}
	i_tp_one = p_Me->s_padding_left +p_Me->s_padding_right + p_this->text_width;

	if (p_this->bg_image){
		i_tp_two = p_this->bg_image->p_oper->get_image_real_width(p_this->bg_image);
		return (p_Me->s_prefered_width = (i_tp_two > i_tp_one ? (i_tp_two) : (i_tp_one)));
	}else{
		return (p_Me->s_prefered_width = i_tp_one);
	}
}
/*** end preferred ***/

/*** return preferred label height ***/
static short preferred_height(HWidget *p_Me){
	HPushButton		*p_this = (HPushButton *)p_Me;
	short			i_tp_one = 0, i_tp_two = 0;

	i_tp_one = vm_graphic_get_character_height() + p_Me->s_padding_bottom + p_Me->s_padding_top;
	if (p_this->bg_image){
		i_tp_two = p_this->bg_image->p_oper->get_image_real_height(p_this->bg_image);
		return (p_Me->s_prefered_height = (i_tp_two > i_tp_one) ? (i_tp_two) : (i_tp_one));
	}else{
		return (p_Me->s_prefered_height = i_tp_one);
	}

}
/*** end preferred***/

/*** draw ***/
static void draw(HWidget *p_widget, int i_layer, short s_x, short s_y){
	HPushButton			*p_Me = (HPushButton *)p_widget;
	HRect				s_tp = {0};
	HPoint				s_p_tp = {0};

	s_tp.s_x = s_x;
	s_tp.s_y = s_y;

	if (p_Me->bg_image && ((unsigned char)1 == p_Me->uc_isautosize)){
		s_tp.s_width = p_Me->bg_image->p_oper->get_image_real_width(p_Me->bg_image);
		s_tp.s_height = p_Me->bg_image->p_oper->get_image_real_height(p_Me->bg_image);
		p_widget->s_height = s_tp.s_height;
		p_widget->s_width = s_tp.s_width;
	}else{
		s_tp.s_width = p_widget->s_width;
		s_tp.s_height = p_widget->s_height;
	}
	
	/*ToDo: draw pushbutton(int i_layer, s_tp, p_widget->i_color, p_widget->bg_color)*/
	/*draw_button(i_layer, s_x, s_y, VM_COLOR_BLUE);*/
	if (-1 == p_widget->i_bgcolor){
		draw_rect_by_hrect_trans(i_layer, s_tp, p_widget->i_color);
	}else{
		draw_rect_by_hrect(i_layer, s_tp, p_widget->i_color, p_widget->i_bgcolor);
	}
	if (p_Me->bg_image){
		p_Me->bg_image->p_oper->set_size(p_Me->bg_image,s_tp.s_width, s_tp.s_height);
		p_Me->bg_image->p_oper->paint(p_Me->bg_image, i_layer, s_x, s_y);
	}
	if (p_Me->text){
		if ((unsigned char)1 == p_Me->uc_ismid){
			if ((unsigned char)1 != p_Me->uc_ps){
				text_width_height(p_Me);
			}
			s_p_tp.s_y = s_y + (p_widget->s_height - p_Me->text_height)/2;
			s_p_tp.s_x = s_x + (p_widget->s_width - p_Me->text_width)/2;
		}else{
			s_p_tp.s_x = s_x + p_widget->s_padding_left;
			s_p_tp.s_y = s_y + p_widget->s_padding_top;
		}

		/*ToDo: text output(int i_layer, HRect s_tp, char * p_Me->text, p_widget->i_color)*/
		/*output_text(i_layer, p_Me->text, s_x, s_y, VM_COLOR_GREEN);*/
/*		draw_gb2312_text_by_layer_handle(i_layer, p_Me->text, s_p_tp, p_widget->i_color);	*/
		draw_gb2312_text_by_hpoint(i_layer, p_Me->text, s_p_tp, p_widget->i_color);
	}

}
/*** end draw ***/

/*** initialize the pushbutton ***/
static void initialize(HPushButton *p_Me){

	p_Me->text = NULL;
	p_Me->bg_image = NULL;
	p_Me->text_height = 0;
	p_Me->text_width = 0;
	p_Me->uc_ps = (unsigned char)1;
	p_Me->uc_ismid = (unsigned char)0;
	p_Me->uc_isautosize = (unsigned char)0;
	i_num_button++;
}
/*** end initialize ***/

/*** create a pushbutton ***/
HPushButton *hpushbutton_new(const char *pc_str){
	HPushButton		*p_this = (HPushButton *)vm_malloc(sizeof(HPushButton));
	HWidget			*p_parent = NULL;

	if (NULL == p_this){
		return NULL;
	}
	memset(p_this, 0, sizeof(HPushButton));

	p_parent = (HWidget *)p_this;

	hwidget_init(p_parent);
	if (!p_button_ops){
		p_button_ops = (HPUSHBUTTON_OPS *)vm_malloc(sizeof(HPUSHBUTTON_OPS));
		if (!p_button_ops){
			vm_free(p_this);
			p_button_ops = NULL;
			return NULL;
		}
		p_button_ops->set_text = text_set;
		p_button_ops->set_image_from_resource = image_set_resouce;
		p_button_ops->set_image_from_buffer = image_set_buffer;
		p_button_ops->set_text_mid = text_set_mid;
		p_button_ops->clean_bgimage = image_clean;
		p_button_ops->set_button_autosize_image = button_autosize_image;
		p_button_ops->set_text_point = text_point_set;
	}

	if (!p_base_ops_button){
		p_base_ops_button = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		if (!p_base_ops_button){
			vm_free(p_button_ops);
			vm_free(p_this);
			p_button_ops = NULL;
			p_base_ops_button = NULL;
			return NULL;
		}
		hwidget_operation_init(p_base_ops_button);
		p_base_ops_button->paint = draw;
		p_base_ops_button->get_prefered_width = preferred_width;
		p_base_ops_button->get_prefered_height = preferred_height;
		p_base_ops_button->pen_press = press_pen;
		p_base_ops_button->pen_release = release_pen;
		p_base_ops_button->pen_move = move_pen;
		p_base_ops_button->pen_enter = enter_pen;
		p_base_ops_button->pen_leave = leave_pen;
		p_base_ops_button->key_press = press_key;

	}

	p_parent->c_font = FONT_SMALL;
	p_parent->s_padding_top = 6;
	p_parent->s_padding_right = 6;
	p_parent->s_padding_left = 6;
	p_parent->s_padding_bottom = 6;
	p_parent->p_widget_ops->set_enable_bgcolor(p_parent, 1);
	p_parent->i_bgcolor = -1;
	p_parent->i_color = VM_COLOR_BLACK;

	p_parent->p_widget_ops = p_base_ops_button;
	p_this->ops = p_button_ops;

	initialize(p_this);
	if (pc_str){
		text_set(p_this, pc_str);
	}

	return p_this;
}
/*** end pushbutton_create ***/

/*** destroy a pushbutton ***/
void hpushbutton_delete(HPushButton *p_Me){
	if (p_Me){
		if (p_Me->text){
			vm_free(p_Me->text);
		}
		if (p_Me->bg_image){
			himage_delete(p_Me->bg_image);
		}
		i_num_button--;
		if (i_num_button <= 0){
			if (p_base_ops_button){
				vm_free(p_base_ops_button);
				p_base_ops_button = NULL;
			}
			if (p_button_ops){
				vm_free(p_button_ops);
				p_button_ops = NULL;
			}
		}

		vm_free(p_Me);
	}
}
/*** end pushbutton_destroy ***/

/******************************* EOF ***********************************/
