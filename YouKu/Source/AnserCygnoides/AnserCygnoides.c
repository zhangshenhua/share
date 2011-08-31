#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmlog.h"

#include "hwindow.h"


void vm_main(void) {

	hwindow_init();

	/* register system events handler */
	vm_reg_sysevt_callback(window->sys_event_handle);

	/* register keyboard events handler */
	vm_reg_keyboard_callback(window->key_event_handle);

	/* register pen events handler */
	vm_reg_pen_callback(window->pen_event_handle);
}
