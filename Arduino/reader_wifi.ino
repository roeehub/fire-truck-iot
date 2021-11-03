#include <SPI.h>
#include "MFRC522.h"
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "fire-truck-iot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "xseu33wYRo60cQHOJwuevCzMokATlwMg13rZq5Wo"
#define WIFI_SSID "OnePlus 5"
#define WIFI_PASSWORD "26111996"

#define RST_PIN  5
#define SS_PIN  4

const char* ids[8] = {"0", "1", "2", "3", "4", "5", "6", "7"};
const char *uids[8] = {"4e3abeee",
                       "e1bc2ee",
                       "feca65ee",
                       "1e69bbee",
                       "aefa6ee",
                       "aeb72ee",
                       "fe99afee",
                       "ce7e74ee"};


MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(115200);    // Initialize serial communications
  delay(500);
  
  Serial.print("Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522

  Serial.println(F("Ready!"));
}

void loop() { 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    Serial.print(".");
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    Serial.print(",");
    delay(50);
    return;
  }
  Serial.print(F("Card UID:"));  
  String card_id = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println(card_id);
  Firebase.setInt(card_id, 0);
  Serial.print("uploaded, and got from firebase: ");
  Serial.println(Firebase.getInt(card_id));
}

// Helper routine to dump a byte array as hex values to Serial
String dump_byte_array(byte *buffer, byte bufferSize) {
  String content= "";
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i], HEX)); 
  }
  Serial.println();
  Serial.println(content);

  for (int i = 0; i < 8; i++) {
    if (content == uids[i]) {
      Serial.print("found card: ");
      Serial.println(i);
      return ids[i];
    }
  }
  Serial.println("not found");
  
 }
