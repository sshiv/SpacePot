#ifndef FASTLEDPATTERNS_H
#define FASTLEDPATTERNS_H

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

uint32_t g_LedLutIndex = 0;


void rainbow(CRGB (&leds)[], uint32_t numLeds, uint8_t hue) 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, numLeds, hue, 7);
}

void addGlitter(CRGB (&leds)[], uint32_t numLeds, uint8_t hue, fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(numLeds) ] += CRGB::White;
  }
}

void rainbowWithGlitter(CRGB (&leds)[], uint32_t numLeds, uint8_t hue) 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow(leds, numLeds, hue);
  addGlitter(leds, numLeds, hue, 80);
}

void confetti(CRGB (&leds)[], uint32_t numLeds, uint8_t hue) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, numLeds, 10);
  int pos = random16(numLeds);
  leds[pos] += CHSV( hue + random8(64), 200, 255);
}

void sinelon(CRGB (&leds)[],  uint32_t numLeds, uint8_t hue)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, numLeds, 20);
  int pos = beatsin16( 13, 0, numLeds-1 );
  leds[pos] += CHSV(hue, 255, 192);
}


void simpletrain1(CRGB (&leds)[],  uint32_t numLeds, uint8_t hue)
{
    const int c_TrainSize = 45;
    static int startIndex = 0;
  	fadeToBlackBy( leds, numLeds, 40);
		
    for (int i = 0; i < c_TrainSize; ++i)
    {
      leds[startIndex + i] += CRGB::White;
    }
    startIndex = (startIndex + c_TrainSize) % numLeds;
}

struct LEDTrainData
{
  uint32_t StartIndex;
  uint32_t EndIndex;
  uint32_t Step;
};

LEDTrainData c_LED_LUT[] = {
            {16, 60, 1}, //0
            {60, 109, 1}, //1
            {109, 157, 1}, //2
            {157, 204, 1}, //3
            {204, 253, 1}, //4
            {253, 300, 1}, //5
            {300, 345, 1}, //6
            {345, 390, 1}, //7
            {390, 443, 1}, //8
            {441, 488, 1}, //9
            {488, 538, 1},
            {538, 600, 1}
};

void simpletrain(CRGB (&leds)[],  uint32_t numLeds, uint8_t hue)
{
    fadeToBlackBy( leds, numLeds, 40);
		
    uint32_t lutIndex = g_LedLutIndex;

    if (lutIndex >= sizeof(c_LED_LUT) / sizeof(c_LED_LUT[0]))
    {
      return;
    }

    for (uint32_t i = c_LED_LUT[lutIndex].StartIndex; i < c_LED_LUT[lutIndex].EndIndex; i+=c_LED_LUT[lutIndex].Step)
    {
      leds[i] = CRGB::White;
    }
}

void PulseColorsFromPalette(CRGB (&leds)[], uint32_t numLeds, uint8_t hue, const CRGBPalette16& palette)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  //CRGBPalette16 palette = LavaColors_p ;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( uint32_t i = 0; i < numLeds; i++) { //9948
    leds[i] = ColorFromPalette(palette, hue+(i*2), beat-hue+(i*10));
  }
}

void PartyBPM(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  PulseColorsFromPalette(leds, numLeds, hue, PartyColors_p);
  simpletrain(leds, numLeds, hue);
}

void LavaBPM(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  PulseColorsFromPalette(leds, numLeds, hue, LavaColors_p);
}

void OceanBPM(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  PulseColorsFromPalette(leds, numLeds, hue, OceanColors_p);
}

void HeatBPM(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  PulseColorsFromPalette(leds, numLeds, hue, HeatColors_p );
}

void CloudBPM(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  PulseColorsFromPalette(leds, numLeds, hue, CloudColors_p );
}


void juggle(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, numLeds, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, numLeds-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void AllOff(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  fill_solid(leds, numLeds, CRGB::Black);
}

enum PatternEnum
{
  eAllOff = 0,
  eSineLon,
  eJuggle,
  ePartyBPM,
  eOceanBPM,
  eLavaBPM,
  eHeatBPM,
  eCloudBPM,
  eSimpleTrain,
  eRainbow,
  eConfetti
};

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])(CRGB (&leds)[], uint32_t numLeds, uint8_t hue);
SimplePatternList gPatterns = {AllOff, sinelon, juggle, PartyBPM, OceanBPM, LavaBPM, HeatBPM, CloudBPM, simpletrain, rainbowWithGlitter, confetti };


#endif //FASTLEDPATTERNS_H
