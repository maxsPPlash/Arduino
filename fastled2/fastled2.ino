#include <FastLED.h>
#define LED_PIN     12
#define NUM_LEDS    300
CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  
}
int k = 0;
void loop() {
  for (int  i = 0; i < NUM_LEDS; ++i) {
    if (i == k)
      leds[i] = CRGB(255, 0, 0);
    else
      leds[i] = CRGB(0, 0, 0);
  }
  k++;
  k%= NUM_LEDS;
  FastLED.show();
  delay(500);
}
