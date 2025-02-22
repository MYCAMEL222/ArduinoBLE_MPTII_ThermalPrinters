# ArduinoBLE_MPTII_Thermal Printer Sketches
A collection of Arduino sketches to use ArduinoBle to control MPTII style printers without a dedicated Thermal Printer library.

This is not an official library for Arduino, but the sketches can be copied and hacked to meet your needs. You can also write your own functions based on ESC/POS commands (documentation is included in this repository).

Testing was done using a GOOJPRT PT-210. It runs the MPTII firmware.

![Untitled design](https://github.com/user-attachments/assets/2e5e4e18-1b1e-4957-b95b-a8c34ccc7023)
![Untitled design (3)](https://github.com/user-attachments/assets/f4fa5be8-f062-4a49-8bc0-1b9a99a19a31)
![testImage (2)](https://github.com/user-attachments/assets/a875c97d-428e-4afb-90e1-d4b59788035c)


ArduinoBLE allows multiple methods of scanning to find the proper printer (MAC, Name and UUID).

If you need to find printer info like a scannable printer name or MAC address, you can run a test print and that info will be printed out (most printers will do this). 

To run a test print:

1.  Make sure printer power is OFF.
2.  Hold down the form feed button.
3.  Press the power button to ON.
4.  Printer should automatically print a test page.

To find printer UUID, you can use a BLE scanner app like…

https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&pcampaignid=web_share

Our ArduinoBLE_ScanForUUID.ino scans for: BLEService "18f0" & BLECharacteristic "2af1".

You may be able to use these sketches with other Thermal Printers by changing the UUID settings to match your printer. That has not been tested.





