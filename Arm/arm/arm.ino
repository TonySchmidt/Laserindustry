#include <FtduinoSimple.h>
#include "RICv2.h"

/* Taster:
 I1 unten und laser fertig
 I2 oben 
 I3 hinten 
 I4 vorne 
 I5
 I7 laserpos 
 I6 aufnahme 
 
 Motoren: 
 M1 magnet 
 M2 vor zurück 
 M3 hoch runter 
 M4 drehen 
 */
RobotikInterConnect *ric;

bool on = false;

void setup() {
  //ric = new RobotikInterConnect(3);
  //ric->send(255, "Setup");
  moveback();
  moveup();
  movetostart();
  movedown();
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  delay(1800);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  //ric->send(255, "Setup completed");
}

void loop() {
  //ric->send(255, "READY");
  //String msg = ric->recv();
  Magnet(true);
  delay(1000);
  moveupabit();
  moveup();
  movetolaser();
  movetofront();
  Magnet(false);
  delay(500);
  movedownbit();
  moveback();
  moveback();
  movetostart();
  movedown();
}

void Magnet(bool on) {  //magnetkontrolle
  if (on) {
    ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  }
  if (!on) {
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
  }
}

void moveupabit() {  //etwas höher
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);
  delay(4000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
}

void movedownbit() {  //etwas runter
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  delay(4000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
}

void moveup() {  //arm hoch
  while (!ftduino.input_get(Ftduino::I2)) {
    ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
}

void movedown() {
  while (!ftduino.input_get(Ftduino::I1)) {
    ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
}

void movetostart() {  // arm zum empfang
  while (!ftduino.input_get(Ftduino::I6)) {
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
}

void movetolaser() {  // arm zur übergabe
  while (!ftduino.input_get(Ftduino::I7)) {
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
}

void moveback() {  //arm nach hinten
  while (!ftduino.input_get(Ftduino::I3)) {
    ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
}

void movetofront() {  //arm nach vorne
  while (!ftduino.input_get(Ftduino::I4)) {
    ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
}
