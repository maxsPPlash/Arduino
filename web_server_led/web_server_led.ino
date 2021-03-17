#include <WiFi.h>

const char* ssid = "ARRIS-62C0";  
const char* password = "C863FCB262C0";

WiFiServer server(8888);
String header;

int LED_BUILTIN = 2;
bool cur_led = false;

void setup() {
  Serial.begin(115200);

  pinMode (LED_BUILTIN, OUTPUT);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   
  
    if (client) {                           
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) 
    {            // loop while the client's connected
      if (client.available()) {             
        char c = client.read();            

        if (c == 'L') {
          Serial.println("Led change");
          cur_led = !cur_led;
          digitalWrite(LED_BUILTIN, cur_led ? HIGH : LOW);
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
