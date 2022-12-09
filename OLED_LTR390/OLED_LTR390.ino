

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_LTR390.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
Adafruit_LTR390 ltr = Adafruit_LTR390();

// OLED FeatherWing buttons map 
// 32u4, M0, M4, nrf52840, esp32-s2 and 328p
  #define BUTTON_A  9
  #define BUTTON_B  6
  #define BUTTON_C  5

void setup() {
 
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  //  display setup
  display.setTextColor(SH110X_WHITE);
  display.setRotation(3); // rotate screen 0,1,2 or 3

  // text display tests
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Adafruit LTR-390 test");

  if ( ! ltr.begin() ) {
    display.println("Couldn't find LTR sensor!");
    while (1) delay(10);
  }

  ltr.setMode(LTR390_MODE_UVS);
  if (ltr.getMode() == LTR390_MODE_ALS) {
    display.println("In ALS mode");
  } else {
    display.println("In UVS mode");
  }

  ltr.setGain(LTR390_GAIN_18);
  display.print("Gain : ");
  switch (ltr.getGain()) {
    case LTR390_GAIN_1: display.println(1); break;
    case LTR390_GAIN_3: display.println(3); break;
    case LTR390_GAIN_6: display.println(6); break;
    case LTR390_GAIN_9: display.println(9); break;
    case LTR390_GAIN_18: display.println(18); break;
  }

  ltr.setResolution(LTR390_RESOLUTION_20BIT);
  display.print("Resolution : ");
  switch (ltr.getResolution()) {
    case LTR390_RESOLUTION_13BIT: display.println(13); break;
    case LTR390_RESOLUTION_16BIT: display.println(16); break;
    case LTR390_RESOLUTION_17BIT: display.println(17); break;
    case LTR390_RESOLUTION_18BIT: display.println(18); break;
    case LTR390_RESOLUTION_19BIT: display.println(19); break;
    case LTR390_RESOLUTION_20BIT: display.println(20); break;
  }

  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);

}

void loop() {
  if (ltr.newDataAvailable()) {

  display.clearDisplay();
  display.setRotation(3);
  display.setTextSize(1);
  
  display.setCursor(0,0);
  display.print("* Sun Check "); 

  display.drawLine(15,10,110,10,1); 
  
  display.setCursor(0,25);
  display.print("Sensor Count:"); 

  display.setCursor(80,25);
  display.print(ltr.readUVS());

  display.setCursor(0,45);
  display.print("UV Index:"); 

  display.setTextSize(2);
  display.setCursor(55,42);
  display.print(ltr.readUVS()/2300);
  }

  const int dark = 0;
  const int low = 6900;
  const int moderate = 13800;
  const int high = 18400;
  const int veryhigh = 23000;
  const int UVread = ltr.readUVS();

  display.setTextSize(1);
    
  if (UVread > 23000) {
  display.setCursor(80,45);
  display.print("Extreme!");
  } 

 if (UVread > high && UVread < veryhigh) {
  display.setCursor(68,45);
  display.print("Very High!");
  } 

   if (UVread > moderate && UVread < high) {
  display.setCursor(70,45);
  display.print("High!");
  } 

  if (UVread > low && UVread < moderate) {
  display.setCursor(70,45);
  display.print("Moderate");
  } 

   if (UVread > dark && UVread < low) {
  display.setCursor(70,45);
  display.print("Low");
  } 
  
  display.display();
  delay(1000);
 
}
