#include <pebble.h>

#include "vars.h"
#include "funcs.h"


void showAbout()
{
	// GRect bounds = layer_get_bounds(window_get_root_layer(aboutW));
	// APP_LOG(201, "X:%d, Y:%d, W:%d, H:%d", bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h);

	// bounds = layer_get_bounds(text_layer_get_layer(abountW_msg));
	// APP_LOG(201, "X:%d, Y:%d, W:%d, H:%d", bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h);

	// layer_set_bounds(text_layer_get_layer(abountW_msg), layer_get_bounds(window_get_root_layer(aboutW)));
	// layer_set_bounds(text_layer_get_layer(abountW_msg), GRect(0,0,144,168));
	text_layer_set_size(abountW_msg, GSize(144,168));
	// bounds = layer_get_bounds(text_layer_get_layer(abountW_msg));
	// APP_LOG(201, "X:%d, Y:%d, W:%d, H:%d", bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h);

	text_layer_set_text(abountW_msg, "I decided that to be a real computer, the Pebble needed a hex editor.  Have fun, and try not to brick your Pebble.");

	layer_mark_dirty(text_layer_get_layer(abountW_msg));

	// layer_add_child(window_get_root_layer(aboutW), text_layer_get_layer(abountW_msg));
	
	window_stack_push(aboutW, true);

	// APP_LOG(201, "X:%d, Y:%d, W:%d, H:%d", bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h);
}
