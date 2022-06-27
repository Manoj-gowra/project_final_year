#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DFRobot_MAX30102.h>
#include <DHT.h>  
#include <LiquidCrystal.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
 
 // initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
 
 
#define DHTPIN 4       
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

const int PushButton1=2;
const int PushButton2=4; 

/********************************************************************/
// Data wire is plugged into pin 7 on the Arduino 
#define ONE_WIRE_BUS 5 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 

 
//sda D21, scl D122
DFRobot_MAX30102 particleSensor;
int buzzer=5;
  int accel=33;
int ecgsensor=34; 
  
BlynkTimer t; 
WidgetBridge bridge1(V50);

char auth[] = "2dZP6AG1Y5sBubzZJiodxsNWo5ZoUMI5";

char ssid[] = "door";
char pass[] = "12345678";
 
int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid 

    void adxl()   //accelerometer
    {
      int ff1=analogRead(accel);
      Blynk.virtualWrite(V9,ff1);
      if(ff1>2150 || ff1<1600)
      {
      Blynk.virtualWrite(V8,"Person fallen");
      //digitalWrite(buzzer,HIGH);
        }
        else
        {
           Blynk.virtualWrite(V8,"Normal");
        //   digitalWrite(buzzer,LOW);
          }
      }
 
  
void heartoxygen()
{
    Serial.println(F("Wait about four seconds"));
  particleSensor.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);
  //Print result 
 Serial.print(F("heartRate="));
  Serial.print(heartRate, DEC);
  Serial.print(F(", heartRateValid="));
  Serial.print(heartRateValid, DEC);
  Serial.print(F("; SPO2="));
  Serial.print(SPO2, DEC);
  Serial.print(F(", SPO2Valid="));
  Serial.println(SPO2Valid, DEC);
lcd.clear();
  lcd.setCursor(0,0);
lcd.print("H:");
lcd.print(heartRate);

lcd.setCursor(7,0);
lcd.print("O2:");
lcd.print(SPO2);
Blynk.virtualWrite(V0,heartRate);
Blynk.virtualWrite(V1,SPO2);
if(SPO2<60)
{
  digitalWrite(buzzer,HIGH);
  Blynk.notify("Oxygen level is abnormal");
  }
  else 
  {
    digitalWrite(buzzer,LOW);
    }
  
  }
 
void btempv()
{
  
  sensors.requestTemperatures(); // Send the command to get temperature readings 
 
/********************************************************************/
 Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempFByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
 //lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("TEMP:");
 lcd.setCursor(5,1);
 lcd.print(sensors.getTempFByIndex(0));
 lcd.setCursor(11,1);
 lcd.print("F");
Blynk.virtualWrite(V5,sensors.getTempFByIndex(0));  
 }
 

void ecg()
{
int ec=analogRead(ecgsensor);
Serial.println(ec);
delay(200); 
 
Blynk.virtualWrite(V4,ec);
}

 
void setup()
{
  //Init serial
  Serial.begin(115200);
   Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
   
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("WiFi connected");
  lcd.begin(16,2);
  Blynk.begin(auth, ssid, pass);

    while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    delay(1000);
    }
    lcd.begin(16, 2);
  sensors.begin(); 
pinMode(buzzer,OUTPUT);
  pinMode(ecgsensor,INPUT);
    pinMode(accel,INPUT); 
 pinMode(PushButton1, INPUT);
 pinMode(PushButton2, INPUT);

 
    particleSensor.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4, \
                        /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100, \
                        /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);
 t.setInterval(2000, push);

}

BLYNK_CONNECTED() {
  bridge1.setAuthToken("oMHWwGmdfSEsWAaEzS9H7p1Z_uDXOIrX"); // Place the AuthToken of the second hardware here
}
void push()
{
 int Push_button_state1 = digitalRead(PushButton1); 
 int Push_button_state2 = digitalRead(PushButton2);

  if(Push_button_state1 == HIGH)
  {
    Blynk.virtualWrite(V2, "Lane one cleared");
    bridge1.virtualWrite(V15, 1);
  }
  else if(Push_button_state2 == HIGH)
  {
    Blynk.virtualWrite(V2, "Lane two cleared");
    bridge1.virtualWrite(V15, 2);
  }
  else
  {
    Blynk.virtualWrite(V2, "No tarffic");
    bridge1.virtualWrite(V15, 0);
  }
    
  }

void loop()
{
 
Blynk.run();
 t.run();  

btempv();

heartoxygen();
//tempv();
adxl();
 ecg();
 }


