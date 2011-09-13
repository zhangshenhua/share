/************************************************************************
* Name : hpopumenu.c
* Copyright : Hisoft
* Autor : ruanqingyong
* Date : 2011.09.01
* Description : 
************************************************************************/

/************************* INCLUDE FILES ***********************************/
#include					"vmsys.h"
#include					"hpopupmenu.h"
#include					"hlookandfeel.h"

#include					"string.h"
/************************* END INCLUDE FILES *******************************/
#define						POPUP_WIDTH_PADDING					6
#define						POPUP_MEM_WIDTH						vm_graphic_get_screen_width() / 3 + POPUP_WIDTH_PADDING
#define						POPUP_MEM_HEIGHT					vm_graphic_get_character_height() + POPUP_WIDTH_PADDING
#define						POPUP_DEFAULT_WIDTH					0/*POPUP_MEM_WIDTH + POPUP_WIDTH_PADDING*/
#define						POPUP_DEFAULT_HEIGHT				0/*POPUP_MEM_HEIGHT*/
#define						POPUP_DISTANCE						2
#define						POPUP_PADDING_LRTB					4


static HPopupOps			*g_popup_ops = NULL;


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
	HPopupMenu			*p_Me = (HPopupMenu *)p_par;
	HEvent				h_event = {0};

	if (p_par->action_performed){
		if (!p_Me->uc_style){
			h_event.i_event_type = VM_PEN_EVENT_RELEASE;
			p_par->action_performed(p_par, (HEvent *)&h_event, (void *)(s_y / p_Me->s_mem_height));
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

/*** member_search***/
static HPOPUPMEMBER	*member_search(HPopupMenu *p_Me, const unsigned char uc_id){
	HPOPUPMEMBER	*p_This = p_Me->member;

	while (p_This){
		if (uc_id == p_This->uc_id){
			return p_This;
		}
		p_This = p_This->next;
	}
	return NULL;
}
/*** end member_search ***/

/*** member_set_width_height ***/
static void member_set_width_height(HPopupMenu *p_Par, HPOPUPMEMBER *p_Mem){
	short			s_tp_w = 0, s_tp_h = 0;
	HWidget			*p_parent = (HWidget *)p_Par;

	if (!p_Mem || !p_Par){
		return;
	}

	if ((p_Mem->rect_img.s_x + p_Mem->rect_img.s_width) >= (p_Mem->rect_text.s_x + p_Mem->rect_text.s_width)){
		s_tp_w = p_Mem->rect_img.s_x + p_Mem->rect_img.s_width;
	}else{
		s_tp_w = p_Mem->rect_text.s_x + p_Mem->rect_text.s_width;
	}

	if ((p_Mem->rect_img.s_y + p_Mem->rect_img.s_height) >= (p_Mem->rect_text.s_y + p_Mem->rect_text.s_height)){
		s_tp_h = p_Mem->rect_img.s_y + p_Mem->rect_img.s_height;
	}else{
		s_tp_h = p_Mem->rect_text.s_y + p_Mem->rect_text.s_height;
	}

	if (s_tp_h > p_Par->s_mem_height){
		p_Par->s_mem_height = s_tp_h + POPUP_WIDTH_PADDING;
		p_parent->s_height = p_Par->s_mem_height * p_Par->uc_mem_num + POPUP_PADDING_LRTB;
	}
	if (s_tp_w > p_Par->s_mem_width){
		p_Par->s_mem_width = s_tp_w + POPUP_WIDTH_PADDING;
		p_parent->s_width = p_Par->s_mem_width + POPUP_PADDING_LRTB;
	}
}
/*** end member_set_width_height ***/

/*** submeber_point***/
static void submember_point(HPopupMenu *p_Me, const unsigned char uc_id, POPUP_TYPE sub_type, HPoint point_Me){
	HPOPUPMEMBER	*p_mem = NULL;

	if (p_mem = member_search(p_Me, uc_id)){
		if (TEXT == sub_type){
			p_mem->rect_text.s_x = point_Me.s_x;
			p_mem->rect_text.s_y = point_Me.s_y;
		}else if (IMAGE == sub_type){
			p_mem->rect_img.s_x = point_Me.s_x;
			p_mem->rect_img.s_y = point_Me.s_y;
		}
	}

}
/*** end submember_point ***/

/*** submember_color ***/
static void submember_color(struct _Popup *p_Me, const unsigned char uc_id, POPUP_TYPE sub_type, int i_color){
	HPOPUPMEMBER	*p_mem = NULL;

	if (p_mem = member_search(p_Me, uc_id)){
		if (FG_COLOR == sub_type){
			p_mem->i_fg_color = i_color;
		}else if (BG_COLOR == sub_type){
			p_mem->i_bg_color = i_color;
		}else if (BORDER_COLOR == i_color){
			p_mem->i_border_color = i_color;
		}
	}
}
/*** end submember_color ***/

/*** initialize the popupmenu's member ***/
static void member_init(HPOPUPMEMBER *p_Me){
	if (!p_Me){
		return;
	}
	memset(p_Me, 0, sizeof(HPOPUPMEMBER));

	p_Me->i_border_color = -1;
	p_Me->i_fg_color = VM_COLOR_BLACK;
	p_Me->i_bg_color = -1;

	p_Me->rect_text.s_x = POPUP_WIDTH_PADDING / 2;
	p_Me->rect_text.s_y = POPUP_WIDTH_PADDING / 2;
	p_Me->rect_img.s_x = POPUP_WIDTH_PADDING * 2;
	p_Me->rect_img.s_y = POPUP_WIDTH_PADDING * 2;
}
/*** end member_init ***/

/*** member_text_modified ***/
static void member_text_modified(HPopupMenu *p_Par, HPOPUPMEMBER *p_Mem){
	int				i_tp_w = 0, i_tp_h = 0;

	if (!p_Mem || !p_Par){
		return;
	}
	if (!p_Par->uc_style){
		if (p_Mem->pc_text){
			get_gb2312_width_and_height(p_Mem->pc_text, &i_tp_w, &i_tp_h);
			p_Mem->rect_text.s_width = i_tp_w;
			p_Mem->rect_text.s_height = i_tp_h;
			member_set_width_height(p_Par, p_Mem);
		}else{
			p_Mem->rect_text.s_width = 0;
			p_Mem->rect_text.s_height = 0;
			member_set_width_height(p_Par, p_Mem);
		}
	}
}
/*** end member_text_modified ***/

/*** member_image_modified ***/
static void member_image_modified(HPopupMenu *p_Par, HPOPUPMEMBER *p_Mem){
	if (!p_Mem || !p_Par){
		return;
	}

	if (!p_Par->uc_style){
		if (p_Mem->p_img){
			p_Mem->rect_img.s_height = p_Mem->p_img->s_height;
			p_Mem->rect_img.s_width = p_Mem->p_img->s_width;
			member_set_width_height(p_Par, p_Mem);
		}else{
			p_Mem->rect_img.s_height = 0;
			p_Mem->rect_img.s_width = 0;
			member_set_width_height(p_Par, p_Mem);
		}
	}
}
/*** end member_image_modified ***/

/*** set pushbutton text ***/
static void text_set(HPopupMenu *p_Me, const unsigned char uc_id, const char *pc_str){
	HPOPUPMEMBER	*p_mem = NULL;
	int				i_len = 0;
	char			*pc_sstr = NULL;

	if (!(p_mem = member_search(p_Me, uc_id)) || !pc_str){
		return;
	}
	i_len = strlen(pc_str);
	if (0 >= i_len){
		if (p_mem->pc_text){
			vm_free(p_mem->pc_text);
		}
		p_mem->pc_text = NULL;
	}else{
		if (p_mem->pc_text){
			vm_free(p_mem->pc_text);
		}
		pc_sstr = (char *)vm_malloc(i_len + 1);
		if (NULL == pc_sstr){
			p_mem->pc_text = NULL;
			member_text_modified(p_Me, p_mem);
			return;
		}
		p_mem->pc_text = pc_sstr;
		memset(p_mem->pc_text, 0, i_len + 1);
		strcpy(p_mem->pc_text, pc_str);
	}
	member_text_modified(p_Me, p_mem);
}
/*** end text_text ***/

/*** loading image for the pushbutton ***/
static void image_set_resouce(HPopupMenu *p_Me, const unsigned char uc_id, char *pc_image_name){
	HPOPUPMEMBER	*p_mem = NULL;

	if (!(p_mem = member_search(p_Me, uc_id)) || !pc_image_name){
		return;
	}
	if (p_mem->p_img){
		himage_delete(p_mem->p_img);
	}
	p_mem->p_img = himage_new();
	if (!p_mem->p_img){
		p_mem->p_img = NULL;
		member_image_modified(p_Me, p_mem);
		return;
	}
	p_mem->p_img->p_oper->load_image(p_mem->p_img, pc_image_name);
	member_image_modified(p_Me, p_mem);
}
/*** end image_set_resource ***/

/*** loading image for the pushbutton ***/
static void image_set_buffer(HPopupMenu *p_Me, const unsigned char uc_id, VMUINT8 *pi_buf, VMINT i_size){
	HPOPUPMEMBER	*p_mem = NULL;

	if (!(p_mem = member_search(p_Me, uc_id))){
		return;
	}
	if (p_mem->p_img){
		himage_delete(p_mem->p_img);
	}
	p_mem->p_img = himage_new();
	if (!p_mem->p_img){
		p_mem->p_img = NULL;
		member_image_modified(p_Me, p_mem);
		return;
	}
	p_mem->p_img->p_oper->load_image_2(p_mem->p_img, pi_buf, i_size);

	member_image_modified(p_Me, p_mem);
}
/*** end image_set_buffer ***/

/*** member_free***/
static void member_free(HPOPUPMEMBER *p_Me){
	if (!p_Me){
		return;
	}

	if (p_Me->pc_text){
		vm_free(p_Me->pc_text);
	}
	if (p_Me->p_img){
		himage_delete(p_Me->p_img);
	}
	vm_free(p_Me);
}
/*** end member_free ***/

/*** member_delete ***/
static void member_del(HPopupMenu *p_Me, const unsigned char uc_id){
	HPOPUPMEMBER		*p_del = NULL;

	if (!p_Me){
		return;
	}
	if ((0 > uc_id) && (uc_id > p_Me->uc_mem_num - 1)){
		return;
	}

	p_del = p_Me->member;
	while (p_del){
		if (uc_id == p_del->uc_id){
			if (!p_del->prev && !p_del->next){
				member_free(p_del);
				p_Me->member = p_Me->tail = NULL;
				p_Me->uc_mem_num = 0;
			}else if (!p_del->prev){
				p_Me->member = p_del->next;
				p_del->next->prev = NULL;
				member_free(p_del);
				p_Me->uc_mem_num--;
			}else if (!p_del->next){
				p_Me->tail = p_del->prev;
				p_Me->tail->next = NULL;
				member_free(p_del);
				p_Me->uc_mem_num--;
			}else{
				p_del->prev->next = p_del->next;
				p_del->next->prev = p_del->prev;
				member_free(p_del);
				p_Me->uc_mem_num--;
			}
		}
		p_del = p_del->next;
	}
}
/*** end member_delete ***/


/*** member_clean ***/
static void member_clean(HPopupMenu *p_Me){
	HPOPUPMEMBER		*p_This = p_Me->member, *p_tp = NULL;

	while (p_This){
		p_tp = p_This;
		p_This = p_This->next;
		member_free(p_tp);
	}
	p_Me->member = p_Me->tail = NULL;
}
/*** end member_clean ***/


/*** end add_to_popupmenu ***/

/*** create a popupmenu's member ***/
static short member_add(HPopupMenu *p_Me, const unsigned char uc_id){
	HPOPUPMEMBER	*p_This = NULL;

	if (!p_Me){
		return POINT_EMPTY;
	}

	if (MEM_FULL <= p_Me->uc_mem_num){
		return MEM_FULL;
	}
	if (member_search(p_Me, uc_id)){
		return MEM_EXIST;
	}

	p_This = (HPOPUPMEMBER *)vm_malloc(sizeof(HPOPUPMEMBER));
	if (!p_This){
		return MEM_MALLOC_ERROR;
	}
	member_init(p_This);

	if (p_Me->member){
		p_This->prev = p_Me->tail;
		p_Me->tail->next = p_This;
		p_Me->tail = p_This;
	}else{
		p_Me->member = p_Me->tail = p_This;
	}
	p_This->uc_id = uc_id;
	p_Me->uc_mem_num++;

	((HWidget *)p_Me)->s_height = p_Me->s_mem_height * p_Me->uc_mem_num + POPUP_PADDING_LRTB;
	((HWidget *)p_Me)->s_width = p_Me->s_mem_width + POPUP_PADDING_LRTB;

	return (p_Me->uc_mem_num - 1);
}
/*** end member_new ***/

/*** property_popup ***/
static void property_popup(HPopupMenu *p_Me, HPOPUP_PROPERTY property_Me){
	HWidget			*p_Par = (HWidget *)p_Me;

	if (!p_Me){
		return;
	}

	p_Me->s_mem_height = property_Me.s_mem_height;
	p_Me->s_mem_width = property_Me.s_mem_width;

	if ((unsigned char)0 > property_Me.uc_mem_distance){
		p_Me->uc_mem_distance = 0;
	}else if ((unsigned char)218 < property_Me.uc_mem_distance){
		p_Me->uc_mem_distance = 218;
	}else{
		p_Me->uc_mem_distance = property_Me.uc_mem_distance;
	}

	if (1 == property_Me.uc_style){
		p_Me->uc_style = property_Me.uc_style;
	}else{
		p_Me->uc_style = 0;
	}

	p_Par->i_color = property_Me.i_color;
	p_Par->i_bgcolor = property_Me.i_bg_color;
	p_Par->s_width = property_Me.s_width;
	p_Par->s_height = property_Me.s_height;
	p_Par->s_padding_bottom = property_Me.s_padding_bottom;
	p_Par->s_padding_left = property_Me.s_padding_left;
	p_Par->s_padding_right = property_Me.s_padding_right;
	p_Par->s_padding_top = property_Me.s_padding_top;
}
/*** end property_popup ***/

/*** return preferred label width ***/
static short preferred_width(HWidget *p_Par){
	return (p_Par->s_width);
}
/*** end preferred ***/

/*** return preferred label height ***/
static short preferred_height(HWidget *p_Par){
	return (p_Par->s_height);
}
/*** end preferred***/

/*** draw ***/
static void draw(HWidget *p_widget, int i_layer, short s_x, short s_y){
	HPopupMenu		*p_Me = (HPopupMenu *)p_widget;
	HPOPUPMEMBER	*p_mem = p_Me->member;
	HRect			rect_tp = {0};
	HPoint			point_tp = {0};
	short			s_count = 1;

	rect_tp.s_x = s_x;
	rect_tp.s_y = s_y;
	rect_tp.s_height = p_widget->s_height;
	rect_tp.s_width = p_widget->s_width;

	if (-1 != p_widget->i_color){
		if (-1 != p_widget->i_bgcolor){
			draw_rect_by_hrect(i_layer, rect_tp, p_widget->i_color, p_widget->i_bgcolor);
		}else{
			draw_rect_by_hrect_trans(i_layer, rect_tp, p_widget->i_color);
		}
	}else{
		if (-1 != p_widget->i_bgcolor){
			draw_rect_by_hrect(i_layer, rect_tp, p_widget->i_bgcolor, p_widget->i_bgcolor);
		}
	}

	while (p_mem){

		rect_tp.s_x = p_widget->s_top_x + POPUP_PADDING_LRTB / 2;
		rect_tp.s_y = p_widget->s_top_y + p_Me->s_mem_height * (s_count - 1) + POPUP_PADDING_LRTB / 2;
		rect_tp.s_height = p_Me->s_mem_height;
		rect_tp.s_width = p_Me->s_mem_width;

		if (-1 != p_mem->i_border_color){
			if (-1 != p_mem->i_bg_color){
				draw_rect_by_hrect(i_layer, rect_tp, p_mem->i_border_color, p_mem->i_bg_color);
			}else{
				draw_rect_by_hrect_trans(i_layer, rect_tp, p_mem->i_border_color);
			}
		}else{
			if (-1 != p_mem->i_bg_color){
				draw_rect_by_hrect(i_layer,rect_tp, p_mem->i_bg_color, p_mem->i_bg_color);
			}
		}

		if (p_mem->p_img){
			point_tp.s_x = p_widget->s_top_x + POPUP_PADDING_LRTB / 2 + p_mem->rect_img.s_x;
			point_tp.s_y = p_widget->s_top_y + p_Me->s_mem_height * (s_count - 1) + POPUP_PADDING_LRTB / 2 + p_mem->rect_img.s_y;
			p_mem->p_img->p_oper->paint(p_mem->p_img, i_layer, point_tp.s_x, point_tp.s_y);
		}

		if (p_mem->pc_text){
			point_tp.s_x = p_widget->s_top_x + POPUP_PADDING_LRTB / 2 + p_mem->rect_text.s_x;
			point_tp.s_y = p_widget->s_top_y + p_Me->s_mem_height * (s_count - 1) + POPUP_PADDING_LRTB / 2 + p_mem->rect_text.s_y;
			draw_gb2312_text_by_hpoint(i_layer, p_mem->pc_text, point_tp, p_mem->i_fg_color);
		}

		p_mem = p_mem->next;
		s_count++;
	}

}
/*** end draw ***/


/*** initialize the popupmenu ***/
static void initialize(HPopupMenu *p_Me){
	p_Me->member = p_Me->tail = NULL;
	p_Me->uc_style = 0;
	p_Me->uc_mem_num = 0;
	p_Me->uc_mem_distance = POPUP_DISTANCE;
	p_Me->s_mem_width = POPUP_MEM_WIDTH;
	p_Me->s_mem_height = POPUP_MEM_HEIGHT;
}
/*** end initialize ***/

/*** create a popupmenu ***/
HPopupMenu	*hpopupmenu_new(){
	HPopupMenu		*p_Me = (HPopupMenu *)vm_malloc(sizeof(HPopupMenu));
	HWidget			*p_par = NULL;

	if (NULL == p_Me){
		return NULL;
	}
	memset(p_Me, 0, sizeof(HPopupMenu));
	p_par = (HWidget *)p_Me;
	hwidget_init(p_par);

	if (!g_popup_ops){
		g_popup_ops = (HPopupOps *)vm_malloc(sizeof(HPopupOps));
		if (!g_popup_ops){
			vm_free(p_Me);
			g_popup_ops = NULL;
			return NULL;
		}
		hwidget_operation_init(&g_popup_ops->base_ops);
		g_popup_ops->base_ops.paint = draw;
		g_popup_ops->base_ops.get_prefered_height = preferred_height;
		g_popup_ops->base_ops.get_prefered_width = preferred_width;
		g_popup_ops->base_ops.pen_press = press_pen;
		g_popup_ops->base_ops.pen_release = release_pen;
		g_popup_ops->base_ops.pen_move = move_pen;
		g_popup_ops->base_ops.pen_enter = enter_pen;
		g_popup_ops->base_ops.pen_leave = leave_pen;
		g_popup_ops->base_ops.key_press = press_key;

		g_popup_ops->set_member_image_buffer = image_set_buffer;
		g_popup_ops->set_member_image_resource = image_set_resouce;
		g_popup_ops->set_member_text = text_set;
		g_popup_ops->add_member = member_add;
		g_popup_ops->del_member = member_del;
		g_popup_ops->clean_member = member_clean;
		g_popup_ops->set_submember_point = submember_point;
	/*	g_popup_ops->set_member_rect = member_rect;
		g_popup_ops->set_submember_isborder = member_border;	*/
		g_popup_ops->set_popup_property = property_popup;
		g_popup_ops->set_submember_color = submember_color;

		g_popup_ops->i_num_pop = 0;
		
	}
	
	p_Me->ops = g_popup_ops;
	p_par->p_widget_ops = &g_popup_ops->base_ops;

	p_par->c_font = FONT_SMALL;
	p_par->s_padding_top = 0;
	p_par->s_padding_bottom = 0;
	p_par->s_padding_left = 0;
	p_par->s_padding_right = 0;
	p_par->s_top_x = 0;
	p_par->s_top_y = 0;
	p_par->s_height = POPUP_DEFAULT_HEIGHT/* 0 */;
	p_par->s_width = POPUP_DEFAULT_WIDTH/* 0 */;
	p_par->p_widget_ops->set_enable_bgcolor(p_par, 1);
	p_par->i_bgcolor = -1;
	p_par->i_color = -1;

	initialize(p_Me);

	return p_Me;
}
/*** end hpopupmenu_new ***/

/*** delete a popupmenu ***/
void	hpopupmenu_delete(HPopupMenu *p_Me){
	if (p_Me){
		p_Me->ops->clean_member(p_Me);
		if (0 < g_popup_ops->i_num_pop){
			g_popup_ops->i_num_pop--;
		}else{
			vm_free(g_popup_ops);
			g_popup_ops = NULL;
		}
		vm_free(p_Me);
	}
}
/*** end hpopupmenu_delete ***/

/******************************* EOF ***********************************/