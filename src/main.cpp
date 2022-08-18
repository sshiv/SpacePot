

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixelBus.h>
#include <FastLED.h>

//#define NEOPIXELBUS
#define FASTLED

#define LED 2

#define PIN D2
const uint32_t TOTAL_LED_COUNT = 64;

#ifdef NEOPIXEL
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel _RedStripBottom = Adafruit_NeoPixel(TOTAL_LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
//const uint32_t VEL_COLOR = strip.Color(139, 69, 19);
//const uint32_t ASURAN_COLOR = _RedStripBottom.Color(255, 0, 0);
#endif

#ifdef NEOPIXELBUS
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(TOTAL_LED_COUNT, PIN);
#define colorSaturation 128

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

#endif

#ifdef NEOPIXEL

struct ColorInfo
{
  uint32_t Red;
  uint32_t Green;
  uint32_t Blue;
};

struct StripInfo
{
  Adafruit_NeoPixel Strip;
  ColorInfo Color;
  uint32_t Start;
  uint32_t End;
  uint32_t Step;
  uint32_t Size;
  uint32_t UpdateFrequency;
  uint32_t Current;
  
};

StripInfo _EngineBottom = {_RedStripBottom, // Strip
                             {0, 0, 255},   // Color
                              1,            // Start
                              44,           // End
                              2,            // Step
                              4,            // Size
                              10,           // UpdateFrequency
                              1,            // Current
                              };


void UpdateStrip(StripInfo& stripInfo)
{
  for (uint32_t i = stripInfo.Current; i < (stripInfo.Current + stripInfo.Size) && i < stripInfo.End; i++)
  {
    stripInfo.Strip.setPixelColor(i, stripInfo.Color.Red, stripInfo.Color.Green, stripInfo.Color.Blue); 
  } 

  stripInfo.Current = stripInfo.Current + stripInfo.Size + stripInfo.Step;
  if (stripInfo.Current >= stripInfo.End)
  {
    stripInfo.Current = stripInfo.Start;
  }

}


//=======================================================================
void ICACHE_RAM_ATTR onTimerISR(){
    
    UpdateStrip(_EngineBottom);
    _EngineBottom.Strip.show();
    timer1_write(600000);//12us
    Serial.println("In onTimerISR");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  #ifdef NEOPIXEL
  _EngineBottom.Strip.begin();

  UpdateStrip(_EngineBottom);
  _EngineBottom.Strip.show();
  #endif

  #ifdef NEOPIXELBUS
  strip.Begin();
  strip.Show();
  #endif


  //FastLED.addLeds<WS2811, 
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED, HIGH);
  //Serial.println("LED is on");
  //_EngineBottom.Strip.setPixelColor(20, 255, 0, 0);
  //_EngineBottom.Strip.show();
  //delay(1000);
  //digitalWrite(LED, LOW);
  //_EngineBottom.Strip.setPixelColor(20, 0);
  
  //Serial.println("LED is off");
  #ifdef NEOPIXEL
  UpdateStrip(_EngineBottom);

  _EngineBottom.Strip.show();
  #endif
  //delay(100);
}
#endif  // NEOPIXEL


#ifdef NEOPIXELBUS
void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();


    Serial.println();
    Serial.println("Running...");
}


void loop()
{
    delay(5000);

    Serial.println("Colors R, G, B, W...");

    // set the colors, 
    // if they don't match in order, you need to use NeoGrbFeature feature
    strip.SetPixelColor(0, red);
    strip.SetPixelColor(1, green);
    strip.SetPixelColor(2, blue);
    strip.SetPixelColor(3, white);
    // the following line demonstrates rgbw color support
    // if the NeoPixels are rgbw types the following line will compile
    // if the NeoPixels are anything else, the following line will give an error
    //strip.SetPixelColor(3, RgbwColor(colorSaturation));
    strip.Show();


    delay(5000);

    Serial.println("Off ...");

    // turn off the pixels
    strip.SetPixelColor(0, black);
    strip.SetPixelColor(1, black);
    strip.SetPixelColor(2, black);
    strip.SetPixelColor(3, black);
    strip.Show();

    delay(5000);

    Serial.println("HSL Colors R, G, B, W...");

    // set the colors, 
    // if they don't match in order, you may need to use NeoGrbFeature feature
    strip.SetPixelColor(0, hslRed);
    strip.SetPixelColor(1, hslGreen);
    strip.SetPixelColor(2, hslBlue);
    strip.SetPixelColor(3, hslWhite);
    strip.Show();


    delay(5000);

    Serial.println("Off again...");

    // turn off the pixels
    strip.SetPixelColor(0, hslBlack);
    strip.SetPixelColor(1, hslBlack);
    strip.SetPixelColor(2, hslBlack);
    strip.SetPixelColor(3, hslBlack);
    strip.Show();

}
#endif //NEOPIXELBUS

#ifdef FASTLED

CRGB leds[TOTAL_LED_COUNT];

void setup() { 
       FastLED.addLeds<NEOPIXEL,PIN>(leds,TOTAL_LED_COUNT);
	    FastLED.setBrightness(84);
   }

const uint32_t NUM_LEDS = TOTAL_LED_COUNT;
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() { 
	static uint8_t hue = 0;
  // First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i+=2) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show(); 
		// now that we've shown the leds, reset the i'th led to black
		//leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
	Serial.print("x");

	// Now go in the other direction.  
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 100);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
  
}
#endif // FASTLED