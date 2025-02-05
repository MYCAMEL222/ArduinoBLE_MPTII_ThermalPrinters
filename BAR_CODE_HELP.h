/* This is from https://download4.epson.biz/sec_pubs/pos/reference_en/escpos/gs_lk.html

[Notes for UPC-A (m = 0, 65) process]
Modular check character (1 character) is processed as follows:
Automatically added when processing data is 11 byte.
The 12th byte data is processed as a modular check character when processing data is 12 byte. In this case, modular check character is not checked.
Left guard bar/center bar/right guard bar are added automatically.

[Notes for UPC-E (m = 1, 66) process]
Some models do not support settings of 6, 7, or 8 bytes for the amounts of data to process.
If the amount of data to process is 6 bytes, the number system character (NSC) 0 is added automatically.
If the amount of data to process is any of (7, 8, 11, 12 bytes), 
the first data (d1) is processed as number system character (NSC) so 0 must be specified.
Modular check character (1 character) is processed as follows:
If the amount of data to process is any of (6, 7, 11 bytes), it is added automatically.
If the amount of data to process is 8 bytes, the 8th byte data is processed as a modular check character. 
However, the modular check character is not checked.
The 12th byte data is processed as a modular check character when processing data is 12 byte. 
In this case, modular check character is not checked.
Modular check characters are data to decide the bar pattern; they are not printing data.
Prints a 6-column short code that except NSC and modular check characters.
If the amount of data to process is 6, 7, or 8 bytes, 
the shortened 6-digit code specified by (d1 ... dk) is printed.
If the amount of data to process is either of (11, 12 bytes), 
a shortened 6-digit code generated from the (d2 ... d11) data in accordance with the table below is printed.
Specify 0 at indicated data by "−" in the table.
When d6 = 1 – 9, be sure to specify (d11 = 5 – 9).
Left guard bar/right guard bar are added automatically.

[Notes for JAN13/EAN13 (m = 2, 67) process]
Modular check character (1 character) is processed as follows:
Automatically added when processing data is 12 byte.
The 13th byte data is processed as a modular check character when processing data is 13 byte. 
In this case, modular check character is not checked.
Left guard bar/center bar/right guard bar are added automatically.

[Notes for JAN8/EAN8 (m = 3, 68) process]
Modular check character (1 character) is processed as follows:
Automatically added when processing data is 7 byte.
The 8th byte data is processed as a modular check character when processing data is 8 byte. In this case, modular check character is not checked.
Left guard bar/center bar/right guard bar are added automatically.

[Notes for CODE39 (m = 4, 69) process]
The printer processes the start character (ASCII = * Hex = 2Ah/Decimal = 42) as follows:
When the first barcode (d1) is *, the printer processes the data as a first character.
If the first barcode (d1) is not *, the printer adds a start character * automatically.
The printer processes the stop character (ASCII = * Hex = 2Ah/Decimal = 42) as follows:
When the last barcode (dk or dn) is *, the printer processes the data as a last character.
If the last barcode (dk or dn) is not *, the printer adds a last character * automatically.
When '*' is processed during barcode data processing, the printer processes * as a stop character. 
The printer prints data preceding * and finishes command processing. 
Therefore, data following * are processed as normal data.
Check digits are not calculated and added.

[Notes for ITF (Interleaved 2 of 5) (m = 5, 70) process]
Start code and stop code are added automatically.
Check digits are not calculated and added.

[Notes for CODABAR (NW-7) (m = 6, 71) process]
Start character and stop character are not added automatically. Transmit data including the codes.
Specify the start character (ASCII = "A"–"D", "a"–"d" / Hex = 41h–44h, 61h–64h / Decimal = 65–68, 97–100) at beginning of the data (d1).
Specify the stop character (ASCII = "A"–"D", "a"–"d" / Hex = 41h–44h, 61h–64h / Decimal = 65–68, 97–100) at end of the data (dk or dn).
Start character or stop character (ASCII = "A"–"D", "a"–"d" / Hex = 41h–44h, 61h–64h / Decimal = 65–68, 97–100) cannot be specified in barcode data.
The character (ASCII = "a"–"d" / Hex = 61h–64h / Decimal = 97–100) is supported by some printer models.
Check digits are not calculated and added.

[Notes for CODE93 (m = 72) process]
Start character and stop character are added automatically.
Check digits (2 character) are calculated and added automatically.
When HRI characters are designated to be added, special character HRI characters are processed as follows.
The printer prints an HRI character "□" as start and stop character.
The printer prints HRI characters "■ + an alphabetic character" as control characters (not printable characters).

[Notes for CODE128 (m = 73) process]
The first two-byte (d1, d2) start character must be code set selection character 
(any of CODE A, CODE B, or CODE C) which selects the first code set.
Check digit (1 character) is calculated and added automatically.
Special character HRI is processed as follows:
The printer does not print HRI characters that correspond to the shift character or 
code set selection character (CODE A, CODE B, or CODE C).
HRI characters of the function characters (FNC1, FNC2, FNC3, or FNC4) and 
control characters (in Hexadecimal: 00h – 1Fh, 7Fh / in Decimal 0 – 31, 127) are printed as spaces.
The range of data (d) that can be processed in each code set (CODE A, CODE B, CODE C) is 
shown in the table below. Data where the character field is blank cannot be used. 
Transmit the 2-byte data shown in the following table ([Hex = 7Bh / Decimal = 123] + character code) from 
the host for the special characters and barcode data "{".

 */