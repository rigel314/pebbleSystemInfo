#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

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
	text_layer_set_text(&editorW_page, pageNum);

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

void dump_up(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;
	
	if(selector_loc == 0)
	{
		printScreenAtAddress(address-=DATUMS);
		layer_mark_dirty(&editorW_dump.layer);
	}
	else
	{
		selector_loc--;
		/*editorW_selector.layer.frame.origin.y = selector_loc*10+1;
		layer_mark_dirty(&editorW_selector.layer);*/
		layer_set_frame(&editorW_selector.layer, GRect(6*7-1,selector_loc*10+1, 6*11+1, 10));
	}
}

void dump_down(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	if(selector_loc == LINES-1)
	{
		printScreenAtAddress(address+=DATUMS);
		layer_mark_dirty(&editorW_dump.layer);
	}
	else
	{
		selector_loc++;
		/*editorW_selector.layer.frame.origin.y = selector_loc*10+1;
		layer_mark_dirty(&editorW_selector.layer);*/
		layer_set_frame(&editorW_selector.layer, GRect(6*7-1,selector_loc*10+1, 6*11+1, 10));
	}
}

void dump_select(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;
	
	static char addrStr[10];
	
	addFullAddressToStr(addrStr, address+4*selector_loc);
	addrStr[8] = ':';
	addrStr[9] = 0;
	
	showSetVal((int32_t*)(address+4*selector_loc), addrStr, true);
}

void dump_click_config_provider(ClickConfig **config, Window *window) {
	(void)window;

	config[BUTTON_ID_UP]->click.handler = (ClickHandler) dump_up;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;
		
	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) dump_down;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;

	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) dump_select;
}

void showHexDump()
{
	selector_loc = 0;

	window_init(&editorW, "Hex Editor");
	window_set_fullscreen(&editorW, true);

	//for(int x=0; x<LEN*LINES+1; x++)
	//  strHexDump[x] = 'm';

	printScreenAtAddress(address);

	text_layer_init(&editorW_page, GRect(0,2,144,12));
	text_layer_set_font(&editorW_page, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_10)));
	text_layer_set_text_alignment(&editorW_page, GTextAlignmentCenter);
	addByteToStr(pageNum+6, (address & 0xFF000000) >> 24);
	text_layer_set_text(&editorW_page, pageNum);
	layer_add_child(&editorW.layer, &editorW_page.layer);

	text_layer_init(&editorW_dump, GRect(0,15,144,168-15));
	text_layer_set_font(&editorW_dump, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_10)));
	text_layer_set_text(&editorW_dump, strHexDump);
	layer_add_child(&editorW.layer, &editorW_dump.layer);

	inverter_layer_init(&editorW_selector, GRect(6*7-1,selector_loc*10+1, 6*11+1, 10));
	layer_add_child(&editorW_dump.layer, &editorW_selector.layer);

	window_set_click_config_provider(&editorW, (ClickConfigProvider) dump_click_config_provider);

	window_stack_push(&editorW, true);
}
