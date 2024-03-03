#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>
#include <TimeLib.h>        //http://www.arduino.cc/playground/Code/Time
#include <Timezone.h> 

#define PIN             6
#define NUMPIXELS      20
#define BUTTON_PIN      7

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
ezButton button(BUTTON_PIN);
RTC_DS3231 rtc;
TimeChangeRule *tcr;        //pointer to the time change rule, use to get the TZ abbrev
time_t utc;

uint8_t value[6] = {0, 0, 0, 0, 0, 0};
uint8_t ledColor[3] = {0, 0, 0};
uint8_t ledState = 1;


void setup() {
  // code here, to run once:
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  button.setDebounceTime(50);
  pixels.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  button.loop();

  if (button.isPressed()) {
    // toggle the state off the leds
    ledState++;
    if (ledState >= 4) {
      ledState = 0;
    }
  }

  // turn the leds off
  if(ledState == 0) {
    for (uint8_t i = 0; i < 20; i++) {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
      pixels.show();
    }
  }

  if(ledState != 0) {
    uint8_t binNumbers[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    DateTime dt = rtc.now();
    utc = dt.unixtime();
    time_t t = CE.toLocal(utc, &tcr);

    /*
    Serial.print(hour(t));
    Serial.print(':');
    Serial.print(minute(t));
    Serial.print(':');
    Serial.print(second(t));
    Serial.println(); 
    Serial.print("Day= ");
    Serial.print(weekday(t));
    Serial.println(); 
    Serial.print(day(t));
    Serial.print(".");
    Serial.print(month(t));
    Serial.print(".");
    Serial.print(year(t));
    Serial.println();
    Serial.print("Temperature= ");
    Serial.print(rtc.getTemperature());
    Serial.println();
    */

    // show time
    if(ledState == 1) {
      setValues(hour(t), minute(t), second(t));

      // set ledColor to according to the weekday value
      switch(weekday(t)) {
        // Sunday - Violet
	      case 1:
          setLedColor(170,0,255);
          break;
        
        // Monday - Red
        case 2:
          setLedColor(255,0,0);
          break;

        // Tuesday - Orange
        case 3:
          setLedColor(255,128,0);
          break;  

        // Wednesday - Yellow
        case 4:
          setLedColor(255,255,0);
          break;

        // Thursday - Green
        case 5:
          setLedColor(0,255,0);
          break;  

        // Friday - Aqua
        case 6:
          setLedColor(0,255,255);
          break;

        // Saturday - Blue
        case 7:
          setLedColor(0,0,255);
          break;    
      }  
    }

    // show date
    if (ledState == 2) {
      setValues(day(t), month(t), (year(t) % 2000));

      // set led color to white
      setLedColor(255,255,255);
    }

    // show temperature
    if (ledState == 3) {
      setValues(0, (uint8_t)rtc.getTemperature(), (uint8_t)((rtc.getTemperature() - (uint8_t)rtc.getTemperature())*10));

      setLedColor(255,255,255);
    }

    //Serial.print(value[0]);
    //Serial.println();
    //Serial.print(value[1]);
    //Serial.println();
    //Serial.print(value[2]);
    //Serial.println();
    //Serial.print(value[3]);
    //Serial.println();
    //Serial.print(value[4]);
    //Serial.println();
    //Serial.print(value[5]);
    //Serial.println();

    uint8_t k = 0;

    for (uint8_t i = 0; i < 6; i++) {
    
      while (value[i] > 0) {
        binNumbers[k] = value[i] % 2;
        value[i] = value[i] >> 1;
        k++;
      }

      if (i == 0) {
        k = 4;
      }
      if (i == 1) {
        k = 7;
      }
      if (i == 2) {
       k = 11;
      }
      if (i == 3) {
        k = 14;
      }
      if (i == 4) {
        k = 18;
      }

    }

    for (uint8_t i = 0; i < 20; i++) {
      if (binNumbers[i] == 1) {
        pixels.setPixelColor(i, pixels.Color(ledColor[0], ledColor[1], ledColor[2])); // set led number and given color
        pixels.show();                                     // This sends the updated pixel color to the hardware.
      } else {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
        pixels.show();
      }
    }

  }

}

void setValues(uint8_t firstValue, uint8_t secondValue, uint8_t thirdValue) {
  value[0] = thirdValue % 10;
  value[1] = thirdValue / 10;
  value[2] = secondValue % 10;
  value[3] = secondValue / 10;
  value[4] = firstValue % 10;
  value[5] = firstValue / 10;
}

void setLedColor(uint8_t red, uint8_t green, uint8_t blue) {
  // Testwise, put the brightness to a quater
  ledColor[0] = red >> 3;
  ledColor[1] = green >> 3;
  ledColor[2] = blue >> 3;
}