#include "hcheckbox.h"
#include "hwindow.h"
#include "vmlog.h"

void app_enter()
{
#if(0)
	HCheckBox *p_box = hcheckbox_new("hello world");
	HPlane *p_plane = window->get_default_plane();
	p_plane->p_plane_ops->add_widget(p_plane, p_box);
	window->validate();
#else
	;


#endif
}