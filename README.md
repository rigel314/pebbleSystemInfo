pebbleSystemInfo
================

Hex Editor and Disassembler for pebble.

When launched, you get a menu.  Up, down, and select work as you would expect.

Choosing hex editor will present a hex dump of RAM.  Up and down will scroll.
Select will start editing the selected word.

In the word editor, up and down change the value of the selected nibble, and
select will advance.  Holding select will set the value.  Clicking back at
this point will not save any changes.

Set Address functions the same as the word editor.

Disassembler will open a dissassembly view of RAM.  Pressing Down will scroll
by the instruction size at the top of the screen.  Pressing Up will always
scroll up by the minimum instruction size(2). Pressing Select will toggle
showing instruction bytes.

About will display informative text.

![Pebble image of hex dump](https://raw.github.com/rigel314/pebbleSystemInfo/db98eba7b59fb6cf8af94aefcc2c9cebe0a5595e/images/hexdump.png)

![Pebble image of disassembly](https://raw.github.com/rigel314/pebbleSystemInfo/27d4f74c459ff7e5831fd0cc5ba285e17779eb98/images/disasm.png)

Please don't blame me if you brick your pebble while playing with this hex editor.  This is a DANGEROUS tool, not a toy.  Be careful.

~~0x2001C08C is where one of my app's global varables is stored on my Pebble.~~
This is very old information.  I think it has changed since I wrote it.

Thanks to [radare](https://github.com/radare) for his help getting the
disassembler code and for making it's output prettier.
