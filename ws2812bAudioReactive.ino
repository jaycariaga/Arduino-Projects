#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 60

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN. 
#define DATA_PIN 6
#define minima .11
#define slowness 2
static int huecount = 0;
//from soundlvl - senses amplitude of sound received
const int sampleWindow = 40; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  delay(1000); //safety delay
  Serial.begin(9600);
  //Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(0);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() { 

    //adding code from soundlvl to here to test out brightness changes
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 100; //make this higher to decrease sensitivity
   unsigned int signalMin = 1024;

   // collect data for 20 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

  // First slide the led in one direction
  if(volts > 1){
    volts = 1;
  }
  else if(volts < minima){
    volts = 0;
  }
   
   LEDS.setBrightness(round(volts*100));
  //Serial.println(round(volts*100));
  static uint8_t hue = 0;
  
  int ratio = round(volts*NUM_LEDS/2);
  for(int i = 0; i < ratio; i++) { //NUM_LEDS is: 60
    // if else block determines how fast the strip changes its color: slowness increase means longer time on one color hue
    if(huecount == slowness){
      hue++; huecount = 0;}
    else{
      huecount++;}
    leds[NUM_LEDS/2] = CHSV(hue, 255, 255);
    leds[NUM_LEDS/2-i] = CHSV(hue, 255, 255);
    leds[NUM_LEDS/2+i] = CHSV(hue, 255, 255);

    // Show the leds
    FastLED.show(); 
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(0);
  }

}
