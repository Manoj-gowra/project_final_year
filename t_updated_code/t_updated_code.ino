#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "oMHWwGmdfSEsWAaEzS9H7p1Z_uDXOIrX";
int r1 = D1;
int g1 = D2;
int r2 = D3;
int g2 = D4;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "door";
char pass[] = "12345678";

BLYNK_WRITE(V15)
{
  int v1 = param.asInt();
  if(v1 == 1)
  {
    digitalWrite(r1, LOW);
    digitalWrite(g1, HIGH);
    digitalWrite(r2, HIGH);
    digitalWrite(g2, LOW);
  }
  else if(v1 == 2)
  {
    digitalWrite(r1, HIGH);
    digitalWrite(g1, LOW);
    digitalWrite(r2, LOW);
    digitalWrite(g2, HIGH);
  }
  else
  {
        digitalWrite(r1, LOW);
    digitalWrite(g1, LOW);
    digitalWrite(r2, LOW);
    digitalWrite(g2, LOW);
    
  }
}




void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(g2, OUTPUT);
          digitalWrite(r1, LOW);
    digitalWrite(g1, LOW);
    digitalWrite(r2, LOW);
    digitalWrite(g2, LOW);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
}

