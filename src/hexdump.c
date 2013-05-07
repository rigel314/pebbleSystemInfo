#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"

#include "funcs.h"

#define MY_UUID { 0xE7, 0xBB, 0x71, 0x92, 0xD9, 0x35, 0x45, 0x3A, 0xAE, 0xE2, 0x01, 0xC8, 0x22, 0x8E, 0xCC, 0x38 }
PBL_APP_INFO(MY_UUID,
						 "Sysinfo", "Computing Eureka",
						 1, 0, /* App version */
						 RESOURCE_ID_SYSINFO_MENU_ICON,
						 APP_INFO_STANDARD_APP);

Window mainW;
/*TextLayer mainW_editor;
TextLayer mainW_disasm;
TextLayer mainW_address;
Layer mainW_lines;*/
MenuLayer mainW_menu;

Window editorW;
TextLayer editorW_dump;

Window setAddrW;
TextLayer setAddrW_lbl;
TextLayer setAddrW_word;
TextLayer setAddrW_nibble;

Window disasmW;
TextLayer disasmW_warn;

Window aboutW;
TextLayer abountW_msg;

int32_t address;
int menuSel;
TextLayer* menuItems[3];
const char nib2hex[] = "0123456789ABCDEF";


void handle_init(AppContextRef ctx) {
	(void)ctx;

	menuSel = 0;
	address = 0;
	resource_init_current_app(&RES_FOR_SYSINFO);

	showMenu();
}


void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init
	};
	app_event_loop(params, &handlers);
}
