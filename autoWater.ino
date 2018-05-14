// TM1638lite - Version: Latest
#include <TM1638lite.h>
#define SOIL A0 //connected to moisture reader
#define STROBE 7
#define CLOCK 9
#define DATA 8
#define PUMP 13
TM1638lite tm(STROBE, CLOCK, DATA); //(strobe, clock, data)

//User tweaks:
int readings = 1000; //number of reading to take for average
float plotTime = 600000; //milliseconds between plots
int minTrigger = 200; //trigger to turn on water
float pumpRestTime = 60000; //min time between waters
int pumpForSeconds = 4; //number of seconds to pump for
int requiredMoisture = 400; //moisture level to stop pumping
int errorPumping = 20; //moisture level min increase per pump
byte brightness = 0x8f; //brightness and enable = 1111
//end of user tweaks

bool error = 0; //error pumping
bool triggered = 0; // Is pump triggered?
int pumpedAtMoisture = 0; //moisture when last pumped
float pumpedAt = 0; //last watering time
float lastPlot = 0; //when last plot recorded
int minMoist; //minimum moisture
int maxMoist; //max moisture
int averageMoisture; // average Moisture
uint8_t memButtons = 0; //buttons pressed

/*Errors:
Error 1 : Pumped water not raising moisture sufficiently
*/

void setup() {
  pinMode(SOIL, INPUT);
  pinMode(PUMP, OUTPUT);
  Serial.begin(57600);
  tm.sendCommand(brightness);
  tm.displayText("Starting");
  delay(1000);
  minMoist = getAverageMoisture();
  maxMoist = minMoist;

}

int getAverageMoisture() {
  float totalread = 0;
  for (int i = 0; i < readings; i++) {
    totalread += getRealMoisture();
  }
  averageMoisture = totalread / readings;
  if (minMoist > averageMoisture) { minMoist = averageMoisture; }
  if (maxMoist < averageMoisture) { maxMoist = averageMoisture; }
  return averageMoisture;
}

int getRealMoisture() {
  return(1000-analogRead(SOIL));
}

void lightLEDs(uint8_t button) {
  for (uint8_t i = 0; i < 8; i++) {
    tm.setLED(i, button & 1);
    button = button >> 1;
  }
  if (triggered == 1) {
    tm.setLED(6, 1);
  } else {
    tm.setLED(6, 0);
  }
  if (error == 1) {
    tm.setLED(7, 1);
  } else {
    tm.setLED(7, 0);
  }
}

void showPlot() {
  if (millis() > (lastPlot + plotTime)) {
    lastPlot = millis();
    Serial.println("Reading = " + String(averageMoisture));
  }
}

void buttonPressed(uint8_t button) {
  if (button == 1) {
    tm.displayText(String(averageMoisture)); //1 - display average 1-10
  }
  if (button == 2) {
    tm.displayText(String(averageMoisture/10)); //2 - average data full reading
  }
  if (button == 4) {
    tm.displayText(String(getRealMoisture())); //3 - full reading live data
  }
  if (button == 8) {
    tm.displayText("Lo " + String(minMoist)); //4 - Min level
  }
  if (button == 16) {
    tm.displayText("Hi " + String(maxMoist)); //5 - Max level
  }
  if (button == 32) {     //6th button

  }
  if (button == 64) {   //7th

  }
  if (button == 128) {

  }
}

void checkTriggers() {
  if (averageMoisture < minTrigger) {
    if (triggered == 0) { Serial.println("Trigger at " + String(averageMoisture)); }
    triggered = 1;
  }
  if (averageMoisture > requiredMoisture) {
    if (triggered == 1) { Serial.println("Trigger removed " + String(averageMoisture)); }
    triggered = 0;
  }
}

void trigger() {
  if (triggered == 1 && (millis() > (pumpedAt + pumpRestTime))) {
    if (averageMoisture < pumpedAtMoisture + errorPumping) {
      error = 1;
      Serial.println("Error logged");
    } else {
      pump();
      pumpedAt = millis();
      pumpedAtMoisture = averageMoisture;
      Serial.println("Pumped at millis " + String(pumpedAt) + " and moisture "+ pumpedAtMoisture);
    }
  }
}

void pump(){
  digitalWrite(PUMP, 1);
  delay(pumpForSeconds * 1000);
  digitalWrite(PUMP, 0);
}

void checkButtons() {
  uint8_t buttons = tm.readButtons();
  if (buttons > 0) { memButtons = buttons; }
}

void loop() {
  getAverageMoisture();
  showPlot();
  checkButtons();
  if (error == 0) {
    buttonPressed(memButtons);
    lightLEDs(memButtons);
    checkTriggers();
    trigger();
  } else {
    tm.displayText("Er1 " + String(averageMoisture));
    if (memButtons == 128) {
      error = 0;
      triggered = 0;
      Serial.println("Reset pressed");
    }
  }
}
