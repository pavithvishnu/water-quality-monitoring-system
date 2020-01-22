
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define phpin A0
#define tdspin A1
#define turbiditypin A3
#define buzzer 3
#define greenled 4
#define redled 5

float sensor1volt,sensor2volt;
float sensor1,sensor2,sensor3;
float phvalue=0,acidvoltage=2032.44,neutralvoltage=1500.0; // data for phsensor
float ecvalue,ecvalue25,analogval2,tdsvalue=0,aref=5.0,adcrange=1024.0,tdsfactor =0.5,kvalue=1.0; // data for tdssensor
float turbidityvalue; // data for turbidity sensor
float temperature=25;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(9, 10);

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  pinMode(greenled,OUTPUT);
  pinMode(redled,OUTPUT);
  lcd.init();                      // initialize the lcd 
  
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("WATER  QUALITY");
  lcd.setCursor(1,1);
  lcd.print("MONITORING  SYSTEM ");
  delay(5000);
  lcd.clear();
 

}

void loop() {
  // put your main code here, to run repeatedly:
  phvalue=readph();
  delay(1000);
  tdsvalue=readtds();
  delay(1000);
  turbidityvalue=readturbidity();
  delay(1000);
  
   

  if (phvalue <=6.0 || phvalue>=7.9)
  {
    Serial.println("water is not safe to drink - acidic/alkaline");
    digitalWrite(buzzer,HIGH);
    digitalWrite(redled,HIGH);
    digitalWrite(greenled,LOW);
    lcd.setCursor(1,3);
    lcd.print("NOT SAFE TO DRINK");
    gsmnotsafe();
  }
  else if (tdsvalue >=900)
  {
    Serial.println("water is not safe to drink - high tds value ");
    digitalWrite(buzzer,HIGH);
    digitalWrite(redled,HIGH);
    digitalWrite(greenled,LOW);
    lcd.setCursor(1,3);
    lcd.print("NOT SAFE TO DRINK");
    gsmnotsafe();
    
    }
    else if (turbidityvalue <=4.0)
    {
      Serial.println("water is not safe to drink - water is not clear"); 
      digitalWrite(buzzer,HIGH);
      digitalWrite(redled,HIGH);
      digitalWrite(greenled,LOW);
      lcd.setCursor(1,3);
      lcd.print("NOT SAFE TO DRINK");
      gsmnotsafe();
      
    }
    else
    {
      Serial.println("water is safe to drink");
      digitalWrite(buzzer,LOW);
      digitalWrite(redled,LOW);
      digitalWrite(greenled,HIGH);
      lcd.setCursor(1,3);
      lcd.print(" SAFE TO DRINK    ");
      gsmsafe();
    }
}
float readph()
{
    sensor1volt = analogRead(phpin)/1024.0*5000;
    float slope = (7.0-4.0)/((neutralvoltage-1500.0)/3.0 - (acidvoltage-1500.0)/3.0);  // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
    float intercept =  7.0 - slope*(neutralvoltage-1500.0)/3.0;
    sensor1 = slope*(sensor1volt-1500.0)/3.0+intercept;
    Serial.print("phvalue:");
    Serial.println(sensor1);
    lcd.setCursor(1,0);
    lcd.print("pH:");
    lcd.setCursor(5,0);
    lcd.print( sensor1);
    return sensor1;
  }
  
  float readtds()
  {
    analogval2 = analogRead(tdspin);
    sensor2volt = analogval2/adcrange*aref;
    ecvalue=(133.42*sensor2volt*sensor2volt*sensor2volt - 255.86*sensor2volt*sensor2volt + 857.39*sensor2volt)*kvalue;
    ecvalue25  =  ecvalue / (1.0+0.02*(temperature-25.0));  //temperature compensation
    sensor2 = ecvalue25 * tdsfactor;
    Serial.print(tdsvalue);
    Serial.println("ppm");
     lcd.setCursor(1,1);
  lcd.print("TDS:");
  lcd.setCursor(6,1);
 lcd.print( sensor2);
    return sensor2;
  }

 float readturbidity()
 {
  int sensor3volt = analogRead(turbiditypin);// read the input on analog pin 0:
  sensor3 = sensor3volt * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(sensor3);
   lcd.setCursor(1,2);
  lcd.print("TURBIDITY:");
  lcd.setCursor(12,2);
  lcd.print(sensor3);
  if(sensor3 <4.0)
  {
    Serial.println("dirty water"); // print out the value you read:
  }
  else if(sensor3 >=4.0)
  {
    Serial.println("clean water"); // print out the value you read:
  }
  delay(1000);
  return sensor3;
  
 }
 void gsmsafe()
 {
   mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
   delay(1000);  // Delay of 1000 milli seconds or 1 second
   mySerial.println("AT+CMGS=\"+918903165116\"\r"); // Replace x with mobile number
   delay(1000);
   mySerial.println("Water is safe to drink");// The SMS text you want to send
   delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
   delay(5000);
   Serial.println("messsage send");
 }

void gsmnotsafe()
 {
   mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
   delay(1000);  // Delay of 1000 milli seconds or 1 second
   mySerial.println("AT+CMGS=\"+918903165116\"\r"); // Replace x with mobile number
   delay(1000);
   mySerial.println("Water is not safe to drink");// The SMS text you want to send
   delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
   delay(5000);
   Serial.println("messsage send");
 }
