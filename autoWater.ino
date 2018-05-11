// TM1638lite - Version: Latest
#include <TM1638lite.h>

TM1638lite tm(7, 9, 8); //(strobe, clock, data)

#define SOIL A0

void setup() {
  pinMode(SOIL, INPUT);
  Serial.begin(57600);
  tm.displayText("88888888");
  delay(500);
for (int i = 0; i < 20; i++){
  tm.displayText(String(i));
  //delay(100);
}
int i;
}

void loop() {
  int rr = 0;
  int r, i;
  for (r = 0; r < 24; r++) {
    i = analogRead(SOIL);
    rr = rr + i;
  }
 i = rr / (r * 100);
 //Serial.println(String(i));
 tm.displayText(String(i));
 delay(2000);

}
