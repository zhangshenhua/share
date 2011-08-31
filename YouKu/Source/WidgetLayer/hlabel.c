/************************************************************************
* Name : hlabel.c
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.08.19
* Description : 
************************************************************************/
#define			WSTR_LEN_MAX			256
#define			WSTR_LINE_MAX		32
/************************* INCLUDE FILES ***********************************/
#include "HLabel.h"
#include "vmsys.h"
#include "vmchset.h"
#include "vmgraph.h"

#include	"string.h"
/************************* END INCLUDE FILES *******************************/
static HWidgetOperation		*p_label_class_ops = NULL;

/*** ***/
/*** ***/

/*** set label spacing ***/
static void spacing_set(HLabel *p_Me, int i_spacing){
	if ((i_spacing > 0) && (i_spacing < vm_graphic_get_character_height() * 8)){
		p_Me->text_row_spacing = i_spacing;
	}
}
/*** end spacing***/

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

/*** set_label_row_width_count***/
static void set_label_row_width_count(HLabel *p_Me){
	HWidget		*p_par = (HWidget *)p_Me;
	VMWCHAR		ac_wstr[WSTR_LEN_MAX] = {0};
	VMWCHAR		*pc_tp = NULL, *pc_str = NULL;
	int			i_len = 0, i_ret = 0, i_wh = 0, i_ht = 0, i_wh_ret = 0;

	if (NULL == p_Me->text){
		return;
	}

	vm_gb2312_to_ucs2(ac_wstr, WSTR_LEN_MAX, p_Me->text);
	pc_tp = ac_wstr;
	while ('\0' != *pc_tp){
		if ('\n' == *pc_tp){
			i_ret++;
		}
	}
	i_wh = p_par->p_widget_ops->get_max_width(p_par) - p_par->uc_padding_left - p_par->uc_padding_right;
	if (!i_ret){
		i_len = vm_graphic_get_string_width(ac_wstr);
		if (i_len > i_wh){
			p_Me->text_row_width = i_wh;
			p_Me->text_row_count = (i_len%i_wh) ? (i_len/i_wh + 1) : (i_len/i_wh);
		}else{
			p_Me->text_row_width = i_len;
			p_Me->text_row_count = 1;
		}
	}else{
		/*	set_label_row_count_width(p_Me);	*/
		pc_tp = pc_str = ac_wstr;
		while (1){
			if ('\0' == *pc_tp){
				i_len = vm_graphic_get_string_width(pc_str);
				if (i_len > i_wh){
					i_wh_ret = i_wh;
					i_ht = (i_len%i_wh) ? (i_ht + (i_len/i_wh) + 1) : (i_ht + i_len/i_wh);
				}else if ((i_len <= i_wh) && (i_len > i_wh_ret)){
					i_wh_ret = i_len;
					i_ht += 1;
				}else{
					i_ht += 1;
				}
				break;
			}else if ('\n' == *pc_tp){
				*pc_tp = '\0';
				i_len = vm_graphic_get_string_width(pc_str);
				if (i_len > i_wh){
					i_wh_ret = i_wh;
					i_ht = (i_len%i_wh) ? (i_ht + (i_len/i_wh) + 1) : (i_ht + i_len/i_wh);
				}else if ((i_len <= i_wh) && (i_len > i_wh_ret)){
					i_wh_ret = i_len;
					i_ht += 1;
				}else{
					i_ht += 1;
				}
				pc_str = pc_tp + 1;
			}
			pc_tp++;
		}
		p_Me->text_row_count = i_ht;
		p_Me->text_row_width = i_wh_ret;
	}
	p_Me->uc_ps = (unsigned char)1;
}
/*** end set_text_row_width_count***/

/*** set label text ***/
static void text_set(HLabel *p_Me, const char *pc_str){
	HWidget		*p_parent = (HWidget *)p_Me;
	int			i_len = 0;
	char		*pc_sstr = NULL;


	if (!pc_str){
		return;
	}
	i_len = strlen(pc_str);
	if (i_len > WSTR_LEN_MAX * 2){
		return;
	}
	if (0 >= i_len){
		if (p_Me->text){
			vm_free(p_Me->text);
			p_Me->text_row_count = 0;
			p_Me->text_row_width = 0;
		}
		p_Me->text = NULL;
	}else{
		if (p_Me->text){
			vm_free(p_Me->text);
			p_Me->text_row_count = 0;
			p_Me->text_row_width = 0;
		}

		pc_sstr = (char *)vm_malloc(i_len + 1);
		if (NULL == pc_sstr){
			p_Me->text = NULL;
			p_Me->text_row_count = 0;
			p_Me->text_row_width = 0;
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

/*** loading image for the label ***/
static void image_set_resouce(HLabel *p_Me, const char *pc_image_name){
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

/*** loading image for the label ***/
static void image_set_buffer(HLabel *p_Me, VMUINT8 *pi_buf, VMINT i_size){
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
	HLabel		*p_this = (HLabel *)p_Me;

	if (p_Me->s_prefered_width){
		return p_Me->s_prefered_width;
	}else{
		if (p_this->text){
			if ((unsigned char)1 != p_this->uc_ps){
				set_label_row_width_count(p_this);
			}
			return (p_Me->s_prefered_width = p_this->text_row_width + p_Me->uc_padding_left + p_Me->uc_padding_right);
		}else{
			return (p_Me->s_prefered_width = p_Me->uc_padding_right + p_Me->uc_padding_left);
		}
	}
}
/*** end preferred ***/

/*** return preferred label height ***/
static short preferred_height(HWidget *p_Me){
	HLabel		*p_this = (HLabel *)p_Me;

	if (p_Me->s_prefered_height){
		return p_Me->s_prefered_height;
	}else{
		if (p_this->text){
			if ((unsigned char)1 != p_this->uc_ps){
				set_label_row_width_count(p_this);
			}
			return (p_Me->s_prefered_height = p_this->text_row_spacing * (p_this->text_row_count - 1) + \
				p_this->text_row_count * vm_graphic_get_character_height() + p_Me->uc_padding_top + p_Me->uc_padding_bottom);
		}else{
			return (p_Me->s_prefered_height = vm_graphic_get_character_height() + p_Me->uc_padding_bottom + p_Me->uc_padding_top);
		}
	}
}
/*** end preferred***/

/*** draw_output***/
static void draw_output(HLabel *p_Me, int i_layer, short s_x, short s_y, int *i_row, VMWCHAR *pc_str){
	HWidget		*p_par = (HWidget *)p_Me;
	VMWCHAR		*pc_tp = NULL, *pc_wstr = NULL;
	VMWCHAR		ac_wstr[WSTR_LINE_MAX] = {0};
	int			i_ch = 0, i_spa = 0, i_tp = 0;

	if (vm_graphic_get_string_width(pc_str) <= p_Me->text_row_width){
		/*ToDo:(i_layer, s_x, s_y, pc_str, p_par->i_color)*/
	}else{
		i_ch = vm_graphic_get_character_height();
		i_spa = p_Me->text_row_spacing;
		i_tp = 0;
		pc_tp = pc_str;

		memset(ac_wstr, 0, sizeof(ac_wstr));
		while (1){
			ac_wstr[i_tp] = *pc_tp;
			if ('\0' == *pc_tp){
				/*ToDo:(i_layer, s_x, s_y + (*i_row - 1) * (i_ch + i_spa), ac_wstr, p_par->i_color)*/
				break;
			}
			if (vm_graphic_get_string_width(ac_wstr) > p_Me->text_row_width){
				ac_wstr[i_tp] = '\0';
				/*ToDo:(i_layer, s_x, s_y + (*i_row - 1) * (i_ch + i_spa), ac_wstr, p_par->i_color)*/
				*i_row++;
				i_tp = 0;
				pc_tp--;
				memset(ac_wstr, 0, sizeof(ac_wstr));
			}
			i_tp++;
			pc_tp++;
		}
	}

}
/*** and draw_output***/

/*** draw text***/
static void draw_text(HLabel *p_Me, int i_layer, short s_x, short s_y){
	HWidget		*p_par = (HWidget *)p_Me;
	VMWCHAR		ac_wstr[WSTR_LEN_MAX] = {0};
	VMWCHAR		*pc_tp = NULL, *pc_str = NULL;
	int			i_row = 0;

	if (NULL == p_Me->text){
		return;
	}
	vm_gb2312_to_ucs2(ac_wstr, WSTR_LEN_MAX, p_Me->text);
	pc_str = pc_tp = ac_wstr;

	while (1){
		if ('\0' == *pc_tp){
			draw_output(p_Me, i_layer, s_x + p_par->uc_padding_left, s_y, &i_row, pc_str);
			break;
		}else if ('\n' == *pc_tp){
			*pc_tp = '\0';
			draw_output(p_Me, i_layer, s_x + p_par->uc_padding_left, s_y, &i_row, pc_str);
			pc_str = pc_tp + 1;
		}
		i_row++;
		pc_tp++;
	}
}
/*** end draw_text***/

/*** draw ***/
static void draw(HWidget *p_widget, int i_layer, short s_x, short s_y){
	HLabel			*p_Me = (HLabel *)p_widget;
	HRect			s_tp = {0};

	s_tp.s_x = s_x;
	s_tp.s_y = s_y;
	s_tp.s_width = p_widget->s_width;	
	s_tp.s_height = p_widget->s_height;

	/*ToDo: draw label(int i_layer, s_tp, p_widget->i_color, p_widget->i_bgcolor)	*/
	if (p_Me->bg_image){
		p_Me->bg_image->p_oper->paint(p_Me->bg_image, i_layer, s_x, s_y);
	}
	if (p_Me->text){
		if ((unsigned char)1 != p_Me->uc_ps){
			set_label_row_width_count(p_Me);
		}
		draw_text(p_Me, i_layer, s_x, s_y);
	}
}
/*** end draw ***/


/*** initialize label ***/
static void initialize(HLabel *p_Me){
	HWidget		*p_parent = (HWidget *)p_Me;

	p_Me->set_text = text_set;
	p_Me->set_image_from_resource = image_set_resouce;
	p_Me->set_image_from_buffer = image_set_buffer;
	p_Me->set_spacing = spacing_set;
	p_Me->text = NULL;
	p_Me->bg_image = NULL;
	p_Me->text_row_width = 0;
	p_Me->text_row_count = 0;
	p_Me->text_row_spacing = vm_graphic_get_character_height();
	p_Me->uc_ps = (unsigned char)1;

}
/*** end init ***/

/*** create a label ***/
HLabel *hlabel_new(){
	HWidget		*p_parent = NULL;
	HLabel		*p_this = (HLabel *)vm_malloc(sizeof(HLabel));

	if (NULL == p_this){
		return NULL;
	}
	memset(p_this, 0, sizeof(HLabel));
	p_parent = (HWidget *)p_this;

	hwidget_init(p_parent);
	if (!p_label_class_ops){
		p_label_class_ops = (HWidgetOperation *)vm_malloc(sizeof(HWidgetOperation));
		if (!p_label_class_ops){
			vm_free(p_this);
			return NULL;
		}
		hwidget_operation_init(p_label_class_ops);
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
/*** end label_create ***/

/*** destroy label ***/
void hlabel_delete(HLabel *p_Me){
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
		p_Me->set_spacing = NULL;

		vm_free(p_Me);
	}
}
/*** end destroy ***/



/******************************* EOF ***********************************/
