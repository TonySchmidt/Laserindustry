#include <FtduinoSimple.h>
#include "RICv2.h"
/*
Taster:
I1 unten und laser fertig
I2 oben
I3 hinten
I4 vorne
I5 abgabe
I6 laserpos
I7 aufnahme

Motoren:
M1 magnet
M2 vor zurück
M3 hoch runter
M4 drehen
*/

RobotikInterConnect *ric;

bool on = false;
void setup() {
  Serial.begin(9600);
  Serial.println("Starte Initialposition...");
  ric = new RobotikInterConnect(3);
  ric->send(255,"Connected to Interconnect");
  ric->send(255,"Setup");
  moveback();
  moveup();
  movetostart();
  movedown();
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  delay(1800);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  Serial.println("Initialisierung abgeschlossen");
  //send ready
  ric->send(255,"Setup completed");
}

void loop() {
  ric->send(255,"READY");
  String msg = ric->recv();
  Magnet(true);
  delay(3000);
  Serial.println("erhebe Arm (M3 hoch) etwas");
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT); // arm etwashoch
  delay(4000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  Serial.println("etwas hoch fertig"); 

  moveup();

  movetolaser();

  movetofront();//arm in laser rein

  Magnet(false);
  delay(500);

  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT); // arm etwas runter
  delay(3500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M2, Ftduino::LEFT); // arm back
  delay(15000);
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  Serial.println("I3 erreicht, M2 gestoppt 12");
  ric->send(255, "Bereit zum Lasern");
  while(!ftduino.input_get(Ftduino::I1)){
    delay(100);
  }
  //send und taster wait for laser end

  movetofront();//nach lasern wieder rein

  Magnet(true);
  delay(1000);
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT); // arm etwas hoch
  delay(5000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  moveback();

  moveup();

  Serial.println("Drehe M4 (zurück) bis I5 erreicht (abgabe)");
  while (!ftduino.input_get(Ftduino::I5)) {
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT); // arm to abgabe
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  Serial.println("I5 erreicht, M4 gestoppt");

  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT); // arm vor
  delay(6000);
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  delay(500);
  Magnet(false);
  //send go
  movedown();

  //send ok
  delay(1000);
  moveup();
  moveback();
  ric->send(10, msg);

  Serial.println("Drehe M4 (zurück) bis I7 erreicht (Startposition)");
  while (!ftduino.input_get(Ftduino::I7)) {
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT); // arm to startpos
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  Serial.println("I7 erreicht, M4 gestoppt");
  
  movedown();
}

void Magnet(bool on){
  if(on){
    ftduino.motor_set(Ftduino::M1, Ftduino::LEFT);
  }
    if(!on){
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF);
  }
}

void moveupabit(){

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT); // arm etwas hoch
  delay(4000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
}

void movedownbit(){
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT); // arm etwas runter
  delay(4000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
}

void moveup(){
  Serial.println("erhebe Arm (M3 hoch) bis I2 erreicht 1");
  while (!ftduino.input_get(Ftduino::I2)) {
    ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT); // arm hoch
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  Serial.println("I2 erreicht, M3 gestoppt 2");
}

void movedown(){
  Serial.println("senke Arm (M3 runter) bis I1 erreicht 7");
  while (!ftduino.input_get(Ftduino::I1)) {
    ftduino.motor_set(Ftduino::M3, Ftduino::LEFT); // arm runter
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  Serial.println("I1 erreicht, M3 gestoppt 8"); 
}

void movetostart(){
  Serial.println("Drehe M4 (zurück) bis I7 erreicht (Startposition)");
  while (!ftduino.input_get(Ftduino::I7)) {
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT); // arm to startpos
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  Serial.println("I7 erreicht, M4 gestoppt");
}

void movetolaser(){
   Serial.println("Drehe M4 (vor) bis I6 erreicht (laserpos) 5");
  while (!ftduino.input_get(Ftduino::I6)) {
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT); // arm to laser
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  Serial.println("I6 erreicht, M4 gestoppt 6"); 
}

void movetoabgabe(){
  Serial.println("Drehe M4 (zurück) bis I5 erreicht (abgabe)");
  while (!ftduino.input_get(Ftduino::I5)) {
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT); // arm to abgabe
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  Serial.println("I5 erreicht, M4 gestoppt");
}

void moveback(){
  Serial.println("Fahre M2 (zurück) bis I3 erreicht");
  while (!ftduino.input_get(Ftduino::I3)) {
    ftduino.motor_set(Ftduino::M2, Ftduino::LEFT); // arm back
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  Serial.println("I3 erreicht, M2 gestoppt");
}

void movetofront(){
  Serial.println("Fahre M2 (vor) bis I4 erreicht 9");
  while (!ftduino.input_get(Ftduino::I4)) {
    ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT); // arm vor
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  Serial.println("I4 erreicht, M2 gestoppt 10");
}