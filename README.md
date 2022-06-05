# PicoLCD_I2C
A library for the Rasperry Pi Pico (rp2040) to run LC displays (HD4478 controller) using the Arduino framework

This is a (simplified) version of the LiquidCrystal_I2C library created by Marco Schwartz, adapated for the rp2040 microcontroller.
The library supports the following functions:

Constructors:
  - PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda);  
  - PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda, uint8_t linesize);  
  - PicoLCD_I2C(uint8_t i2c_port, uint8_t addr, uint8_t scl, uint8_t sda, uint8_t linesize, uint32_t i2c_speed);  
   
Public functions:  
  - void begin();  
  - void clear(void);  
  - void write(char value);  
  - void print(const char *s);  
  - void setCursor(uint8_t x, uint8_t y);  
  - void home(void);  
  - void setBacklight(bool backlight);  
  - void displayOn(void);  
  - void displayOff(void);  
  - void cursorOn(void);  
  - void cursorOff(void);  
  - void blinkOn(void);  
  - void blinkOff(void);  
  - void createChar(uint8_t location, uint8_t charmap[]);  
    
Wiring:  
  - Display: GND -> Pico: anyground pins  
  - Display: Vcc -> Pico: Vbus (+5.0 V) (pin 40)  
  - Display: SDA -> Pico: any of the SDA pins that are available for the selected I2C bus, e.g. 4 for i2c0 (GPIO4 is pin 6)  
  - Display: SCL -> Pico: any of the SCL pins that are available for the selected I2C bus, e.g. 5 for i2c0 (GPIO5 is pin 7)  
  
Tested:  
  Tested with standard displays of size 2*16 and 4*20
  
Notes:  
  - The user can select the respective I2C port (0 for i2c0, 1 for i2c1) and GPIOs for sda and scl. 
  - Please refer to the rp2024 documentation to see the GPIO pins available for the selected I2C bus.  
  - Internal pull up resistors for SDA and SCL will be activates.  
  - I2C clock speed of 100000 to 400000 are supported. Some displays can't handle higher speed.  
  - At this stage. the functionality is limited to what I need for now. Other functions can be added easily.  
  

