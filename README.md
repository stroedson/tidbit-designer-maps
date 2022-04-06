# tidbit-designer-maps
QMK files for flashing your Nullbits Tidbit macropad with keymap layers for Procreate and Adobe Illustrator.

* This configuration assumes a 19-key Tidbit with one rotary encoder and the OLED-screen installed. I use my Tidbit in horizontal configuration, so the keymap and OLED images are aligned accordingly.
* Switch between Procreate and Illustrator layers by pressing on the encoder. Illustrator has a second layer that is activated by pressing the bottom left key (mod). This repository includes a visual keymap as reference for all the bindings.
* Each layer will show a keymap on the OLED - these are bitmaps #included in the keymap.c file. They were converted to byte arrays using LCDassistant. You can replace them with your own images if you modify the keymap. Link to LCDassistant (windows only): https://en.radzio.dxp.pl/bitmap_converter/
* The underglow will change for each layer. Current setting is a magenta/purple gradient for Procreate, and two shades of orange for Illustrator.

Big thanks to Jay Greco, creator of the Tidbit and many other cool keyboards, as well as all the other clever people I borrowed code from to hack this together. Enjoy!
