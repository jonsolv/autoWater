// TM1638lite - Version: Latest
#include <TM1638lite.h>
TM1638lite tm(7, 9, 8); //(strobe, clock, data)

#define SOIL A0; //connected to moisture reader
int moist; //read moisture value
int readings = 24; //number of reading to take for average
uint8_t memButtons = 0; //buttons pressed

void setup() {
  pinMode(SOIL, INPUT);
  Serial.begin(57600);
  tm.displayText("Starting");
  delay(500);
}



int getAverageMoisture() {
  int totalread = 0;
  for (int i = 0; i < readings; i++) {
    totalread += getRealMoisture();
  }
 return(totalread / (readings * 100));
}

int getRealMoisture() {
  return(analogRead(SOIL));
}

void loop() {
  uint8_t buttons = tm.readButtons();
  if (buttons > 0) { memButtons = buttons; }
  if ((memButtons >> 7) == 1) {
    tm.displayText(String(getRealMoisture())); //full reading
  }
  if ((memButtons >> 1) == 1) {
    readings += 10;
    String Disp = "A";
    tm.displayText(Disp + String(readings));
  }
  if ((memButtons >> 2) == 1) {
    readings -= 10;
    String Disp = "A";
    tm.displayText(Disp + String(readings));
  }

  if ((memButtons >> 6) == 1) {
    tm.displayText(String(getAverageMoisture())); //display 1-10
    delay(2000);
  }

  if (memButtons == 0){
    tm.displayText(String(getAverageMoisture())); //display 1-10
    delay(2000);
  }
  Serial.println(String(memButtons));

}
