
#include <FtduinoSimple.h>
#include "RICv2.h"
int color;
RobotikInterConnect *ric;
/*

M1 
M2 Schiene (x-achse)
M3 hoch runter
M4 vor zurück


I1 Schiene Stopp Annahme und Regal 1
I2 Schiene Stopp Regal 2
I3 Schiene Stopp Ausgabe und Regal 3
I4 regal unten
I5 regal mitte
I6 Arm hinten stopp
I7  regal unten
I8 Arm vorne stopp
*/

/*



*/

void armsetup();
void annehmen();
void einlagern(int fach);
void ausgeben();
void auslagern(int fach);
void fach0_auslagern();
void fach1_auslagern();
void fach2_auslagern();
void fach3_auslagern();
void fach4_auslagern();
void fach5_auslagern();
void fach6_auslagern();

void fach0_einlagern();   
void fach1_einlagern();   
void fach2_einlagern();   
void fach3_einlagern();   
void fach4_einlagern();
void fach5_einlagern();   
void fach6_einlagern();     

int hrl[7] = { 0, 0, 0, 0, 0, 0, 0 };

void setup() {
  ric = new RobotikInterConnect(10);
  ric->send(255,"Connected to Interconnect");
  ric->send(255,"Setup");
  armsetup();
  ric->send(255,"Setup completed");

  for (int i=0; i < 7; i++) {
      //request new from maschienen lager
      ric->send(1,"GO"); 
  }
}

void loop() {
  armsetup();
  ric->send(255,"READY");
  String msg = ric->recv();
  color = atoi(&msg[2]);
    msg[2] = 0; 
   

    if (msg == "GO") {
      int i = 0;
      for ( i=0; i < 7; i++) {
        if (hrl[i] == 0) break;
      }
      if (i == 7) {
        annehmen();
        ausgeben();
      } else {  // else hinzufügen, um nur auszuführen, wenn ein freies Fach gefunden wurde
        einlagern(i);
        hrl[i] = color;
    }


    } else if (msg == "OD") {
      //output order
      int i = 0;
      for (i=0; i < 7; i++) {
        if (hrl[i] == color) break;
      }
      if (i == 7) {
        ric->send(255, "order UNAVAILABLE");
      } else{//wasn't found
      ric->send(255, "order found");

      //was found
      auslagern(i);
      hrl[i] = 0;
      ric->send(1,"GO");//request new from maschienen lager
      }
      
  
    }
}//loop


// auslager funktion
void auslagern(int fach){
  
  switch(fach) {
    //auslagern
    case 0:
      
           
        //auslagern
        fach0_auslagern();
        ric->send(255,"Order geliefert");
        break;

      
   case 1:
      
           
        //auslagern
        fach1_auslagern();
        ric->send(255,"Order geliefert");
        break;
      
    case 2:
      
           
        //auslagern
        fach2_auslagern();
        ric->send(255,"Order geliefert");
        break;
    case 3:
      
           
        //auslagern
        fach3_auslagern();
        ric->send(255,"Order geliefert");
        break;
    case 4:
        
        //auslagern
        fach4_auslagern();
        ric->send(255,"Order geliefert");
        break;
    case 5:
       
        //auslagern
        fach5_auslagern();
        ric->send(255,"Order geliefert");
        break;      
    case 6:
         
        //auslagern
        fach6_auslagern();
        ric->send(255,"Order geliefert");
        break;
    
  }
 }




//einlager funktion
void einlagern(int fach){
  armsetup();
    switch (fach){
      
      case 0:
        annehmen();     
        fach0_einlagern(); 
        ric->send(255,"eingelagert");
        break;
    case 1: 
        annehmen();        
        fach1_einlagern();     
        ric->send(255,"eingelagert");
        break;

    case 2:
      annehmen(); 
      fach2_einlagern();          
      ric->send(255,"eingelagert"); 
      break;
      
    case 3:      
      annehmen();   
      fach3_einlagern();                
      ric->send(255,"eingelagert"); 
      break;
      
    case 4:
      annehmen();      
      fach4_einlagern();          
      ric->send(255,"eingelagert"); 
      break;
      
    case 5:      
      annehmen();   
      fach5_einlagern();               
      ric->send(255,"eingelagert");  
      break;
      
    case 6:      
      annehmen();
      fach6_einlagern();                   
      ric->send(255,"eingelagert");
      break;
      
    }
    
}

//auslager funktionen
void fach6_auslagern(){
  while(!ftduino.input_get(Ftduino::I3)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur seite
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I7)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm  hoch
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);// arm etwas runter(auslagern)
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ausgeben();
  armsetup();
}
void fach5_auslagern(){
  while(!ftduino.input_get(Ftduino::I3)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur seite
  }
  
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
 Serial.println(2);
  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  Serial.println(3);
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);// arm etwas hoch(auslagern)
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ausgeben();
  armsetup();
}
void fach4_auslagern(){
  while(!ftduino.input_get(Ftduino::I2)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur ende
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I7)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm  hoch
  }

  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);//arm  hoch
  delay(1500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }

  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);// arm etwas hoch(auslagern)
  delay(1500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ausgeben();
  armsetup();
}
void fach3_auslagern(){
  while(!ftduino.input_get(Ftduino::I2)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur mitte
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I5)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm  hoch zur mitte
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);// arm etwas hoch(auslagern)
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ausgeben();
  armsetup();
}
void fach2_auslagern(){
  while(!ftduino.input_get(Ftduino::I2)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur mitte
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm hoch
  delay(3000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);// arm etwas hoch(auslagern)
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ausgeben();
  armsetup();
}
void fach1_auslagern(){
  while(!ftduino.input_get(Ftduino::I7)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm hoch
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);//arm etwas runter
  delay(1500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);// arm etwas hoch(auslagern)
  delay(2500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ausgeben();
  armsetup();
}
void fach0_auslagern(){
  while(!ftduino.input_get(Ftduino::I5)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm hoch
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);// arm etwas hoch(auslagern)
  delay(3500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm vorne hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  ausgeben();
  armsetup();
}
//einlager funktionen
void fach0_einlagern(){
  while(!ftduino.input_get(Ftduino::I5)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm hoch
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm höher
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);// arm etwas runter(einlagern)
  delay(1500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm vorne hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  //eingelagert senden
}
void fach1_einlagern(){
  while(!ftduino.input_get(Ftduino::I7)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm runter
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);
  

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);// arm etwas runter(einlagern)
  delay(1500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  //eingelagert senden
}
void fach2_einlagern(){
  while(!ftduino.input_get(Ftduino::I2)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur mitte
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm etwas hoch
  delay(4000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  while(!ftduino.input_get(Ftduino::I4)){
    ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);// arm etwas runter(einlagern)
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  //eingelagert senden
}
void fach3_einlagern(){
  while(!ftduino.input_get(Ftduino::I2)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur mitte
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I5)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm  hoch zur mitte
  }
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);// arm etwas runter(einlagern)
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  //eingelagert senden
}
void fach4_einlagern(){
  while(!ftduino.input_get(Ftduino::I2)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur mitte
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I7)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm  hoch
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }

  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  

  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);// arm etwas runter(einlagern)
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  //eingelagert senden
}
void fach6_einlagern(){

  while(!ftduino.input_get(Ftduino::I3)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur seite
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I5)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm  hoch
  }
  delay(2200);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);// arm etwas runter(einlagern)
  delay(2500);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);
  //eingelagert senden
}
void fach5_einlagern(){
  while(!ftduino.input_get(Ftduino::I3)){
  ftduino.motor_set(Ftduino::M2, Ftduino::RIGHT);//arm  zur seite
  }
  ftduino.motor_set(Ftduino::M2, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I7)){
  ftduino.motor_set(Ftduino::M3, Ftduino::RIGHT);//arm  runter
  }
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I8)){
  ftduino.motor_set(Ftduino::M4, Ftduino::LEFT);//arm vorne
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  ftduino.motor_set(Ftduino::M3, Ftduino::LEFT);// arm etwas runter(einlagern)
  delay(2000);
  ftduino.motor_set(Ftduino::M3, Ftduino::OFF);

  while(!ftduino.input_get(Ftduino::I6)){
  ftduino.motor_set(Ftduino::M4, Ftduino::RIGHT);//arm  hinten
  }
  ftduino.motor_set(Ftduino::M4, Ftduino::OFF);

  //eingelagert senden
}
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

















































































































































