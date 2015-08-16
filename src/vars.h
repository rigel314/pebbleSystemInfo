#include "EffectLayer/src/effect_layer.h"

extern Window* mainW;
extern MenuLayer* mainW_menu;

extern Window* editorW;
extern TextLayer* editorW_page;
extern TextLayer* editorW_dump;
extern EffectLayer* editorW_selector;

extern Window* setAddrW;
extern TextLayer* setAddrW_lbl;
extern TextLayer* setAddrW_word;
extern TextLayer* setAddrW_nibble;

extern Window* disasmW;
extern TextLayer* disasmW_warn;

extern Window* aboutW;
extern TextLayer* abountW_msg;

extern int32_t address;
extern int menuSel;
extern TextLayer* menuItems[3];

extern const char nib2hex[];

extern GFont monaco10;
extern GFont monaco30;
