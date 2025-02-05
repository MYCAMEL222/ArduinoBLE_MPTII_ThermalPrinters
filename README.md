# ArduinoBLE_MPTII
A collection of Arduino sketches to use ArduinoBle to control MPTII style printers without a dedicated Thermal Printer library.

This is not an official library for Arduino, but the sketches can be copied and hacked to meet your needs. You can also write your own functions based on ESC/POS commands (documentation is included in this repository).

Testing was done using a GOOJPRT PT-210. It runs the MPTII firmware. 

ArduinoBLE allows multiple methods of scanning to find the proper printer. I will add every method of scanning eventually.

If you need to find printer info like a scannable printer name or MAC address, you can run a test print and that info will be printed out (most printers will do this). 

To run a test print:

1.  Make sure printer power is OFF.
2.  Hold down the form feed button.
3.  Press the power button to ON.
4.  Printer should automatically print a test page.

Make sure printer power is OFF.
Hold down the form feed button.
Press the power button to ON.
Printer should automatically print a test page.

To find printer UUID, you can use a BLE scanner app like…

https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&pcampaignid=web_share

Our ArduinoBLE_ScanForUUID.ino scans for:

BLEService "18f0" & BLECharacteristic "2af1"

You may be able to use these sketches with other Thermal Printers by changing the UUID settings to match your printer. That has not been tested.


