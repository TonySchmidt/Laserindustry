#include <FtduinoSimple.h>
#include "RICv2.h"
/*
M1 Aufzug
M2 Förderband
<<<<<<< HEAD
M3 Licht-Lichtschranke füge hinzu wenn lcihtschranke verwendet wird nicht genutzt
M4 Förderband to HRL nicht genutzt
I1 Lichtschranke Nicht genuzt
=======
M4 Förderband to HRL
>>>>>>> 50a2cebecb4fa828e6e27a4be56a159d3bc7e366
I2 Stop unten
I3 Stop oben
*/
RobotikInterConnect *ric;

void setup() {
ric = new RobotikInterConnect(2);
ric->send(255,"Connected to Interconnect");
ric->send(255,"Setup");
aufzug_unten(true);

ric->send(255,"Setup completed");


}
void loop() {

  ric->send(255,"READY");
  String msg = ric->recv();
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);
  delay(2500);//roll time till going up
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  go_up();
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
  delay(7000);
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);     
  delay(1000);
  ric->send(3,"msg");
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


String aufzug_unten(bool t_f){
  String oben_unten = "";
    if (t_f == true){
    while (!ftduino.input_get(Ftduino::I2)){
      ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
    }
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);  
    oben_unten = "unten";
  }
  else{
    while (!ftduino.input_get(Ftduino::I3)){
      ftduino.motor_set(Ftduino::M1, Ftduino::RIGHT);
    }
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
    oben_unten = "oben";
  }
  return "oben_unten";
  }
