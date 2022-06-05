/* 
 *  Example for HD44780 display with I2C interface, hosted by rp2040
 *  Display with 4 lines and 20 characters per line
 */

#include <PicoLCD_I2C.h>

const uint8_t addr = 0x27;      // I2C address of the display
const uint8_t sda = 4;          // ports to be used for the I2C bus
const uint8_t scl = 5;
const uint8_t linesize = 20;    // length of a line on the display

// Initiate LCD on i2c0, ports 4 and 5
PicoLCD_I2C lcd(0, addr, sda, scl, linesize);

int cnt = 0;

// custom chars: vertical bars, incrementing from left to right
byte l0[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111};
byte l1[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111};
byte l2[8] = {0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11111};
byte l3[8] = {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11111};
byte l4[8] = {0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11111};
byte l5[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

//---------------------------------------------------------
void setup() {
  lcd.begin();
  lcd.print("Hello from Pico");
  lcd.createChar(0, l0); 
  lcd.createChar(1, l1); 
  lcd.createChar(2, l2); 
  lcd.createChar(3, l3); 
  lcd.createChar(4, l4);
  lcd.createChar(5, l5);
}

//---------------------------------------------------------
void loop() {
  char buf[10];

  for (int i = 1; i < 4; ++i) { 
    lcd.setCursor(4 * i, i);      // x and y position
    itoa(cnt, buf, 10);               // show running counter
    lcd.print(buf);
    
    lcd.setCursor(linesize - 1, 0);   // move to upper left corner
    lcd.write(cnt % 6);               // animate bar with custom characters
    
    cnt += 1;
    delay(500);
  }
}
