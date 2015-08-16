#include <pebble.h>

#include "vars.h"
#include "funcs.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


char* choices[] = {
		"Hex Editor",
		"Set Address",
		"Disassembler",
		"About"
};

void mainMenu_select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
	switch(cell_index->row)
	{
		case 0:
			showHexDump();
			break;
		case 1:
			showSetVal(&address, "Address:", false);
			break;
		case 2:
			showDisasm();
			break;
		case 3:
			showAbout();
			break;
	}
}
void mainMenu_draw_row(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
	GRect frame = layer_get_frame(cell_layer);
	#ifndef PBL_PLATFORM_BASALT
		graphics_context_set_text_color(ctx, GColorBlack); // This is important.
	#endif
	graphics_draw_text(ctx, choices[cell_index->row], fonts_get_system_font(FONT_KEY_GOTHIC_28), GRect(0,5,frame.size.w,frame.size.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
	// Just saying cell_layer->frame for the 4th argument doesn't work.  Probably because the GContext is relative to the cell already, but the cell_layer.frame is relative to the menulayer or the screen or something.
}
void mainMenu_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context)
{
	return;
}
int16_t mainMenu_get_header_height(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
	return 0;
}
int16_t mainMenu_get_cell_height(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
	return 48;
}
uint16_t mainMenu_get_num_rows_in_section(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
	return ARRAY_SIZE(choices);
}
uint16_t mainMenu_get_num_sections(struct MenuLayer *menu_layer, void *callback_context)
{
	return 1;
}

void showMenu()
{
	GRect frame = layer_get_frame(window_get_root_layer(mainW));
	layer_set_frame(menu_layer_get_layer(mainW_menu), GRect(0,0,frame.size.w,frame.size.h));
	scroll_layer_set_frame(menu_layer_get_scroll_layer(mainW_menu), GRect(0,0,frame.size.w,frame.size.h));
	menu_layer_set_click_config_onto_window(mainW_menu, mainW);
	static MenuLayerCallbacks cbacks = 
	{
		.get_num_sections = &mainMenu_get_num_sections,
		.get_num_rows = &mainMenu_get_num_rows_in_section,
		.get_cell_height = &mainMenu_get_cell_height,
		.get_header_height = &mainMenu_get_header_height,
		.select_click = &mainMenu_select_click,
		.draw_row = &mainMenu_draw_row,
		.draw_header = mainMenu_draw_header,
		#ifdef PBL_PLATFORM_BASALT
			.selection_will_change = NULL,
			.draw_background = NULL,
		#endif
		.select_long_click = NULL,
		.selection_changed = NULL
	};
	menu_layer_set_callbacks(mainW_menu, NULL, cbacks);
	// layer_add_child(window_get_root_layer(mainW), menu_layer_get_layer(mainW_menu));

	window_stack_push(mainW, true);
}
