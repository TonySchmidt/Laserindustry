#include <FtduinoSimple.h>
// I1 90 grad
// I2 Links
// I3 Rechs


void setup() {
  // put your setup code here, to run once:
 //zusammen ziehen
  delay(200);
  //zusammen ziehen
  while (! ftduino.input_get(Ftduino::I1)){
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
    if ((ftduino.input_get(Ftduino::I2))){
      ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);
    }
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF); 
}

void loop() {
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT); // austrecken
  delay(200);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF); // austrecken
  ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);
  delay(1000);
  ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  while (! ftduino.input_get(Ftduino::I3)){
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT); //zusammen ziehen
  delay(200);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF); //zusammen ziehen
  while (! ftduino.input_get(Ftduino::I2)){
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  }
