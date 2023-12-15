//  Koda za digispark na katerega smo pirklopili rotacijsko številčnico starega telefona
//  Gradnja je opisana v reviji TIM
//  Avtor članka: Miha Kočar
  
  //Namestitev digispark knjiznice
  // http://digistump.com/wiki/digispark/tutorials/connecting
  // http://digistump.com/wiki/digispark/tutorials/linuxtroubleshooting
  
  #include "DigiKeyboardDe.h"
  //https://github.com/adnan-alhomssi/DigistumpArduinoDe
  //knjižnica, za eumilacijo tipkovnice
  //ker SLO tipkovnica ni qwerty temerč qwertz je razporeditev bolj podobna nemški.
  
  //globalne spremenljvke, ki bodo hranile prebrana stanja vhodov
unsigned int Stikala_stanje = 0;
bool Semafor_sedaj = 0;
bool Semafor_prej = 0;
bool Pulz_sedaj = 0;
bool Pulz_prej = 0;

  //konstante spremenljivke za pregledneje programiranje
const byte Stikala_Pin = A1;
const byte Semafor_stikalo_Pin = 1;
const byte Pulz_stikalo_Pin = 0;

  //globalna spremenljivka, ki bo štela pulze, ki predstavljajo izbrano številko
short Stevec_Pulzov = 0;

  //ob vklopu samo nastavi kateri pini so vhodni
void setup() {
  pinMode(Semafor_stikalo_Pin, INPUT);
  pinMode(Pulz_stikalo_Pin, INPUT);
  pinMode(Stikala_Pin, INPUT);
}

  //nato do izklopa ponavljaj v neskončni zanki
void loop() {
    //preveri kako je stanje stikala, ki signalizira, da je bila številčnica zavrtena
  Semafor_sedaj = digitalRead(Semafor_stikalo_Pin);

    //številčnica se je torej zavrtela dokler je to stikalo sklenjeno bodo na drugem stikalu impulzi za vsako številko
  if(Semafor_sedaj){
      //preveri stanje drugega stikala, ki predstavlja impulze za vsako številko
    Pulz_sedaj = digitalRead(Pulz_stikalo_Pin);

      //ker se koda vrši ekstremno hitro štejemo kot impulz, ko je bilo stikalo eno branje nazaj ne sklenjeno
    if(Pulz_prej>Pulz_sedaj){
      Stevec_Pulzov++;
    }
     
  }

    //če je prejšnje štetje bilo stikalo sklenjeno sedaj pa več ne, pomeni da se je številčnica odvrtela nazaj
  if(Semafor_sedaj<Semafor_prej){
      //če smo prešteli vsaj 1 impulz, je bila številčnica zavrtena najmanj do 1
    if(Stevec_Pulzov>0){    
        //če smo prešteli 10 (ali več) impulzov, je bila zavrtena v resnici 0
      if(Stevec_Pulzov==10){Stevec_Pulzov=0;} 
        //izpišemo na USB koliko impulzov smo prešteli, kar je enako zavrteni številki
      DigiKeyboardDe.print(Stevec_Pulzov);
    }else{
        //številčnica ni bila zavrtena do 1, torej bomo izpisali znak vezan na izbrano stikalo
        //preberemo analogno vrednost vhoda stikal
      Stikala_stanje=analogRead(Stikala_Pin);

        //glede na izbrano prebrano izšemo na USB želeni znak
      if((Stikala_stanje>900)&&(Stikala_stanje<1000)){DigiKeyboardDe.print('.');}
      if((Stikala_stanje>500)&&(Stikala_stanje<600)){DigiKeyboardDe.print(',');}
      if((Stikala_stanje>300)&&(Stikala_stanje<400)){DigiKeyboardDe.print(';');}
      if((Stikala_stanje>400)&&(Stikala_stanje<500)){DigiKeyboardDe.print('-');}
      if(Stikala_stanje>1000){DigiKeyboardDe.sendKeyStroke(KEY_BACKSPACE); }//vračalka - briši 1 nazaj
      if((Stikala_stanje>200)&&(Stikala_stanje<300)){
      /* koda če bi hoteli uporabljat za  telefon, bližnjice za zagon aplikacije telefon ne poznam, 
       *zato premikanje do ikone, ki je privzeto skrajno levo na začetnem meniju. 
       *Najprej se premaknemo zavsak slučaj skrajno desno in šele na levo
        */
       /* DigiKeyboardDe.sendKeyStroke(0x4F); //desno
        DigiKeyboardDe.sendKeyStroke(0x4F); //desno
        DigiKeyboardDe.sendKeyStroke(0x4F); //desno
        DigiKeyboardDe.sendKeyStroke(0x50); //levo
        DigiKeyboardDe.sendKeyStroke(0x50); //levo
        DigiKeyboardDe.sendKeyStroke(0x50); //levo
        DigiKeyboardDe.print("\n");
        */
        DigiKeyboardDe.sendKeyStroke(0x57);//znak+
          //če ne deluje znak preko .print,
          //poglej na ang tipkovnici kje je želeni znak nato ga poišči na https://www.usb.org/sites/default/files/hid1_11.pdf stran 52
          //in uporabi najdeno številko podobno kot zgoraj za znak +
        }
      if((Stikala_stanje>100)&&(Stikala_stanje<200)){DigiKeyboardDe.print(' ');}  
      if(Stikala_stanje<100){DigiKeyboardDe.print("\n");}   
    }
      //štetje smo torej zakjučili in izpisali, števec resetiramo na 0
    Stevec_Pulzov=0;
  }

    //pred naslednjo ponovitvijo, shranimo prebrane stanje stikal
  Pulz_prej = Pulz_sedaj;
  Semafor_prej= Semafor_sedaj;

    //to pogosto ni potrebno a nekateri sistemi rabijo, sicer spregledajo prvi poslan znak na USB
  DigiKeyboardDe.sendKeyStroke(0);
    //sedaj pa spet na začetek ponavljanja vsega v neskončni zanki
}
