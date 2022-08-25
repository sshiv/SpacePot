

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixelBus.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

#include "FastLedPatterns.h"
#include "WebServer.h"

//#define NEOPIXELBUS
#define FASTLED

#define LED 2

#define ONBOARD_LED_PIN 2 
#define PIN0 D0
#define PIN1 D1

#define PIN2 D4
#define PIN3 D5

#define PIN4 D7
#define PIN5 D8

const uint32_t TOTAL_LED_COUNT = 30;

#define MAX_POWER_MILLIAMPS 500
#define BRIGHTNESS 90
#define FRAMES_PER_SECOND  120

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80


#ifdef FASTLED

String g_CurrentMode = "off";

CRGB leds0[TOTAL_LED_COUNT];
CRGB leds1[TOTAL_LED_COUNT];

CRGB leds2[TOTAL_LED_COUNT];
CRGB leds3[TOTAL_LED_COUNT];

CRGB leds4[TOTAL_LED_COUNT];
CRGB leds5[TOTAL_LED_COUNT];


const uint32_t NUM_LEDS = TOTAL_LED_COUNT;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])(CRGB (&leds)[], uint32_t numLeds, uint8_t hue);
SimplePatternList gPatterns = {AllOff, sinelon, juggle, bpm,  simpletrain };


void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page;
 s.replace("$MODE$", g_CurrentMode);
 server.send(200, "text/html", s); //Send web page
}


void handleForm() { 
  Serial.println("In HandleForm");
  g_CurrentMode = server.arg("mode");
 Serial.print("Got Mode : ");Serial.println(g_CurrentMode);

 if (g_CurrentMode == "ascend")
 {
    gCurrentPatternNumber = 2;
 }
 else if (g_CurrentMode == "descend")
 {
    gCurrentPatternNumber = 3;
 }
 else if (g_CurrentMode == "ground")
 {
    gCurrentPatternNumber = 4;
 }
 else
 {
  // off
   gCurrentPatternNumber = 0;
 }
 
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");

}


void setup() { 

      pinMode(ONBOARD_LED_PIN, OUTPUT);
      digitalWrite(ONBOARD_LED_PIN,LOW);

     Serial.begin(115200);
     Serial.println("Starting Serial");


      FastLED.addLeds<NEOPIXEL,PIN0>(leds0,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN1>(leds1,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;

	    FastLED.addLeds<NEOPIXEL,PIN2>(leds2,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN3>(leds3,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;

	    FastLED.addLeds<NEOPIXEL,PIN4>(leds4,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN5>(leds5,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    
      FastLED.setBrightness(BRIGHTNESS);

      if (c_ENABLE_AS_ACCESS_POINT)
      {
        Serial.println("Initializing as AP...");
        WiFi.softAP(ssid, password);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);

        Serial.print("AP Local IP address: ");
        Serial.println(WiFi.localIP());
        
      }
      else
      {
        WiFi.begin(ssid, password);     //Connect to your WiFi router
        Serial.println("Connecting to Wi-Fi");

          // Wait for connection
        while (WiFi.status() != WL_CONNECTED) {
          digitalWrite(ONBOARD_LED_PIN,LOW);
          delay(500);
          Serial.print(".");
          digitalWrite(ONBOARD_LED_PIN,HIGH);
        }
      
      //If connection successful show IP address in serial monitor
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());  //IP address assigned to your ESP
      }

      digitalWrite(ONBOARD_LED_PIN,LOW);
      
    server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
    server.on("/action", handleForm); 

  server.begin();                  //Start server
  Serial.println("HTTP server started");
      
      //FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);

   }

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void loop()
{
  server.handleClient();

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber](leds0, NUM_LEDS, gHue);
  gPatterns[gCurrentPatternNumber](leds1, NUM_LEDS, gHue);
  gPatterns[gCurrentPatternNumber](leds2, NUM_LEDS, gHue);
  gPatterns[gCurrentPatternNumber](leds3, NUM_LEDS, gHue);
  gPatterns[gCurrentPatternNumber](leds4, NUM_LEDS, gHue);
  gPatterns[gCurrentPatternNumber](leds5, NUM_LEDS, gHue);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

  EVERY_N_MILLISECONDS( 1000 ) {
    // heartbeat 
    if (WiFi.status() == WL_CONNECTED)
    {
       Serial.println("WiFi Still connected");
        digitalWrite(ONBOARD_LED_PIN, !digitalRead(ONBOARD_LED_PIN));   // Turn the LED on by making the voltage LOW
    }
    else
    {
        digitalWrite(ONBOARD_LED_PIN, HIGH);
    }
  }
}

#endif // FASTLED