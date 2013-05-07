#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "vars.h"
#include "funcs.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


char* choices[] = {
		"Hex Editor",
		"Set Address",
		"Disassembler",
		"About"
};

/*void menu_up(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	menuItems[menuSel]->background_color = GColorWhite;
	menuItems[menuSel]->text_color = GColorBlack;
	layer_mark_dirty(&menuItems[menuSel]->layer);
	
	if(menuSel > 0)
		menuSel --;
	else
		menuSel = 2;

	menuItems[menuSel]->background_color = GColorBlack;
	menuItems[menuSel]->text_color = GColorWhite;
	layer_mark_dirty(&menuItems[menuSel]->layer);
}

void menu_down(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	menuItems[menuSel]->background_color = GColorWhite;
	menuItems[menuSel]->text_color = GColorBlack;
	layer_mark_dirty(&menuItems[menuSel]->layer);

	if(menuSel < 2)
		menuSel++;
	else
		menuSel = 0;

	menuItems[menuSel]->background_color = GColorBlack;
	menuItems[menuSel]->text_color = GColorWhite;
	layer_mark_dirty(&menuItems[menuSel]->layer);
}

void menu_select(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;

	static char addrStr[15];

	switch(menuSel)
	{
	case 0:
		showHexDump();
		break;
	case 1:
		showDisasm();
		break;
	case 2:
		// showSetVal(&address, "Address:");
		addFullAddressToStr(addrStr, (int32_t) &address);
		showSetVal(&address, addrStr);
		break;
	}
}

void menu_click_config_provider(ClickConfig **config, Window *window) {
	(void)window;

	config[BUTTON_ID_UP]->click.handler = (ClickHandler) menu_up;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;
		
	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) menu_down;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;

	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) menu_select;
}*/

void mainMenu_select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
	static char addrStr[15];

	switch(cell_index->row)
	{
		case 0:
			showHexDump();
			break;
		case 1:
			addFullAddressToStr(addrStr, (int32_t) &address);
			showSetVal(&address, addrStr);
			break;
		case 2:
			showDisasm();
			break;
		case 3:
			showAbout();
			break;
	}
}
void mainMenu_draw_row(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{ // Adding the row number as text on the row cell.
	graphics_context_set_text_color(ctx, GColorBlack); // This is important.
	graphics_text_draw(ctx, choices[cell_index->row], fonts_get_system_font(FONT_KEY_GOTHIC_28), GRect(0,5,cell_layer->frame.size.w,cell_layer->frame.size.h), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
	// Just saying cell_layer->frame for the 4th argument doesn't work.  Probably because the GContext is relative to the cell already, but the cell_layer.frame is relative to the menulayer or the screen or something.
}
//void mainMenu_draw_header(GContext *ctx, Layer *cell_layer, uint16_t section_index, void *callback_context)
// Don't need to draw anything on header cells.  Especially since they are 0px tall.
int16_t mainMenu_get_header_height(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{ // Always 30px tall for a header cell.  They can't be selected anyway.
	return 0;
}
int16_t mainMenu_get_cell_height(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{ // Always 20px tall for a normal cell
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

/*void menu_text_layer_init(TextLayer* tl, char* txt, GRect frame, int index)
{
	text_layer_init(tl, frame);
	text_layer_set_font(tl, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text(tl, txt);
	text_layer_set_text_alignment(tl, GTextAlignmentCenter);
	layer_add_child(&mainW.layer, &tl->layer);
	menuItems[index] = tl;
}*/

/*void menu_draw_lines(Layer *me, GContext* ctx)
{
	graphics_context_set_stroke_color(ctx, GColorBlack);
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	
	graphics_draw_line(ctx, GPoint(0, 47), GPoint(144, 47));
	graphics_draw_line(ctx, GPoint(0, 95), GPoint(144, 95));
	graphics_draw_line(ctx, GPoint(0, 143), GPoint(144, 143));
}*/

void showMenu()
{
	window_init(&mainW, "System Info");

	/*menu_text_layer_init(&mainW_editor, "Hex Editor", GRect(0,0,144,48), 0);
	mainW_editor.text_color = GColorWhite;
	mainW_editor.background_color = GColorBlack;

	menu_text_layer_init(&mainW_disasm, "Disassembler", GRect(0,48,144,48), 1);

	menu_text_layer_init(&mainW_address, "Set Address", GRect(0,96,144,48), 2);

	layer_init(&mainW_lines, mainW.layer.frame);
	mainW_lines.update_proc = &menu_draw_lines;
	layer_add_child(&mainW.layer, &mainW_lines);

	
	window_set_click_config_provider(&mainW, (ClickConfigProvider) menu_click_config_provider);*/

	menu_layer_init(&mainW_menu, GRect(0,0,mainW.layer.frame.size.w,mainW.layer.frame.size.h-15)); // -15 because of the title bar.  I could go full screen, but opted not to.
	menu_layer_set_click_config_onto_window(&mainW_menu, &mainW);
	MenuLayerCallbacks cbacks = 
	{
		.get_num_sections = &mainMenu_get_num_sections,
		.get_num_rows = &mainMenu_get_num_rows_in_section,
		.get_cell_height = &mainMenu_get_cell_height,
		.get_header_height = &mainMenu_get_header_height,
		.select_click = &mainMenu_select_click,
		.draw_row = &mainMenu_draw_row
	};
	menu_layer_set_callbacks(&mainW_menu, NULL, cbacks);
	layer_add_child(&mainW.layer, menu_layer_get_layer(&mainW_menu));

	window_stack_push(&mainW, true);
}