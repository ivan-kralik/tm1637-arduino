#include "TM1637.h"
#include <Arduino.h>

const uint8_t TM1637Display::digit_tab[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 }; //0~9, A, b, C, d, E, F

TM1637Display::TM1637Display(uint8_t clk_pin, uint8_t data_pin): m_clk_pin(clk_pin), m_data_pin(data_pin)
{
  pinMode(m_clk_pin,OUTPUT);
  pinMode(m_data_pin,OUTPUT);
  digitalWrite(m_clk_pin,HIGH);
  digitalWrite(m_data_pin,HIGH);
}

void TM1637Display::set_brightness(uint8_t brightness)
{
  m_brightness = brightness;

  update_display_control();
}

void TM1637Display::switch_display(bool enabled)
{
  m_enabled = enabled;

  update_display_control();
}

void TM1637Display::display_number(int number)
{
  display_number(number, 0);
}

void TM1637Display::display_number(float number, uint8_t decimal_places)
{
  for (byte i = 0; i < decimal_places; i++) {
    number *= 10;
  }

  display_number((int) number, decimal_places);
}

void TM1637Display::display_clock(uint8_t hour, uint8_t minute)
{
  uint8_t digits = 4;
  uint8_t encoded_data[digits];

  int number = hour * 100 + minute;

  encode_number(number, 2, encoded_data, digits);
  display_raw(encoded_data, digits);
}

void TM1637Display::display_raw(uint8_t data[], uint8_t data_size, uint8_t start_pos)
{
  start();
  write_byte(TM1637_ADDR_AUTO);
  stop();

  start();
  write_byte(TM1637_STARTADDR | (start_pos & 0x07));
  
  for(byte i = 0; i < data_size; i++) {
    write_byte(data[i]);
  }

  stop();
  
  update_display_control();
}

void TM1637Display::display_number(int number, uint8_t decimal_places)
{
  uint8_t digits = 4;
  uint8_t encoded_data[digits];
  
  encode_number(number, decimal_places, encoded_data, digits);
  display_raw(encoded_data, digits);
}

void TM1637Display::encode_number(int number, uint8_t decimal_places, uint8_t encoded_data[], uint8_t encoded_data_size, bool zero_fill)
{
  char signum = number < 0 ? -1 : (number == 0 ? 0 : 1);
  
  for (byte i = 0; i < encoded_data_size; i++) {
    uint8_t encoded_number = (!zero_fill && number == 0 && i > decimal_places) ? 0x00 : digit_tab[(signum < 0 ? -number : number) % 10];

    if (decimal_places > 0 && i == decimal_places) {
      encoded_number |= decimal;
    }

    if (i == encoded_data_size - 1 && signum < 0) {
      encoded_number = (number < 0 ? digit_tab[1] : 0x00) | minus;
    }
    
    encoded_data[encoded_data_size - 1 - i] = encoded_number;
    number = number / 10;
  }
}

void TM1637Display::update_display_control()
{
  start();
  write_byte(TM1637_CMD_DISP_CTRL | (m_brightness & 0x07) | (m_enabled ? 0x08 : 0x00));
  stop();
}

void TM1637Display::write_byte(int8_t wr_data)
{ 
  for (uint8_t i = 0; i < 8; i++) {
    switch_pin(m_clk_pin, LOW);
    switch_pin(m_data_pin, wr_data & 0x01 ? HIGH : LOW);
    switch_pin(m_clk_pin, HIGH);
    
    wr_data >>= 1;
  }
  
  pinMode(m_data_pin, INPUT);
  switch_pin(m_clk_pin, LOW);

  uint8_t ack = digitalRead(m_data_pin);

  switch_pin(m_clk_pin, HIGH);
  switch_pin(m_clk_pin, LOW);
  
  pinMode(m_data_pin,OUTPUT);
}

void TM1637Display::start(void)
{
  switch_pin(m_data_pin, LOW);
}

void TM1637Display::stop(void)
{
  switch_pin(m_clk_pin, HIGH);
  switch_pin(m_data_pin, HIGH);
}

void TM1637Display::bit_delay()
{
  delayMicroseconds(50);
}

void TM1637Display::switch_pin(uint8_t pin, uint8_t value)
{
  digitalWrite(pin, value);
  bit_delay();
}

