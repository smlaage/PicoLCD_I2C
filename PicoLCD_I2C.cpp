/* PicoLCD_I2C.cpp
 SLW June 2022
 
 Simple implementation of the I2C interface for HD44780 alpha displays 
 Host: Raspberry Pi Pico rp2040
 - Based on github.com/ParicBat/rpi-pico-i2c-display-lib/ and 
   the Arduino library LiquidCrystal_I2C from Marco Schwartz
 - Adapted for Arduino
 The library provides only a subset of the full functionality.
 */

#include "PicoLCD_I2C.h"
#include <Arduino.h>

PicoLCD_I2C::PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda) {
  _i2c_port = i2c_port;
  _addr = addr;
  _scl = scl;
  _sda = sda;
  _linesize = 20;
  _i2c_speed = 100000;
  _backlight = LCD_NOBACKLIGHT;
}

PicoLCD_I2C::PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda, uint8_t linesize) {
  _i2c_port = i2c_port;
  _addr = addr;
  _scl = scl;
  _sda = sda;
  _linesize = linesize;
  _i2c_speed = 100000;
  _backlight = LCD_NOBACKLIGHT;
}

PicoLCD_I2C::PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda, uint8_t linesize, uint32_t i2c_speed) {
  _i2c_port = i2c_port;
  _addr = addr;
  _scl = scl;
  _sda = sda;
  _linesize = linesize;
  _i2c_speed = i2c_speed;
  _backlight = LCD_NOBACKLIGHT;
}

void PicoLCD_I2C::i2c_write_byte(uint8_t value) {
  if (_i2c_port == 0)
    i2c_write_blocking(i2c0, _addr, &value, 1, false);
  else
    i2c_write_blocking(i2c1, _addr, &value, 1, false);
}

void PicoLCD_I2C::toggle_enable(uint8_t value) {
    // Toggle enable pin on LCD display
    i2c_write_byte(value | LCD_ENABLE_BIT);
    delayMicroseconds(1);
    i2c_write_byte(value & ~LCD_ENABLE_BIT);
    delayMicroseconds(DELAY_US);
}

void PicoLCD_I2C::send_byte(uint8_t val, uint8_t mode) {
    uint8_t upper_nibble;
    uint8_t lower_nibble;

    upper_nibble = mode | (val & 0xF0) | _backlight;
    lower_nibble = mode | ((val << 4) & 0xF0) | _backlight;
 
    i2c_write_byte(upper_nibble);
    toggle_enable(upper_nibble);
    i2c_write_byte(lower_nibble);
    toggle_enable(lower_nibble);
}

void PicoLCD_I2C::begin(void) {
  if (_i2c_port == 0)
    i2c_init(i2c0, _i2c_speed);
  else
    i2c_init(i2c1, _i2c_speed);
  gpio_set_function(_sda, GPIO_FUNC_I2C);
  gpio_set_function(_scl, GPIO_FUNC_I2C);
  gpio_pull_up(_sda);
  gpio_pull_up(_scl);
  
  _backlight = LCD_BACKLIGHT;
  
  delay(DELAY_US);
  send_byte(0x00, LCD_COMMAND);
  delay(1000);
  send_byte(0x03, LCD_COMMAND);
  delayMicroseconds(4500);
  send_byte(0x03, LCD_COMMAND);
  delayMicroseconds(4500);
  send_byte(0x03, LCD_COMMAND);
  delayMicroseconds(150);
  send_byte(0x02, LCD_COMMAND);

  send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
  send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
  send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
  _displaycontrol = LCD_DISPLAYON;
  clear();
}

void PicoLCD_I2C::clear(void) {
    send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
    delayMicroseconds(2000);
}

void PicoLCD_I2C::write(char value) {
    send_byte(value, LCD_CHARACTER);
}

void PicoLCD_I2C::print(const char *s) {
  while (*s) {
    write(*s++);
  }
}

void PicoLCD_I2C::setBacklight(bool backlight) {
  if (backlight) 
    _backlight = LCD_BACKLIGHT;
  else
    _backlight = LCD_NOBACKLIGHT;
}

void PicoLCD_I2C::displayOn(void) {
  _displaycontrol |= LCD_DISPLAYON;
  send_byte(_displaycontrol | LCD_DISPLAYCONTROL, LCD_COMMAND);
}	

void PicoLCD_I2C::displayOff(void) {
  _displaycontrol &= ~LCD_DISPLAYON;
  send_byte(_displaycontrol | LCD_DISPLAYCONTROL, LCD_COMMAND);
}

void PicoLCD_I2C::cursorOn(void) {
	_displaycontrol |= LCD_CURSORON;
	send_byte(_displaycontrol | LCD_DISPLAYCONTROL, LCD_COMMAND);
}

void PicoLCD_I2C::cursorOff(void) {
	_displaycontrol &= ~LCD_CURSORON;
	send_byte(_displaycontrol | LCD_DISPLAYCONTROL, LCD_COMMAND);
}

void PicoLCD_I2C::blinkOn(void) {
	_displaycontrol |= LCD_BLINKON;
	send_byte(_displaycontrol | LCD_DISPLAYCONTROL, LCD_COMMAND);
}

void PicoLCD_I2C::blinkOff(void) {
	_displaycontrol &= ~LCD_BLINKON;
	send_byte(_displaycontrol | LCD_DISPLAYCONTROL, LCD_COMMAND);
}

void PicoLCD_I2C::setCursor(uint8_t x, uint8_t y) {
  uint8_t line_offsets[] = { 0x00, 0x40, _linesize, (uint8_t) (0x40 + _linesize) };
  uint8_t value = 0x80 + line_offsets[y] + x;
  send_byte(value, LCD_COMMAND);
}

void PicoLCD_I2C::home(void) {
  setCursor(0, 0);
  delayMicroseconds(DELAY_US);
}

void PicoLCD_I2C::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; 
  send_byte(LCD_SETCGRAMADDR | (location << 3), LCD_COMMAND);
  for (int i=0; i<8; i++) {
    send_byte(charmap[i], LCD_CHARACTER);
  }
}
