#include <FtduinoSimple.h>
int color;
/*

M1 
M2 Schiene (x-achse)
M3 hoch runter
M4 vor zurück




I1 Schiene Stopp Annahme und Regal 1 (x) tony ist gay
I2 Schiene Stopp Regal 2             (x) tony ist gay
I3 Schiene Stopp Ausgabe und Regal 3 (x) tony ist gay
I4 regal unten                       (y)tony ist gay
I5 regal oben                        (y) tony ist gay
I6 Arm hinten stopp                  (z)tony ist gay
I7  regal unten                      (remove) tony ist gay
I8 Arm vorne stopp                   (z) tony ist gay

*/

void armsetup();
void einlagern();
void auslagern();
void moveto(int zx, int zy, int ax, int ay);

int hrl[7] = { 0, 0, 0, 0, 0, 0}; //info über wo was drin ist
int hrlx[7] = { 2, 1, 2, 2 }; // shema für seite
int hrly[7] = { 1, 2, 2, 3 }; // shema für höhe
int ax = 1; //X position
int ay = 1; //Y position

void setup() {
  armsetup();
}

void loop() {
  armsetup();
  String msg = "GO1";
  color = atoi(&msg[2]);
    msg[2] = 0; 
    if (msg == "GO") {
      int i = 0;
      for ( i=0; i < 4; i++) {
        if (hrl[i] == 0) break;
      }
      if (i == 5) {
        annehmen();
        ausgeben();
      } else {  // else hinzufügen, um nur auszuführen, wenn ein freies Fach gefunden wurde
      //MARK: hier wird eingelagert
        einlagern();//objekt auf arm
        moveto( hrlx[i], hrly[i], ax, ay);
        auslagern();
        hrl[i] = color;
        moveto( 1, 1, ax, ay);
    }


    } else
    if (msg == "OD") {
      //output order
      int i = 0;
      for (i=0; i < 4; i++) {
        if (hrl[i] == color) break;
      }
      if (i == 4) {
        // wasnt found
        // send message error
      } else{// found
      moveto( hrlx[i], hrly[i], ax, ay); // zum fach bewegen
      auslagern();// von fach entfernen
      moveto( 3, 1, ax, ay); //ausgabe
      ausgeben();//auf den shoute legen
      hrl[i] = 0;
      moveto( 1, 1, ax, ay); 
      }
      
  
    }
}//loop


// auslager funktion
void auslagern(){
  while(!ftduino.input_get(Ftduino::I8)){
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vor
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm leicht hoch
  delay(500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);//arm oben
  while(!ftduino.input_get(Ftduino::I6)){
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm zurück
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  delay(500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
}

void einlagern(){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm leicht hoch
  delay(500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);//arm oben
  while(!ftduino.input_get(Ftduino::I8)){
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vor
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  delay(500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I6)){
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm zurück
  }
}


void armsetup() {
  while(!ftduino.input_get(Ftduino::I6)){
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm hinten
    }
    ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I4)){
    ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);//arm runter
    }
    ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I1)){
    ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);//arm anfang
  }
    ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
}


void moveto(int zx, int zy, int ax, int ay){//richtung rausfinden
  if(zy<ay){
    while(!ftduino.input_get(Ftduino::I4)){//taster unten Y = 1
      ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);//arm runter
    }
  }else if(zy>ay){
    while(!ftduino.input_get(Ftduino::I5)){//taster oben Y = 2
      ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm hoch
    }
  }

  if(zx<ax){
    if(zx==1){
      while(!ftduino.input_get(Ftduino::I1)){//taster links X = 1
        ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);//arm links
      }
    }
    else{
      while(!ftduino.input_get(Ftduino::I2)){//taster links X = 2
        ftduino.motor_set(Ftduino::M2, Ftduino::LEFT);//arm links
      }
    }
  }else if(zx==2){//zx>ax
      while(!ftduino.input_get(Ftduino::I2)){//taster links X = 2
        ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm rechts
      }
    }
    else{
      while(!ftduino.input_get(Ftduino::I3)){//taster links X = 3
        ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm rechts
    }
  }
}// tony ist gay


void ausgeben(){
  while(!ftduino.input_get(Ftduino::I4)){
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);//arm runter
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I3)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//zur ausgabe
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);
  delay(5000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I4)){
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);//arm runter
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm hinten
  delay(2000);
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm hoch
  delay(5000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  armsetup();
}
void annehmen(){
  armsetup();
  while(!ftduino.input_get(Ftduino::I8)){
    ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm hoch
  delay(5000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I6)){
    ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I4)){
    ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);//arm runter
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF); 
}