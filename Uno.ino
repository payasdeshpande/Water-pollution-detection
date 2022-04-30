#include <DTH_Turbidity.h>


#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
#define TURBIDITY_SENSOR_PIN 2
DTH_Turbidity turbSensor(TURBIDITY_SENSOR_PIN);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float calibration = 27.00; 
const int analogInPin = A0;

   
int sensorValue = 0;
unsigned long int avgValue;
float b;
int buf[10],temp;
int sensorVal = 0;
char Incoming_value = 0;

String temperature;


String turbidity;
String phValue;

String values;
void setup() {

  pinMode(analogInPin,INPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);

   Serial.begin(9600);

sensors.begin();

   

   delay(2000);
}


 

void loop() {

   //PH sensor value
  for(int i=0;i<10;i++)
{
  buf[i]=analogRead(analogInPin);
  delay(30);
  }
  int temp = 0;
  for(int i=0;i<9;i++)
  {
  for(int j=i+1;j<10;j++)
  {
  if(buf[i]>buf[j])
  {
  temp=buf[i];
  buf[i]=buf[j];
  buf[j]=temp;
  }
  }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
  avgValue+=buf[i];
  float pHVol=(float)avgValue*5.0/1024/6;
  float phVal = -5.70 * pHVol + calibration;
  phValue = String(phVal);

 //Temperature
 sensors.requestTemperatures();
 
float tempera = sensors.getTempFByIndex(0);
tempera=tempera+273;
 temperature = String(tempera);
 
 //Turbidity
 sensorVal = turbSensor.readTurbidity();
 
 turbidity = String(sensorVal);
 

 values= (temperature+','+turbidity+','+phValue);

  Serial.flush();
  delay(1000);
 
  if(Serial.available() > 0)  
  {
    Incoming_value = Serial.read();     
    //Serial.print(Incoming_value);       
    //Serial.print("\n");      
    if(Incoming_value == '1')           
      digitalWrite(LED_BUILTIN, HIGH);  
    else if(Incoming_value == '0')      
      digitalWrite(LED_BUILTIN, LOW);   
  }                       
  Serial.print(values);
  delay(2000);
}
