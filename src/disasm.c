#include <pebble.h>

#include "vars.h"
#include "funcs.h"


void showDisasm()
{
	text_layer_set_size(disasmW_warn, GSize(144,168));

	text_layer_set_text(disasmW_warn, "Disassembler not available yet.  The code is available at http://github.com/ rigel314/ pebbleSystemInfo");
	
	window_stack_push(disasmW, true);
}