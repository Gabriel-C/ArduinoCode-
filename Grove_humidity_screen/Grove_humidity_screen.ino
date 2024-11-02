#include <Arduino.h>
#include <U8x8lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define LCD 0x3C
#define Humidity_Sensor 0x77
#define SEALEVELPRESSURE_HPA (1013.25)
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;
U8X8_SH1107_SEEED_128X128_HW_I2C u8x8;
Adafruit_BME280 bme; // I2C

char temp_str[5];
char humidity_str[5];
char pressure_str[7];

void setup() { 
  Serial.begin(9600);

  Serial.println(F("BME280 test"));
  if (!bme.begin(Humidity_Sensor)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  Serial.println(F("Display test"));
  u8x8.setI2CAddress(0x78); //0x3C * 2
  if (!u8x8.begin()) {
    Serial.println("Could not activate display, check wiring!");
    while (1);
  }
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(1,1,"OK");
}

void loop() {
    float temperature = bme.readTemperature();
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" *C");

    float pressure = bme.readPressure() / 100.0F;
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" hPa");

    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.print("Approx. Altitude = ");
    Serial.print(altitude);
    Serial.println(" m");

    float humidity = bme.readHumidity();
    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.println();
    
    u8x8.setFont(u8x8_font_chroma48medium8_r);

    u8x8.setInverseFont(1);
    u8x8.drawString(0,1,"Sensor 1        ");
    u8x8.setInverseFont(0);

    dtostrf(temperature,5,2,temp_str);
    u8x8.drawString(1,2,"T:");
    u8x8.drawString(17,2,temp_str);
    u8x8.drawString(57,2,"*C");

    dtostrf(humidity,5,2,humidity_str);
    u8x8.drawString(1,3,"H:");
    u8x8.drawString(17,3,humidity_str);
    u8x8.drawString(57,3,"%");

    dtostrf(pressure,7,2,pressure_str);
    u8x8.drawString(1,4,"P:");
    u8x8.drawString(17,4,pressure_str);
    u8x8.drawString(81,4,"hPa");

    u8x8.setInverseFont(1);
    u8x8.drawString(0,5,"Sensor 2        ");
    u8x8.setInverseFont(0);

    u8x8.drawString(1,6,"T: ");
    u8x8.drawString(1,7,"H:");
    u8x8.drawString(1,8,"P: ");
    u8x8.refreshDisplay();		// only required for SSD1606/7  
    delay(2000);
}