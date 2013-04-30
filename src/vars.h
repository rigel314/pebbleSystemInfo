extern Window mainW;
extern TextLayer mainW_editor;
extern TextLayer mainW_disasm;
extern TextLayer mainW_address;
extern Layer mainW_lines;

extern Window editorW;
extern TextLayer editorW_dump;

extern Window disasmW;
extern TextLayer disasmW_warn;

extern Window setAddrW;
extern TextLayer setAddrW_word;
extern TextLayer setAddrW_nibble;

extern int32_t address;
extern int menuSel;
extern TextLayer* menuItems[3];

extern const char nib2hex[];
