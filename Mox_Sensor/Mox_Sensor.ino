
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SGP40.h>
#include <sensirion_arch_config.h>
#include <sensirion_voc_algorithm.h>

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
Adafruit_SGP40 sgp;

// OLED FeatherWing buttons map 
// 32u4, M0, M4, nrf52840, esp32-s2 and 328p
  #define BUTTON_A  9
  #define BUTTON_B  6
  #define BUTTON_C  5

void setup() {
 
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  // Show image buffer on the display hardware.
  
  display.display();
  delay(2500);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

    // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.setRotation(1); // rotate 270 degrees
  display.println("Mox SGP 40");

  if (! sgp.begin()){
    display.println("Sensor not found :(");
    while (1);
  }
  display.print("Found SGP40 serial #");
  display.print(sgp.serialnumber[0], HEX);
  display.print(sgp.serialnumber[1], HEX);
  display.println(sgp.serialnumber[2], HEX);

}
int counter = 0;
void loop() {

  uint16_t raw;
  
  raw = sgp.measureRaw();

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(5,0);
  display.setRotation(1); // rotate 270 degrees
  display.print("Air Quality 0 to 500"); 

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(30,15);
  display.setRotation(1); // rotate 270 degrees
  display.print("100 is good"); 

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,27);
  display.setRotation(1); // rotate 270 degrees
  display.print("VOC index ="); 

  display.drawLine(0,50,35,50,2); 

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(65,40);
  display.setRotation(1); // rotate 270 degrees 
  display.println(raw);

  display.display();
  delay(1500);
 
}
