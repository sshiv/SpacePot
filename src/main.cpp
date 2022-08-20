

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
#define PIN D2
#define PIN2 D3
const uint32_t TOTAL_LED_COUNT = 300;

#define MAX_POWER_MILLIAMPS 500
#define BRIGHTNESS 90
#define FRAMES_PER_SECOND  120

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

#ifdef FASTLED

String g_CurrentMode = "off";

CRGB leds1[TOTAL_LED_COUNT];
CRGB leds2[TOTAL_LED_COUNT];


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
    gCurrentPatternNumber = 1;
    Serial.println("LED on");
    digitalWrite(ONBOARD_LED_PIN,LOW); //LED is connected in reverse
 }
 else if (g_CurrentMode == "descend")
 {
    gCurrentPatternNumber = 2;
    Serial.println("LED on");
    digitalWrite(ONBOARD_LED_PIN,LOW); //LED is connected in reverse
 }
 else if (g_CurrentMode == "ground")
 {
    gCurrentPatternNumber = 3;
    Serial.println("LED on");
    digitalWrite(ONBOARD_LED_PIN,LOW); //LED is connected in reverse
 }
 else
 {
   gCurrentPatternNumber = 0;
    Serial.println("LED off");
    digitalWrite(ONBOARD_LED_PIN,HIGH); //LED off
 }
 
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");

}


void setup() { 

     Serial.begin(115200);
     Serial.println("Starting Serial");

  
      WiFi.begin(ssid, password);     //Connect to your WiFi router
      Serial.println("Connecting to Wi-Fi");

      pinMode(ONBOARD_LED_PIN, OUTPUT);
      digitalWrite(ONBOARD_LED_PIN,LOW);

      FastLED.addLeds<NEOPIXEL,PIN>(leds1,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN2>(leds2,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    
      FastLED.setBrightness(BRIGHTNESS);

      // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(ONBOARD_LED_PIN,LOW);
      delay(500);
      Serial.print(".");
      digitalWrite(ONBOARD_LED_PIN,HIGH);
    }
    digitalWrite(ONBOARD_LED_PIN,LOW);

    //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
      
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
  gPatterns[gCurrentPatternNumber](leds1, NUM_LEDS, gHue);
  gPatterns[gCurrentPatternNumber](leds2, NUM_LEDS, gHue);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#endif // FASTLED