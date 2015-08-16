#include <pebble.h>
#include "EffectLayer/src/effect_layer.h"

#include "funcs.h"
#include "vars.h"

enum keys
{
	KEY_ADDRESS,
};

Window* mainW;
MenuLayer* mainW_menu;

Window* editorW;
TextLayer* editorW_page;
TextLayer* editorW_dump;
EffectLayer* editorW_selector;

Window* setAddrW;
TextLayer* setAddrW_lbl;
TextLayer* setAddrW_word;
TextLayer* setAddrW_nibble;

Window* disasmW;
TextLayer* disasmW_warn;

Window* aboutW;
TextLayer* abountW_msg;

int32_t address;
int menuSel;
TextLayer* menuItems[3];
const char nib2hex[] = "0123456789ABCDEF";

GFont monaco10;
GFont monaco30;


void init()
{
	menuSel = 0;
	address = 0x2001ADA0;

	if (persist_exists(KEY_ADDRESS))
		address = persist_read_int(KEY_ADDRESS);

	mainW = window_create();
	mainW_menu = menu_layer_create(GRect(0,0,0,0));
	layer_add_child(window_get_root_layer(mainW), menu_layer_get_layer(mainW_menu));

	editorW = window_create();
	editorW_page = text_layer_create(GRect(0,2,144,12));
	editorW_dump = text_layer_create(GRect(0,15,144,168-15));
	editorW_selector = effect_layer_create(GRect(0,0,0,0));
	effect_layer_add_effect(editorW_selector, effect_invert, NULL);
	layer_add_child(window_get_root_layer(editorW), text_layer_get_layer(editorW_page));
	layer_add_child(window_get_root_layer(editorW), text_layer_get_layer(editorW_dump));
	layer_add_child(text_layer_get_layer(editorW_dump), effect_layer_get_layer(editorW_selector));

	setAddrW = window_create();
	setAddrW_lbl = text_layer_create(GRect(0,0,144,20));
	setAddrW_word = text_layer_create(GRect(0,50,144,48));
	setAddrW_nibble = text_layer_create(GRect(0,0,0,0));
	layer_add_child(window_get_root_layer(setAddrW), text_layer_get_layer(setAddrW_lbl));
	layer_add_child(window_get_root_layer(setAddrW), text_layer_get_layer(setAddrW_word));
	layer_add_child(window_get_root_layer(setAddrW), text_layer_get_layer(setAddrW_nibble));

	disasmW = window_create();
	disasmW_warn = text_layer_create(GRect(0,0,0,0));
	layer_add_child(window_get_root_layer(disasmW), text_layer_get_layer(disasmW_warn));


	aboutW = window_create();
	abountW_msg = text_layer_create(GRect(0,0,0,0));
	layer_add_child(window_get_root_layer(aboutW), text_layer_get_layer(abountW_msg));

	monaco10 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_10));
	monaco30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_30));

	showMenu();
}

void deinit()
{
	if (address == 0x2001ADA0)
		persist_delete(KEY_ADDRESS);
	else
		persist_write_int(KEY_ADDRESS, address);

	window_destroy(mainW);
	menu_layer_destroy(mainW_menu);

	window_destroy(editorW);
	text_layer_destroy(editorW_page);
	text_layer_destroy(editorW_dump);
	effect_layer_destroy(editorW_selector);

	window_destroy(setAddrW);
	text_layer_destroy(setAddrW_lbl);
	text_layer_destroy(setAddrW_word);
	text_layer_destroy(setAddrW_nibble);

	window_destroy(disasmW);
	text_layer_destroy(disasmW_warn);

	window_destroy(aboutW);
	text_layer_destroy(abountW_msg);

	fonts_unload_custom_font(monaco10);	
	fonts_unload_custom_font(monaco30);
}


int main(void)
{
	init();

	app_event_loop();

	deinit();
}
