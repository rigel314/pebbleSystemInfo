#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "vars.h"
#include "funcs.h"


void showDisasm()
{
	window_init(&disasmW, "Disassembler");

	text_layer_init(&disasmW_warn, disasmW.layer.frame);
	text_layer_set_text(&disasmW_warn, "Disassembler not available yet.  The code is available at http://github.com/ rigel314/ pebbleHexdump");

	layer_add_child(&disasmW.layer, &disasmW_warn.layer);
	
	window_stack_push(&disasmW, true);
}