### Video Tutorial Connect ESP32 & 240x240 IPS LCD (ST7789 SPI interface) :

https://www.youtube.com/watch?v=7vYIGGlBRkU&ab_channel=anErik

### Artikel Tutorial Connect ESP32 & 240x240 IPS LCD (ST7789 SPI interface) :

https://arduino-er.blogspot.com/2020/07/esp32-13-inch-240x240-ips-lcd-st7789.html

### github library TFT_eSPI :

https://github.com/Bodmer/TFT_eSPI.git

### pinout ESP32 WROOM DEVKIT :

https://esp32io.com/images/tutorial/ESP-WROOM-32-Dev-Module-pinout.jpg

### github SmartWatch Code EXAMPLE (Credit to : Volos Project) :

https://github.com/VolosR/watchESP
    
7 pins are used to connect to MCU:

# Pin Label Description
1. GND LCD Power ground
2. VCC LCD power supply is positive (3.3V)
3. SCL  LCD SPI bus clock signal - connect to ESP32 18 TFT_SCLK
4. SDA LCD SPI bus write data signal - connect to ESP32 23 TFT_MOSI
5. RES  LCD reset control signal(Low level reset) - connect to ESP32 4 TFT_RST
6. DC   LCD register/data - connect to EST32 2 TFT_DC
7. BLK LCD backlight control signal - Connect to 3.3V

Setelah Semua Pinout ESP32 disambungkan ke Layar TFT, Anda bisa melakukan percobaan apakah layarnya berfungsi dengan baik atau tidak.
Dengan cara Pergi ke File -> Example -> TFT_eSPI -> 320 x 240 -> UTFT_Demo
