#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "vars.h"
#include "funcs.h"

#define LEN 24
#define DATA_START 7
#define TEXT_START 19
#define LINES 14
#define DATUMS 4


char strHexDump[LEN*LINES+1+15];


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
	strcpy(strHexDump+LEN*LINES, "       Page:   ");
	addByteToStr(strHexDump+LEN*LINES+13, (addr & 0xFF000000) >> 24);
	strHexDump[LEN*LINES+15] = 0;
}

void dump_up(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;
	
	printScreenAtAddress(address-=DATUMS);
	layer_mark_dirty(&editorW_dump.layer);
}

void dump_down(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	printScreenAtAddress(address+=DATUMS);
	layer_mark_dirty(&editorW_dump.layer);
}

void dump_select(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	showSetVal((int32_t*)address, "Address:");
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
	window_init(&editorW, "Hex Editor");

	//for(int x=0; x<LEN*LINES+1; x++)
	//  strHexDump[x] = 'm';

	printScreenAtAddress(address);

	text_layer_init(&editorW_dump, editorW.layer.frame);
	text_layer_set_font(&editorW_dump, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONACO_10)));
	text_layer_set_text(&editorW_dump, strHexDump);

	layer_add_child(&editorW.layer, &editorW_dump.layer);

	window_set_click_config_provider(&editorW, (ClickConfigProvider) dump_click_config_provider);

	window_stack_push(&editorW, true);
}
