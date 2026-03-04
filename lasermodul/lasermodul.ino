#include <FtduinoSimple.h>
#include "RICv2.h"
RobotikInterConnect *ric;

/*
druckluft M4
clamp auf M3
clamp zu M2
laser taster I8

förderband M1

Ablauf: 
arm bringt zu laser
siganl 
clamp zu
arm zurück senden
taster zum bestätigen
laser
taster zum beenden
clamp auf
förderband an 
an aufzug send
*/
void setup() {
  ric = new RobotikInterConnect(4);
  ric->send(255,"Connected to Interconnect");
  ric->send(255,"Setup");
  clampauf(true);
  ric->send(255,"Setup completed");
}

void loop() {
  ric->send(255,"READY");
  String msg = ric->recv();
  clampauf(false);
  while (!ftduino.input_get(Ftduino::I8)){}
  ric->send(255,"arm_back");//replace with arm
  delay(5000);
  while (!ftduino.input_get(Ftduino::I8)){}
  clampauf(true);
  forderband(true);
  delay(5000);
  ric->send(2,"GO");
  forderband(false);
  delay(10000);
  clampauf(false);

}


void clampauf(bool t_f){
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
    if (t_f == true){
    ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
    ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
    delay(1000);
    ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  }
  else{
      ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);
      delay(1000);
      ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  


}

void forderband(bool t_f){
  if (t_f == true){
    ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  }
  else{
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
  }
}