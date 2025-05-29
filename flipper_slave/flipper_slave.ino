#include <Wire.h>
#include <FtduinoSimple.h>

void setup() {
  Wire.begin(42); // Slave-Adresse 42
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Hier könnte zusätzliche Logik für den Slave implementiert werden
}

void receiveEvent(int numBytes) {
  if (Wire.available() >= 2) {
    int motor = Wire.read(); // Motoradresse (0-3)
    int direction = Wire.read(); // Drehrichtung

    switch (motor) {
      case 0:
        ftduino.motor_set(Ftduino::M1, direction);
        break;
      case 1:
        ftduino.motor_set(Ftduino::M2, direction);
        break;
      case 2:
        ftduino.motor_set(Ftduino::M3, direction);
        break;
      case 3:
        ftduino.motor_set(Ftduino::M4, direction);
        break;
      default:
        // Ungültige Motoradresse
        break;
    }
  }
}
