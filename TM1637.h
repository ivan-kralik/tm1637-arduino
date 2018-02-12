#ifndef TM1637_H
#define TM1637_h

#include <Arduino.h>

#define TM1637_ADDR_AUTO  0x40
#define TM1637_ADDR_FIXED 0x44
#define TM1637_STARTADDR  0xc0
#define TM1637_CMD_DISP_CTRL 0x80

class TM1637Display
{
  private:
  static const uint8_t digit_tab[];
  static const uint8_t minus = 0x40;
  static const uint8_t decimal = 0x80;

  bool m_enabled = true;
  uint8_t m_brightness = 2;
  uint8_t m_clk_pin;
  uint8_t m_data_pin;
  
  public:

  /**
   * Constructor
   * 
   * @param clk_pin Arduino pin which is connected to CLK on TM1637
   * @param data_pin Arduino pin which is connected to DIO on TM1637
   */
  TM1637Display(uint8_t clk_pin, uint8_t data_pin);

  /**
   * Turn display on/off
   * 
   * @param enabled Whether the display should be turned on (true = on, false = off)
   */
  void switch_display(bool enabled);

  /**
   * Set brightness (0 - darkset, 7 - brightest)
   */
  void set_brightness(uint8_t brightness);

  /**
   * Displays data containing bit masks of enabled segments
   * 
   * @param data The bit masks of segments
   * @param data_size Length of the data array
   * @param start_pos Position in display to start displaying from (0 = leftmost digit, 3 = rightmost digit)
   */
  void display_raw(uint8_t data[], uint8_t data_size, uint8_t start_pos = 0);

  /**
   * Displays decimal integer (can be both positive and negative)
   * 
   * @param number The integer to display
   */
  void display_number(int number);

  /**
   * Displays decimal number with decimal places separated by decimal point
   * 
   * @param number The decimal number
   * @param decimal_places The number of decimal places (a.k.a. scale)
   */
  void display_number(float number, uint8_t decimal_places);

  /**
   * Displays two numbers in style of clock - i.e. (1 - 2 digits) (colon) (2 digits)
   * 
   * @param hour The number on the left side of the colon
   * @param minute The number on the right side of the colon
   */
  void display_clock(uint8_t hour, uint8_t minute);

  private:
  void start();
  void stop();
  void update_display_control();
  void display_number(int number, uint8_t decimal_places);
  void encode_number(int number, uint8_t decimal_places, uint8_t encoded_data[], uint8_t encoded_data_size, bool zero_fill = false);
  void write_byte(int8_t wr_data);
  void bit_delay();
  void switch_pin(uint8_t pin, uint8_t value);
};

#endif // TM1637_H
