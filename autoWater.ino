// TM1638lite - Version: Latest
#include <TM1638lite.h>
TM1638lite tm(7, 9, 8); //(strobe, clock, data)

#define SOIL A0 //connected to moisture reader
int moist; //read moisture value
int readings = 1000; //number of reading to take for average
uint8_t memButtons = 0; //buttons pressed
float plotTime = 600000; //milliseconds between plots
float lastPlot = 0; //when last plot recorded

void setup() {
  pinMode(SOIL, INPUT);
  Serial.begin(57600);
  tm.displayText("Starting");
  delay(500);

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
  if (memButtons == 2) {
    tm.displayText(String(getRealMoisture())); //full reading
  }
  if (memButtons == 4) {
    tm.displayText(String(getAverageMoisture())); //full reading /100
  }
    if (memButtons == 8) {
    tm.displayText(String(getRealMoisture()/10)); //full reading /100
  }
  if (memButtons == 128) {
    readings += 10;
    String Disp = "A";
    tm.displayText(Disp + String(readings));
    memButtons = 3;
    delay(100);
  }
  if (memButtons == 64) {
    readings -= 10;
    String Disp = "A";
    tm.displayText(Disp + String(readings));
    memButtons = 3;
    delay(100);
  }

  if (memButtons == 1) {
    tm.displayText(String(getAverageMoisture()/10)); //display 1-10
  }
if (millis() > (lastPlot + plotTime)) {
  lastPlot = millis();
  Serial.println(String(getAverageMoisture()/10));
}
  //Serial.println(String(readings));

}
