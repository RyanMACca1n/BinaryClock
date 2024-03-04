#include <Adafruit_NeoPixel.h>

#define PIN             6
#define NUMPIXELS      2

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // set led number and given color
  pixels.show();  
  delay(500);

  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // set led number and given color
  pixels.show();
  delay(500);

  pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // set led number and given color
  pixels.show();
  delay(500);

  pixels.setPixelColor(1, pixels.Color(255, 0, 0)); // set led number and given color
  pixels.show();
  delay(500);

  pixels.setPixelColor(1, pixels.Color(0, 255, 0)); // set led number and given color
  pixels.show();
  delay(500);

  pixels.setPixelColor(1, pixels.Color(0, 0, 255)); // set led number and given color
  pixels.show();
}
