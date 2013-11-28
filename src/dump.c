#include <pebble.h>

#include "vars.h"
#include "funcs.h"

#define LEN 24
#define DATA_START 7
#define TEXT_START 19
#define LINES 15
#define DATUMS 4


char strHexDump[LEN*LINES+1];
char pageNum[] = "Page: XX";
int selector_loc;


void addByteToStr(char* str, int val)
{
	str[0] = nib2hex[(val & 0xF0) >> 4];
	str[1] = nib2hex[val & 0x0F];
}

void addAddressToStr(char* str, int32_t val)
{
	for(int i=0;i<3;i++)
		addByteToStr(str+2*(2-i), (val & (0xFF << 8*i)) >> (8*i));
}
void addFullAddressToStr(char* str, int32_t val)
{
	for(int i=0;i<4;i++)
		addByteToStr(str+2*(3-i), (val & (0xFF << 8*i)) >> (8*i));
}

char npcFilterByte(char val)
{
	if(val<32||val>126)
		return '.';
	return val;
}

void printScreenAtAddress(int32_t addr)
{
	addByteToStr(pageNum+6, (address & 0xFF000000) >> 24);
	text_layer_set_text(editorW_page, pageNum);

	for(int i=0; i<LINES; i++)
	{
		addAddressToStr(strHexDump+i*LEN,i*DATUMS+addr);
		strHexDump[i*LEN+6] = ' ';
		
		for(int j=0; j<DATUMS; j++)
		{
			char byte = *((char* )(i*4+j+addr));
			addByteToStr(strHexDump+i*LEN+DATA_START+3*j, byte);
			strHexDump[i*LEN+DATA_START+3*j+2] = ' ';

			strHexDump[i*LEN+TEXT_START+j] = npcFilterByte(byte);
		}

		strHexDump[(i+1)*LEN-1] = '\n';
	}
	strHexDump[LEN*LINES] = 0;
}

void dump_up(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;
	
	if(selector_loc == 0)
	{
		printScreenAtAddress(address-=DATUMS);
		layer_mark_dirty(text_layer_get_layer(editorW_dump));
	}
	else
	{
		selector_loc--;
		/*editorW_selector.layer.frame.origin.y = selector_loc*10+1;
		layer_mark_dirty(&editorW_selector.layer);*/
		layer_set_frame(inverter_layer_get_layer(editorW_selector), GRect(6*7-1,selector_loc*10+1, 6*11+1, 10));
	}
}

void dump_down(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;

	if(selector_loc == LINES-1)
	{
		printScreenAtAddress(address+=DATUMS);
		layer_mark_dirty(text_layer_get_layer(editorW_dump));
	}
	else
	{
		selector_loc++;
		/*editorW_selector.layer.frame.origin.y = selector_loc*10+1;
		layer_mark_dirty(&editorW_selector.layer);*/
		layer_set_frame(inverter_layer_get_layer(editorW_selector), GRect(6*7-1,selector_loc*10+1, 6*11+1, 10));
	}
}

void dump_select(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;
	
	static char addrStr[10];
	
	addFullAddressToStr(addrStr, address+4*selector_loc);
	addrStr[8] = ':';
	addrStr[9] = 0;
	
	showSetVal((int32_t*)(address+4*selector_loc), addrStr, true);
}

void dump_click_config_provider(void* context)
{
	window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, dump_up);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, dump_down);
	window_single_click_subscribe(BUTTON_ID_SELECT, dump_select);
}

void showHexDump()
{
	selector_loc = 0;

	window_set_fullscreen(editorW, true);

	printScreenAtAddress(address);
	APP_LOG(APP_LOG_LEVEL_INFO, "String used to store the hexdump: %p\n", strHexDump);

	text_layer_set_font(editorW_page, monaco10);
	text_layer_set_text_alignment(editorW_page, GTextAlignmentCenter);
	addByteToStr(pageNum+6, (address & 0xFF000000) >> 24);
	text_layer_set_text(editorW_page, pageNum);
	// layer_add_child(window_get_root_layer(editorW), text_layer_get_layer(editorW_page));

	text_layer_set_font(editorW_dump, monaco10);
	text_layer_set_text(editorW_dump, strHexDump);
	// layer_add_child(window_get_root_layer(editorW), text_layer_get_layer(editorW_dump));

	layer_set_frame(inverter_layer_get_layer(editorW_selector), GRect(6*7-1,selector_loc*10+1, 6*11+1, 10));
	// layer_add_child(text_layer_get_layer(editorW_dump), inverter_layer_get_layer(editorW_selector));

	window_set_click_config_provider(editorW, (ClickConfigProvider) dump_click_config_provider);

	window_stack_push(editorW, true);
}
