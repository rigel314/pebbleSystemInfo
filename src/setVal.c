#include <pebble.h>

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


void addr_up(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;

	newVal = (newVal | (0x0F << nibNum*4)) & ~((0x0F & ~((newVal>>nibNum*4)+1)) << nibNum*4);
	addFullAddressToStr(word, newVal);
	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << nibNum*4))>>nibNum*4];
	layer_mark_dirty(text_layer_get_layer(setAddrW_nibble));
	layer_mark_dirty(text_layer_get_layer(setAddrW_word));
}

void addr_down(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;

	newVal = (newVal | (0x0F << nibNum*4)) & ~((0x0F & ~((newVal>>nibNum*4)-1)) << nibNum*4);
	addFullAddressToStr(word, newVal);
	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << nibNum*4))>>nibNum*4];
	layer_mark_dirty(text_layer_get_layer(setAddrW_nibble));
	layer_mark_dirty(text_layer_get_layer(setAddrW_word));
}

void addr_select(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;

	GRect frame = layer_get_frame(text_layer_get_layer(setAddrW_nibble));

	if(nibNum > 0)
		nibNum = nibNum-1;
	else
		nibNum = 7;
	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << nibNum*4))>>nibNum*4];

	frame.origin.x = (7-nibNum)*charWidth + startOffset;
	layer_set_frame(text_layer_get_layer(setAddrW_nibble), frame);
	layer_mark_dirty(text_layer_get_layer(setAddrW_nibble));
}

void addr_select_long(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;

	if(!big_end)
		*oldValPtr = newVal;
	else
		*oldValPtr = ((newVal & 0xFF)<<24) | ((newVal & 0xFF00)<<8) | ((newVal & 0xFF0000)>>8) | ((newVal & 0xFF000000)>>24);

	if(big_end)
	{ // If called from the hex editor.  I suppose I should use another variable, but this one is only ever true when called from the hex editor.
		printScreenAtAddress(address);
		layer_mark_dirty(text_layer_get_layer(editorW_dump));
	}

	window_stack_pop(true);
}

void addr_click_config_provider(void* context)
{
	window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, addr_up);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, addr_down);
	window_single_click_subscribe(BUTTON_ID_SELECT, addr_select);
	window_long_click_subscribe(BUTTON_ID_SELECT,0, addr_select_long, NULL);
}

void showSetVal(int32_t* val, char* lbl, bool big_endian)
{
	nibNum = 7;

	oldValPtr = val;
	big_end = big_endian;
	if(!big_end)
		newVal = *val;
	else
		newVal = (((*val) & 0xFF)<<24) | (((*val) & 0xFF00)<<8) | (((*val) & 0xFF0000)>>8) | (((*val) & 0xFF000000)>>24);

	text_layer_set_text(setAddrW_lbl, lbl);
	text_layer_set_font(setAddrW_lbl, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_text_alignment(setAddrW_lbl, GTextAlignmentCenter);
	// layer_add_child(window_get_root_layer(setAddrW), text_layer_get_layer(setAddrW_lbl));

	addFullAddressToStr(word, newVal);
	word[8] = 0;
	text_layer_set_text(setAddrW_word, word);
	text_layer_set_font(setAddrW_word, monaco30);
	// layer_add_child(window_get_root_layer(setAddrW), text_layer_get_layer(setAddrW_word));

	nibble[0] = nib2hex[(unsigned)(newVal & (0x0F << 7*4))>>7*4];
	nibble[1] = 0;
	layer_set_frame(text_layer_get_layer(setAddrW_nibble), GRect(0,50,18,37));
	text_layer_set_text(setAddrW_nibble, nibble);
	text_layer_set_font(setAddrW_nibble, monaco30);
	// setAddrW_nibble.text_color = GColorWhite;
	// setAddrW_nibble.background_color = GColorBlack;
	text_layer_set_text_color(setAddrW_nibble, GColorWhite);
	text_layer_set_background_color(setAddrW_nibble, GColorBlack);
	// layer_add_child(window_get_root_layer(setAddrW), text_layer_get_layer(setAddrW_nibble));
	
	window_set_click_config_provider(setAddrW, (ClickConfigProvider) &addr_click_config_provider);
	window_stack_push(setAddrW, true);
}
