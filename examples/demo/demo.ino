#include <TM1637.h>
#include <math.h>

#define CLK 2
#define DIO 3

uint8_t hello_world[] = { 0x00, 0x00, 0x00, 0x00, 0x76, 0x79, 0x38, 0x38, 0x3f, 0x00, 0x3e, 0x3f, 0x50, 0x38, 0x5e, 0x00, 0x00, 0x00, 0x00 };
uint8_t hello_world_length = 19;

uint8_t bye[] = { 0x7c, 0x6e, 0x79, 0x00 };
uint8_t bye_length = 4;

TM1637Display display(CLK, DIO);

void setup() {
}

void loop() {
  // RAW segment display demo
  for (byte i = 0; i < hello_world_length - 4; i++) {
    display.display_raw(hello_world + i, 4);
    delay(500);
  }

  // Integer display demo
  for (int number = -1999; number < 10000; number++) {
    display.display_number(number);
    
    int increment_delay;

    if (number <= -1990) {
      increment_delay = 100;
    } else if (number <= -1900) {
      increment_delay = 10;
    } else if (number <= 9900) {
      increment_delay = 0;
    } else if (number <= 9990) {
      increment_delay = 10;
    } else {
      increment_delay = 100;
    }

    if (number >= -1900 && number < 9900) {
      number += 9;
    }
    
    delay(increment_delay);
  }

  delay(1000);

  // Decimal display demo
  float decimal_number = 1.0f;
  float decimal_increment = 1.0f;
  uint8_t decimal_places = 0;

  while (decimal_number < 10) {
    display.display_number(decimal_number, decimal_places);

    if (fabs(decimal_number - 9.99) < 0.001) {
      decimal_increment = 0.001f;
      decimal_places = 3;
    } else if (fabs(decimal_number - 9.9) < 0.001) {
      decimal_increment = 0.01f;
      decimal_places = 2;
    } else if (fabs(decimal_number - 9) < 0.001) {
      decimal_increment = 0.1f;
      decimal_places = 1;
    }
    
    decimal_number += decimal_increment;
    delay(100);
  }

  delay(1000);

  // clock display and display control demo
  int milliseconds_left = 20000;
  unsigned long previous_time = millis();
  unsigned int toggle_period = 0;
  unsigned int toggle_period_elapsed = 0;
  bool enabled = true;

  while (milliseconds_left >= 0) {
    display.display_clock(milliseconds_left / 1000, (milliseconds_left / 10) % 100);

    unsigned int elapsed = millis() - previous_time;

    if (milliseconds_left <= 5000) {
      toggle_period = 100;
    } else if (milliseconds_left <= 10000) {
      toggle_period = 200;
    }

    if (toggle_period > 0 && toggle_period_elapsed >= toggle_period) {
      enabled = !enabled;
      toggle_period_elapsed = 0;

      display.switch_display(enabled);
    } else if (toggle_period > 0) {
      toggle_period_elapsed += elapsed;
    }
    
    milliseconds_left -= elapsed;
    previous_time = millis();
  }

  display.switch_display(true);
  display.display_clock(0, 0);

  delay(2000);

  display.display_raw(bye, bye_length);

  for (int brightness = 7; brightness >= 0; brightness--) {
    display.set_brightness(brightness);
    delay(200);
  }

  display.switch_display(false);
  
  delay(10000);
  
  display.switch_display(true);
  display.set_brightness(2);
}
