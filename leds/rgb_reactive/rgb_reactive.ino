
// https://youtu.be/6g7AUs74dV8
#include <FastLED.h>


// rgb global values
int r=152;
int g=0;
int b=10;

// Pin channel definitions
#define LED_PIN 22
#define NUM_LEDS 100
#define MIC_PIN A1

// Allocate color value arrays
CRGB leds[NUM_LEDS];
CRGB led[NUM_LEDS];

// constant of convenience
const int HALF_LEDS = NUM_LEDS / 2;

void setup() {
  // Indicate to library the type and number of LEDs
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  // initial display
  for (int i = HALF_LEDS; i >= 0; i--)
  {
     leds[i] = CRGB(r,g,b);
     leds[NUM_LEDS-i] = CRGB(r,g,b);
     delay(40);
    FastLED.show();
  }
  Serial.begin(9600);

  // turn on input MIC channel
  pinMode(MIC_PIN,INPUT);
}

void loop()
{
  int s = 2 * analogRead(MIC_PIN);

  if((s>=450) && (s<=550))
  {
    leds[(HALF_LEDS)-1] = CRGB(0, 0, 255);
    leds[HALF_LEDS] = CRGB(0, 0, 255);
  }
  else if( (s>=400) && (s<=450))
  {
    leds[(HALF_LEDS)-1] = CRGB(153, 153, 0);
    leds[HALF_LEDS] = CRGB(153, 153, 0);
  }
  else if((s>=350) && (s<=400))
   {
    leds[(HALF_LEDS)-1] = CRGB(255, 50, 255);
    leds[HALF_LEDS] = CRGB(255, 50, 255);
   }
  else if((s>=300) && (s<=350))
  {
    leds[(HALF_LEDS)-1] = CRGB(10, 25, 217);
    leds[HALF_LEDS] = CRGB(10, 25, 217);
  }
  else if((s>=276) && (s<=300))
   {
    leds[(HALF_LEDS)-1] = CRGB(50, 50, 150);
    leds[HALF_LEDS] = CRGB(50, 50, 150);
   }
   else if((s>=250) && (s<=275))
   {
    leds[(HALF_LEDS)-1 ] = CRGB(230, 0, 10);
    leds[HALF_LEDS] = CRGB(230, 0, 10);
   }
  else if((s>=235) && (s<=250))
   {
    leds[(HALF_LEDS)-1] = CRGB(0, 160, 0);
    leds[HALF_LEDS] = CRGB(0, 160, 0);
   }
   else if((s>=200)&&(s<=230))
   {
    leds[(HALF_LEDS)-1] = CRGB (1, 0, 1);
    leds[HALF_LEDS] = CRGB (1, 0, 1);
   }
  else
  {
    leds[(HALF_LEDS)-1] = CRGB(r, s-100, b);
    leds[HALF_LEDS] = CRGB(r, s-100, b);
  }

  // propagate colors down the LEDs
  for (int i = 0; i <= ((HALF_LEDS)-2); i++)
  {
    leds[i] = leds[i+1];
    leds[NUM_LEDS-1-i] = leds[(NUM_LEDS)-i-2];
  }

 FastLED.show();
 delay(25);
}
