/************************************************************************
* Name : hpushbutton.c
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.08.19
* Description : 
************************************************************************/
#define			WSTR_LINE_MAX				32
/************************* INCLUDE FILES ***********************************/
#include "hwidget.h"
#include "vmsys.h"
#include "hpushbutton.h"
#include "vmchset.h"
#include "vmgraph.h"

#include	"string.h"
/************************* END INCLUDE FILES *******************************/

static HWidgetOperation		*p_pushbutton_class_ops = NULL;

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

/*** set text width and height ***/
static void text_width_height(HPushButton *p_Me){
	HWidget		*p_par = (HWidget *)p_Me;
	VMWCHAR		ac_wstr[WSTR_LINE_MAX] = {0};
	int			i_ret = 0, i_len = 0;

	i_len = p_par->p_widget_ops->get_max_width(p_par) - p_par->uc_padding_left - p_par->uc_padding_right;
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
static void image_set_resouce(HPushButton *p_Me, const char *pc_image_name){
	if (p_Me->bg_image){
		vm_free(p_Me->bg_image);
		p_Me->bg_image = NULL;
	}
	p_Me->bg_image = (HImage *)vm_malloc(sizeof(HImage));
	if (NULL == p_Me->bg_image){
		p_Me->bg_image = NULL;
		return;
	}
	himage_init(p_Me->bg_image);
	p_Me->bg_image->p_oper->load_image(p_Me->bg_image, pc_image_name);
}
/*** end image_set_resource ***/

/*** loading image for the pushbutton ***/
static void image_set_buffer(HPushButton *p_Me, VMUINT8 *pi_buf, VMINT i_size){
	if (p_Me->bg_image){
		vm_free(p_Me->bg_image);
		p_Me->bg_image = NULL;
	}
	p_Me->bg_image = (HImage *)vm_malloc(sizeof(HImage));
	if (NULL == p_Me->bg_image){
		p_Me->bg_image = NULL;
		return;
	}
	himage_init(p_Me->bg_image);
	p_Me->bg_image->p_oper->load_image_2(p_Me->bg_image, pi_buf, i_size);

}
/*** end image_set_buffer ***/

/*** return preferred label width ***/
static short preferred_width(HWidget *p_Me){
	HPushButton		*p_this = (HPushButton *)p_Me;

	if (p_Me->s_prefered_width){
		return p_Me->s_prefered_width;
	}else{
		if ((unsigned char)1 != p_this->uc_ps){
			text_width_height(p_this);
		}
		return (p_Me->s_prefered_width = p_Me->uc_padding_left +p_Me->uc_padding_right + p_this->text_width);
	}
}
/*** end preferred ***/

/*** return preferred label height ***/
static short preferred_height(HWidget *p_Me){
	HPushButton		*p_this = (HPushButton *)p_Me;

	if (p_Me->s_prefered_height){
		return p_Me->s_prefered_height;
	}else{
		return (p_Me->s_prefered_height = vm_graphic_get_character_height() + p_Me->uc_padding_bottom + p_Me->uc_padding_top);
	}
}
/*** end preferred***/

/*** draw ***/
static void draw(HWidget *p_widget, int i_layer, short s_x, short s_y){
	HPushButton			*p_Me = (HPushButton *)p_widget;
	HRect				s_tp = {0};

	s_tp.s_x = s_x;
	s_tp.s_y = s_y;
	s_tp.s_width = p_widget->s_width;
	s_tp.s_height = p_widget->s_height;
	/*ToDo: draw pushbutton(int i_layer, s_tp, p_widget->i_color, p_widget->bg_color)*/
	if (p_Me->bg_image){
		p_Me->bg_image->p_oper->paint(p_Me->bg_image, i_layer, s_x, s_y);
	}
	if (p_Me->text){
		s_tp.s_x += p_widget->uc_padding_left;
		s_tp.s_y += p_widget->uc_padding_top;
		s_tp.s_height = 0;
		s_tp.s_width = 0;
		/*ToDo: text output(int i_layer, HRect s_tp, char * p_Me->text, p_widget->i_color)*/
	}
}
/*** end draw ***/

/*** initialize the pushbutton ***/
static void initialize(HPushButton *p_Me){

	p_Me->set_text = text_set;
	p_Me->set_image_from_resource = image_set_resouce;
	p_Me->set_image_from_buffer = image_set_buffer;
	p_Me->text = NULL;
	p_Me->bg_image = NULL;
	p_Me->text_height = 0;
	p_Me->text_width = 0;
	p_Me->uc_ps = (unsigned char)1;
}
/*** end initialize ***/

/*** create a pushbutton ***/
HPushButton *hpushbutton_new(){
	HPushButton		*p_this = (HPushButton *)vm_malloc(sizeof(HPushButton));
	HWidget			*p_parent = NULL;
	if (NULL == p_this){
		return NULL;
	}
	memset(p_this, 0, sizeof(HPushButton));

	p_parent = (HWidget *)p_this;

	hwidget_init(p_parent);
	if (!p_pushbutton_class_ops){
		p_pushbutton_class_ops = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		if (!p_pushbutton_class_ops){
			vm_free(p_this);
			return NULL;
		}
		hwidget_operation_init(p_pushbutton_class_ops);
		p_parent->p_widget_ops->paint = draw;
		p_parent->p_widget_ops->get_prefered_width = preferred_width;
		p_parent->p_widget_ops->get_prefered_height = preferred_height;
		p_parent->p_widget_ops->pen_press = press_pen;
		p_parent->p_widget_ops->pen_release = release_pen;
		p_parent->p_widget_ops->pen_move = move_pen;
		p_parent->p_widget_ops->pen_enter = enter_pen;
		p_parent->p_widget_ops->pen_leave = leave_pen;
		p_parent->p_widget_ops->key_press = press_key;

		p_parent->p_widget_ops->set_font(p_parent, FONT_SMALL);
	}

	initialize(p_this);

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
			vm_free(p_Me->bg_image);
		}

		p_Me->set_text = NULL;
		p_Me->set_image_from_buffer= NULL;
		p_Me->set_image_from_resource = NULL;

		vm_free(p_Me);
	}
}
/*** end pushbutton_destroy ***/

/******************************* EOF ***********************************/
