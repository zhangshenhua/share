#include "../WidgetLayer/hcheckbox.h"
#include "../WindowLayer/hwindow.h"
#include "vmlog.h"
#include "mainpage.h"

void action_performed(HWidget *p_source, void *p_param);

void app_enter()
{
#if 0
	HCheckBox *p_box1 = hcheckbox_new("hello world");
	HCheckBox *p_box2 = hcheckbox_new("good night");
	HCheckBox *p_box3 = hcheckbox_new("that's wright");
	HPlane *p_plane = window->get_default_plane();

	p_box1->base.s_top_x = 10;
	p_box1->base.s_top_y = 20;
	p_plane->p_plane_ops->add_widget(p_plane,(HWidget *) p_box1);

	p_box2->base.s_top_x = 10;
	p_box2->base.s_top_y = 70;
	p_plane->p_plane_ops->add_widget(p_plane,(HWidget *) p_box2);

	p_box3->base.s_top_x = 10;
	p_box3->base.s_top_y = 120;
	p_plane->p_plane_ops->add_widget(p_plane,(HWidget *) p_box3);

	p_box3->base.action_performed = action_performed;

#else
	MainPage* p_mainpage = MainPage_new();
	p_mainpage->show( p_mainpage );

#endif

}

void action_performed(HWidget *p_source, void *p_param)
{
#if 0
	HPlane *p_plane = window->get_popupmenu_plane();
	HCheckBox *p_box1 = hcheckbox_new("I did it!");
	p_box1->base.s_top_x = 100;
	p_box1->base.s_top_y = 100;
	p_plane->p_plane_ops->add_widget(p_plane, (HWidget *)p_box1);
	((HWidget *)p_plane)->p_widget_ops->set_visible((HWidget *)p_plane, 1);
	window->validate();
	window->repaint();
#endif
}