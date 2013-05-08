#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "vars.h"
#include "funcs.h"


void showAbout()
{
	window_init(&aboutW, "About");

	text_layer_init(&abountW_msg, aboutW.layer.frame);
	text_layer_set_text(&abountW_msg, "I decided that to be a real computer, the Pebble needed a hex editor.  Have fun, and try not to brick your Pebble.");

	layer_add_child(&aboutW.layer, &abountW_msg.layer);
	
	window_stack_push(&aboutW, true);
}
