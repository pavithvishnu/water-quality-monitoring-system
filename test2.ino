

#define phpin A0
#define tdspin A1
#define turbiditypin A3

float sensor1volt,sensor2volt;
float sensor1,sensor2,sensor3;
float phvalue=0,acidvoltage=2032.44,neutralvoltage=1500.0; // data for phsensor
float ecvalue,ecvalue25,analogval2,tdsvalue=0,aref=5.0,adcrange=1024.0,tdsfactor =0.5,kvalue=1.0; // data for tdssensor
float turbidityvalue; // data for turbidity sensor
float temperature=25;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //phvalue=readph();
  //delay(1000);
  //tdsvalue=readtds();
  //delay(1000);
  turbidityvalue=readturbidity();
  delay(1000);

}
float readph()
{
    sensor1volt = analogRead(phpin)/1024.0*5000;
    float slope = (7.0-4.0)/((neutralvoltage-1500.0)/3.0 - (acidvoltage-1500.0)/3.0);  // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
    float intercept =  7.0 - slope*(neutralvoltage-1500.0)/3.0;
    sensor1 = slope*(sensor1volt-1500.0)/3.0+intercept;
    Serial.print("phvalue:");
    Serial.println(sensor1);
    delay(1000);
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
    delay(1000);
    return sensor2;
  }

 float readturbidity()
 {
  int sensor3volt = analogRead(turbiditypin);// read the input on analog pin 0:
  sensor3 = sensor3volt * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(sensor3);
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
