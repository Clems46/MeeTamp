/* Programme permettant de piloter un radiateur 
 *  électrique par le fils pilote. 6 ordres possibles
 *  Commande manuelle avec un bouton ou requette http
 *  
 */


// Les E/S
const int altPos = D1;
const int altNeg = D2;
const int BpMode = D5;

int state;
int bpModeState = 0;
int mOde = 0;
unsigned long time1 = 297000;   //4'57''
unsigned long time2 = 293000;   //4'53''
unsigned long time3 = 3000;
unsigned long time4 = 7000;

unsigned long previousMillis;
unsigned long currentMillis;

int answer;
int Value = 0;


void setup() {
  
  pinMode(altPos, OUTPUT);
  pinMode(altNeg, OUTPUT);
  pinMode(BpMode, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);// Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);
  Serial.println("Loading");
  delay(10);


}

void confort() {    //Mode confort : Pas de signal
    digitalWrite(altPos, LOW);
    digitalWrite(altNeg, LOW);
    delay(10);
}

void eco() {    //Mode Eco : Pleine alternance
    digitalWrite(altPos, HIGH);
    digitalWrite(altNeg, HIGH);
    delay(10);
}

void arret() {    //Mode Arrêt : Alternance Positive
    digitalWrite(altPos, HIGH);
    digitalWrite(altNeg, LOW);
    delay(10); 
}

void horsGel() {    //Mode Hors Gel : Alternance negative
    digitalWrite(altPos, LOW);
    digitalWrite(altNeg, HIGH);
    delay(10); 
}

void confortMoins1(){   //Mode confort - 1 : Rien pendant 4'57'' puis Pleine alternance pendant 3 ''
    previousMillis = millis();    //Initialize the buffer
    int i = 1;

    //Loop 4'57
    
    while(i == 1){
     currentMillis = millis();
     digitalWrite(altPos, LOW);
     digitalWrite(altNeg, LOW);
     delay(20);
     Serial.println("4 minutes 57");

     unsigned long calcul;
     calcul = time1 - (currentMillis - previousMillis);
     Serial.print("temps restant = ");
     Serial.println (calcul);
   
     // If an order is available, then exit the loop
     
     if (Serial.available() > 0 || bpModeState == 1) {
        i = 0;
        delay(10);
        }
        
     // Time condition
        
     if (currentMillis - previousMillis >= time1) {
      previousMillis =millis();

      // Loop 3''
      
      int j = 1;
      while(j == 1) {

             // If an order is available, then exit the loop
             
             if (Serial.available() > 0  || bpModeState == 1) {
            j = 0;
            delay(10);
            }   
        currentMillis = millis();
        digitalWrite(altPos, HIGH);
        digitalWrite(altNeg, HIGH);
        Serial.println("3 secondes");
        delay(20);

        // Time condition
        
        if (currentMillis - previousMillis >= time3) {
          i = 0;
          j = 0;
        }
      }
     }
    }


 
}

void confortMoins2(){   //Mode confort - 2 : Rien pendant 4'53'' puis Pleine alternance pendant 7 ''
    previousMillis = millis();    //Initialize the buffer
    int i = 1;

    //Loop 4'53
    
    while(i == 1){
     currentMillis = millis();
     digitalWrite(altPos, LOW);
     digitalWrite(altNeg, LOW);
     delay(20);
     Serial.println("4 minutes 53");

     unsigned long calcul;
     calcul = time2 - (currentMillis - previousMillis);
     Serial.print("temps restant = ");
     Serial.println (calcul);
   
     // If an order is available, then exit the loop
     
     if (Serial.available() > 0  || bpModeState == 1) {
        i = 0;
        delay(10);
        }
        
     // Time condition
        
     if (currentMillis - previousMillis >= time2) {
      previousMillis =millis();

      // Loop 7''
      
      int j = 1;
      while(j == 1) {

             // If an order is available, then exit the loop
             
             if (Serial.available() > 0 || bpModeState == 1) {
            j = 0;
            delay(10);
            }   
        currentMillis = millis();
        digitalWrite(altPos, HIGH);
        digitalWrite(altNeg, HIGH);
        Serial.println("7 secondes");
        delay(20);

        // Time condition
        
        if (currentMillis - previousMillis >= time4) {
          i = 0;
          j = 0;
        }
      }
     }
    }
}

void loop() {
  
/*Action du bouton -> changement de mode,
* mode 0 Auto (requette HTTP)
* mode 1 confort
* mode 2 confort -1
* mode 3 confort -2
* mode 4 Eco
* mode 5 Hors Gel
* mode 6 Arrêt
 */
 bpModeState = digitalRead(BpMode);
 delay(10);   
 if (bpModeState == 1) { 
  mOde++;
  Serial.print("Bouton Mode appuyé. Mode selectionné : ");
  Serial.println(mOde);
  delay(500);  
 }

 if (mOde == 7) {
  mOde = 0;
 }
 
 if (Serial.available() > 0 && mOde == 0) {
    // get incoming byte:
    answer = Serial.read();
    delay(10);
    Serial.print("answer = ");
    Serial.println(answer);
    
    }
 if (answer == 48  || mOde == 6) {
    arret();
    Serial.println ("ARRET CHAUFFAGE");
    digitalWrite(LED_BUILTIN, LOW);
    }
 else if (answer == 50  || mOde ==  4) {
    eco();
    Serial.println ("Mode ECO");
    digitalWrite(LED_BUILTIN, LOW);
    }
 else if (answer == 51 || mOde == 5) {
    horsGel();
    Serial.println ("Mode Hors Gel");
    digitalWrite(LED_BUILTIN, LOW);
    }
 else if (answer == 52 || mOde == 2) {
    confortMoins1();
    Serial.println ("Mode confort - 1");
    digitalWrite(LED_BUILTIN, LOW);
    }
  
 else {
    confort();
    Serial.println ("Mode confort");
    digitalWrite(LED_BUILTIN, HIGH);

     }
     if ( mOde == 1) {
        confort();
        Serial.println ("Mode confort");
        digitalWrite(LED_BUILTIN, HIGH);
     }
}
