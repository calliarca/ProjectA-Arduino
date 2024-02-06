#include <TFT_eSPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "Arsyade";
const char* password = "Trobosch";
const char* ntpServer = "pool.ntp.org";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer);

TFT_eSPI tft = TFT_eSPI();

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg=0, mdeg=0, hdeg=0;
uint16_t osx=120, osy=120, omx=120, omy=120, ohx=120, ohy=120;  // Saved H, M, S x & y coords
uint16_t x0=0, x1=0, yy0=0, yy1=0;
uint32_t targetTime = 0;                    // for next 1 second timeout

static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

bool initial = 1;

void setup() {
  tft.init();
  tft.setRotation(1);
  
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
  
  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  // Get current time
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();
  
  // Display the clock
  displayClock(hours, minutes, seconds);
  
  delay(1000); // Update every second
}

void displayClock(int hours, int minutes, int seconds) {
  // Draw clock face
  tft.fillCircle(120, 120, 118, TFT_GREEN);
  tft.fillCircle(120, 120, 110, TFT_BLACK);

  // Draw 12 lines
  for(int i = 0; i<360; i+= 30) {
    sx = cos((i-90)*0.0174532925);
    sy = sin((i-90)*0.0174532925);
    x0 = sx*114+120;
    yy0 = sy*114+120;
    x1 = sx*100+120;
    yy1 = sy*100+120;

    tft.drawLine(x0, yy0, x1, yy1, TFT_GREEN);
  }

  // Draw 60 dots
  for(int i = 0; i<360; i+= 6) {
    sx = cos((i-90)*0.0174532925);
    sy = sin((i-90)*0.0174532925);
    x0 = sx*102+120;
    yy0 = sy*102+120;
    // Draw minute markers
    tft.drawPixel(x0, yy0, TFT_WHITE);
    
    // Draw main quadrant dots
    if(i==0 || i==180) tft.fillCircle(x0, yy0, 2, TFT_WHITE);
    if(i==90 || i==270) tft.fillCircle(x0, yy0, 2, TFT_WHITE);
  }

  tft.fillCircle(120, 121, 3, TFT_WHITE);

  // Draw text at position 120,260 using fonts 4
  // Only font numbers 2,4,6,7 are valid. Font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : . - a p m
  // Font 7 is a 7 segment font and only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : .
  tft.drawCentreString("Time flies",120,260,4);

  targetTime = millis() + 1000; 

  // Pre-compute hand degrees, x & y coords for a fast screen update
  sdeg = seconds*6;                  // 0-59 -> 0-354
  mdeg = minutes*6+sdeg*0.01666667;  // 0-59 -> 0-360 - includes seconds
  hdeg = hours*30+mdeg*0.0833333;  // 0-11 -> 0-360 - includes minutes and seconds
  hx = cos((hdeg-90)*0.0174532925);    
  hy = sin((hdeg-90)*0.0174532925);
  mx = cos((mdeg-90)*0.0174532925);    
  my = sin((mdeg-90)*0.0174532925);
  sx = cos((sdeg-90)*0.0174532925);    
  sy = sin((sdeg-90)*0.0174532925);

  if (seconds==0 || initial) {
    initial = 0;
    // Erase hour and minute hand positions every minute
    tft.drawLine(ohx, ohy, 120, 121, TFT_BLACK);
    ohx = hx*62+121;    
    ohy = hy*62+121;
    tft.drawLine(omx, omy, 120, 121, TFT_BLACK);
    omx = mx*84+120;    
    omy = my*84+121;
  }

  // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
  tft.drawLine(osx, osy, 120, 121, TFT_BLACK);
  osx = sx*90+121;    
  osy = sy*90+121;
  tft.drawLine(osx, osy, 120, 121, TFT_RED);
  tft.drawLine(ohx, ohy, 120, 121, TFT_WHITE);
  tft.drawLine(omx, omy, 120, 121, TFT_WHITE);
  tft.drawLine(osx, osy, 120, 121, TFT_RED);

  tft.fillCircle(120, 121, 3, TFT_RED);
}

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
