#include <Ftduino.h>
#include <Wire.h>

#define SLAVE1_ADDR 43  // Adresse des ersten Slaves
#define OUTPUT_PORT Ftduino::O1

//#include "RICv2.h"



// I1 annahme von presse und weiter zum auftzug drehkreuz
// I2 anname von flipper drehkreuz
// I3 presse oben
// i4 presse unten
// i5 detektor presse
// i6 detektor drehkreuz
// i7 schranke  zu
// i8 schrnake offen

//m1 schranke
//m2 presse
//M3 drehen gummi
//m4 laufen gummi und presse förderband

//anderer ftdunio
// M1 drehen swapper
// m2 förderband swapper
// M3 hoch runter swapper
// 07 auf swapper
// 08 zu swapper

// i1 flipper oben
// i2 flipper unten
// i3 flipper front
// i4 flipper back

//RobotikInterConnect *ric;

void setup() {
  ftduino.init();
  Wire.begin();  // Master am I2C-Bus

  // Lokale Eingänge konfigurieren
  for(int i = 0; i < 8; i++) {
    ftduino.input_set_mode(Ftduino::I1 + i, Ftduino::SWITCH);
  }

  delay(100); // Warten, bis Slaves bereit sind

  // Beispiel: Slave O2 (Port 1) als Ausgang gegen 9V konfigurieren
  set_output_mode(SLAVE1_ADDR, 1, 0x01);  // HI-Modus
}


// Ausgänge (O1–O8): Dekodiert
// 0x00 = OFF     → Ausgang hochohmig
// 0x01 = HI      → 9V-Pegel
// 0x02 = LO      → 0V-Pegel

// Motoren (M1–M4):
// 0x10 = OFF     → Motor aus
// 0x11 = BRAKE   → Motorbremse
// 0x12 = LEFT    → Motor dreht links
// 0x13 = RIGHT   → Motor dreht rechts

// m1 = 0
// m2 = 2
// m3 = 4
// m4 = 6
// Loop mit Beispielen:
void loop() {
  // 1️⃣ Lokalen Taster I1 abfragen
  if(ftduino.input_get(Ftduino::I1)) {
    ftduino.output_set(Ftduino::O1, Ftduino::HI, Ftduino::MAX);
  } else {
    ftduino.output_set(Ftduino::O1, Ftduino::OFF, 0);
  }

  // 2️⃣ Slave-Taster I1 (Port 0) abfragen
  if(read_input_from_slave(SLAVE1_ADDR, 0) > 512) {
    // Beispiel: Slave-Motor M3 (Port 4) nach links fahren
    set_output_mode(SLAVE1_ADDR, 4, 0x12); // LEFT
    set_output_value(SLAVE1_ADDR, 4, 255); // volle Geschwindigkeit
  } else {
    // Stoppen
    set_output_mode(SLAVE1_ADDR, 4, 0x10); // OFF
    set_output_value(SLAVE1_ADDR, 4, 0);
  }

  // 3️⃣ Beispiel: Lokaler Taster I2 → Slave-Ausgang O2 HIGH
  if(ftduino.input_get(Ftduino::I2)) {
    set_output_value(SLAVE1_ADDR, 1, 255); // Port 1 = O2 auf HIGH
  } else {
    set_output_value(SLAVE1_ADDR, 1, 0);   // O2 auf LOW
  }

  delay(100);
}

// 📦 Funktion: Setzt Modus eines Ausgangs (auch Motor)
void set_output_mode(uint8_t slave_addr, uint8_t port, uint8_t mode) {
  Wire.beginTransmission(slave_addr);
  Wire.write((port << 1));  // Modus-Register
  Wire.write(mode);
  Wire.endTransmission();
}

// 📦 Funktion: Setzt PWM-Wert eines Ausgangs
void set_output_value(uint8_t slave_addr, uint8_t port, uint8_t value) {
  Wire.beginTransmission(slave_addr);
  Wire.write((port << 1) + 1);  // Wert-Register
  Wire.write(value);
  Wire.endTransmission();
}

// 📦 Funktion: Liest Eingang I1–I8 (port 0–7) vom Slave
uint16_t read_input_from_slave(uint8_t slave_addr, uint8_t input_port) {
  uint8_t addr = 0x10 + (input_port << 1);  // even register = low byte

  Wire.beginTransmission(slave_addr);
  Wire.write(addr);
  Wire.endTransmission(false);  // Restart für Leseanforderung

  Wire.requestFrom(slave_addr, (uint8_t)2);
  if(Wire.available() < 2) return 0xFFFF;

  uint8_t lo = Wire.read();
  uint8_t hi = Wire.read();
  return (hi << 8) | lo;
}

void schranke(bool i){
  if(i){
    while(!ftduino.input_get(Ftduino::I7)){
      ftduino.motor_set(Ftduino::M1, Ftduino::RIGHT, Ftduino::MAX);
    }
    ftduino.motor_set(Ftduino::M1, Ftduino::OFF,Ftduino::MAX);
  }
  else{
      while(!ftduino.input_get(Ftduino::I8)){
      ftduino.motor_set(Ftduino::M1, Ftduino::LEFT, Ftduino::MAX);
      }
      ftduino.motor_set(Ftduino::M1, Ftduino::OFF, Ftduino::MAX);
  }
}
void presserunter(bool i){
  if(i){
    while(!ftduino.input_get(Ftduino::I4)){
      ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT, Ftduino::MAX);
    }
    ftduino.motor_set(Ftduino::M2, Ftduino::OFF,Ftduino::MAX);
  }
  else{
      while(!ftduino.input_get(Ftduino::I3)){
      ftduino.motor_set(Ftduino::M2, Ftduino::LEFT, Ftduino::MAX);
      }
      ftduino.motor_set(Ftduino::M2, Ftduino::OFF, Ftduino::MAX);
  }
}
void drehkreuzgerade(bool gerade){
  if(gerade){
    while(!ftduino.input_get(Ftduino::I1)){
      ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT, Ftduino::MAX);
    }
    ftduino.motor_set(Ftduino::M3, Ftduino::OFF,Ftduino::MAX);
  }
  else{
      while(!ftduino.input_get(Ftduino::I2)){
      ftduino.motor_set(Ftduino::M3, Ftduino::LEFT, Ftduino::MAX);
      }
      ftduino.motor_set(Ftduino::M3, Ftduino::OFF, Ftduino::MAX);
  }
}
void motor0aktiv(bool aktiv, int motor, bool right){
  int y = motor - 1;
  if(aktiv){
    if(right){
      set_output_mode(SLAVE1_ADDR, y, 0x13); // RIGHT
      set_output_value(SLAVE1_ADDR, y, 255); // volle Geschwindigkeit
    }else{
      set_output_mode(SLAVE1_ADDR, y, 0x12); // LEFT
      set_output_value(SLAVE1_ADDR, y, 255); // volle Geschwindigkeit
    }

  }else{set_output_mode(SLAVE1_ADDR, y, 0x10);}//OFF
}
void motoraktivhaltenbis(int taster, int motor, int right){
  int motorinput = motor - 1;
  int tasterinput = taster - 1;
  //failsafe
  set_output_mode(SLAVE1_ADDR, motor, 0x10); // OFF
  bool waiter = true;
  if(right){
      while(waiter){
        if(read_input_from_slave(SLAVE1_ADDR, tasterinput) < 512) {
          set_output_mode(SLAVE1_ADDR, motorinput, 0x13); // Right
          set_output_value(SLAVE1_ADDR, motorinput, 255); // volle Geschwindigkeit
          delay(10);
        }else{
          waiter = false;
      }
    }
    set_output_mode(SLAVE1_ADDR, motorinput, 0x10); // OFF
    set_output_value(SLAVE1_ADDR, motorinput, 0);
  }else{
      while(waiter){
        if(read_input_from_slave(SLAVE1_ADDR, tasterinput) < 512) {
          set_output_mode(SLAVE1_ADDR, motorinput, 0x12); // LEFT
          set_output_value(SLAVE1_ADDR, motorinput, 255); // volle Geschwindigkeit
          delay(10);
        }else{
          waiter = false;
      }
    }
    set_output_mode(SLAVE1_ADDR, motorinput, 0x10); // OFF
    set_output_value(SLAVE1_ADDR, motorinput, 0);
  }
}
  

/* ventile anstueren

// nur zu sicherheit
set_output_mode(SLAVE1_ADDR, 6, 0x00); // O7 = OFF

// Schritt 2: O8 (Port 7) als HI setzen
set_output_mode(SLAVE1_ADDR, 7, 0x01); // O8 = HI
set_output_value(SLAVE1_ADDR, 7, 255); // volle Spannung (9V)
*/