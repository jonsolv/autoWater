// TM1638lite - Version: Latest
#include <TM1638lite.h>
TM1638lite tm(7, 9, 8); //(strobe, clock, data)

#define SOIL A0; //connected to moisture reader
int moist; //read moisture value

void setup() {
  pinMode(SOIL, INPUT);
  Serial.begin(57600);
  tm.displayText("Starting");
  delay(500);
}

int getAverageMoisture() {
  int totalread = 0;
  int readings = 24; //number of reading to take for average
  for (int i = 0; i < readings; i++) {
    moist = analogRead(SOIL);
    totalread = totalread + moist;
  }
 return(totalread / (readings * 100));
}

void loop() {

  tm.displayText(String(getAverageMoisture())); //display 1-10
  delay(2000);


}
