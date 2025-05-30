#include <FtduinoSimple.h>
#include <Wire.h>
//#include "RICv2.h"
// I1 annahme von presse
// I2 anname von flipper
// I3 presse oben
// i4 presse unten
// i5 detektor presse
// i6 detektor drehkreuz

//m1 föderband presse
//m2 presse
//M3 drehen gummi
//m4 laufen gummi

//anderer ftdunio
// M1 drehen
// m2 förderband
// m3 zusammendrücken
// m4 AUSEIANDERRRR

//RobotikInterConnect *ric;
void setup() {
  Wire.begin();


}

void loop() {
  //ready
 //wait for go
  forderbandmainpresse();
  presserunter();
  delay(2000);
  pressehoch();
  ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  delay(3000);
  while(!ftduino.input_get(Ftduino::I6)){
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M1, Ftduino::OFF);//auf drehkruz
  delay(50);
  while(!ftduino.input_get(Ftduino::I2)){//drehen
    ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  set_output(Ftduino::M2, Ftduino::RIGHT);//send
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
  delay(3000);
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);



  //wait for slave
    while(!ftduino.input_get(Ftduino::I6)){
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I1)){// zum nächsten spot
    ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  //send go to aufzug
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
  delay(3000);
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
}
set_output(Ftduino::M1, Ftduino::LEFT);//m1-m4 LEFT RIGHT und OFF


void set_motor(int motor, int direction) {
  Wire.beginTransmission(42); // Slave-Adresse 42
  Wire.write(motor - Ftduino::M1); // Motoradresse (0-3)
  Wire.write(direction); // Drehrichtung
  Wire.endTransmission();
}
void forderbandmainpresse(){
  while (!ftduino.input_get(Ftduino::I5)){
    ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
}
void presserunter(){
    while (!ftduino.input_get(Ftduino::I4)){
    ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
}
void pressehoch(){
  while (!ftduino.input_get(Ftduino::I3)){
    ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
}
