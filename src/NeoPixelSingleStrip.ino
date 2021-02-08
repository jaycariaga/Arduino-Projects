// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PINa     6
#define LED_PINb    7
// How many NeoPixels are attached to the Arduino?
//#define LED_COUNT  60
#define LED_COUNT  3

// NeoPixel brightness, 0 (min) to 255 (max)
//#define BRIGHTNESS 100 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:

Adafruit_NeoPixel stripa(LED_COUNT, LED_PINa, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel stripb(LED_COUNT, LED_PINb, NEO_GRBW + NEO_KHZ800);

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

//from soundlvl file
const int sampleWindow = 40; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


void setup() {
  Serial.begin(9600);
  stripa.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  stripa.show();            // Turn OFF all pixels ASAP
  stripb.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  stripb.show();            // Turn OFF all pixels ASAP
  
}

void loop() {

//adding code from soundlvl to here to test out brightness changes
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 20;
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
   Serial.println(round(100*volts));

   stripa.setBrightness(round(100*volts));
   stripb.setBrightness(round(100*volts));

  
  // Fill along the length of the strip in various colors...
  //RainbowCycle(sampleWindow/1000, direction dir = FORWARD); // Green
  
  //colorWipe(stripa.Color(  255, 0,   0)     , sampleWindow/1000); // Green
  colorWipe(stripb.Color(  0, 255,   0)     , sampleWindow/1000); // Green
  
  //colorWipe(strip.Color(  0,   0, 255)     , 50); // Blue
  //colorWipe(strip.Color(  0,   0,   0, 255), 50); // True white (not RGB white)

  //whiteOverRainbow(75, 5);

  //pulseWhite(5);

  //rainbowFade2White(3, 3, 1);
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<stripa.numPixels(); i++) { // For each pixel in strip...
    stripa.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    stripa.show();                          //  Update strip to match
    stripb.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    stripb.show();                          //  Update strip to match
    //delay(wait);                           //  Pause for a moment; comment for less flickering experience
  }
}


//
//void pulseWhite(uint8_t wait) {
//  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
//    // Fill entire strip with white at gamma-corrected brightness level 'j':
//    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
//    strip.show();
//    delay(wait);
//  }
//
//  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
//    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
//    strip.show();
//    delay(wait);
//  }
//}
//
//void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
//  int fadeVal=0, fadeMax=100;
//
//  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
//  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
//  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
//  // advance around the wheel at a decent clip.
//  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
//    firstPixelHue += 256) {
//
//    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
//
//      // Offset pixel hue by an amount to make one full revolution of the
//      // color wheel (range of 65536) along the length of the strip
//      // (strip.numPixels() steps):
//      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
//
//      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
//      // optionally add saturation and value (brightness) (each 0 to 255).
//      // Here we're using just the three-argument variant, though the
//      // second value (saturation) is a constant 255.
//      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
//        255 * fadeVal / fadeMax)));
//    }
//
//    strip.show();
//    delay(wait);
//
//    if(firstPixelHue < 65536) {                              // First loop,
//      if(fadeVal < fadeMax) fadeVal++;                       // fade in
//    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
//      if(fadeVal > 0) fadeVal--;                             // fade out
//    } else {
//      fadeVal = fadeMax; // Interim loop, make sure fade is at max
//    }
//  }
//
//  for(int k=0; k<whiteLoops; k++) {
//    for(int j=0; j<256; j++) { // Ramp up 0 to 255
//      // Fill entire strip with white at gamma-corrected brightness level 'j':
//      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
//      strip.show();
//    }
//    delay(1000); // Pause 1 second
//    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
//      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
//      strip.show();
//    }
//  }
//
//  delay(500); // Pause 1/2 second
//}
