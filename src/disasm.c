#include <pebble.h>

#include "vars.h"
#include "funcs.h"


void showDisasm()
{
	layer_set_frame(text_layer_get_layer(disasmW_warn), layer_get_frame(window_get_root_layer(disasmW)));
	// text_layer_set_size(disasmW_warn, layer_get_bounds(window_get_root_layer(disasmW)));
	text_layer_set_text(disasmW_warn, "Disassembler not available yet.  The code is available at http://github.com/ rigel314/ pebbleSystemInfo");

	// layer_add_child(window_get_root_layer(disasmW), text_layer_get_layer(disasmW_warn));
	
	window_stack_push(disasmW, true);
}