#include <pebble.h>

#include "vars.h"
#include "funcs.h"


void showAbout()
{
	text_layer_set_size(abountW_msg, GSize(144,168));

	text_layer_set_text(abountW_msg, "I decided that to be a real computer, the Pebble needed a hex editor.  Have fun, and try not to brick your Pebble.");

	window_stack_push(aboutW, true);
}
