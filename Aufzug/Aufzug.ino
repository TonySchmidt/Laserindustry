#include <FtduinoSimple.h>
#include "RICv2.h"
/*
M1 Aufzug
M2 Förderband
M3 Licht-Lichtschranke
M4 Förderband to HRL
I1 Lichtschranke 
I2 Stop unten
I3 Stop oben
*/
RobotikInterConnect *ric;

void setup() {
ric = new RobotikInterConnect(9);
ric->send(255,"Connected to Interconnect");
ric->send(255,"Setup");
go_down();
ric->send(255,"Setup completed");

}
void loop() {
ric->send(255,"READY");
String msg = ric->recv();
  //DO STUFF
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  delay(1500);
  while (ftduino.input_get(Ftduino::I1)){
    ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  go_up();
  ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
  delay(500);
  ric->send(10, msg);
  delay(4000);
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);    
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  go_down();
}

void go_down(){
  while (!ftduino.input_get(Ftduino::I2)){
    ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
}

void go_up(){
  while (!ftduino.input_get(Ftduino::I3)){
    ftduino.motor_set(Ftduino::M1, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
}