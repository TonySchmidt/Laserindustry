#include <FtduinoSimple.h>

const uint8_t inputs[] = {
  Ftduino::I1, Ftduino::I2, Ftduino::I3, Ftduino::I4,
  Ftduino::I5, Ftduino::I6, Ftduino::I7
};

bool lastStates[7];

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Anfangszustände ausgeben
  for (int i = 0; i < 7; i++) {
    bool state = ftduino.input_get(inputs[i]);
    lastStates[i] = state;
    Serial.print("I");
    Serial.print(i + 1); // Damit korrekt I1 bis I7 ausgegeben wird
    Serial.print(": ");
    Serial.println(state ? "HIGH" : "LOW");
  }
}

void loop() {
  for (int i = 0; i < 7; i++) {
    bool state = ftduino.input_get(inputs[i]);
    if (state != lastStates[i]) {
      lastStates[i] = state;
      Serial.print("I");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(state ? "HIGH" : "LOW");
    }
  }
  delay(100);
}
