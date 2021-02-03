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
const int ledstrips = 2; //existing led strips
const int pixelPin[ledstrips] = {6,7};
Adafruit_NeoPixel strip[ledstrips];

//Adafruit_NeoPixel stripa(LED_COUNT, LED_PINa, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel stripb(LED_COUNT, LED_PINb, NEO_GRBW + NEO_KHZ800);

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

//from soundlvl file
const int sampleWindow = 30; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


void setup() {
  for ( byte i = 0; i < ledstrips; i++ ){
    strip[ i ]= Adafruit_NeoPixel(LED_COUNT, pixelPin[ i ], NEO_GRBW + NEO_KHZ800);
    strip[i].begin();
    strip[i].show();
  }
  Serial.begin(9600);
//  stripa.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//  stripa.show();            // Turn OFF all pixels ASAP
//  stripb.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//  stripb.show();            // Turn OFF all pixels ASAP
  
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

  for ( int i = 0; i < ledstrips; i++ ){
       strip[i].setBrightness(round(100*volts));
  }
  
  colorWipe(0,strip[0].Color(  0, 255,   0)     , sampleWindow/1000); // Green
  colorWipe(1,strip[1].Color(  255, 0,   0)     , sampleWindow/1000); // Green
  
  //colorWipe(strip.Color(  0,   0, 255)     , 50); // Blue
  //colorWipe(strip.Color(  0,   0,   0, 255), 50); // True white (not RGB white)
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(int j, uint32_t color, int wait) {
  for(int i=0; i<strip[j].numPixels(); i++) { // For each pixel in strip...
    strip[j].setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip[j].show();                          //  Update strip to match
    //delay(wait);                           //  Pause for a moment
  }

}
