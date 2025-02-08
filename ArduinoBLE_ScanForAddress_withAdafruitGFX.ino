/*
Here is a resource to find,view and download TrueType fonts......
   https://fonts.google.com/

Here is a great resource to convert the .ttf files to a usable header file
so you can define your own custom fonts and add them to your sketch.......
  https://rop.nl/truetype2gfx/

Think of the GFXcanvas1 as a blank sheet of paper that you can write to.
The blank image is defined by canvasWidth x canvasHeight (in pixles).
You can write custom text, shapes, lines, etc as found in the Adafruit GFX library.
Once you are finised drawing to the blank image, you run the PrintDefinedCanvas function.
All of your graphics and text will print out as one image. 
We've printed images as large as 360x1000 but your mileage may vary.*/

#include <ArduinoBLE.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include "PermanentMarker_Regular20pt7b.h"

// canvasWidth & canvasHeight can be dynmically changed in code using setCanvasSize()
int canvasWidth = 360;   // Must be divisible by 8
int canvasHeight = 100;  // Adjustable height

//GFXcanvas1 gfx(canvasWidth, canvasHeight);  // Create a 1-bit canvas
GFXcanvas1 *gfx = nullptr;
uint8_t *imageData = nullptr;  // Pointer for dynamically allocated image buffer

/* The following arrays define printer mode functions.
  These command shortcuts do not work with custom test used in GFX commands.
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

BLEDevice peripheral;
BLEService TPserv("18f0");                                                   ///// UUID must match your printers UUID's
BLECharacteristic TPChar("2af1", BLEWrite, BLEWriteWithoutResponse, "150");  ///// UUID must match your printers UUID's

String myPrinterAddress = "86:67:7a:b3:d9:38";  ///// Replace "86:67:7a:b3:d9:38" with your printer MAC address

void setup(void) {

  Serial.begin(115200);
  while (!Serial)
    ;

  BLE.begin();
  BLE.setEventHandler(BLEDiscovered, blePeripheralDiscoverHandler);
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  BLEDevice peripheral = BLE.available();
  BLE.scanForAddress(myPrinterAddress, true);  ///// starts looking for your printer address
}
void loop() {

  if (validprinter) {
    Serial.println("PRINTING..............\n\n");
    TPChar.writeValue(TP_CENTER, 3);  // Sets Justify Center to justify canvas, not canvas contents

    setCanvasSize(360, 600);  // Dynamically set canvas size
    clearCanvas();           // Clear and allocate the buffer

    gfx->fillRect(0, 0, canvasWidth, canvasHeight, 1);
    gfx->fillRoundRect(10, 10, canvasWidth - 20, canvasHeight - 20, 15, 0);  //creates 10pix wide border box

    for (int i = 0; i <= 5; i++) {
      gfx->fillCircle(random(canvasWidth), random(canvasHeight), 5 * i, 1);  // x, y, r, color
    }
    for (int i = 0; i <= 5; i++) {
      gfx->drawCircle(random(canvasWidth), random(canvasHeight), 5 * i, 1);  // x, y, r, color
    }
    for (int i = 0; i <= 5; i++) {
      gfx->fillRect(random(canvasWidth), random(canvasHeight), 50, 50, 1);  // xpos, ypos, x_width, y_height, color
    }
    for (int i = 0; i <= 5; i++) {
      gfx->drawRect(random(canvasWidth), random(canvasHeight), 50, 50, 1);  // xpos, ypos, x_width, y_height, color
    }
    for (int i = 0; i <= 5; i++) {
      gfx->fillRoundRect(random(canvasWidth), random(canvasHeight), 50, 100, 20, 1);  // xpos, ypos, x_width, y_height, radius, color
    }
    for (int i = 0; i <= 5; i++) {
      gfx->drawRoundRect(random(canvasWidth), random(canvasHeight), 50, 100, 20, 1);  // xpos, ypos, x_width, y_height, radius, color
    }
    for (int i = 0; i <= 5; i++) {
      int x0 = random(canvasWidth);
      int y0 = random(canvasHeight);
      gfx->drawTriangle(x0, y0, x0 + 25, y0 - 50, x0 + 50, y0, 1);  // x0, y0, x1, y1, x2, y2, color
    }
    for (int i = 0; i <= 5; i++) {
      int x0 = random(canvasWidth);
      int y0 = random(canvasHeight);
      gfx->fillTriangle(x0, y0, x0 + 25, y0 - 50, x0 + 50, y0, 1);  // x0, y0, x1, y1, x2, y2, color
    }
    for (int i = 0; i <= 50; i++) {
      gfx->drawLine(random(canvasWidth), random(canvasHeight), random(canvasWidth), random(canvasHeight), 1);
    }
    gfx->setFont(&PermanentMarker_Regular20pt7b);
    gfx->setTextSize(1);
    gfx->setTextWrap(0);   // 1 for wrapping, 0 for clipping
    gfx->setTextColor(1);  // text, bg....1=black, 0=white
    gfx->setCursor(40, canvasHeight * .25);
    gfx->fillRoundRect(20, (canvasHeight * .25) - 40, canvasWidth - 40, 60, 20, 0);  // xpos, ypos, x_width, y_height, radius,  color
    gfx->print("Hello World!!!");
    gfx->setTextColor(0);  // text, bg....1=black, 0=white
    gfx->setCursor(40, canvasHeight * .5);
    gfx->fillRoundRect(20, (canvasHeight * .5) - 40, canvasWidth - 40, 60, 20, 1);  // xpos, ypos, x_width, y_height, radius,  color
    gfx->print("Hello World!!!");
    gfx->setTextColor(1);  // text, bg....1=black, 0=white
    gfx->setCursor(40, canvasHeight * .75);
    gfx->fillRoundRect(20, (canvasHeight * .75) - 40, canvasWidth - 40, 60, 20, 0);  // xpos, ypos, x_width, y_height, radius,  color
    gfx->print("Hello World!!!");

    PrintDefinedCanvas(TPChar);  // Print the canvas

    validprinter = false;
    Serial.println("DONE PRINTING...................\n\n\n\n\n");
  }
  BLE.poll();
  peripheral.poll();
  delay(10);
}
void setCanvasSize(int width, int height) {
  if (width % 8 != 0) {
    Serial.println("ERROR: Width must be divisible by 8!");
    return;
  }
  // Free old canvas before creating a new one
  if (gfx) {
    delete gfx;
    gfx = nullptr;
  }
  // Update canvas size
  canvasWidth = width;
  canvasHeight = height;

  // Allocate new canvas
  gfx = new GFXcanvas1(canvasWidth, canvasHeight);
  if (!gfx) {
    Serial.println("ERROR: Memory allocation failed!");
  } else {
    Serial.print("Canvas resized: ");
    Serial.print(canvasWidth);
    Serial.print("x");
    Serial.println(canvasHeight);
  }
}

void PrintDefinedCanvas(BLECharacteristic TPChar) {
  if (!imageData) {
    Serial.println("ERROR: Canvas not initialized!");
    return;
  }
  Serial.println("Sending canvas to printer...");
  uint8_t gs_v_cmd[] = {
    0x1D, 0x76, 0x30, 0x00,
    (uint8_t)(canvasWidth / 8), 0,
    (uint8_t)(canvasHeight & 0xFF),
    (uint8_t)((canvasHeight >> 8) & 0xFF)
  };

  TPChar.writeValue(gs_v_cmd, sizeof(gs_v_cmd));
  Serial.println("GS v 0 command sent");

  int index = 0;
  for (int y = 0; y < canvasHeight; y++) {
    for (int x = 0; x < canvasWidth / 8; x++) {
      uint8_t byte = 0;
      for (int bit = 0; bit < 8; bit++) {
        int pixel = gfx->getPixel(x * 8 + bit, y);
        if (pixel > 0) byte |= (1 << (7 - bit));
      }
      imageData[index++] = byte;
    }
  }
  Serial.println("Canvas converted to raster data");

  for (int i = 0; i < (canvasWidth * canvasHeight / 8); i += 100) {
    TPChar.writeValue(&imageData[i], min((size_t)100, (size_t)((canvasWidth * canvasHeight / 8) - i)));
    delay(10);
  }

  free(imageData);  // Free memory AFTER sending image
  imageData = nullptr;

  Serial.println("Image printed successfully!");
  TPChar.writeValue("\n\n");  // Advance paper
}
void clearCanvas() {  // Clears the canvas buffer before drawing
  if (!gfx) {
    Serial.println("ERROR: Canvas not initialized!");
    return;
  }
  // Clear the canvas before drawing
  gfx->fillScreen(0);

  // Allocate or resize image buffer dynamically
  if (imageData) {
    free(imageData);
    imageData = nullptr;
  }
  imageData = (uint8_t *)malloc((canvasWidth * canvasHeight) / 8);
  if (!imageData) {
    Serial.println("ERROR: Memory allocation failed!");
    return;
  }

  memset(imageData, 0, (canvasWidth * canvasHeight) / 8);
  Serial.println("Canvas prepared!");
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
      BLE.scanForAddress(myPrinterAddress, true);  ///// starts looking for your printer Address
      peripheral = BLE.available();
    }
    Serial.print("Valid Printer = ");
    Serial.println(validprinter);
    Serial.println();
  }
}
//  If printer disconnects, automatically starts searching for it again by myPrinterAddress //
void blePeripheralDisconnectHandler(BLEDevice peripheral) {
  Serial.println("Disconnected BLE event");
  validprinter = false;
  BLE.scanForAddress(myPrinterAddress, true);  ///// starts looking for your printer address
  peripheral = BLE.available();
}
