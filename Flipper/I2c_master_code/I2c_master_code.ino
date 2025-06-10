#include <Ftduino.h>
#include <Wire.h>

#define SLAVE1_ADDR 43  // Adresse des ersten Slaves
#define OUTPUT_PORT Ftduino::O1

#include "RICv2.h"



void setupsequenz();  
void drehkreuzcontrol(int position);  
void schranke(bool i);
void presseband(bool an, bool direction);
void presserunter(bool i);
void bandflipperan(bool aktiv, bool direction);
// I1 annahme von presse 
// I2 anname von flipper drehkreuz
// I3 presse oben
// i4 presse unten
// i5 abgabe aufzug
// i7 schranke  zu
// i6 schrnake offen

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
  Serial.begin(9600);
  ftduino.init();
  delay(2000); 
  Wire.begin();  // Master am I2C-Bus
  // Lokale Eingänge konfigurieren
  for(int i = 0; i < 8; i++) {
    ftduino.input_set_mode(Ftduino::I1 + i, Ftduino::SWITCH);
  }
  delay(100); // Warten, bis Slaves bereit sind
  ric = new RobotikInterConnect(1);
  ric->send(255,"Connected to Interconnect");
  ric->send(255,"Setup");
  set_output_mode(SLAVE1_ADDR, 6, 0x01); // O7 as HI
  set_output_value(SLAVE1_ADDR, 6, 255); // turn on O7 to 9V

  //setupsequenz();
  //drehkreuzcontrol(1);
  set_output_mode(SLAVE1_ADDR, 6, 0x01); // O7 as HI
  set_output_value(SLAVE1_ADDR, 6, 255); // turn on O7 to 9V
  ric->send(255,"Setup completed");
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

// m1 = 2 wenn man motor 0 aktiv macht dann immer bei den m ports einen ausrücken bzw die funktion macht das für dich
// m2 = 4
// m3 = 6
// m4 = 8
// Loop mit Beispielen:
void loop() {
  schranke(true);
  //send ready
  //wait for signal
  presseband(true, true);
  delay(4000);
  presseband(false, true);
  presserunter(true);
  delay(200);
  presserunter(false);
  schranke(false);//pressung fertig
  /*
  presseband(true, true);
  delay(5500);
  presseband(false, true);
  drehkreuzcontrol(2);//zum flipper
  bandflipperan(true, true);
  presseband(true, false);
  delay(2500);
  presseband(true, false);
  delay(2000);
  bandflipperan(false, true);// am flipper
  flipperfront(true);
  zusammendrucken(true);
  flipperfront(false);
  zusammendrucken(false); 
  flipperfront(true);
  flipperoben(true);//flip abgeschlossen
  bandflipperan(true, false);
  delay(500);
  presseband(true, true);
  delay(2500);
  presseband(false, true);
  bandflipperan(false, false);
  drehkreuzcontrol(3);//zum aufzug
  //send go to aufzug
  //maybe select laser picture here?
  presseband(true, false);
  delay(4000);
  presseband(false, false);
  drehkreuzcontrol(1);//fertig
  */
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
      while(!ftduino.input_get(Ftduino::I6)){
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
void motor0aktiv(bool aktiv, int motor, bool right, bool failsave){
  int y = motor - 2;
  int failsafeport = motor - 1;
  if (failsave){
    set_output_mode(SLAVE1_ADDR, failsafeport, 0x10);//failsafe
  }
  if(aktiv){
    if(right){
      set_output_mode(SLAVE1_ADDR, y, 0x13); // RIGHT
      set_output_value(SLAVE1_ADDR, y, 255); // volle Geschwindigkeit
    }else{
      set_output_mode(SLAVE1_ADDR, y, 0x12); // LEFT
      set_output_value(SLAVE1_ADDR, y, 255); // volle Geschwindigkeit
    }

  }else{
    set_output_mode(SLAVE1_ADDR, y, 0x10); // OFF
    set_output_value(SLAVE1_ADDR, y, 0);
    Serial.println("sollte jezt aus sein");
  }
}
void flipperfront(bool vorne){
  if(vorne){
    motoraktivhaltenbis(3, 2, true);//flipper nach vorne drehen also m1 zu i3
  }else{
    motoraktivhaltenbis(4, 2, false);//flipper nach hinten drehen also m1 zu i4
  }
}
void flipperoben(bool oben){
  if(oben){
  motoraktivhaltenbis(1, 6, false);//flipper runter fahren also m3 left bis i4 gedrückt wird
  }else{
  motoraktivhaltenbis(2, 6, true);//flipper runter fahren also m3 left bis i4 gedrückt wird
  }

}
void motoraktivhaltenbis(int taster, int motor, int right){
  int motorinput = motor - 1;
  int tasterinput = taster - 1;

  Serial.println("Waiting for taster " + String(tasterinput) + " input...");
  uint16_t val = read_input_from_slave(SLAVE1_ADDR, tasterinput);
  Serial.println("START value: " + String(val));

  //failsafe
  set_output_mode(SLAVE1_ADDR, motor, 0x10); // OFF
  bool waiter = true;
  if(right){
      while(waiter){
        if(read_input_from_slave(SLAVE1_ADDR, tasterinput) == 0) {
          set_output_mode(SLAVE1_ADDR, motorinput, 0x13); // Right
          set_output_value(SLAVE1_ADDR, motorinput, 255); // volle Geschwindigkeit
          delay(10);
        }else{
          waiter = false;
          Serial.println("got taster " + String(tasterinput) + " input");
      }
    }
    set_output_mode(SLAVE1_ADDR, motorinput, 0x10); // OFF
    set_output_value(SLAVE1_ADDR, motorinput, 0);
  }else{
      while(waiter){
        if(read_input_from_slave(SLAVE1_ADDR, tasterinput) == 0) {
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
void bandflipperan(bool aktiv, bool dircetion){
  if(aktiv){
    if(dircetion){
      motor0aktiv(true, 4, true, false); // m2 an
    }else{
      motor0aktiv(true, 4, false, false); // m2 an
    }

  }else{
    motor0aktiv(false, 4, true, false); //m2 aus
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
void setupsequenz(){
  schranke(false);
  presserunter(false);
  flipperoben(false);
  flipperfront(true);
  delay(100);
  flipperfront(false);
  flipperoben(true);
  Serial.print("3");
  bandflipperan(true, true);
  delay(3000);
  bandflipperan(false, true);
}
void drehkreuzcontrol(int position){
  switch (position) {
      case 1:
        //band annahme
          while(!ftduino.input_get(Ftduino::I1)){
            ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT, Ftduino::MAX);
          }
          ftduino.motor_set(Ftduino::M3, Ftduino::OFF, Ftduino::MAX);
          break;
      case 2:
        // flipper annahme
          while(!ftduino.input_get(Ftduino::I2)){
            ftduino.motor_set(Ftduino::M3, Ftduino::LEFT, Ftduino::MAX);
          }
          ftduino.motor_set(Ftduino::M3, Ftduino::OFF, Ftduino::MAX);
        break;
      case 3:
        //aufzug annahme
          while(!ftduino.input_get(Ftduino::I5)){
            ftduino.motor_set(Ftduino::M3, Ftduino::LEFT, Ftduino::MAX);
          }
          ftduino.motor_set(Ftduino::M3, Ftduino::OFF, Ftduino::MAX);
        break;
      default:
          while(!ftduino.input_get(Ftduino::I1)){//band anahmne
            ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT, Ftduino::MAX);
          }
          ftduino.motor_set(Ftduino::M3, Ftduino::OFF, Ftduino::MAX);
        break;
    }

}

void zusammendrucken(bool zu){
  if(zu){
    //missing code
  }else{
    //missing code
  }
}

/* ventile anstueren

// nur zu sicherheit
set_output_mode(SLAVE1_ADDR, 6, 0x00); // O7 = OFF

// Schritt 2: O8 (Port 7) als HI setzen
set_output_mode(SLAVE1_ADDR, 7, 0x01); // O8 = HI
set_output_value(SLAVE1_ADDR, 7, 255); // volle Spannung (9V)
*/


  /*
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
  */