#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "vars.h"
#include "funcs.h"


void menu_up(ClickRecognizerRef recognizer, Window *window) {
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

	switch(menuSel)
	{
	case 0:
		showHexDump();
		break;
	case 1:
		showDisasm();
		break;
	case 2:
		showSetAddr();
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
}

void menu_text_layer_init(TextLayer* tl, char* txt, GRect frame, int index)
{
	text_layer_init(tl, frame);
	text_layer_set_font(tl, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text(tl, txt);
	text_layer_set_text_alignment(tl, GTextAlignmentCenter);
	layer_add_child(&mainW.layer, &tl->layer);
	menuItems[index] = tl;
}

void menu_draw_lines(Layer *me, GContext* ctx)
{
	graphics_context_set_stroke_color(ctx, GColorBlack);
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	
	graphics_draw_line(ctx, GPoint(0, 47), GPoint(144, 47));
	graphics_draw_line(ctx, GPoint(0, 95), GPoint(144, 95));
	graphics_draw_line(ctx, GPoint(0, 143), GPoint(144, 143));
}

void showMenu()
{
	window_init(&mainW, "System Info");

	menu_text_layer_init(&mainW_editor, "Hex Editor", GRect(0,0,144,48), 0);
	mainW_editor.text_color = GColorWhite;
	mainW_editor.background_color = GColorBlack;

	menu_text_layer_init(&mainW_disasm, "Disassembler", GRect(0,48,144,48), 1);

	menu_text_layer_init(&mainW_address, "Set Address", GRect(0,96,144,48), 2);

	layer_init(&mainW_lines, mainW.layer.frame);
	mainW_lines.update_proc = &menu_draw_lines;
	layer_add_child(&mainW.layer, &mainW_lines);

	
	window_set_click_config_provider(&mainW, (ClickConfigProvider) menu_click_config_provider);

	window_stack_push(&mainW, true);
}