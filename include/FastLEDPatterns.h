#ifndef FASTLEDPATTERNS_H
#define FASTLEDPATTERNS_H

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>


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


void simpletrain(CRGB (&leds)[],  uint32_t numLeds, uint8_t hue)
{
  	fadeToBlackBy( leds, numLeds, 40);
		
    uint8_t i = beat16(3000);

		// Set the i'th led to red
		leds[i] = CHSV(255, 255, 192);
		leds[i+10] = CHSV(255, 255, 192);
		leds[i+20] = CHSV(255, 255, 192);
		leds[i+30] = CHSV(255, 255, 192);
}

void PulseColorsFromPalette(CRGB (&leds)[], uint32_t numLeds, uint8_t hue, const CRGBPalette16& palette)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  //CRGBPalette16 palette = LavaColors_p ;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < numLeds; i++) { //9948
    leds[i] = ColorFromPalette(palette, hue+(i*2), beat-hue+(i*10));
  }
}

void PartyBPM(CRGB (&leds)[], uint32_t numLeds, uint8_t hue)
{
  PulseColorsFromPalette(leds, numLeds, hue, PartyColors_p);
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
  eSimpleTrain,
  eRainbow,
  eConfetti
};

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])(CRGB (&leds)[], uint32_t numLeds, uint8_t hue);
SimplePatternList gPatterns = {AllOff, sinelon, juggle, PartyBPM, OceanBPM, LavaBPM, HeatBPM,  simpletrain, rainbowWithGlitter, confetti };


#endif //FASTLEDPATTERNS_H
