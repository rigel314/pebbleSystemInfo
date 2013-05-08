pebbleSystemInfo
================

Hex Editor and ~~Disassembler~~ for pebble.  The dissassembler is not yet available.

When launched, you get a menu.  Up, down, and select work as you would expect.

Choosing hex editor will present a hex dump.  Up and down will scroll.  Select will start editing the selected word.

In the word editor, up and down change the value of the selected nibble, and select will advance.  Holding select will set the value.  Clicking back at this point will not save any changes.

Set Address will present the word editor, but it will change the address.

![Pebble image of hex dump](https://raw.github.com/rigel314/pebbleSystemInfo/master/src/hexdump.png)
