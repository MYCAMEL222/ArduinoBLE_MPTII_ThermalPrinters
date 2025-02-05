#include <ArduinoBLE.h>
#include "IMAGE.h"          //// Image data for printImage() function
#include "BAR_CODE_HELP.h"  //// Image data for printImage() function


/* The following arrays define printer mode functions.
  You can build your own for other printer functions if desired.
  Refer to esc/pos documentation below to learn more....
  https://download4.epson.biz/sec_pubs/pos/reference_en/escpos/index.html
*/

const uint8_t TP_RESET[2] = { 0x1B, 0x40 };
const uint8_t NL[1] = { 0x0a };  // Newline
const uint8_t TP_LEFT[3] = { 0x1B, 0x61, 0 };
const uint8_t TP_CENTER[3] = { 0x1B, 0x61, 1 };
const uint8_t TP_RIGHT[3] = { 0x1B, 0x61, 2 };
const uint8_t TP_INVON[3] = { 0x1D, 0x42, 1 };
const uint8_t TP_INVOFF[3] = { 0x1D, 0x42, 0 };
const uint8_t TP_FONT1[3] = { 0x1B, 0x21, 0 };
const uint8_t TP_FONT2[3] = { 0x1B, 0x21, 1 };
const uint8_t TP_SHSW[3] = { 0x1B, 0x21, 0b00000000 };
const uint8_t TP_DHSW[3] = { 0x1B, 0x21, 0b00010000 };
const uint8_t TP_SHDW[3] = { 0x1B, 0x21, 0b00100000 };
const uint8_t TP_DHDW[3] = { 0x1B, 0x21, 0b00110000 };
const uint8_t TP_SHSW_B[3] = { 0x1B, 0x21, 0b00001000 };
const uint8_t TP_DHSW_B[3] = { 0x1B, 0x21, 0b00011000 };
const uint8_t TP_SHDW_B[3] = { 0x1B, 0x21, 0b00101000 };
const uint8_t TP_DHDW_B[3] = { 0x1B, 0x21, 0b00111000 };
const uint8_t TP_SHSW_U[3] = { 0x1B, 0x21, 0b10000000 };
const uint8_t TP_DHSW_U[3] = { 0x1B, 0x21, 0b10010000 };
const uint8_t TP_SHDW_U[3] = { 0x1B, 0x21, 0b10100000 };
const uint8_t TP_DHDW_U[3] = { 0x1B, 0x21, 0b10110000 };
const uint8_t TP_SHSW_BU[3] = { 0x1B, 0x21, 0b10001000 };
const uint8_t TP_DHSW_BU[3] = { 0x1B, 0x21, 0b10011000 };
const uint8_t TP_SHDW_BU[3] = { 0x1B, 0x21, 0b10101000 };
const uint8_t TP_DHDW_BU[3] = { 0x1B, 0x21, 0b10111000 };
const uint8_t TP_BOLD_OFF[3] = { 0x1B, 0x45, 0 };
const uint8_t TP_BOLD_ON[3] = { 0x1B, 0x45, 1 };
const uint8_t TP_NO_UNDERLINE[3] = { 0x1B, 0x2D, 0 };
const uint8_t TP_UNDERLINE[3] = { 0x1B, 0x2D, 1 };
const uint8_t TP_BOLD_UNDERLINE[3] = { 0x1B, 0x2D, 2 };
const uint8_t TP_FONT_A[3] = { 0x1B, 0x4D, 0 };  /// NORMAL SIZE
const uint8_t TP_FONT_B[3] = { 0x1B, 0x4D, 1 };  // 1/2 SIZE

bool validprinter = false;
int myint = 1234;
float myfloat = 1234.56;
char CharArray[100];  // set large enough to handle the largest float, +1 for null character

BLEDevice peripheral;
BLEService TPserv("18f0");
BLECharacteristic TPChar("2af1", BLEWrite, BLEWriteWithoutResponse, "150");

void setup(void) {

  Serial.begin(115200);
  while (!Serial)
    ;

  BLE.begin();
  BLE.setEventHandler(BLEDiscovered, blePeripheralDiscoverHandler);
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  BLEDevice peripheral = BLE.available();
  BLE.scanForUuid("18f0", true);  ///// starts looking for printer Service UUID
}
void loop() {

  // validprinter is determined by blePeripheralConnectHandler....
  if (validprinter) {
    Serial.println("PRINTING..............\n\n");
    delay(3000);  //delay so user can watch serial monitor as each item prints.

    TPChar.writeValue(TP_RESET, 2);               // Initialize Printer to Defaults
    TPChar.writeValue(TP_LEFT, 3);                // Sets Justify Left
    TPChar.writeValue((char *)"DEFAULT TEXT\n");  // Printed info with newline \n
    Serial.println("DEFAULT TEXT");
    delay(3000);  

    TPChar.writeValue(TP_RESET, 2);                  // Reset Printer to Defaults
    TPChar.writeValue(TP_RIGHT, 3);                  // Sets Justify right
    TPChar.writeValue(TP_DHSW_BU, 3);                // Double Height, Single Width, Bold, Underlined
    TPChar.writeValue((char *)"TP_DHSW_BU TEXT\n");  // Printed info with newline \n
    Serial.println("TP_DHSW_BU TEXT");
    delay(3000);  

    TPChar.writeValue(TP_RESET, 2);                 // Reset Printer to Defaults
    TPChar.writeValue(TP_CENTER, 3);                // Sets Justify Center
    TPChar.writeValue(TP_FONT_A, 3);                // Select font a
    TPChar.writeValue((char *)"THIS IS FONT A\n");  // Printed info with newline \n
    Serial.println("THIS IS FONT A");
    delay(3000);  

    TPChar.writeValue(TP_FONT_B, 3);                // Select font b
    TPChar.writeValue((char *)"THIS IS FONT B\n");  // Printed info with newline \n
    Serial.println("THIS IS FONT B");
    delay(3000);  

    TPChar.writeValue(TP_BOLD_ON, 3);                    // Select BOLD MODE
    TPChar.writeValue((char *)"THIS IS FONT B BOLD\n");  // Printed info with newline \n
    Serial.println("THIS IS FONT B BOLD");
    delay(3000);  

    TPChar.writeValue(TP_FONT_A, 3);                     // Select font a
    TPChar.writeValue((char *)"THIS IS FONT A BOLD\n");  // Printed info with newline \n
    Serial.println("THIS IS FONT A BOLD");
    delay(3000);  

    TPChar.writeValue(TP_RESET, 2);                  // Reset Printer to Defaults
    TPChar.writeValue(TP_CENTER, 3);                 // Sets Justify Center
    TPChar.writeValue(TP_DHDW_BU, 3);                // Double Height, Double Width, Bold, Underlined
    TPChar.writeValue((char *)"TP_DHDW_BU TEXT\n");  // Printed info with newline \n
    Serial.println("TP_DHDW_BU TEXT");
    delay(3000);  

    TPChar.writeValue(TP_RESET, 2);   // Reset Printer to Defaults
    TPChar.writeValue(TP_LEFT, 3);    // Sets Justify Left
    TPChar.writeValue(TP_DHSW_B, 3);  // Single Height, Single Width, Bold
    TPChar.writeValue((char *)"myfloat = ");
    dtostrf(myfloat, -10, 2, CharArray);                      // - sign left justifiys, 2 decimal places
    TPChar.writeValue((char *)CharArray, sizeof(CharArray));  // prints float converted by dtostrf
    Serial.println("PRINT A FLOAT");
    delay(3000);  

    TPChar.writeValue(NL, 1);  //same as \n newline. writes array NL[] = { 0x0a };
    TPChar.writeValue((char *)"myint = ");
    sprintf(CharArray, "%d\n", myint);  //// converts myint integer to string
    TPChar.writeValue((char *)CharArray, sizeof(CharArray));
    Serial.println("PRINT AN INTEGER");
    delay(3000);  

    // Leaves main loop to print from a function
    printFromFunction(TPChar);  // must pass (TPChar) to function
    Serial.println("PRINT FROM A FUNCTION");
    delay(3000);  

    TPChar.writeValue(TP_RESET, 2);    // Initialize Printer to Defaults
    TPChar.writeValue(TP_CENTER, 3);   // Sets Justify Center
    TPChar.writeValue(TP_BOLD_ON, 3);  // Select BOLD MODE
    TPChar.writeValue(TP_FONT_A, 3);   // Select font a

    TPChar.writeValue((char *)"UPC_A\n");  // Printed info with newline \n
    printUPC_A(TPChar);                    // Method to send barcode data (must be 12 digits), numbers only
    Serial.println("PRINT UPC_A");
    delay(3000);  

    TPChar.writeValue((char *)"UPC_E\n");  // Printed info with newline \n
    printUPC_E(TPChar);                    // Method to send barcode data (must be 6 digits), numbers onlY
    Serial.println("PRINT UPC_E");
    delay(3000);  

    TPChar.writeValue((char *)"EAN13\n");  // Printed info with newline \n
    printEAN13(TPChar);                    //Print bar code EAN13. 12/13 Character, numbers only 0~9
    Serial.println("PRINT EAN13");
    delay(3000);

    TPChar.writeValue((char *)"EAN8\n");  // Printed info with newline \n
    printEAN8(TPChar);                    //Print bar code EAN8. 8 Character, numbers only 0~9
    Serial.println("PRINT EAN8");
    delay(3000);

    TPChar.writeValue((char *)"CODE39\n");  // Printed info with newline \n
    printCODE39(TPChar);                    //Print bar code CODE39. Unlimited Character, 0~9 A~Z - . SP $ / + % *
    Serial.println("PRINT CODE39");
    delay(3000);

    TPChar.writeValue((char *)"INTERLEAVED25\n");  // Printed info with newline \n
    printINTERLEAVED25(TPChar);                    //Print bar code INTERLEAVED 25. Even number of characters, numbers only 0~9
    Serial.println("PRINT INTERLEAVED25");
    delay(3000);

    TPChar.writeValue((char *)"CODE93\n");  // Printed info with newline \n
    printCODE93(TPChar);
    Serial.println("PRINT CODE93");
    delay(3000);

    TPChar.writeValue((char *)"CODE128\n");  // Printed info with newline \n
    printCODE128(TPChar);
    Serial.println("PRINT CODE128");
    delay(3000);

    TPChar.writeValue((char *)"QRCODE\n");  // Printed info with newline \n
    printQRCODE(TPChar);
    Serial.println("PRINT QRCODE");
    delay(3000);

    printImage(TPChar);  // Prints test image
    Serial.println("PRINT IMAGE");
    delay(3000);  

    // stops printer so it doesn't keep printing
    validprinter = false;
    Serial.println("DONE PRINTING...................\n\n\n\n\n");
  }
  BLE.poll();
  peripheral.poll();
  delay(10);
}
void printFromFunction(BLECharacteristic TPChar) {
  TPChar.writeValue(TP_RESET, 2);                 // Reset Printer to Defaults
  TPChar.writeValue(TP_RIGHT, 3);                 // Justify CENTER
  TPChar.writeValue(TP_SHDW_B, 3);                // Single Height, Double Width, Bold
  TPChar.writeValue((char *)"Print Function\n");  // Printed info
  BLE.poll();
  peripheral.poll();
}
void printUPC_A(BLECharacteristic TPChar) {
  uint8_t Height = 40;  // Set bar code height (1 to 40)
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  uint8_t Width = 3;  // Set bar code width (1,2,3 or 4)
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Select font information for Human Readable Interpretation (HRI) characters
  uint8_t HriLoc = 2;  //HRI characters location 0 = none, 1 = above, 2 = below
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  uint8_t HriFont = 1;  //write font for HRI characters. 0 = font a, 1 = font b
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  uint8_t COMMAND[] = { 0x1D, 0x6B, 0 };  // UPC-A barcode command
  TPChar.writeValue(COMMAND, sizeof(COMMAND));

  // Method to send barcode data (must be 12 digits)
  const char barcodeData[] = "123456789012";  // 12-digit UPC-A code
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
  // peripheral.poll();
}
void printUPC_E(BLECharacteristic TPChar) {
  uint8_t Height = 100;  // Set barcode height (1 to 40)
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  uint8_t Width = 4;  // Set barcode width (1,2,3 or 4)
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Select font information for Human Readable Interpretation (HRI) characters
  uint8_t HriLoc = 2;  //HRI characters location 0 = none, 1 = above, 2 = below
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  uint8_t HriFont = 0;  //write font for HRI characters. 0 = font a, 1 = font b
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  // UPC-E barcode command (corrected)
  uint8_t COMMAND[] = { 0x1d, 0x6b, 1 };  // ESC/POS UPC-E command
  TPChar.writeValue(COMMAND, sizeof(COMMAND));

  // UPC_E is very difficult to understand.
  // For more info go to https://download4.epson.biz/sec_pubs/pos/reference_en/escpos/gs_lk.html

  const char barcodeData[] = { "123456" };  // 6-digit UPC-E code
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
  peripheral.poll();
}
void printEAN13(BLECharacteristic TPChar) {
  //Print bar code EAN8. 12/13 Character, numbers only 0~9

  uint8_t Height = 40;  // Set barcode height (1 to 40)
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  uint8_t Width = 3;  // Set barcode width (1,2,3 or 4)
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Select font information for Human Readable Interpretation (HRI) characters
  uint8_t HriLoc = 2;  //HRI characters location 0 = none, 1 = above, 2 = below
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  uint8_t HriFont = 1;  //write font for HRI characters. 0 = font a, 1 = font b
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  uint8_t COMMAND[] = { 0x1D, 0x6B, 2 };  // EAN13 barcode command
  TPChar.writeValue(COMMAND, sizeof(COMMAND));

  // Method to send barcode data (must be 12/13 digits)
  const char barcodeData[] = "123456789012";  // EAN13 UPC-A code...
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
  // peripheral.poll();
}
void printEAN8(BLECharacteristic TPChar) {
  //Print bar code EAN8. 8 Character, numbers only 0~9

  uint8_t Height = 40;  // Set barcode height (1 to 40)
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  uint8_t Width = 3;  // Set barcode width (1,2,3 or 4)
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Select font information for Human Readable Interpretation (HRI) characters
  uint8_t HriLoc = 2;  //HRI characters location 0 = none, 1 = above, 2 = below
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  uint8_t HriFont = 1;  //write font for HRI characters. 0 = font a, 1 = font b
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  uint8_t COMMAND[] = { 0x1D, 0x6B, 3 };  // EAN8 barcode command
  TPChar.writeValue(COMMAND, sizeof(COMMAND));

  // Method to send barcode data (must be 8 digits)
  const char barcodeData[] = "12345678";  // EAN8 UPC-A code...
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
  // peripheral.poll();
}
void printCODE39(BLECharacteristic TPChar) {
  //Print bar code CODE39. Unlimited Character, 0~9 A~Z - . SP $ / + % *

  uint8_t Height = 40;  // Set bar code height (1 to 40)
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  uint8_t Width = 2;  // Set bar code width (1,2,3 or 4)
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Select font information for Human Readable Interpretation (HRI) characters
  uint8_t HriLoc = 2;  //HRI characters location 0 = none, 1 = above, 2 = below
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  uint8_t HriFont = 1;  //write font for HRI characters. 0 = font a, 1 = font b
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  uint8_t COMMAND[] = { 0x1D, 0x6B, 4 };  // CODE39 barcode command
  TPChar.writeValue(COMMAND, sizeof(COMMAND));

  // //Print bar code CODE39. Unlimited Character, 0~9 A~Z - . SP $ / + % *
  const char barcodeData[] = "-CODE39-";  // CODE39 code...
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
}
void printINTERLEAVED25(BLECharacteristic TPChar) {

  uint8_t Height = 40;  // Set barcode height (1 to 40)
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  uint8_t Width = 2;  // Set barcode width (1,2,3 or 4)
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Select font information for Human Readable Interpretation (HRI) characters
  uint8_t HriLoc = 2;  //HRI characters location 0 = none, 1 = above, 2 = below
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  uint8_t HriFont = 0;  //write font for HRI characters. 0 = font a, 1 = font b
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  uint8_t COMMAND[] = { 0x1D, 0x6B, 5 };  // code INTERLEAVED 25 barcode command
  TPChar.writeValue(COMMAND, sizeof(COMMAND));

  // //Print bar code CODE39. Unlimited Character, 0~9 A~Z - . SP $ / + % *
  const char barcodeData[] = "1234567890";  // code INTERLEAVED 25
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
}
void printCODE93(BLECharacteristic TPChar) {

  uint8_t Height = 40;  // Set barcode height (1 to 40)
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  uint8_t Width = 2;  // Set barcode width (1,2,3 or 4)
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Select font information for Human Readable Interpretation (HRI) characters
  uint8_t HriLoc = 2;  //HRI characters location 0 = none, 1 = above, 2 = below
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  uint8_t HriFont = 0;  //Select font for HRI characters. 0 = font a, 1 = font b
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  // Barcode data
  const char barcodeData[] = "CODE 93 - A%$";  // CODE93 allows 0-9, A-Z, and special characters (- . SP $ / + % *)

  // CODE93 Command: 0x1D 0x6B 0x48 <length> <data> <0x00>
  uint8_t command[] = { 0x1D, 0x6B, 0x48, (uint8_t)strlen(barcodeData) };
  TPChar.writeValue(command, sizeof(command));

  // Send barcode data
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
}
void printCODE128(BLECharacteristic TPChar) {

  // Set barcode height (1 to 40)
  uint8_t Height = 40;
  uint8_t TP_SBCH[] = { 0x1D, 0x68, Height };
  TPChar.writeValue(TP_SBCH, sizeof(TP_SBCH));

  // Set barcode width (1 to 4)
  uint8_t Width = 2;
  uint8_t TP_SBCW[] = { 0x1D, 0x77, Width };
  TPChar.writeValue(TP_SBCW, sizeof(TP_SBCW));

  // Set HRI (Human Readable Interpretation) location (0 = none, 1 = above, 2 = below)
  uint8_t HriLoc = 2;
  uint8_t TP_HRIP[] = { 0x1D, 0x48, HriLoc };
  TPChar.writeValue(TP_HRIP, sizeof(TP_HRIP));

  // Select HRI font (0 = font A, 1 = font B)
  uint8_t HriFont = 0;
  uint8_t TP_HRIF[] = { 0x1D, 0x66, HriFont };
  TPChar.writeValue(TP_HRIF, sizeof(TP_HRIF));

  // Define the barcode data (modifiable inside the function)
  const char barcodeData[] = "CODE 128";  // Example CODE128 barcode

  // CODE128 Command: 0x1D 0x6B 0x49 <length> <data> <0x00>
  uint8_t command[] = { 0x1D, 0x6B, 0x49, (uint8_t)strlen(barcodeData) };
  TPChar.writeValue(command, sizeof(command));

  // Send barcode data
  TPChar.writeValue((uint8_t *)barcodeData, strlen(barcodeData));

  // Send termination byte
  uint8_t terminator = 0x00;
  TPChar.writeValue(&terminator, 1);
}
void printQRCODE(BLECharacteristic TPChar) {
  // User-configurable settings
  uint8_t qrModel = 2;       // QR Code model (1 = Model 1, 2 = Model 2)
  uint8_t qrSize = 8;        // QR Code size (1-16, where 8 means ~32x32 pixels)
  uint8_t qrErrorLevel = 2;  // Error correction (0 = Low, 1 = Medium, 2 = Quartile, 3 = High)

  // QR Code data (modifiable inside the function)
  const char qrData[] = "https://www.youtube.com/";  // Example QR Code content
  uint16_t dataLen = strlen(qrData) + 3;             // Add 3 bytes for (0x31, 0x50, 0x30)

  // Set QR Code model
  uint8_t modelCmd[] = { 0x1D, 0x28, 0x6B, 0x04, 0x00, 0x31, 0x41, qrModel, 0x00 };
  TPChar.writeValue(modelCmd, sizeof(modelCmd));

  // Set QR Code size
  uint8_t sizeCmd[] = { 0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x43, qrSize };
  TPChar.writeValue(sizeCmd, sizeof(sizeCmd));

  // Set QR Code error correction level
  uint8_t errorLevelCmd[] = { 0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x45, qrErrorLevel };
  TPChar.writeValue(errorLevelCmd, sizeof(errorLevelCmd));

  // Store QR Code data
  uint8_t storeDataCmd[] = { 0x1D, 0x28, 0x6B, (uint8_t)(dataLen & 0xFF), (uint8_t)((dataLen >> 8) & 0xFF), 0x31, 0x50, 0x30 };
  TPChar.writeValue(storeDataCmd, sizeof(storeDataCmd));
  TPChar.writeValue((uint8_t *)qrData, strlen(qrData));

  // Print QR Code
  uint8_t printCmd[] = { 0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x51, 0x30 };
  TPChar.writeValue(printCmd, sizeof(printCmd));
}
void printImage(BLECharacteristic TPChar) {

  // change to 1 if you want to invert image colors
  bool invertImage = 0;
  int ImageSizeTest = (TESTIMAGE_WIDTH * TESTIMAGE_HEIGHT) / 8;  // used to make sure array and #defines match.
  if (ImageSizeTest != sizeof(testImage)) {
    Serial.println("ERROR: TESTIMAGE_WIDTH and/or TESTIMAGE_HEIGHT is not consistant with defined array size.");
    return;  // Stop execution
  }
  // Check if image width is valid
  if (TESTIMAGE_WIDTH % 8 != 0) {
    Serial.println("ERROR: Image must be resized. Width must be divisible by 8!");
    return;  // Stop execution
  }
  if (TESTIMAGE_WIDTH > 384) {
    Serial.println("ERROR: Image must be resized. Maximum width is 384 pixels!");
    return;  // Stop execution
  }
  Serial.println("Sending Image via GS v 0");
  uint8_t width_bytes = (TESTIMAGE_WIDTH) / 8;  //Divide image width by 8. Eash array byte is 8 pixels.
  uint8_t xL = width_bytes & 0xFF;              // Lower byte of width
  uint8_t xH = (width_bytes >> 8) & 0xFF;       // Higher byte of width
  uint8_t yL = TESTIMAGE_WIDTH & 0xFF;          // Lower byte of height
  uint8_t yH = (TESTIMAGE_WIDTH >> 8) & 0xFF;   // Higher byte of height

  // GS v 0 command (Raster Bit Image Mode)
  uint8_t cmdDefine[] = {
    0x1D, 0x76, 0x30,  // GS v 0
    0x00,              // Mode: Normal (0 = Normal, 1 = Double Width, etc.)
    xL, xH,            // Image width in bytes
    yL, yH             // Image height in pixels
  };

  TPChar.writeValue(cmdDefine, sizeof(cmdDefine));  // Send command
  delay(50);

  if (invertImage) {
    uint8_t *invertedImage = (uint8_t *)malloc(sizeof(testImage));  // Allocate on heap
    if (!invertedImage) {
      Serial.println("ERROR: Memory allocation failed!");
      return;
    }
    // Invert entire image in memory
    for (size_t i = 0; i < sizeof(testImage); i++) {
      invertedImage[i] = ~testImage[i];
    }
    // Send inverted image in 100-byte BLE packets
    for (size_t i = 0; i < sizeof(testImage); i += 100) {
      TPChar.writeValue(invertedImage + i, min((size_t)100, sizeof(testImage) - i));
    }
    free(invertedImage);  // Free allocated memory
  } else {
    // Send original image in 100-byte BLE packets
    for (size_t i = 0; i < sizeof(testImage); i += 100) {
      TPChar.writeValue(testImage + i, min((size_t)100, sizeof(testImage) - i));
    }
  }
  // Move to the next line after printing
  TPChar.writeValue(NL, 1);  //same as \n newline. writes array NL[] = { 0x0a };
  TPChar.writeValue(NL, 1);
  Serial.println("Image printed!");
}
////  Used to find printer being scanned for
void blePeripheralDiscoverHandler(BLEDevice peripheral) {
  Serial.println("Discovered BLE event");
  BLE.stopScan();
  Serial.println("Discovered ...");
  if (!peripheral.connected()) {
    if (peripheral.connect()) {
      Serial.println("Connected");
    } else {
      Serial.println("Failed to connect!");
      return;
    }
  }
}
//  Connects to printer being scanned for and verifies TPServ and TPChar UUIDs //
void blePeripheralConnectHandler(BLEDevice peripheral) {
  Serial.println("Connection BLE event");
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }
  Serial.println();
  Serial.print("Device name: ");
  Serial.println(peripheral.deviceName());
  Serial.print("Device address: ");
  Serial.println(peripheral.address());
  Serial.println();
  if (peripheral.hasService("18f0")) {
    TPserv = peripheral.service("18f0");
    Serial.println("peripheral.service = 18f0");
    if (TPserv.hasCharacteristic("2af1")) {
      TPChar = TPserv.characteristic("2af1");
      Serial.println("TPserv.characteristic = 2af1");
      // bool flag to confirm a valid printer is found and connected///
      validprinter = true;
    } else {
      validprinter = false;
      peripheral.disconnect();
      BLE.scanForUuid("18f0", true);
      peripheral = BLE.available();
    }
    Serial.print("Valid Printer = ");
    Serial.println(validprinter);
    Serial.println();
  }
}
//  If printer disconnects, automatically starts searching for it again by UUID //
void blePeripheralDisconnectHandler(BLEDevice peripheral) {
  Serial.println("Disconnected BLE event");
  validprinter = false;
  BLE.scanForUuid("18f0", true);
  peripheral = BLE.available();
}
