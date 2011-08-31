/*********************************
*file: hwidget.h
*author: yan.gao
*date: 2011.08.19
*description:  base class widget
***********************************/
#ifndef _HWIDGET_H_
#define _HWIDGET_H_

#define H_DEBUG 1

typedef struct _HWidget HWidget;
typedef struct _HWidgetOperation HWidgetOperation;


/*init HWidget*/
void hwidget_init(HWidget *p_widget);

/*init HWidget Operation*/
void hwidget_operation_init(HWidgetOperation *p_ops);


/* Font class*/
typedef enum _HFont{
	FONT_SMALL = 1,
	FONT_MEDIUM = 2,
	FONT_LAGER = 4,
	FONT_BOLD = 8,
	FONT_ITALIC = 16,
	FONT_UNDERLINE = 32
} HFont;

/* Point class*/
typedef struct _HPoint {
	short s_x;
	short s_y;
} HPoint;

typedef struct _HRect {
	short s_x;
	short s_y;
	short s_width;
	short s_height;
} HRect;

typedef enum _HClass {
	CLASS_WIDGET,			/*widget class*/
	CLASS_CONTAINER,		/*container class*/
	CLASS_LABEL,			/*label class*/
	CLASS_BUTTON,			/*button class*/
	CLASS_CHECKBOX,			/*check box class*/
	CLASS_POPUPMENU,		/*pop up menu class*/
	CLASS_PLANE,			/*plane class*/
	CLASS_TEXTINPUT,		/*text input class*/
	CLASS_TEXTAREA,				/*text area class*/
	CLASS_STATUS_BAR,		/*status bar class*/
} HClass;


/* Base class */
struct _HWidget {

	/*********************** private member*********************************/	

	/*a attribute of flags ,visible, enable focus, has focus, enable bgcolor, enable drag and enable */
	short s_attr;

	/*widget font, if c_font is 0, this means the widget's font inherits its parent*/
	char c_font;

	/*foreground color*/
	int i_color;

	/*background color, default the bgcolor is not enable, you should call set_enable_bgcolor to enable it*/
	int i_bgcolor;

	/*padding left*/
	unsigned char uc_padding_left;

	/*padding right*/
	unsigned char uc_padding_right;

	/*padding top*/
	unsigned char uc_padding_top;

	/*padding bottom*/
	unsigned char uc_padding_bottom;

	/* The x coordinates relative to the parent container */
	short s_top_x;

	/* The y coordinates relative to the parent container */
	short s_top_y;

	/*the widget's real width*/
	short s_width;

	/*the widget's real height*/
	short s_height;

	/*the widget's preferred width*/
	short s_prefered_width;

	/*the widget's preferred height*/
	short s_prefered_height;

	/*the widget's max width*/
	short s_max_width;

	/*the widget's max height*/
	short s_max_height;

#ifdef H_DEBUG
	char *p_class_name;
#endif

	/*********************** public member *********************************/

	/*the widget's parent container*/
	struct _HContainer *p_parent;

	/*operation*/
	HWidgetOperation *p_widget_ops;

	/*event action performed callback*/
	void (*action_performed)(HWidget *p_source, void *p_param);
};

/* Base widget operation*/
struct _HWidgetOperation {

	/*whether the widget visible, return 0 or 1. default is 1*/
	int (*is_visible)(HWidget *p_widget);

	/*set the widget visible, its 0 or 1*/
	void (*set_visible)(HWidget *p_widget, int i_visible);

	/*whether the widget enable focus, return 0 or 1. default is 1*/
	int (*is_enable_focus)(HWidget *p_widget);

	/*set the widget enable focus, its 0 or 1*/
	void (*set_enable_focus)(HWidget *p_widget, int i_enable_focus);

	/*whether the widget has focus, return 0 or 1. default is 0*/
	int (*has_focus)(HWidget *p_widget);

	/*set the widget whether has focus, i_focus is 0 or 1*/
	void (*set_focus)(HWidget *p_widget, int i_focus);

	/*whether the widget is enable, return 0 or 1. default is 1*/
	int (*is_enable)(HWidget *p_widget);

	/*set the widget whether  enable, i_enable is 0 or 1*/
	void (*set_enable)(HWidget *p_widget, int i_enable);

	/*whether the widget bgcolor enable, return 0 or 1. default is 0*/
	int (*is_enable_bgcolor)(HWidget *p_widget);

	/*set the widget whether bgcolor enable, i_bgcolor_enable is 0 or 1*/
	void (*set_enable_bgcolor)(HWidget *p_widget, int i_bgcolor_enable);

	/*whether the widget drag enable, return 0 or 1. default is 0*/
	int (*is_enable_drag)(HWidget *p_widget);

	/*set the widget whether drag enable, i_enable_drag is 0 or 1*/
	void (*set_enable_drag)(HWidget *p_widget, int i_enable_drag);

	/*get the widget color*/
	int (*get_color)(HWidget *p_widget);

	/*set the widget color*/
	void (*set_color)(HWidget *p_widget, int i_rgb);
	
	/*get the widget bgcolor*/
	int (*get_bgcolor)(HWidget *p_widget);

	/*set the widget bgcolor*/
	void (*set_bgcolor)(HWidget *p_widget, int i_rgb);

	/*get the widget default bgcolor, inherit from its ancestor*/
	int (*get_default_bgcolor)(HWidget *p_widget);

	/*get the widget font*/
	HFont (*get_font)(HWidget *p_widget);

	/*set the widget font*/
	void (*set_font)(HWidget *p_widget, HFont font);

	/*get the widget width*/
	short (*get_width)(HWidget *p_widget);

	/*set the widget width. if s_width is 0, then will generate a best width for you*/
	void (*set_width)(HWidget *p_widget, short s_width);

	/*get the widget height*/
	short (*get_height)(HWidget *p_widget);

	/*set the widget height. if s_height is 0, then will generate a best height for you*/
	void (*set_height)(HWidget *p_widget, short s_height);

<<<<<<< HEAD
	/*[pure abstract function] get the widget preferred width*/
	short (*get_prefered_width)(HWidget *p_widget);

	/*[pure abstract function]get the widget preferred height*/
=======
	/*[abstract function] get the widget preferred width*/
	short (*get_prefered_width)(HWidget *p_widget);

	/*[abstract function]get the widget preferred height*/
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	short (*get_prefered_height)(HWidget *p_widget);

	/*get the widget max width*/
	short (*get_max_width)(HWidget *p_widget);

	/*set the widget max width. if s_max_width is 0, then will auto generate a best  max width for you*/
	void (*set_max_width)(HWidget *p_widget, short s_max_width);

	/*get the widget max height*/
	short (*get_max_height)(HWidget *p_widget);

	/*set the widget max height. if s_max_height is 0, then will auto generate a best  max height for you*/
	void (*set_max_height)(HWidget *p_widget, short s_max_height);

	/*set the widget padding left*/
	void (*set_padding_left)(HWidget *p_widget, unsigned char s_padding_left);

	/*set the widget padding right*/
	void (*set_padding_right)(HWidget *p_widget, unsigned char s_padding_right);

	/*set the widget padding top*/
	void (*set_padding_top)(HWidget *p_widget, unsigned char s_padding_top);

	/*set the widget padding bottom*/
	void (*set_padding_bottom)(HWidget *p_widget, unsigned char s_padding_bottom);

<<<<<<< HEAD
	/*[pure abstract function] paint the widget*/
=======
	/*[abstract function] paint the widget*/
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	void (*paint)(HWidget *p_widget, int i_handle, short s_screen_x, short s_screen_y);

	/*only repaint the widget itself*/
	void (*repaint)(HWidget *p_widget);
 
	/*validate the widget preferred size, size , and max size*/
	void (*validate)(HWidget *p_widget);

	/*invalidate the widget preferred size, size , and max size*/
	void (*invalidate)(HWidget *p_widget);

	/*translate the widget point to screen point, if the p_widget is not in a HPlane, then return Point{-1,-1}*/
	HPoint (*get_screen_point)(HWidget *p_widget, short s_x, short s_y);

	/*when the widget size(width/height) change call it to notify parent recalculate its size*/
	void (*notify_size_changed)(HWidget *p_widget);

	/*notify the widget parent its visible changed*/
	void (*notify_visible_changed)(HWidget *p_widget);

	/*notify the widget parent its coordinate changed*/
	void (*notify_coordinate_changed)(HWidget *p_widget);

	/*[abstract function] pen press event callback*/
	void (*pen_press)(HWidget *p_widget, short s_x, short s_y);

	/*[abstract function] pen release event callback*/
	void (*pen_release)(HWidget *p_widget, short s_x, short s_y);

	/*[abstract function] pen move event callback*/
	void (*pen_move)(HWidget *p_widget, short s_x, short s_y);

	/*[abstract function] pen move enter callback*/
	void (*pen_enter)(HWidget *p_widget, short s_x, short s_y);

	/*[abstract function] pen move leave callback*/
	void (*pen_leave)(HWidget *p_widget, short s_x, short s_y);

	/*[abstract function] keyboard press event callback*/
	void (*key_press)(HWidget *p_widget, int keycode);

	/*[abstract function]  whether the widget is a container*/
	int (*is_container)(HWidget *p_widget);

	/*[abstract function] whether the widget is a plane*/
	int (*is_plane)(HWidget *p_widget);

	/*[abstract function] get the widget class info*/
	HClass (*get_class)(HWidget *p_widget);

<<<<<<< HEAD
	/*[abstract function] whether the direction key can travel in the widget. if not , focus will goto next widget*/
	int (*can_travel)(HWidget *p_widget, int keycode);

	/*[abstract function] i_gained_focus is 0 means focus lost, 1 means gain focus*/
//	void (*focus_changed)(HWidget *p_widget, int i_gained_focus);

=======
>>>>>>> f80d0d445eaf66846a228a82d7a9c713794052cc
	/*get the widget root widget(its a HPlane), if the widget not in a plane, NULL will be returned*/
	struct _HPlane* (*get_root)(HWidget *p_widget);

	/*get the paint handle. if the p_widget is not in a HPlane, then will return -1*/
	int (*get_paint_handle)(HWidget *p_widget);

	/*clone the operation structure*/
	HWidgetOperation* (*clone)(HWidget *p_widget);

	/*[abstract function] destroy a widget*/
	void (*destroy)(HWidget *p_widget);
};


#endif