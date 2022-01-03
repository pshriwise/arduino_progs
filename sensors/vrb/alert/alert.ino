#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Pin definitions
#define DATA_PIN 22
#define PIR_PIN 24
#define VRB_IN A0
#define LED_PIN LED_BUILTIN

// LED parameters
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS  100
#define BRIGHTNESS 30
#define FRAMES_PER_SECOND  120

// Array of colors for the LEDs
CRGB leds[NUM_LEDS];

// rotating "base color" used by the rainbow pattern
uint8_t gHue = 0;

// state variables for the vibration sensor
int present_condition = 0;
int previous_condition = 0;

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for(int i = 0; i < 8; i++) {
    leds[beatsin16(i+7, 0, NUM_LEDS-1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

// lights up the LED strip
void light_up() {
  // call the lighting pattern
  rainbow();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  delay(3000);
}

void setup() {
  delay(3000);
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(VRB_IN, INPUT);

    // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  FastLED.clear();
  FastLED.show();
}

void loop() {
  // slowly cycle the "base color" through the rainbow
  // (runs concurrent with loop)
  EVERY_N_MILLISECONDS(20) {gHue++;}

  // update state variables
  previous_condition = present_condition;
  present_condition = digitalRead(VRB_IN);

  // light LEDs if virbation sensor state has changed
  // or the PIR sensor is tripped
  if (previous_condition != present_condition || digitalRead(PIR_PIN) == HIGH) {
      light_up();
  } else {
    FastLED.clear();
    FastLED.show();
  }
}

