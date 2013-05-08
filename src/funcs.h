void showMenu();
void showHexDump();
void showSetVal(int32_t* oldVal, char* lbl, bool big_endian);
void showDisasm();
void showAbout();


void addFullAddressToStr(char* str, int32_t val, bool big_endian);
