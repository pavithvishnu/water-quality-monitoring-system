#include <EEPROM.h>

#include "LiquidCrystal_I2C.h"
#include "GravityTDS.h"
#include "DFRobot_PH.h"

#define turbiditypin A0
#define TdsSensorPin A1
#define PH_PIN A2
GravityTDS gravityTds;
DFRobot_PH ph;

float voltage1,voltage,sen1,sen2,sen3,phValue;
float temperature = 25,tdsValue = 0;
int sensorValue;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
    Serial.begin(115200);
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
    ph.begin();
     lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
}

void loop()
{
    //temperature = readTemperature();  //add your temperature sensor and read it
   sen1=tds();
  sen2=phsen();
   sen3=tursen();
   if(sen2<=6.0 || sen2>=8.0 || sen3<=4.0 || sen1>=120)
   {
    Serial.println("water not safe");
   }
   else 
   {
    Serial.println("water is safe");
   }
}


float phsen()
{
  static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U){                  //time interval: 1s
        timepoint = millis();
        voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        Serial.print("  pH:");
        Serial.println(phValue);
        return phValue;
    }
}

float tds()
{
   gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue);
    Serial.println("ppm");
    delay(1000);
    return tdsValue;
}
float tursen()
{
  sensorValue = analogRead(turbiditypin);// read the input on analog pin 0:
  voltage1 = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage1); // print out the value you read:
  delay(500);
  return voltage1;
}
