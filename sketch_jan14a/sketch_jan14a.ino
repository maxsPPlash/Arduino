#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    20
CRGB leds[NUM_LEDS];

int status = WL_IDLE_STATUS;
char ssid[] = "ARRIS-62C0"; //  your network SSID (name)
char pass[] = "C863FCB262C0";// your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 15000;      // local port to listen on

//int LED_BUILTIN = 2;
//bool cur_led = false;

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

void setup() {

//  pinMode (LED_BUILTIN, OUTPUT);

  //Initialize serial and wait for port to open:

  Serial.begin(9600);

//  while (!Serial) {

//    ; // wait for serial port to connect. Needed for native USB port only

//  }

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:

    delay(10000);

  }

  Serial.println("Connected to wifi");

  printWifiStatus();

  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:

  Udp.begin(localPort);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

   xTaskCreate(
    taskLED,          /* Task function. */
    "taskLED",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL);            /* Task handle. */
}

int k = 0;

void loop() {

  // if there's data available, read a packet

  int packetSize = Udp.parsePacket();

  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);

    if (len > 0) {
      // read the packet into packetBufffer
      packetBuffer[len] = 0;

    }

    Serial.println("Contents:");

    Serial.println(packetBuffer);

    if (packetBuffer[0] == 'L') {
      k++;
      k%= NUM_LEDS;
      Serial.println("Led next");
      //cur_led = !cur_led;
      //digitalWrite(LED_BUILTIN, cur_led ? HIGH : LOW);
    }

    Serial.print("Received packet of size ");

    Serial.println(packetSize);

    Serial.print("From ");

    IPAddress remoteIp = Udp.remoteIP();

    Serial.print(remoteIp);

    Serial.print(", port ");

    Serial.println(Udp.remotePort());
    
    // send a reply, to the IP address and port that sent us the packet we received

    Udp.beginPacket(Udp.remoteIP(), 15001);

    Serial.println("Reply:");
    Serial.println(ReplyBuffer);
    int i = 0;
    while (ReplyBuffer[i] != 0)
      Udp.write((uint8_t)ReplyBuffer[i++]);

    Udp.endPacket();

  }
}

void taskLED( void * parameter )
{
  
  while (1) {
    for (int  i = 0; i < NUM_LEDS; ++i) {
      if (i == k)
        leds[i] = CRGB(255, 0, 0);
      else
        leds[i] = CRGB(0, 0, 0);
    }
    //k++;
    //k%= NUM_LEDS;
    FastLED.show();
    delay(1000);
  }

  Serial.println("Ending task LED");
  vTaskDelete( NULL );
}


void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}
