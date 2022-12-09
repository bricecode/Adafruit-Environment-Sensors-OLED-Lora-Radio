#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SGP40.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_LTR390.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_MAX1704X.h>

Adafruit_MAX17048 maxlipo;
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
Adafruit_LTR390 ltr = Adafruit_LTR390();
Adafruit_AHTX0 aht;
Adafruit_SGP40 sgp;
Adafruit_BMP3XX bmp;

#define SEALEVELPRESSURE_HPA (1013.25)
#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5


void setup() {

Serial.begin(115200);
maxlipo.begin();
display.begin(0x3C, true); // Address 0x3C default Show image buffer on the display hardware.

display.display();
delay(2000);
display.clearDisplay();
display.display();

pinMode(BUTTON_A, INPUT_PULLUP);
pinMode(BUTTON_B, INPUT_PULLUP);
pinMode(BUTTON_C, INPUT_PULLUP);

// Set LTR390
 if (ltr.begin()) {
    Serial.println("Found LTR390");
  } else {
    Serial.println("Didn't find LTR390");
  } 
 ltr.setMode(LTR390_MODE_UVS);
 ltr.setGain(LTR390_GAIN_18);
 ltr.setResolution(LTR390_RESOLUTION_20BIT);
 ltr.setThresholds(100, 1000);
 ltr.configInterrupt(true, LTR390_MODE_UVS);
 
// Set AHT20 
 if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  
// Set SGP40
if (sgp.begin()) {
    Serial.println("Found SGP40");
  } else {
    Serial.println("Didn't find SGP40");
  }  
// Set BMP388
if (bmp.begin_I2C()) {
    Serial.println("Found SGP40");
  } else {
    Serial.println("Didn't find SGP40");
  }  
}

int counter = 0;

void loop() {

display.clearDisplay(); 
// Set text mode
display.setRotation(1);
display.setTextSize(1);
display.setTextColor(SH110X_WHITE);  

// show SGP40
 uint16_t raw;
 raw = sgp.measureRaw();
  display.setCursor(0,0);
  display.print("VOC index"); 
  display.setCursor(65,0);
  display.print(raw);

// show AHT20
  sensors_event_t humidity, temp; 
  aht.getEvent(&humidity, &temp);
  display.setCursor(0,10);
  display.print("Temp"); 
  display.setCursor(65,10);
  display.print((temp.temperature), 1); display.println(" C");
  
  display.setCursor(0,20);
  display.print("Humidity"); 
  display.setCursor(65,20);
  display.print((humidity.relative_humidity), 1); display.println(" %");
  yield();
  
// show LTR390
  display.setCursor(0,30);
  display.print("UV/Index");
  display.setCursor(65,30);
  display.print(ltr.readUVS()); display.print("/"); display.print(ltr.readUVS()/2300);
 
// show BMP388
  display.setCursor(0,40);
  display.print("Pressure"); 
  display.setCursor(65,40);
  display.print((bmp.pressure / 100.0), 0); display.println(" hPa");
  
  display.setCursor(0,50);
  display.print("Altitude"); 
  display.setCursor(65,50);
  display.print(bmp.readAltitude(SEALEVELPRESSURE_HPA), 0); display.println(" m");

// Button C for Batt Check
  if(!digitalRead(BUTTON_C)){ 
display.clearDisplay();  
display.setCursor(0,0);
display.print(F("Batt Voltage: ")); display.print(maxlipo.cellVoltage(), 1); display.println(" V");
display.print(F("Batt Percent: ")); display.print(maxlipo.cellPercent(), 1); display.println(" %");
display.print(F("Rate: ")); display.print(maxlipo.chargeRate(), 1); display.println(" %/hr");
display.display();
delay(3000);
 }

display.display();
delay(1000);
}
