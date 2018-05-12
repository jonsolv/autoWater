// TM1638lite - Version: Latest
#include <TM1638lite.h>
#define SOIL A0 //connected to moisture reader
#define STROBE 7
#define CLOCK 9
#define DATA 8
TM1638lite tm(STROBE, CLOCK, DATA); //(strobe, clock, data)

int readings = 1000; //number of reading to take for average
uint8_t memButtons = 0; //buttons pressed
float plotTime = 6000; //milliseconds between plots
float lastPlot = 0; //when last plot recorded
int minMoist; //minimum moisture
int maxMoist; //max moisture
int averageMoisture; // average Moisture
int realMoisture; //real Moisture
byte brightness = 0x7; //brightness and enable = 1111


void setup() {
  pinMode(SOIL, INPUT);
  Serial.begin(57600);
  tm.displayText("Starting");
  delay(1000);
  minMoist = getAverageMoisture();
  maxMoist = getAverageMoisture();
}

int getAverageMoisture() {
  float totalread = 0;
  for (int i = 0; i < readings; i++) {
    totalread += getRealMoisture();
  }
 return(totalread / (readings));
}

int getRealMoisture() {
  return(1000-analogRead(SOIL));
}

void lightLEDs(uint8_t button) {
  for (uint8_t i = 0; i < 8; i++) {
    tm.setLED(i, button & 1);
    button = button >> 1;
  }
}

void showPlot() {
  if (millis() > (lastPlot + plotTime)) {
    lastPlot = millis();
    Serial.println(String(averageMoisture/10));
  }
}

void buttonPressed() {
  if (memButtons == 1) {
    tm.displayText(String(averageMoisture/10)); //1 - display average 1-10
  }
  if (memButtons == 2) {
    tm.displayText(String(averageMoisture)); //2 - average data full reading
  }
  if (memButtons == 4) {
    tm.displayText(String(getRealMoisture())); //3 - full reading live data
  }
  if (memButtons == 8) {
    tm.displayText("Lo " + String(minMoist)); //4 - Min level
  }
  if (memButtons == 16) {
    tm.displayText("Hi " + String(maxMoist)); //5 - Max level
  }
  if (memButtons == 32) {     //6th button

  }
  if (memButtons == 64) {   //7th

  }
  if (memButtons == 128) {   //8th - brightness/blank display
    if (brightness == 0) {
      tm.displayText("off...");
      delay(2000);
      brightness = 0x7;
      tm.reset();
      memButtons = 0;
    } else {
      brightness--;
      tm.sendCommand(0x88 | brightness);
      delay(500);
      memButtons = 0;
    }
  }
}

void loop() {
  averageMoisture = getAverageMoisture();
  if (minMoist > averageMoisture) { minMoist = averageMoisture; }
  if (maxMoist < averageMoisture) { maxMoist = averageMoisture; }
  uint8_t buttons = tm.readButtons();
  if (buttons > 0) { memButtons = buttons; }
  buttonPressed();
  lightLEDs(memButtons);
  showPlot();
}
