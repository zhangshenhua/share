/**********************************************************************************
*file: hlookandfeel.c
*author: yi.jiang
*date: 2011.08.29
*description: draw specific widget into specify layer by getting widget structure.
***********************************************************************************/
#include "hlookandfeel.h"

/*
* name : get_image_size
* description : get the size of image
*/
int get_image_size(HImage* p_image) 
{
	int					i_cur_num;
	int					i_frame_num;
	int					i_image_size = 0;
	struct frame_prop	*p_Frame = NULL;

	i_frame_num = p_image->p_oper->get_frame_num(p_image);
	for (i_cur_num = 1; i_cur_num <= i_frame_num; i_cur_num++) {
		p_Frame = vm_graphic_get_img_property(p_image->i_canvas, i_cur_num);
		i_image_size += p_Frame->offset;
	}
	return i_image_size;
}

/*
* name : append_property_into_resource
* description : append the property into resource list
*/
void append_property_into_resource(HResource *p_resource, HImage *p_image, char *p_strparth)
{
	int i_image_byte_size = 0;
	HImage *p_max_image = NULL;
	HResourceImageProperty *p_image_property = NULL;

#ifdef H_DEBUG
	vm_log_debug("append_property_into_resource");
#endif

	if ((p_resource == NULL) || (p_image == NULL) ||(p_strparth == NULL)) {
		return;
	}

	p_image_property = (HResourceImageProperty *)vm_malloc(sizeof(HResourceImageProperty));
	if (p_image_property == NULL) {
		return;
	}

	i_image_byte_size = p_resource->get_image_size(p_image);

	p_image_property->p_image_strparth = vm_malloc((strlen(p_strparth) + 1) * sizeof(char) );
	strcpy(p_image_property->p_image_strparth, p_strparth);

	p_image_property->p_image = p_image;

	p_image_property->i_image_byte_size = i_image_byte_size;

	p_image_property->i_reference_count = 1;

	hlist_append(p_resource->p_property_list_head, (void*)p_image_property, NULL);

	p_resource->i_resource_byte_size += i_image_byte_size;

	if ( (p_max_image = p_resource->p_max_image) == NULL || p_resource->get_image_size(p_max_image) < i_image_byte_size) {
		p_resource->p_max_image = p_image;
	}
}

/*
* name : remove_property_from_resource
* description : if the image has no other user, remove the property from resource list 
*/
int remove_property_from_resource(HResource *p_resource, HImage *p_image)
{
	hlist_node_t		*p_node = NULL; 
	HResourceImageProperty *p_image_property = NULL;

#ifdef H_DEBUG
	vm_log_debug("remove_property_from_resource");
#endif

	if ((p_resource == NULL) || (p_image == NULL)) {
		return -1;
	}

	hlist_for_each(p_node, p_resource->p_property_list_head)
	{
		p_image_property = (HResourceImageProperty *)(p_node->pv_data);
		if (p_image_property->p_image == p_image) {
			if (p_image_property->i_reference_count > 1) {
					p_image_property->i_reference_count -= 1;
					return 2;
			}
			else {
				vm_free(p_image_property->p_image_strparth);
				hlist_remove_by_node(p_node);
				p_resource->i_resource_byte_size -= p_resource->get_image_size(p_image);
				return 1;
			}
		}
	}
	return 0;
}

/*
* name : release_maximun_image
* description : it will release the image of the max size
*/
void release_maximun_image(HResource *p_resource)
{
	HImage *p_max_image = NULL;
	hlist_node_t		*p_node = NULL; 
	HResourceImageProperty *p_image_property = NULL;

#ifdef H_DEBUG
	vm_log_debug("release_maximun_image");
#endif

	remove_property_from_resource(p_resource, p_resource->p_max_image);
	p_resource->p_max_image = NULL;

	hlist_for_each(p_node, p_resource->p_property_list_head)
	{
		p_image_property = (HResourceImageProperty *)(p_node->pv_data);
		if ( (p_max_image = p_resource->p_max_image) == NULL || p_resource->get_image_size(p_max_image) < p_resource->get_image_size(p_image_property->p_image) ) {
			p_resource->p_max_image = p_image_property->p_image;
		}
	}
}

/*
* name : load_local_image
* description : load local image, if the image exist, it will return the existing image
* description : if the whole image size is more than a definite value, it will release the image of the max size
*/
HImage* load_local_image(HResource *p_resource, char *p_strparth)
{
	HResourceImageProperty *p_image_property = NULL;
	HImage				*p_image = NULL;
	hlist_node_t		*p_node = NULL; 

#ifdef H_DEBUG
	vm_log_debug("load_local_image");
#endif

	if ((p_resource == NULL) || (p_strparth == NULL)) {
		return NULL;
	}

	while (p_resource->i_resource_byte_size > MAX_RESOURCE_SIZE){
		p_resource->release_maximun_image(p_resource);
	}

	hlist_for_each(p_node, p_resource->p_property_list_head)	
	{
		p_image_property = (HResourceImageProperty *)(p_node->pv_data);
		if (strcmp(p_image_property->p_image_strparth, p_strparth) == 0) {
			p_image_property->i_reference_count += 1;
			return p_image_property->p_image;
		}
	}

	p_image = himage_new();
	p_image->p_oper->load_image(p_image, p_strparth);

	append_property_into_resource(p_resource,p_image, p_strparth);

	return p_image;
}

/*
* name : free_image_by_hresource
* description : if the image has no other, user free the image 
*/
void free_image_by_hresource(HResource *p_resource, HImage* p_image)
{
	int i_result = 0;

#ifdef H_DEBUG
	vm_log_debug("free_image_by_hresource");
#endif

	if ((p_resource == NULL) || (p_image == NULL)) {
		return;
	}

	i_result = remove_property_from_resource(p_resource, p_image);
	if (i_result == -1) {
		return;
	}

	if (i_result == 1) {
		himage_delete(p_image);
	} 
}

/*
* name : hresource_new
* description : init a new hresource structure
*/
HResource *hresource_new()
{
	HResource *p_resource = NULL;

#ifdef H_DEBUG
	vm_log_debug("hresource_new");
#endif

	p_resource = (HResource *)vm_malloc(sizeof(HResource));
	if (p_resource ==NULL) {
		return NULL;
	}

	p_resource->i_resource_byte_size = 0;
	p_resource->p_max_image = NULL;
	p_resource->p_property_list_head = hlist_new();

	p_resource->get_image_size = get_image_size;
	p_resource->append_property_into_resource = append_property_into_resource;
	p_resource->remove_property_from_resource = remove_property_from_resource;
	p_resource->release_maximun_image = release_maximun_image;

	p_resource->load_local_image = load_local_image;
	p_resource->free_image_by_hresource = free_image_by_hresource;

	return p_resource;
}

/*
* name : hresource_delete
* description :delete the hresource structure
* @para(in) p_resource : the resource structure
*/
void hresource_delete(HResource *p_resource)
{
	hlist_node_t		*p_node = NULL; 
	HResourceImageProperty *p_image_property = NULL;

#ifdef H_DEBUG
	vm_log_debug("hresource_delete");
#endif

	hlist_for_each(p_node, p_resource->p_property_list_head)	
	{
		p_image_property = (HResourceImageProperty *)(p_node->pv_data);
		himage_delete(p_image_property->p_image);
		vm_free(p_image_property->p_image_strparth);
//		vm_free(p_image_property);
	}
	hlist_clear(p_resource->p_property_list_head);

	p_resource->append_property_into_resource = NULL;
	p_resource->free_image_by_hresource = NULL;
	p_resource->get_image_size = NULL;
	p_resource->load_local_image = NULL;
	p_resource->release_maximun_image = NULL;
	p_resource->remove_property_from_resource = NULL;

	vm_free(p_resource);
}

/*
* name : look_draw_plane
* description : draw plane
*/
void look_draw_plane(HPlane *p_plane)
{
	VMINT		i_gray_handle;
	VMUINT8		*p_layer_buf;
	VMUINT8		*p_gray_buf;

#ifdef H_DEBUG
	vm_log_debug("look_draw_plane");
#endif

	i_gray_handle = vm_graphic_create_canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	p_layer_buf = vm_graphic_get_layer_buffer(p_plane->i_handle);
	p_gray_buf = vm_graphic_get_canvas_buffer(i_gray_handle);

	vm_graphic_set_clip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	vm_graphic_fill_rect(p_gray_buf, 0, 0, 
		SCREEN_WIDTH, SCREEN_HEIGHT,
		VM_COLOR_BLACK, VM_COLOR_BLACK);

	vm_graphic_blt_ex(p_layer_buf, 0, 0, p_gray_buf, 0, 0, 
		SCREEN_WIDTH, SCREEN_HEIGHT, 1, 150);

	vm_graphic_release_canvas(i_gray_handle);
}

/*
* name : look_draw_gray_by_hrect
* description : draw gray screen into a layer by hrect structure
*/
void look_draw_trans_color(VMINT i_layer_handle, HRect p_rect, VMUINT16 trans_color, VMINT i_transparency)
{
	VMINT		i_gray_handle;
	VMUINT8		*p_layer_buf;
	VMUINT8		*p_trans_buf;

#ifdef H_DEBUG
	vm_log_debug("look_draw_trans_color");
#endif

	i_gray_handle = vm_graphic_create_canvas(p_rect.s_width, p_rect.s_height);
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	p_trans_buf = vm_graphic_get_canvas_buffer(i_gray_handle);

	set_actual_clip(p_rect.s_x, p_rect.s_y, p_rect.s_width, p_rect.s_height);
	vm_graphic_fill_rect(p_trans_buf, p_rect.s_x, p_rect.s_y, p_rect.s_width, p_rect.s_height,
		trans_color, trans_color);

	reset_parent_clip();
	vm_graphic_blt_ex(p_layer_buf, p_rect.s_x, p_rect.s_y, p_trans_buf, 0, 0, 
		p_rect.s_width,  p_rect.s_height, 1, i_transparency);

	vm_graphic_release_canvas(i_gray_handle);
}

/*
* name : look_draw_highlight_in_trans
* description : draw hight light rect in a trans rect 
*/
void look_draw_highlight_in_trans(VMINT i_layer_handle, HRect p_hightlight_rect, HRect p_trans_rect, VMUINT16 trans_color, VMINT i_transparency)
{
	HRect p_tmp_rect;

#ifdef H_DEBUG
	vm_log_debug("look_draw_highlight_in_trans");
#endif

	if (p_hightlight_rect.s_x < p_trans_rect.s_x || 
		p_hightlight_rect.s_y < p_trans_rect.s_y ||
		p_hightlight_rect.s_x + p_hightlight_rect.s_width > p_trans_rect.s_x + p_trans_rect.s_width ||
		p_hightlight_rect.s_y + p_hightlight_rect.s_height > p_trans_rect.s_y + p_trans_rect.s_height) {
			return;
	}
	if (p_hightlight_rect.s_x > p_trans_rect.s_x) {
		p_tmp_rect.s_x = p_trans_rect.s_x;
		p_tmp_rect.s_y = p_trans_rect.s_y;
		p_tmp_rect.s_width = p_hightlight_rect.s_x - p_trans_rect.s_x;
		p_tmp_rect.s_height = p_hightlight_rect.s_y - p_trans_rect.s_y + p_hightlight_rect.s_height;
		look_draw_trans_color(i_layer_handle, p_tmp_rect, trans_color, i_transparency);
	}
	if (p_hightlight_rect.s_y > p_tmp_rect.s_y) {
		p_tmp_rect.s_x = p_hightlight_rect.s_x;
		p_tmp_rect.s_y = p_trans_rect.s_y;
		p_tmp_rect.s_width = p_trans_rect.s_x + p_trans_rect.s_width - p_hightlight_rect.s_x;
		p_tmp_rect.s_height = p_hightlight_rect.s_y - p_tmp_rect.s_y;
		look_draw_trans_color(i_layer_handle, p_tmp_rect, trans_color, i_transparency);
	}
	if ( p_hightlight_rect.s_x + p_hightlight_rect.s_width < p_trans_rect.s_x + p_trans_rect.s_width) {
		p_tmp_rect.s_x = p_hightlight_rect.s_x + p_hightlight_rect.s_width;
		p_tmp_rect.s_y = p_hightlight_rect.s_y;
		p_tmp_rect.s_width = (p_trans_rect.s_x + p_trans_rect.s_width) - (p_hightlight_rect.s_x + p_hightlight_rect.s_width);
		p_tmp_rect.s_height = p_trans_rect.s_y + p_trans_rect.s_height - p_hightlight_rect.s_y;
		look_draw_trans_color(i_layer_handle, p_tmp_rect, trans_color, i_transparency);
	}
	if (p_hightlight_rect.s_y + p_hightlight_rect.s_height < p_trans_rect.s_y + p_trans_rect.s_height) {
		p_tmp_rect.s_x = p_trans_rect.s_x;
		p_tmp_rect.s_y = p_hightlight_rect.s_y + p_hightlight_rect.s_height;
		p_tmp_rect.s_width = p_hightlight_rect.s_x - p_trans_rect.s_x + p_hightlight_rect.s_width;
		p_tmp_rect.s_height = (p_trans_rect.s_y + p_trans_rect.s_height) - (p_hightlight_rect.s_y + p_hightlight_rect.s_height);
		look_draw_trans_color(i_layer_handle, p_tmp_rect, trans_color, i_transparency);
	}
}

void RGB_to_HSL(double var_R, double var_G, double var_B, double *var_H, double *var_S, double *var_L)
{
	double var_max;
	double var_min;
	double del_max;

	double del_R;
	double del_G;
	double del_B;

	if (var_R >= var_G && var_R >= var_B) {
		var_max = var_R;
	}
	else {
		if (var_G >= var_B) {
			var_max = var_G;
		}
		else {
			var_max = var_B;
		}
	}

	if (var_R <= var_G && var_R <= var_B) {
		var_min = var_R;
	}
	else {
		if (var_G <= var_B) {
			var_min = var_G;
		}
		else {
			var_min = var_B;
		}
	}
	del_max = var_max - var_min;

	*var_L = (var_max + var_min) / 2;

	if (del_max == 0) {
		*var_H = 0;
		*var_S = 0;
	}
	else {
		if ( *var_L < 0.5 ) {
			*var_S = del_max / (var_max + var_min);
		}
		else {
			*var_S = del_max / (2.0 - var_max - var_min);
		}
		del_R = ( ( ( var_max - var_R ) / 6.0 ) + ( del_max / 2.0 ) ) / del_max;
		del_G = ( ( ( var_max - var_G ) / 6.0 ) + ( del_max / 2.0 ) ) / del_max;
		del_B = ( ( ( var_max - var_B ) / 6.0 ) + ( del_max / 2.0 ) ) / del_max;

		if ( var_R == var_max ){
			*var_H = del_B - del_G;
		}
		else if ( var_G == var_max ) {
			*var_H = ( 1.0 / 3.0 ) + del_R - del_B;
		}
		else if ( var_B == var_max ) {
			*var_H = ( 2.0 / 3.0 ) + del_G - del_R;
		}

		if ( *var_H < 0 ) {
			*var_H += 1.0;
		}
		if ( *var_H > 1 ) {
			*var_H -= 1.0;
		}
	}
}

double Hue_to_RGB(double v1, double v2, double vH ) {
	if ( vH < 0.0 ) {
		vH += 1.0;
	}
	if ( vH > 1.0 ) {
		vH -= 1.0;
	}

	if ( ( 6 * vH ) < 1 ) {
		return ( v1 + ( v2 - v1 ) * 6 * vH );
	}
	if ( ( 2 * vH ) < 1 ) {
		return ( v2 );
	}
	if ( ( 3 * vH ) < 2 ) {
		return ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - vH ) * 6 );
	}
	return ( v1 );
}

void HSL_to_RGB(double var_H, double var_S, double var_L, double *var_R, double *var_G, double *var_B)
{
	double var_1;
	double var_2;

	if ( var_S == 0 ) {
		*var_R = var_L;
		*var_G = var_L;
		*var_B = var_L;
	}
	else {
		if ( var_L < 0.5 ) {
			var_2 = var_L * ( 1.0 + var_S );
		}
		else {
			var_2 = ( var_L + var_S ) - ( var_S * var_L );
		}
		var_1 = 2.0 * var_L - var_2;

		*var_R = Hue_to_RGB( var_1, var_2, var_H + 1.0/3.0);
		*var_G = Hue_to_RGB( var_1, var_2, var_H);
		*var_B = Hue_to_RGB( var_1, var_2, var_H - 1.0/3.0);
	}
}

/*
* name : look_draw_highlight_by_change_saturation
* description : draw hight light rect by change the saturation of each pixel color
*/
void look_draw_highlight_by_change_saturation(VMINT i_layer_handle, HRect p_rect, int i_saturation_change)
{
	VMUINT8	*p_layer_buf;
	int		current_x;
	int current_y;

	VMUINT16 old_color;
	VMUINT16 new_color;
	VMUINT16 R_color;
	VMUINT16 G_color;
	VMUINT16 B_color;

	double var_R;
	double var_G;
	double var_B;

	double var_H;
	double var_S;
	double var_L;

	double var_S_change;

	double a = 31;
	double b = 63;


#ifdef H_DEBUG
	vm_log_debug("look_draw_highlight_by_change_saturation");
#endif

	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}

	var_S_change = (double)i_saturation_change/255.0;

	for (current_y = p_rect.s_y; current_y < p_rect.s_y + p_rect.s_height; current_y++) {
		for (current_x = p_rect.s_x; current_x < p_rect.s_x + p_rect.s_width;current_x++) {
			old_color = vm_graphic_get_pixel(p_layer_buf, current_x, current_y);

			R_color = (old_color & 0xF800) >> 11;
			G_color = (old_color & 0x07E0) >> 5;
			B_color = old_color & 0x001F;

			var_R = (double)R_color/a;
			var_G = (double)G_color/b;
			var_B = (double)B_color/a;

			RGB_to_HSL(var_R, var_G, var_B, &var_H, &var_S, &var_L);

			var_S += var_S_change;
			if(var_S > 1.0) {
				var_S = 1.0;
			}
			if (var_S < 0.0) {
				var_S = 0.0;
			}

			HSL_to_RGB(var_H, var_S, var_L, &var_R, &var_G, &var_B);

			R_color = (VMUINT16)(var_R * a);
			G_color = (VMUINT16)(var_G * b);
			B_color = (VMUINT16)(var_B * a);

			new_color = (R_color << 11) | (G_color << 5) | (B_color);

			vm_graphic_set_pixel(p_layer_buf, current_x, current_y, new_color); 
		}
	}
}

/*
 * name : look_paint_pushbutton
 * description : draw pushbutton into a layer
 */
void look_paint_pushbutton(HPushButton *p_pushbutton, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	VMUINT16	f_color;
	HWidget		*p_widget;

#ifdef H_DEBUG
	vm_log_debug("look_paint_pushbutton");
#endif

	p_widget = (HWidget *)p_pushbutton;

	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget)) {
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);
	}

	if (p_widget->p_widget_ops->has_focus(p_widget)) {
		f_color = VM_COLOR_BLACK;
	} 
	else {
		f_color = p_widget->i_bgcolor;
	}
	draw_rect_by_widget_trans(i_layer_handle, p_widget, i_screen_x, i_screen_y, f_color);

	if(p_pushbutton->text == NULL) {
		return;
	}

	draw_gb2312_text_by_widget_center(i_layer_handle, p_widget, p_pushbutton->text, i_screen_x, i_screen_y);
}

/*
* name : look_paint_textinput
* description : draw text input into a layer
*/
void look_paint_textinput(HTextInput *p_textinput, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	int						i;
	VMUINT16		f_color;
	VMWCHAR		*p_ucs2_string;
	HWidget			*p_widget = NULL;

#ifdef H_DEBUG
	vm_log_debug("look_paint_textinput");
#endif

	p_widget = (HWidget *)p_textinput;
	
	/* draw rect */
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget)) {
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);
	}
	if (p_widget->p_widget_ops->has_focus(p_widget)) {
		f_color = VM_COLOR_RED;
	}
	else {
		f_color = VM_COLOR_BLACK;
	}
	draw_rect_by_widget_trans(i_layer_handle, p_widget, i_screen_x, i_screen_y, f_color);

	if(p_textinput->pc_text ==NULL) {
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_textinput->pc_text);

	if (p_textinput->c_is_password) {
		for (i = 0; p_ucs2_string[i] != '\0'; i++) {
			p_ucs2_string[i] = '*';
		}
	}

	draw_ucs2_text_by_widget_oneline(i_layer_handle, p_widget, p_ucs2_string, i_screen_x + p_textinput->s_move_offset, i_screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : look_paint_textarea
* description : draw text area into a layer
*/
extern void look_paint_textarea(HTextArea *p_textarea, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	VMUINT16		f_color;
	HWidget			*p_widget = NULL;
//	VMWCHAR		*p_ucs2_string;

#ifdef H_DEBUG
	vm_log_debug("look_paint_textarea");
#endif

	if (i_layer_handle < 0) {
		return;
	}
	p_widget = (HWidget *)p_textarea;
	if (p_widget == NULL) {
		return;
	}

	/* draw rect */
	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget)) {
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);
	}

	if(p_widget->p_widget_ops->has_focus(p_widget)) {
		f_color = VM_COLOR_RED;
	}
	else {
		f_color = VM_COLOR_BLACK;
	}
	draw_rect_by_widget_trans(i_layer_handle, p_widget, i_screen_x, i_screen_y, f_color);


//  	p_ucs2_string = gb2312_to_ucs2(p_textarea->pc_text);
//  
//  	draw_ucs2_text_by_widget_more_line(i_layer_handle,p_widget,p_ucs2_string,i_screen_x,i_screen_y,p_textarea->c_line_space);
//  
//  	vm_free(p_ucs2_string);
}

/*
* name : look_paint_label
* description : draw label into a layer
*/
void look_paint_label(HLabel *p_label, VMINT i_layer_handle, int i_screen_x, int i_screen_y)
{
	VMUINT16		f_color;
	VMWCHAR		*p_ucs2_string;
	HWidget			*p_widget;

#ifdef H_DEBUG
	vm_log_debug("look_paint_label");
#endif

	if (i_layer_handle < 0) {
		return;
	}
	p_widget = (HWidget *)p_label;
	if (p_widget == NULL) {
		return;
	}

	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget)) {
		draw_rect_by_widget(i_layer_handle, p_widget, i_screen_x, i_screen_y);
	}
	if(p_widget->p_widget_ops->has_focus(p_widget)) {
		f_color = VM_COLOR_BLACK;
	}
	else {
		f_color = p_widget->i_bgcolor;
	}
	draw_rect_by_widget_trans(i_layer_handle, p_widget, i_screen_x, i_screen_y, f_color);

	p_ucs2_string = gb2312_to_ucs2(p_label->text);

	draw_ucs2_text_by_widget_more_line(i_layer_handle,p_widget,p_ucs2_string,i_screen_x,i_screen_y,p_label->text_row_spacing);

	vm_free(p_ucs2_string);
}

/*
* name : look_paint_checkbox
* description : draw check box into a layer
*/
 void look_paint_checkbox(HCheckBox* p_checkbox, int i_layer_handle, short s_screen_x, short s_screen_y)
{
	VMWCHAR	*p_ucs2_string;

	short				s_rect_x;
	short				s_rect_y;
	short				s_rect_width;
	short				s_rect_height;
	short				s_text_x;
	short				s_text_y;

	HWidget		*p_widget;
	VMUINT8		*p_layer_buf;

#ifdef H_DEBUG
	vm_log_debug("look_paint_checkbox");
#endif

	if (i_layer_handle < 0) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);

	p_widget = (HWidget *)p_checkbox;
	if (p_widget == NULL) {
		return;
	}

	vm_graphic_set_font( (p_widget->c_font & 7) >> 1);

	if (p_checkbox->pc_label == NULL) {
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_checkbox->pc_label);

	s_rect_height = vm_graphic_get_string_height(p_ucs2_string);
	s_rect_width = s_rect_height;
	s_rect_x = s_screen_x + p_widget->s_padding_left;
	s_rect_y = s_screen_y + (p_widget->s_height - s_rect_height) / 2;
	s_text_x = s_rect_x + s_rect_width + 4;
	s_text_y = s_rect_y;

	printf("s_screen_x: %d, s_screen_y: %d, p_widget->s_width: %d, p_widget->s_height: %d\n",
		s_screen_x, s_screen_y, p_widget->s_width, p_widget->s_height );
	set_actual_clip(s_screen_x, s_screen_y, s_screen_x + p_widget->s_width, s_screen_y + p_widget->s_height);

#ifdef H_DEBUG	
	vm_graphic_rect(p_layer_buf, s_screen_x, s_screen_y, 
		p_widget->s_width, p_widget->s_height, VM_COLOR_BLUE);
#endif

	if (p_widget->p_widget_ops->is_enable_bgcolor(p_widget))
		vm_graphic_fill_roundrect(p_layer_buf, s_screen_x, s_screen_y, 
		p_widget->s_width, p_widget->s_height, 
		p_widget->s_padding_top/2, p_widget->i_bgcolor);

	if (p_widget->p_widget_ops->has_focus(p_widget))
		vm_graphic_rect(p_layer_buf, s_rect_x, s_rect_y, 
		s_rect_width, s_rect_height, VM_COLOR_RED);
	else
		vm_graphic_rect(p_layer_buf, s_rect_x, s_rect_y, 
		s_rect_width, s_rect_height, VM_COLOR_BLACK);

	if (p_checkbox->c_check_state == HCHECKBOX_SELECTED)
		vm_graphic_fill_rect(p_layer_buf, s_rect_x + 2, s_rect_y + 2, 
		s_rect_width - 4, s_rect_height - 4 ,VM_COLOR_BLACK, VM_COLOR_RED);    /* Todo draw check */

	set_actual_clip(s_screen_x + p_widget->s_padding_left, s_screen_y + p_widget->s_padding_top,
		s_screen_x + p_widget->s_width - p_widget->s_padding_right, s_screen_y + p_widget->s_height - p_widget->s_padding_bottom);

	vm_graphic_textout(p_layer_buf, s_text_x, s_text_y, 
		p_ucs2_string, vm_wstrlen(p_ucs2_string), 
		p_widget->p_widget_ops->get_color(p_widget));

	reset_parent_clip();

	vm_free(p_ucs2_string);

}

/*
* name : draw_rect_by_widget
* description : draw rect into a layer by widget structure
* description : the filled color is background color
*/
void draw_rect_by_widget(VMINT i_layer_handle,HWidget* p_widget, int i_screen_x, int i_screen_y)
{
	VMUINT8*	p_layer_buf = NULL;
	if (i_layer_handle < 0){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL){
		return;
	}
	set_actual_clip(i_screen_x, i_screen_y, i_screen_x + p_widget->s_width, i_screen_y + p_widget->s_height);
	vm_graphic_fill_rect(p_layer_buf, i_screen_x, i_screen_y,
		p_widget->s_width, p_widget->s_height, p_widget->i_bgcolor, p_widget->i_bgcolor);
	reset_parent_clip();
}

/*
* name : draw_rect_by_widget_trans
* description : draw rect into a layer by widget structure
* description : the filled color is background color
*/
void draw_rect_by_widget_trans(VMINT i_layer_handle,HWidget* p_widget, int i_screen_x, int i_screen_y, VMUINT16 f_color)
{
	VMUINT8*	p_layer_buf = NULL;
	if (i_layer_handle < 0){
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL){
		return;
	}

	set_actual_clip(i_screen_x, i_screen_y, i_screen_x + p_widget->s_width, i_screen_y + p_widget->s_height);
	vm_graphic_rect(p_layer_buf, i_screen_x, i_screen_y,
		p_widget->s_width, p_widget->s_height, f_color);
	reset_parent_clip();
}

/*
* name : draw_rect_by_hrect
* description : draw rect into a layer
*/
void draw_rect_by_hrect(VMINT i_layer_handle, HRect p_rect, VMUINT16 f_color, VMUINT16 bg_color)
{
	VMUINT8	*p_layer_buf = NULL;
	if (i_layer_handle < 0) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}

	set_actual_clip(p_rect.s_x, p_rect.s_y, p_rect.s_x + p_rect.s_width, p_rect.s_y + p_rect.s_height);
	vm_graphic_fill_rect(p_layer_buf, p_rect.s_x, p_rect.s_y, 
		p_rect.s_width, p_rect.s_height, f_color, bg_color);
	reset_parent_clip();
}

/*
* name : draw_rect_by_hrect_trans
* description : draw the rect frame into a layer
*/
void draw_rect_by_hrect_trans(VMINT i_layer_handle, HRect p_rect, VMUINT16 f_color)
{
	VMUINT8	*p_layer_buf  = NULL;
	if (i_layer_handle < 0) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}

	set_actual_clip(p_rect.s_x, p_rect.s_y, p_rect.s_x + p_rect.s_width, p_rect.s_y + p_rect.s_height);
	vm_graphic_rect(p_layer_buf, p_rect.s_x, p_rect.s_y, p_rect.s_width, p_rect.s_height, f_color);
	reset_parent_clip();
}

VMWCHAR *gb2312_to_ucs2(char *p_char_str) 
{
	int			i_ucs2_max_len;
	int			i_ucs2_cur_len;
	VMWCHAR		*p_ucs2_string;

	if (p_char_str == NULL){
		return NULL;
	}
	i_ucs2_max_len = strlen(p_char_str) + 1;
	if (i_ucs2_max_len < 1) {
		return NULL;
	}
	p_ucs2_string = vm_malloc(i_ucs2_max_len * sizeof(VMWCHAR));
	if (p_ucs2_string == NULL) {
		return NULL;
	}
	//the second para is the size of bytes,include '\0'
	vm_gb2312_to_ucs2(p_ucs2_string, i_ucs2_max_len * 2, p_char_str);

	i_ucs2_cur_len = vm_wstrlen(p_ucs2_string) + 1;
	p_ucs2_string = vm_realloc(p_ucs2_string, i_ucs2_cur_len* sizeof(VMWCHAR));
	
	return p_ucs2_string;
}

/*
* name : draw_gb2312_text_by_hpoint
* description : draw rect into a layer
*/
void draw_gb2312_text_by_hpoint(VMINT i_layer_handle, char *p_char_str, HPoint p_point, VMUINT16 ui_color)
{
	VMWCHAR		*p_ucs2_string;

	if ((i_layer_handle < 0)||(p_char_str == NULL)){
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_char_str);

	draw_ucs2_text_by_hpoint(i_layer_handle, p_ucs2_string, p_point, ui_color);

	vm_free(p_ucs2_string);
}

/*
* name : draw_gb2312_text_by_widget
* description : draw GB2312 string into a layer,
* description : color of the filled is foreground color
 */
void draw_gb2312_text_by_widget(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y)
{
	VMWCHAR		*p_ucs2_string;

	if ((i_layer_handle < 0)||(p_widget == NULL)||(p_char_str == NULL)) {
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_char_str);

	draw_ucs2_text_by_widget(i_layer_handle, p_widget, p_ucs2_string, i_screen_x, i_screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : draw_gb2312_text_by_hpoint_online
* description : draw GB2312 string into a layer
* description : color of the filled is foreground color
* description : the string is one line
*/
void draw_gb2312_text_by_widget_oneline(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y)
{
	VMWCHAR		*p_ucs2_string;

	if ((i_layer_handle < 0)||(p_widget == NULL)||(p_char_str == NULL)) {
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_char_str);

	draw_ucs2_text_by_widget_oneline(i_layer_handle, p_widget, p_ucs2_string, i_screen_x, i_screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : draw_gb2312_text_by_hpoint_center
* description : draw GB2312 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
*/
void draw_gb2312_text_by_widget_center(VMINT i_layer_handle, HWidget* p_widget, char *p_char_str, int i_screen_x, int i_screen_y)
{
	VMWCHAR		*p_ucs2_string;

	if ((i_layer_handle < 0)||(p_widget == NULL)||(p_char_str == NULL)) {
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_char_str);

	draw_ucs2_text_by_widget_center(i_layer_handle, p_widget, p_ucs2_string, i_screen_x, i_screen_y);

	vm_free(p_ucs2_string);
}

/*
* name : draw_gb2312_text_by_hrect_center
* description : draw GB2312 string into a layer
* description : the string is in the center of the rect
*/
void draw_gb2312_text_by_hrect_center(VMINT i_layer_handle, HRect p_rect, char *p_char_str, char c_font, VMUINT16 us_color)
{
	VMWCHAR		*p_ucs2_string;

	int		i_str_x;
	int		i_str_y;
	int		i_padding_x;
	int		i_padding_y;
	int		i_padding_width;
	int		i_padding_height;

	VMUINT8		*p_layer_buf = NULL;

	if ((i_layer_handle < 0) ||(p_char_str == NULL)) {
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_char_str);

	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}

	vm_graphic_set_font((c_font & 7) >> 1);

	i_padding_x = p_rect.s_x;
	i_padding_y = p_rect.s_y;
	i_padding_width = p_rect.s_width;
	i_padding_height = p_rect.s_height;

	i_str_x = i_padding_x + (i_padding_width - vm_graphic_get_string_width(p_ucs2_string))/2;
	i_str_y = i_padding_y + (i_padding_height - vm_graphic_get_string_height(p_ucs2_string))/2;

	set_actual_clip(i_padding_x, i_padding_y, 
		i_padding_x+i_padding_width, i_padding_y + i_padding_height);
	vm_graphic_textout(p_layer_buf, i_str_x, i_str_y,
		p_ucs2_string, vm_wstrlen(p_ucs2_string), us_color);
	reset_parent_clip();

	vm_free(p_ucs2_string);
}


/*
* name : draw_ucs2_text_by_hpoint
* description : draw rect into a layer
*/
void draw_ucs2_text_by_hpoint(VMINT i_layer_handle, VMWCHAR *p_ucs2_string, HPoint p_point, VMUINT16 ui_color)
{
	VMUINT8		*p_layer_buf = NULL;

	if ((i_layer_handle < 0)||(p_ucs2_string == NULL)) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}

	vm_graphic_textout(p_layer_buf, p_point.s_x, p_point.s_y, 
		p_ucs2_string, vm_wstrlen(p_ucs2_string), ui_color);
}

/*
* name : draw_ucs2_text_by_widget
* description : draw UCS2 string into a layer,
* description : color of the filled is foreground color
 */
void draw_ucs2_text_by_widget(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y)
{
	int		i_padding_x;
	int		i_padding_y;
	int		i_padding_width;
	int		i_padding_height;

	VMUINT8		*p_layer_buf = NULL;

	if ((i_layer_handle < 0)||(p_widget == NULL)||(p_ucs2_string == NULL)) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}
	vm_graphic_set_font((p_widget->c_font & 7) >> 1);

	i_padding_x = i_screen_x + p_widget->s_padding_left;
	i_padding_y = i_screen_y + p_widget->s_padding_top;
	i_padding_width = p_widget->s_width - p_widget->s_padding_left - p_widget->s_padding_right;
	i_padding_height = p_widget->s_height - p_widget->s_padding_bottom - p_widget->s_padding_top;

	set_actual_clip(i_padding_x, i_padding_y, 
		i_padding_x + i_padding_width, i_padding_y+i_padding_height);

	vm_graphic_textout(p_layer_buf, i_padding_x, i_padding_y,
		p_ucs2_string, vm_wstrlen(p_ucs2_string), p_widget->i_color);

	reset_parent_clip();
}

/*
* name : draw_ucs2_text_by_widget_oneline
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is one line
*/
void draw_ucs2_text_by_widget_oneline(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y)
{
	int		i_str_x;
	int		i_str_y;
	int		i_padding_x;
	int		i_padding_y;
	int		i_padding_width;
	int		i_padding_height;

	VMUINT8		*p_layer_buf = NULL;

	if ((i_layer_handle < 0)||(p_widget == NULL)||(p_ucs2_string == NULL)) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}

	vm_graphic_set_font((p_widget->c_font & 7) >> 1);

	i_padding_x = i_screen_x + p_widget->s_padding_left;
	i_padding_y = i_screen_y + p_widget->s_padding_top;
	i_padding_width = p_widget->s_width - p_widget->s_padding_left - p_widget->s_padding_right;
	i_padding_height = p_widget->s_height - p_widget->s_padding_bottom - p_widget->s_padding_top;
	i_str_x = i_padding_x;
	i_str_y = i_padding_y + (i_padding_height - vm_graphic_get_string_height(p_ucs2_string))/2;

	set_actual_clip(i_padding_x, i_padding_y, 
		i_padding_x + i_padding_width, i_padding_y + i_padding_height);

	vm_graphic_textout(p_layer_buf, i_str_x, i_str_y,
		p_ucs2_string, vm_wstrlen(p_ucs2_string), p_widget->i_color);

	reset_parent_clip();
}

/*
* name : draw_ucs2_text_by_widget_more_line
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is more than one line
*/
void draw_ucs2_text_by_widget_more_line(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y, int line_space)
{
	int		line_num;
	int		i_padding_x;
	int		i_padding_y;
	int		i_padding_width;
	int		i_padding_height;
	int		i_current_y;

	VMWCHAR		*p_ucs2_line_string;
	VMWCHAR		*p_ucs2_remain_string;

	VMUINT8		*p_layer_buf = NULL;

	if ((i_layer_handle < 0)||(p_widget == NULL)||(p_ucs2_string == NULL)) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}
	vm_graphic_set_font((p_widget->c_font & 7) >> 1);

	i_padding_x = i_screen_x + p_widget->s_padding_left;
	i_padding_y = i_screen_y + p_widget->s_padding_top;
	i_padding_width = p_widget->s_width - p_widget->s_padding_left - p_widget->s_padding_right;
	i_padding_height = p_widget->s_height - p_widget->s_padding_bottom - p_widget->s_padding_top;

	line_num = 0; 
	p_ucs2_line_string = p_ucs2_string;
	p_ucs2_remain_string = p_ucs2_string;	
	i_current_y = i_padding_y;

	if (vm_graphic_get_character_width(p_ucs2_string[0])  > i_padding_width) {
		return;
	}
	set_actual_clip(i_padding_x, i_padding_y, 
		i_padding_x + i_padding_width, i_padding_y + i_padding_height);
	while(1) {
		if(vm_graphic_get_string_width(p_ucs2_remain_string) - vm_graphic_get_string_width(p_ucs2_line_string) > i_padding_width) {
			p_ucs2_line_string--;
			vm_graphic_textout(p_layer_buf, i_padding_x, i_current_y, 
				p_ucs2_remain_string, p_ucs2_line_string - p_ucs2_remain_string, p_widget->i_color);
			p_ucs2_remain_string = p_ucs2_line_string;
			line_num++;
			i_current_y = i_current_y + vm_graphic_get_string_height(p_ucs2_string) + line_space;
		}
		if(*p_ucs2_line_string == '\n') {
			vm_graphic_textout(p_layer_buf, i_padding_x, i_current_y, 
				p_ucs2_remain_string, p_ucs2_line_string-p_ucs2_remain_string, p_widget->i_color);
			p_ucs2_remain_string = p_ucs2_line_string + 1;
			line_num++;
			i_current_y = i_current_y + vm_graphic_get_string_height(p_ucs2_string) + line_space;
		}
		if(*p_ucs2_line_string =='\0') {
			vm_graphic_textout(p_layer_buf, i_padding_x, i_current_y, 
				p_ucs2_remain_string, p_ucs2_line_string-p_ucs2_remain_string, p_widget->i_color);
			break;
		}
		p_ucs2_line_string++;
	}
	reset_parent_clip();
}

/*
* name : draw_ucs2_text_by_widget_center
* description : draw UCS2 string into a layer
* description : color of the filled is foreground color
* description : the string is in the center of the widget
 */
void draw_ucs2_text_by_widget_center(VMINT i_layer_handle, HWidget* p_widget, VMWCHAR* p_ucs2_string, int i_screen_x, int i_screen_y)
{
	int		i_str_x;
	int		i_str_y;
	int		i_padding_x;
	int		i_padding_y;
	int		i_padding_width;
	int		i_padding_height;

	VMUINT8		*p_layer_buf = NULL;

	if ((i_layer_handle < 0)||(p_widget == NULL)||(p_ucs2_string == NULL)) {
		return;
	}
	p_layer_buf = vm_graphic_get_layer_buffer(i_layer_handle);
	if (p_layer_buf == NULL) {
		return;
	}

	vm_graphic_set_font((p_widget->c_font & 7) >> 1);

	i_padding_x = i_screen_x + p_widget->s_padding_left;
	i_padding_y = i_screen_y + p_widget->s_padding_top;
	i_padding_width = p_widget->s_width - p_widget->s_padding_left - p_widget->s_padding_right;
	i_padding_height = p_widget->s_height - p_widget->s_padding_bottom - p_widget->s_padding_top;

	i_str_x = i_padding_x + (i_padding_width - vm_graphic_get_string_width(p_ucs2_string))/2;
	i_str_y = i_padding_y + (i_padding_height - vm_graphic_get_string_height(p_ucs2_string))/2;

	set_actual_clip(i_padding_x, i_padding_y, 
		i_padding_x+i_padding_width, i_padding_y + i_padding_height);
	vm_graphic_textout(p_layer_buf, i_str_x, i_str_y,
		p_ucs2_string, vm_wstrlen(p_ucs2_string), p_widget->i_color);
	reset_parent_clip();
}

/*
* name : set_actual_clip
* description : set the actual clip 
*/
void set_actual_clip(VMINT i_left_x1, VMINT i_top_y1, VMINT i_right_x2, VMINT i_bottom_y2) {
	VMINT i_parent_left_x1 = window->cur_clip_rect.s_x;
	VMINT i_parent_top_y1 = window->cur_clip_rect.s_y;
	VMINT i_parent_right_x2 = window->cur_clip_rect.s_x + window->cur_clip_rect.s_width;
	VMINT i_parent_bottom_y2 = window->cur_clip_rect.s_y + window->cur_clip_rect.s_height;

	if ( (i_left_x1 >= i_parent_right_x2) || (i_right_x2 <= i_parent_left_x1) 
		|| (i_top_y1 >= i_parent_bottom_y2) || (i_bottom_y2 <= i_parent_top_y1)) {
		vm_graphic_set_clip(0, 0, 0, 0);
		return;
	}

	vm_graphic_set_clip( (i_left_x1 >= i_parent_left_x1) ? (i_left_x1) : (i_parent_left_x1),
		( (i_top_y1 >= i_parent_top_y1) ? (i_top_y1) : (i_parent_top_y1) ),
		( (i_right_x2 <= i_parent_right_x2) ? (i_right_x2) : (i_parent_right_x2) ),
		( (i_bottom_y2 <= i_parent_bottom_y2) ? (i_bottom_y2) : (i_parent_bottom_y2) ) );
}

/*
* name : reset_parent_clip
* description : reset to the parent clip
*/
void reset_parent_clip() {
	VMINT i_parent_left_x1 = window->cur_clip_rect.s_x;
	VMINT i_parent_top_y1 = window->cur_clip_rect.s_y;
	VMINT i_parent_right_x2 = window->cur_clip_rect.s_x + window->cur_clip_rect.s_width;
	VMINT i_parent_bottom_y2 = window->cur_clip_rect.s_y + window->cur_clip_rect.s_height;

	vm_graphic_set_clip( i_parent_left_x1, i_parent_top_y1, i_parent_right_x2, i_parent_bottom_y2);
}

/*
* name : get_gb2312_width_and_height
* description : get the longest width and whole height of GB2312 string
* description : if the string has '\n', it will change line 
*/
void get_gb2312_width_and_height(VMINT8* p_gb2312_string, VMINT *i_width, VMINT * i_height)
{
	VMINT i_cur_width = 0;
	VMINT i_cur_height = 0;
	VMWCHAR* p_string_point = NULL;

	VMWCHAR		*p_ucs2_string;

	if (p_gb2312_string == NULL) {
		return;
	}

	p_ucs2_string = gb2312_to_ucs2(p_gb2312_string);

	*i_width = 0;
	*i_height = 0;
	i_cur_height = vm_graphic_get_string_height(p_ucs2_string);

	p_string_point = p_ucs2_string;

	while (1) {
		if (*p_string_point == '\n' ) {
			*p_string_point = '\0';
			i_cur_width = vm_graphic_get_string_width(p_ucs2_string);

			if (*i_width == 0 || i_cur_width > *i_width) {
				*i_width = i_cur_width;
			}
			*i_height += i_cur_height;
			p_ucs2_string = p_string_point + 1;
		}
		else if (*p_string_point == '\0') {
			i_cur_width = vm_graphic_get_string_width(p_ucs2_string);

			if (*i_width == 0 || i_cur_width > *i_width) {
				*i_width = i_cur_width;
			}
			*i_height += i_cur_height;
			return;
		}
		p_string_point++;
	}
	vm_free(p_ucs2_string);

}

void print_string_state(VMWCHAR* p_ucs2_string)
{
	int i;
	printf("bit/pixel:%d\n",vm_graphic_get_bits_per_pixel());
	printf("c_height:%d, c_width:%d\n",
		vm_graphic_get_character_height(), 
		vm_graphic_get_character_width(p_ucs2_string[0]));
	printf(" s_height:%d, s_width:%d, s_basline:%d\n",
		vm_graphic_get_string_height(p_ucs2_string),
		vm_graphic_get_string_width(p_ucs2_string),
		vm_graphic_get_string_baseline(p_ucs2_string));
	for( i = 0;p_ucs2_string[i] != '\0';i++)
		printf("%x, ",p_ucs2_string[i]);
	printf("\n");
}

