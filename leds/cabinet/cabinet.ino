#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 10

#define DATA_PIN 22

#define FRAMES_PER_SECOND 120

CRGBArray<NUM_LEDS> leds{};

void setup() { 
     FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
     leds.fill_solid(CRGB::Black);
     FastLED.show();
}

CHSV HSV_WHITE(56.09, 68.05, 66.27);


void warm_white() {
  leds.fill_solid(HSV_WHITE);
  FastLED.show();  
}


void rainbow() {
  leds.fill_rainbow(0);
  FastLED.show();
}

float cycle_hue = 0;
float hue_delta = 32;
int N = 800;

void cycle_colors() {


  float new_hue = cycle_hue + hue_delta;
  float delta = (new_hue - cycle_hue) / N;
  
  // fade in
  for(int i = 0; i < N; i++) {
    leds.fill_solid(CHSV(cycle_hue, 255, 127));
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
    cycle_hue += delta;
  }
}

enum COLOR_MODE {WARM_WHITE, RAINBOW, CYCLE};

int mode = COLOR_MODE::CYCLE;

void loop() { 
  
  switch(mode) {
    case COLOR_MODE::WARM_WHITE:
      warm_white();
      break;
    case COLOR_MODE::RAINBOW:
      rainbow();
      break;
    case COLOR_MODE::CYCLE:
      cycle_colors();
      break;
  }  
  delay(7000);
}
