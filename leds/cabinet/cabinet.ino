#include <FastLED.h>

#define NUM_LEDS 100

// data pins
#define DATA_PIN 3
#define BUTTON_PIN 9
#define POT_PIN A0

#define FRAMES_PER_SECOND 120

#define CYCLE_TIME 15000; // time between color cycles in ms

// array of LED data
CRGBArray<NUM_LEDS> leds{};

// warm white (or as warm as we can get with these LEDs)
CRGB HSV_WHITE(255, 147, 41);

// enum to track the different color modes
enum COLOR_MODE {WHITE, RAINBOW, CYCLE};

// state variable for the color mode
int color_mode = COLOR_MODE::WHITE;

// color state variables
float target_hue = 0;
float current_hue = 0;
static float cycle_delta = 32; // hue space between colors

unsigned long cycle_timer;
int N = 800; // number of steps to reach the next color

int brightness = 127;

// push button utility function
void press_wait(int pin) {
  // don't return until button has been released
  while (true) {
    if (digitalRead(pin) == LOW) { return; }
  }
}

// increment color toward the target color
void cycle_colors() {

  // if the color does not need changing, do nothing
  if (abs(current_hue - target_hue) < 0.1) { return; }

  float delta = cycle_delta / N;
  
  // take one step in color change
  current_hue += delta;
  
  leds.fill_solid(CHSV(current_hue, 255, brightness));
  FastLED.show(current_hue);
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

// updates the target hue based on a timer
void update_cycle_hue() {
  bool next_color = (millis() - cycle_timer) > CYCLE_TIME;
  if (next_color) {
    cycle_timer = millis();
    target_hue += cycle_delta;
    if (target_hue > 255) {
      target_hue -= 255;
      current_hue -= 255;
    }
  }  
}

// update color mode based on the "color_mode" state variable
void switch_color_modes() {
  switch(color_mode) {
    case COLOR_MODE::WHITE:
      leds.fill_solid(HSV_WHITE);
      FastLED.show();  
      break;
    case COLOR_MODE::RAINBOW:
      leds.fill_rainbow(0);
      FastLED.show();
      break;
    case COLOR_MODE::CYCLE:
      current_hue = target_hue;
      cycle_timer = millis();
      leds.fill_solid(CHSV(current_hue, 255, brightness));
      FastLED.show();
      FastLED.delay(1000/FRAMES_PER_SECOND);
      break;
  }
}

void update_brightness() {
  int pot_value = analogRead(POT_PIN);
  brightness = map(pot_value, 0, 1024, 0, 255);
  FastLED.setBrightness(brightness);
  FastLED.show();
}


// make changes to colors 
void update_colors() {
  switch(color_mode) {
    case COLOR_MODE::WHITE:
      break;
    case COLOR_MODE::RAINBOW:
      break;
    case COLOR_MODE::CYCLE:
      cycle_colors();
      update_cycle_hue();
      break;
  }  

  // always update brightness
  update_brightness();
}

void setup() { 
  // turn on toggle button pin
  pinMode(BUTTON_PIN, INPUT); 
  
  // register LEDs with libarary
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  // start with all LEDs off
  leds.fill_solid(CRGB::Black);
  FastLED.show();

  // initialize the default color mode
  switch_color_modes();
}

void loop() { 
  if (digitalRead(BUTTON_PIN) == HIGH) {
    press_wait(BUTTON_PIN);
    // toggle state variable
    if (++color_mode > COLOR_MODE::CYCLE) { color_mode = COLOR_MODE::WHITE; }
    switch_color_modes();
  }

  update_colors();
}
