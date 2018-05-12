// TM1638lite - Version: Latest
#include <TM1638lite.h>
#define SOIL A0; //connected to moisture reader
#define STROBE 7;
#define CLOCK 9;
#define DATA 8;
TM1638lite tm(STROBE, CLOCK, DATA); //(strobe, clock, data)

int readings = 1000; //number of reading to take for average
uint8_t memButtons = 0; //buttons pressed
float plotTime = 600000; //milliseconds between plots
float lastPlot = 0; //when last plot recorded

void setup() {
  pinMode(SOIL, INPUT);
  Serial.begin(57600);
  tm.displayText("Starting");
}

int getAverageMoisture() {
  float totalread = 0;
  for (int i = 0; i < readings; i++) {
    totalread += getRealMoisture();
  }
 return(totalread / (readings));
}

int getRealMoisture() {
  return(analogRead(SOIL));
}

void loop() {
  uint8_t buttons = tm.readButtons();
  if (buttons > 0) { memButtons = buttons; }
  if (memButtons == 1) {
    tm.displayText(String(getAverageMoisture()/10)); //display average 1-10
  }
  if (memButtons == 2) {
    tm.displayText(String(getRealMoisture())); //full reading live data
  }
  if (memButtons == 4) {
    tm.displayText(String(getAverageMoisture())); //average data full reading
  }
  if (memButtons == 8) {
    //4th button
  }
  if (memButtons == 16) {
    //5th button
  }
  if (memButtons == 32) {
    //6th button
  }  
  if (memButtons == 64) {
  //7th
  }  
  if (memButtons == 128) {
  //8th
  }
if (millis() > (lastPlot + plotTime)) {
  lastPlot = millis();
  Serial.println(String(getAverageMoisture()/10));
}
  //Serial.println(String(readings));

}
