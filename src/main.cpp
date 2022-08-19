

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixelBus.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include "FastLedPatterns.h"

//#define NEOPIXELBUS
#define FASTLED

#define LED 2

#define PIN D2
#define PIN2 D3
const uint32_t TOTAL_LED_COUNT = 300;

#define MAX_POWER_MILLIAMPS 500
#define BRIGHTNESS 90
#define FRAMES_PER_SECOND  120


#ifdef FASTLED

CRGB leds1[TOTAL_LED_COUNT];
CRGB leds2[TOTAL_LED_COUNT];

void setup() { 
      FastLED.addLeds<NEOPIXEL,PIN>(leds1,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN2>(leds2,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    
      FastLED.setBrightness(BRIGHTNESS);
      //FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
   }

const uint32_t NUM_LEDS = TOTAL_LED_COUNT;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])(CRGB (&leds)[], uint32_t numLeds, uint8_t hue);
SimplePatternList gPatterns = {sinelon, juggle, bpm,  simpletrain };

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber](leds1, NUM_LEDS, gHue);
  gPatterns[gCurrentPatternNumber](leds2, NUM_LEDS, gHue);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#endif // FASTLED