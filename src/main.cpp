

#include <Arduino.h>
//#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixelBus.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>


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

const uint32_t TOTAL_LED_COUNT = 300;

#define MAX_POWER_MILLIAMPS 500
#define BRIGHTNESS 90
#define FRAMES_PER_SECOND  60

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80
ESP8266HTTPUpdateServer httpUpdater;

#ifdef FASTLED

String g_CurrentMode = "off";

CRGB top_leds0[TOTAL_LED_COUNT];
CRGB top_leds1[TOTAL_LED_COUNT];

CRGB mid_leds2[TOTAL_LED_COUNT];
CRGB mid_leds3[TOTAL_LED_COUNT];

CRGB bottom_leds4[TOTAL_LED_COUNT];
CRGB bottom_leds5[TOTAL_LED_COUNT];


const uint32_t NUM_ACTIVE_LEDS = TOTAL_LED_COUNT;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t g_TopCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t g_MidCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t g_BottomCurrentPatternNumber = 0; // Index number of which pattern is current



uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t g_TopHue = 0; // rotating "base color" used by many of the patterns
uint8_t g_MidHue = 0; // rotating "base color" used by many of the patterns
uint8_t g_BottomHue = 0; // rotating "base color" used by many of the patterns




void handleRoot() {
 Serial.println("Reloading root page");
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
    g_TopCurrentPatternNumber = PatternEnum::eOceanBPM;
    g_TopHue = gCurrentPatternNumber;

    g_MidCurrentPatternNumber = PatternEnum::eJuggle;
    g_MidHue = gCurrentPatternNumber;
    
    g_BottomCurrentPatternNumber = PatternEnum::eHeatBPM;
    g_BottomHue = HSVHue::HUE_RED;

 }
 else if (g_CurrentMode == "descend")
 {
    g_TopCurrentPatternNumber = PatternEnum::eOceanBPM;
    g_TopHue = gCurrentPatternNumber;

    g_MidCurrentPatternNumber = PatternEnum::ePartyBPM;
    g_MidHue = gCurrentPatternNumber;
    
    g_BottomCurrentPatternNumber = PatternEnum::eHeatBPM;
    g_BottomHue = gCurrentPatternNumber;
 }
 else if (g_CurrentMode == "ground")
 {
    g_TopCurrentPatternNumber = PatternEnum::ePartyBPM;
    g_TopHue = HSVHue::HUE_AQUA;

    g_MidCurrentPatternNumber = PatternEnum::eLavaBPM;
    g_MidHue = HSVHue::HUE_YELLOW;
    
    g_BottomCurrentPatternNumber = PatternEnum::eHeatBPM;
    g_BottomHue = HSVHue::HUE_RED;
    
 }
 else if (g_CurrentMode == "setadvanced")
 {
    g_BottomCurrentPatternNumber = PatternEnum::eSimpleTrain;
    g_BottomHue = HSVHue::HUE_RED;

    g_MidCurrentPatternNumber = PatternEnum::eLavaBPM;
    g_MidHue = HSVHue::HUE_YELLOW;

    String advmode = server.arg("advmode");
    Serial.print("Got Advanced Mode : ");Serial.println(advmode);
    g_LedLutIndex = ((12 - advmode.toInt()) + 8) % 12;

    g_LedLutIndex = g_LedLutIndex >= 12 ? 0 : g_LedLutIndex;


    
 }
 else
 {
    // off
    g_TopCurrentPatternNumber = PatternEnum::eAllOff;
    g_MidCurrentPatternNumber = PatternEnum::eAllOff;
    g_BottomCurrentPatternNumber = PatternEnum::eAllOff;
 }
 
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");

}


void setup() { 

      pinMode(ONBOARD_LED_PIN, OUTPUT);
      digitalWrite(ONBOARD_LED_PIN,LOW);

     Serial.begin(115200);
     Serial.println("Starting Serial");


      FastLED.addLeds<NEOPIXEL,PIN0>(top_leds0,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN1>(top_leds1,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;

	    FastLED.addLeds<NEOPIXEL,PIN2>(mid_leds2,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN3>(mid_leds3,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;

	    FastLED.addLeds<NEOPIXEL,PIN4>(bottom_leds4,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    FastLED.addLeds<NEOPIXEL,PIN5>(bottom_leds5,TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);;
	    
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

    MDNS.begin(c_DNS_NAME);

    httpUpdater.setup(&server);


    server.begin();                  //Start server

      MDNS.addService("http", "tcp", 80);

    Serial.println("HTTP server started");
      
      //FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);

      // Enable Arduino OTA
      //ArduinoOTA.begin();

   }

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void loop()
{
   server.handleClient();

  // Handle OTA and flash it
  //ArduinoOTA.handle();

 

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[g_TopCurrentPatternNumber](top_leds0, NUM_ACTIVE_LEDS, g_TopHue);
  gPatterns[g_TopCurrentPatternNumber](top_leds1, NUM_ACTIVE_LEDS, g_TopHue);
  
  gPatterns[g_MidCurrentPatternNumber](mid_leds2, NUM_ACTIVE_LEDS, g_MidHue);
  gPatterns[g_MidCurrentPatternNumber](mid_leds3, NUM_ACTIVE_LEDS, g_MidHue);
  
  gPatterns[g_BottomCurrentPatternNumber](bottom_leds4, NUM_ACTIVE_LEDS, g_BottomHue);
  gPatterns[g_BottomCurrentPatternNumber](bottom_leds5, NUM_ACTIVE_LEDS, g_BottomHue);

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
        digitalWrite(ONBOARD_LED_PIN, !digitalRead(ONBOARD_LED_PIN));   // Turn the LED on by making the voltage LOW
    }
    else
    {
        digitalWrite(ONBOARD_LED_PIN, HIGH);
    }
  }
}

#endif // FASTLED