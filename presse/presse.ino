#include <Ftduino.h>
#include "RICv2.h"
// I3 presse oben
// i4 presse unten
// i7 schranke  zu
// i6 schrnake offen2


//m1 schranke
//m2 presse
//m4 Fööörderbaaand


RobotikInterConnect *ric;

void setup() {
  ric = new RobotikInterConnect(1);
  ftduino.init();
  ric->send(255,"Connected to Interconnect");
  ric->send(255,"Setup");
  delay(3000);
  schranke(true);
  presserunter(false);
  ric->send(255,"Setup completed");
}



void loop() {
ric->send(255,"READY");
String msg = ric->recv();
presseband(true, true);
delay(4000);
presseband(false, true);
presserunter(true);
delay(500);
presserunter(false);
schranke(false);
presseband(true, true);
delay(2000);
ric->send(2, msg);
delay(4000);
presseband(false, true);
schranke(true);
}










//Tonys X unnötigen Funktionen
void schranke(bool i){
  if(i){
    while(!ftduino.input_get(Ftduino::I7)){
      ftduino.motor_set(Ftduino::M1, Ftduino::RIGHT, Ftduino::MAX); //SchRaNKE auf
    }
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF,Ftduino::MAX);
  }
  else{
      while(!ftduino.input_get(Ftduino::I6)){                         //SCHRANKE zu
      ftduino.motor_set(Ftduino::M1, Ftduino::LEFT, Ftduino::MAX);
      }
      ftduino.motor_set(Ftduino::M1, Ftduino::OFF, Ftduino::MAX);
  }
}
void presserunter(bool i){
  if(i){
    while(!ftduino.input_get(Ftduino::I4)){
      ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT, Ftduino::MAX);  //presse HoCh
    }
    ftduino.motor_set(Ftduino::M2, Ftduino::OFF,Ftduino::MAX);
  }
  else{
      while(!ftduino.input_get(Ftduino::I3)){
      ftduino.motor_set(Ftduino::M2, Ftduino::LEFT, Ftduino::MAX);  //Presse runter
      }
      ftduino.motor_set(Ftduino::M2, Ftduino::OFF, Ftduino::MAX);
  }
}

void presseband(bool an, bool direction){
  if(an){
    if(direction){  
      ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT, Ftduino::MAX);
    }else{
      ftduino.motor_set(Ftduino::M4, Ftduino::LEFT, Ftduino::MAX);
    }
  }else{
    ftduino.motor_set(Ftduino::M4, Ftduino::OFF, Ftduino::MAX);
  }
  
}
