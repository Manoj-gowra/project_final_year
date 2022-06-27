#include <SoftwareSerial.h>
SoftwareSerial SIM900(2,4);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
String long_lat;

String Link;
String SMS;

#include <TinyGPS++.h>

TinyGPSPlus gps;


static const uint32_t GPSBaud = 9600;

float latitude;
float longitude;

 int tilt=A0;

void GPS()
{
if (gps.charsProcessed() < 10)
  {
    //Serial.println("No GPS detected: check wiring.");
     // Blynk.virtualWrite(V4, "GPS ERROR");         // Value Display widget  on V4 if GPS not detected
  }
}
void displaygpsInfo()
{ 

  if (gps.location.isValid() ) 
  {
    
     latitude = (gps.location.lat());      //Storing the Lat. and Lon. 
     longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 4);                // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 4); 
    lcd.setCursor(0,0);
    lcd.print("LT:");
    lcd.print(latitude, 4);

    lcd.setCursor(9,0);
    lcd.print("LO:");
    lcd.print(longitude, 4);
      }
}

int get_location(String message){

  SIM900.print("AT+CMGF=1\r");     // AT command to set SIM900 to SMS mode
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");       // Set module to send SMS data to serial out upon receipt
  delay(100);
    
  SIM900.println("AT+CMGF=1"); // Replace x with mobile number
  delay(1000);
  SIM900.println("AT+CMGS= \"+918919949421\"\r"); // Replace Number with mobile number
  delay(1000); 
  SIM900.println(message);// The SMS text you want to send
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  
}

void locate()
{
  while (Serial.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(Serial.read()))
        displaygpsInfo();
  }
}

void adxlsensor()
{
int tval=analogRead(tilt);
Serial.println(tval);
delay(500);
if(tval<300 || tval>400)
{  long_lat= "13.0980,77.5776";
   //long_lat = String(float(latitude))+","+String(float(longitude));
   Link = "https://www.google.com/maps/search/?api=1&query="+String(long_lat);
//   SMS = "Emergency, i am in trouble "+ long_lat + " " + Link; //////////////////////////////SMS message
  SMS = "Emergency, i am in trouble" + Link;
   Serial.println("message sent");
   get_location(SMS);
   //lcd.clear();
   lcd.setCursor(0,1);
   lcd.print("message sent");
     //  Serial2.print(1); 
 
  // digitalWrite(buzzer,HIGH);    
}
else
{   
     //lcd.clear();
   lcd.setCursor(0,1);
   lcd.print("              ");
//   digitalWrite(buzzer,LOW);
  }
  }
  
void setup() {
  // put your setup code here, to run once:
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  Serial.begin(9600); 
  SIM900.begin(9600);
 

  pinMode(tilt,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
locate();
adxlsensor();
}
