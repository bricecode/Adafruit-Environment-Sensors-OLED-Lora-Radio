
#include <RadioLib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
  #define BUTTON_A  9
  #define BUTTON_B  6
  #define BUTTON_C  5

SX1278 radio = new Module(8, 3, 4, 7);

void setup() {

  display.begin(0x3C, true); // Address 0x3C default
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  display.setRotation(1);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  Serial.begin(9600);
  // initialize SX1278 with default settings
  int state = radio.begin(434.0, 125.0, 9, 7, RADIOLIB_SX127X_SYNC_WORD, 20, 8, 0);
  radio.setDio0Action(setFlag);
  // start listening for LoRa packets
  Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  }

  volatile bool receivedFlag = false;
  volatile bool enableInterrupt = true;

void setFlag(void) {
  if(!enableInterrupt) {
    return;
  }
  receivedFlag = true;
}

void loop() {
  if(receivedFlag) {
    enableInterrupt = false;
    receivedFlag = false;

    String str;
    int state = radio.readData(str);

    if (state == RADIOLIB_ERR_NONE) {
 
      // print data of the packet
      Serial.print(F("[SX1278] Data:\t\t"));
      Serial.println(str);
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0,0);
      display.println(F("[SX1278] Data:\t\t"));
      display.println(str);
      display.display();
      delay(3000);

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("[SX1278] RSSI:\t\t"));
      Serial.print(radio.getRSSI());
      Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      Serial.print(F("[SX1278] SNR:\t\t"));
      Serial.print(radio.getSNR());
      Serial.println(F(" dB"));
    }

    radio.startReceive();
    enableInterrupt = true;
  }

}
