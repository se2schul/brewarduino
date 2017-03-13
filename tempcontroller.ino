#include <OneWire.h>
#include <DallasTemperature.h>


#define MODE 0 // mode 1 for temp control, otherwise percentage controller
#define PIN 8
//#define PIN 13
#define TEMP 0
#define POWER 65

#define ONE_WIRE_BUS 2

// Setup oneWire instance and Dallas Temperature.
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress hltProbe = { 0x28, 0xE2, 0x69, 0xFA, 0x05, 0x00, 0x00, 0xE4 };

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(hltProbe, 10);

  pinMode(PIN,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(13,OUTPUT);
}

float printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }
  return tempC;
}

void loop(void)
{
  float hltTemp = -127.0;
  Serial.print("Getting temperatures...\n\r");
  sensors.requestTemperatures();

  Serial.print("hltProbe: ");
  hltTemp = printTemperature(hltProbe);
  Serial.println(" ");

  Serial.println(" ");

  if (MODE == 1) {
    // temp control
    Serial.println("Temp Controller");
    if (hltTemp != -127.0) {
      if (hltTemp < TEMP) {
        Serial.println("set pin high");
        digitalWrite(PIN, HIGH);
      } else {
         Serial.println("set pin LOW");
        digitalWrite(PIN, LOW);
      }
    }
    delay(1000);
  } else {
    Serial.println("Percentage Controller");
    // proportional control
    if (POWER > 0) {
      digitalWrite(PIN, HIGH);
       //digitalWrite(13, HIGH);
      delay(POWER * 10);
    }
    if (POWER < 100) {
      digitalWrite(PIN, LOW);
      delay(1000 - (POWER * 10));
    }
  }

}
