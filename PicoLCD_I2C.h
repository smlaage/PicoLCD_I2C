/* PicoLCD_I2C.h
 SLW June 2022
 
 Simple implementation of the I2C interface for HD44780 alpha displays 
 Host: Raspberry Pi Pico rp2040
 - Based on github.com/ParicBat/rpi-pico-i2c-display-lib/ and 
   the Arduino library LiquidCrystal_I2C from Marco Schwartz
 - Adapted for Arduino
 The library provides only a subset of the full functionality.
 */

#ifndef __PICO_LCD_I2C__
#define __PICO_LCD_I2C__

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// commands
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// flags for display entry mode
#define LCD_ENTRYSHIFTINCREMENT   0x01
#define LCD_ENTRYLEFT             0x02

// flags for display and cursor control
#define LCD_BLINKON         0x01
#define LCD_CURSORON        0x02
#define LCD_DISPLAYON       0x04

// flags for display and cursor shift
#define LCD_MOVERIGHT       0x04
#define LCD_DISPLAYMOVE     0x08

// flags for function set
#define LCD_5x10DOTS        0x04
#define LCD_2LINE           0x08
#define LCD_8BITMODE        0x10

// flag for backlight control
#define LCD_BACKLIGHT       0x08
#define LCD_NOBACKLIGHT     0x00

#define LCD_ENABLE_BIT      0x04

// Modes for lcd_send_byte
#define LCD_CHARACTER       1
#define LCD_COMMAND         0

// Timing
#define DELAY_US            70


class PicoLCD_I2C {
  private: 
    uint8_t _i2c_port;   // 0 -> i2c0, 1 -> i2c1
    uint8_t _sda;        // pin (gpio) number
    uint8_t _scl;        // pin (gpio) number
    uint8_t _addr;       // i2c address, usually 0x27 or 0x3f
    uint8_t _linesize;   // usually 16 or 20
    uint32_t _i2c_speed; // usually 100000 or 400000
	uint8_t _displaycontrol;
    uint8_t _backlight;  
    
    void i2c_write_byte(uint8_t value);
    void toggle_enable(uint8_t value);
    void send_byte(uint8_t value, uint8_t mode);

  public:
    PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda);
    PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda, uint8_t linesize);
    PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda, uint8_t linesize, uint32_t i2c_speed);
    void begin();
    void clear(void);
    void write(char value);
    void print(const char *s);
    void setCursor(uint8_t x, uint8_t y);
    void home(void);
    void setBacklight(bool backlight);
	void displayOn(void);
	void displayOff(void);
	void cursorOn(void);
	void cursorOff(void);
	void blinkOn(void);
	void blinkOff(void);
    void createChar(uint8_t location, uint8_t charmap[]);
};

#endif
