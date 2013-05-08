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

// char* newValPtr;
int nibNum;
int32_t* oldValPtr;
int32_t newVal;
bool big_end;

// int32_t transposeInt(int32_t in)
// {
// 	return in & 0xFF |
// }

void addr_up(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	newVal = (newVal | (0x0F << nibNum*4)) & ~((0x0F & ~((newVal>>nibNum*4)+1)) << nibNum*4);
	addFullAddressToStr(word, newVal, false);
	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << nibNum*4))>>nibNum*4];
	layer_mark_dirty(&setAddrW_nibble.layer);
	layer_mark_dirty(&setAddrW_word.layer);
}

void addr_down(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	newVal = (newVal | (0x0F << nibNum*4)) & ~((0x0F & ~((newVal>>nibNum*4)-1)) << nibNum*4);
	addFullAddressToStr(word, newVal, false);
	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << nibNum*4))>>nibNum*4];
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
	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << nibNum*4))>>nibNum*4];
	setAddrW_nibble.layer.frame.origin.x = (7-nibNum)*charWidth + startOffset;
	layer_mark_dirty(&setAddrW_nibble.layer);
}

void addr_select_long(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	if(!big_end)
		*oldValPtr = newVal;
	else
		*oldValPtr = ((newVal & 0xFF)<<24) | ((newVal & 0xFF00)<<8) | ((newVal & 0xFF0000)>>8) | ((newVal & 0xFF000000)>>24);

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

void showSetVal(int32_t* val, char* lbl, bool big_endian)
{
	window_init(&setAddrW, "Editor");

	nibNum = 7;

	oldValPtr = val;
	big_end = big_endian;
	if(!big_end)
		newVal = *val;
	else
		newVal = (((*val) & 0xFF)<<24) | (((*val) & 0xFF00)<<8) | (((*val) & 0xFF0000)>>8) | (((*val) & 0xFF000000)>>24);

	text_layer_init(&setAddrW_lbl, GRect(0,0,144,20));
	text_layer_set_text(&setAddrW_lbl, lbl);
	text_layer_set_font(&setAddrW_lbl, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(&setAddrW_lbl, GTextAlignmentCenter);
	layer_add_child(&setAddrW.layer, &setAddrW_lbl.layer);

	text_layer_init(&setAddrW_word, GRect(0,50,144,48));
	addFullAddressToStr(word, newVal, false);
	word[8] = 0;
	text_layer_set_text(&setAddrW_word, word);
	text_layer_set_font(&setAddrW_word, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_30)));
	layer_add_child(&setAddrW.layer, &setAddrW_word.layer);

	text_layer_init(&setAddrW_nibble, GRect(0,50,18,37));
	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << 7*4))>>7*4];
	nibble[1] = 0;
	text_layer_set_text(&setAddrW_nibble, nibble);
	text_layer_set_font(&setAddrW_nibble, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_30)));
	setAddrW_nibble.text_color = GColorWhite;
	setAddrW_nibble.background_color = GColorBlack;
	layer_add_child(&setAddrW.layer, &setAddrW_nibble.layer);
	
	window_set_click_config_provider(&setAddrW, (ClickConfigProvider) addr_click_config_provider);
	window_stack_push(&setAddrW, true);
}
