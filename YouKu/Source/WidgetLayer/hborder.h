#ifndef _HBORDER_H
#define _HBORDER_H

enum {
	BORDER_TYPE_BASIC,
	BORDER_TYPE_TITLE
};

/* widget border  */
typedef struct _HBorder {
	short s_type;
	short s_default_color;
	short s_hover_color;
	unsigned char uc_left_border;
	unsigned char uc_right_border;
	unsigned char uc_top_border;
	unsigned char uc_bottom_border;
	void (*destroy)(struct _HBorder *p_border);
} HBorder;

typedef struct _HTitleBorder {
	HBorder base;
	char *p_title;
} HTitleBorder;


HBorder * hborder_new();

void hborder_delete(HBorder *p_border);

HTitleBorder * htitleborder_new(const char *p_title);

void htitleborder_delete(HTitleBorder *p_border);

#endif

