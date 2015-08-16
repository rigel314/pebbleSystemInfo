#include <pebble.h>

#include "vars.h"
#include "funcs.h"
#include "pebble-disthumb/src/thumb.h"

#define LEN 24
#define LINES 16

char strDisasm[LEN*(LINES+1)+1];
int showBytes = 0;

void disasmAtAddress(int32_t addr)
{
	char line[2*LEN+1];
	struct thumb_disasm_t dis;
	int instLen = 0;

	snprintf (strDisasm, LEN+1, "       Page: 0x%03x\n", (unsigned int)(addr >> 24));
	for (int i=1; i<LINES; i++)
	{
		unsigned int laddr = (unsigned int) (addr & 0x0fffff);
		dis.buf = (const uint8_t*)(addr);
		dis.pc = addr;
		instLen = thumb_disasm (&dis);

		APP_LOG(APP_LOG_LEVEL_INFO, "Instruction: %s\n", dis.str);

		if (showBytes)
		{
			if (instLen == 2)
			{
				snprintf(line, sizeof(line), "%05x %02x %02x\n", laddr, dis.buf[0], dis.buf[1]);
			}
			else if (instLen == 4)
			{
				snprintf(line, sizeof(line), "%05x %02x %02x %02x %02x\n", laddr, dis.buf[0], dis.buf[1], dis.buf[2], dis.buf[3]);
			}
		}
		else
		{
			int numChars = snprintf(line, sizeof(line), "%05x %s\n", laddr, dis.str);
			if(numChars > LEN)
				i++;
		}
		strcat (strDisasm, line);

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

void disasm_select(ClickRecognizerRef recognizer, void* context)
{
	showBytes = showBytes? 0: 1;
	disasmAtAddress(address);
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
	window_single_repeating_click_subscribe(BUTTON_ID_UP, 50, disasm_up);
	window_single_repeating_click_subscribe(BUTTON_ID_SELECT, 50, disasm_select);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 50, disasm_down);
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
