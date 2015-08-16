#include <pebble.h>

#include "vars.h"
#include "funcs.h"
#include "pebble-disthumb/src/thumb.h"

#define LEN 24
// #define LENwN (LEN+1)
#define LINES 16

char strDisasm[LEN*LINES+1];

void disasmAtAddress(int32_t addr)
{
	struct thumb_disasm_t dis;
	int instLen = 0;

	dis.pc = 0x100;

	for(int i=0; i<LINES/2; i++)
	{
		char s1[11] = {0};
		char s2[LEN+1] = {0};

		dis.buf = (const uint8_t*)(addr);
		
		instLen = thumb_disasm (&dis);

		APP_LOG(APP_LOG_LEVEL_INFO, "Instruction: %s\n", dis.str);

		if(instLen)
		{
			strncpy(s1, dis.str, 10);
			if(strlen(dis.str) > 10)
				strncpy(s2, dis.str+10, 23);
			else
				strncpy(s2, "", 0);
		}
		else
		{
			strncpy(s1, "invalid", 8);
			strncpy(s2, "", 0);
		}

		snprintf(strDisasm+i*LEN*2, LEN+1, "%08x(%1d): %10.10s\n", (unsigned int)(addr), instLen, s1);
		snprintf(strDisasm+i*LEN*2+LEN, LEN+1, "%23.23s\n", s2);

		addr += instLen;
	}
	strDisasm[LEN*LINES] = 0;
}

void disasm_up(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;
	
	disasmAtAddress(address -= 2);
	layer_mark_dirty(text_layer_get_layer(disasmW_dis));
}

void disasm_down(ClickRecognizerRef recognizer, void* context)
{
	(void)recognizer;

	struct thumb_disasm_t dis;
	int instLen = 0;

	dis.pc = 0x100;
	dis.buf = (const uint8_t*)(address);
	instLen = thumb_disasm (&dis);

	disasmAtAddress(address += instLen);
	layer_mark_dirty(text_layer_get_layer(disasmW_dis));
}

void disasm_click_config_provider(void* context)
{
	window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, disasm_up);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, disasm_down);
}

void showDisasm()
{
	#ifndef PBL_PLATFORM_BASALT
		window_set_fullscreen(disasmW, true);
	#endif

	disasmAtAddress(address);
	// APP_LOG(APP_LOG_LEVEL_INFO, "String used to store the hexdump: %p\n", strHexDump);

	text_layer_set_font(disasmW_dis, monaco10);
	text_layer_set_text(disasmW_dis, strDisasm);

	window_set_click_config_provider(disasmW, (ClickConfigProvider) disasm_click_config_provider);

	window_stack_push(disasmW, true);
}
