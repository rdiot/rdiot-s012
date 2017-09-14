/* RFID-RC522 (MFRC-522) [S012] : http://rdiot.tistory.com/172 [RDIoT Demo] */

#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  9  // 
#define SS_PIN  10  //

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
String rfidUid = "";

void setup() {
  
 Serial.begin(9600);  // Initialize serial communications with the PC
 while (!Serial);  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
 SPI.begin();   // Init SPI bus
 mfrc522.PCD_Init();  // Init MFRC522
 ShowReaderDetails(); // Show details of PCD - MFRC522 Card Reader details
 Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
}

void loop() {

  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  //delay(500);
  
 // Look for new cards
 if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;
 }

 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;
 }
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
rfidUid = "";
for (byte i = 0; i < mfrc522.uid.size; i++) {
  rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
  rfidUid += String(mfrc522.uid.uidByte[i], HEX);
}

 // Dump debug info about the card; PICC_HaltA() is automatically called

}

void ShowReaderDetails() {
 // Get the MFRC522 software version
 byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
 Serial.print(F("MFRC522 Software Version: 0x"));
 Serial.print(v, HEX);
 if (v == 0x91)
  Serial.print(F(" = v1.0"));
 else if (v == 0x92)
  Serial.print(F(" = v2.0"));
 else
  Serial.print(F(" (unknown)"));
 Serial.println("");
 // When 0x00 or 0xFF is returned, communication probably failed
 if ((v == 0x00) || (v == 0xFF)) {
  Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
 }
}

void draw(void) {
  u8g.setFont(u8g_font_9x15B);
  u8g.setPrintPos(0, 12); 
  u8g.println("MFRC522");

  u8g.setFont(u8g_font_helvB14);
  u8g.setPrintPos(0,40);

  u8g.print("UID="+rfidUid);

  u8g.setFont(u8g_font_helvB14);
  u8g.setPrintPos(0,60);

  if(rfidUid == "3f0f3e5f")
  u8g.print("OPEN=YES");
  else if(rfidUid == "")
  u8g.print("NONE");
  else
  u8g.print("OPEN=NO");  
}
