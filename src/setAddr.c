#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "vars.h"
#include "funcs.h"


#define charWidth 18
#define startOffset 0

char word[9];
char nibble[2];

// char* newAddrPtr;
int nibNum;
int32_t newAddr;

void addr_up(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	newAddr = (newAddr | (0x0F << nibNum*4)) & ~((0x0F & ~((newAddr>>nibNum*4)+1)) << nibNum*4);
	addFullAddressToStr(word, newAddr);
	nibble[0] = nib2hex[(unsigned)(newAddr & (0x0F << nibNum*4))>>nibNum*4];
	layer_mark_dirty(&setAddrW_nibble.layer);
	layer_mark_dirty(&setAddrW_word.layer);
}

void addr_down(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	newAddr = (newAddr | (0x0F << nibNum*4)) & ~((0x0F & ~((newAddr>>nibNum*4)-1)) << nibNum*4);
	addFullAddressToStr(word, newAddr);
	nibble[0] = nib2hex[(unsigned)(newAddr & (0x0F << nibNum*4))>>nibNum*4];
	layer_mark_dirty(&setAddrW_nibble.layer);
	layer_mark_dirty(&setAddrW_word.layer);
}

void addr_select(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	if(nibNum > 0)
		nibNum = nibNum-1;
	else
		nibNum = 7;
	nibble[0] = nib2hex[(unsigned)(newAddr & (0x0F << nibNum*4))>>nibNum*4];
	setAddrW_nibble.layer.frame.origin.x = (7-nibNum)*charWidth + startOffset;
	layer_mark_dirty(&setAddrW_nibble.layer);
}

void addr_select_long(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	address = newAddr;
	window_stack_pop(true);
}

void addr_click_config_provider(ClickConfig **config, Window *window) {
	(void)window;

	config[BUTTON_ID_UP]->click.handler = (ClickHandler) addr_up;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;
		
	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) addr_down;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;

	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) addr_select;
	config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) addr_select_long;
}

void showSetAddr()
{
	window_init(&setAddrW, "Set Address");

	nibNum = 7;

	newAddr = address;
	// newAddrPtr = &newAddr;

	text_layer_init(&setAddrW_word, GRect(0,50,144,48));
	addFullAddressToStr(word, newAddr);
	word[8] = 0;
	text_layer_set_text(&setAddrW_word, word);
	text_layer_set_font(&setAddrW_word, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_30)));
	layer_add_child(&setAddrW.layer, &setAddrW_word.layer);

	text_layer_init(&setAddrW_nibble, GRect(0,50,18,37));
	nibble[0] = nib2hex[(unsigned)(newAddr & (0x0F << 7*4))>>7*4];
	nibble[1] = 0;
	text_layer_set_text(&setAddrW_nibble, nibble);
	text_layer_set_font(&setAddrW_nibble, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_30)));
	setAddrW_nibble.text_color = GColorWhite;
	setAddrW_nibble.background_color = GColorBlack;
	layer_add_child(&setAddrW.layer, &setAddrW_nibble.layer);
	
	window_set_click_config_provider(&setAddrW, (ClickConfigProvider) addr_click_config_provider);
	window_stack_push(&setAddrW, true);
}
