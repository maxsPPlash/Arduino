#define BLYNK_PRINT Serial

int pin        = 2;

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6dX5ZG8AJ7tmLo0AcN9JyJkD8CVkH5Nc";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ARRIS-62C0";
char pass[] = "C863FCB262C0";

void setup() {  
  //pinMode(pin, OUTPUT); 
  //pinMode(pin, HIGH);
  Serial.begin(9600);

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  

  Blynk.begin(auth, ssid, pass);

}

void loop(){
    Blynk.run();
}
