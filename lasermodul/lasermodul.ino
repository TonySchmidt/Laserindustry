#include <FtduinoSimple.h>
#include "RICv2.h"
RobotikInterConnect *ric;

/*
druckluft
clamp auf
clamp zu
laser taster I1

förderband

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
  while (!ftduino.input_get(Ftduino::I1)){}
  ric->send(255,"arm_back");//replace with arm
  delay(5000);
  while (!ftduino.input_get(Ftduino::I1)){}
  clampauf(true);

}


void clampauf(bool t_f){
    if (t_f == true){
    while (!ftduino.input_get(Ftduino::I2)){
      ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
    }
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);  
  }
  else{
    while (!ftduino.input_get(Ftduino::I3)){
      ftduino.motor_set(Ftduino::M1, Ftduino::RIGHT);
    }
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
  }
}

void forderband(bool t_f){
  if (t_f == true){
    ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  }
  else{
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
  }
}