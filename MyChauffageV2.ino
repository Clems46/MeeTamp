/* Programme permettant de piloter un radiateur  *  électrique par le fils pilote. 6 ordres possibles *  Commande manuelle avec un bouton ou requette http *   *///Biblios pour le server web#include <ESP8266WebServer.h>#include <ESP8266WiFi.h>#include <ESP8266HTTPClient.h>#include <WiFiClientSecure.h>//Infos WIFIconst char* ssid     = "****";const char* password = "****";const char* host     = "****";  //IP du Jeedomconst int   port     = 80;//Infos Jeedomconst char* IdMode = "91";const char* apiKey   = "****";String tokenArduino = "****";// Les E/Sconst int altPos = D1;const int altNeg = D2;const int BpMode = D5;int state;int bpModeState = 0;int mOde = 1;int set = 0;unsigned long time1 = 297000;   //4'57''unsigned long time2 = 293000;   //4'53''unsigned long time3 = 3000;unsigned long time4 = 7000;unsigned long previousMillis;unsigned long currentMillis;const int   watchdog = 600000;const int watchdogHttp = 120000;unsigned long previousMillishttp = millis();unsigned long currentMillishttp = 0;String ModeAuto;ESP8266WebServer server ( 80 );HTTPClient http;int answer = 0;int Value = 0;//Mode JEEDOM, reçoit les infos de Jeedomvoid updateMode(){  Serial.println("Maj modes from Jeedom : ");  for ( int i = 0 ; i < server.args(); i++ ) {    Serial.print(server.argName(i)); Serial.println(server.arg(i));  }  ModeAuto = server.arg("id");   Serial.print("La valeur est : ");  Serial.println(ModeAuto);  String token = server.arg("token");  // Vérifie si le serveur est autorisé - Check if server is authorized  if ( token != tokenArduino ) {    Serial.println("Not authentified ");    return;  }  int etat = server.arg("etat").toInt();  Serial.print("Update Mode ");   if ( etat == 1 ) {    Serial.println("Mode updated : On");    server.send(200, "application/json","success:1");  } else if ( etat == 0 ) {    Serial.println("Mode updated : Off");    server.send(200, "application/json","success:1");  } else {    Serial.println("Bad state Value !");    server.send(200, "application/json","success:0");  }}// Envoie des requetrs http à Jeedomboolean sendToJeedom(String url){  Serial.print("connecting to ");  Serial.println(host);  Serial.print("Requesting URL: ");  Serial.println(url);  http.begin(host,port,url);  int httpCode = http.GET();  http.end();}//   WiFiClient client;void SendData(){     if(WiFi.status() != WL_CONNECTED) {      Serial.println("connection failed");      return;    } else {        Serial.println("Send data to Jeedom");            delay(10);      String baseurl = "/core/api/jeeApi.php?apikey=";       baseurl += apiKey;      baseurl += "&type=virtual&id=";       String url = baseurl + IdMode;      url += url + "&value="; url += String(ModeAuto);  // Envoi la requete au serveur - This will send the request to the server      sendToJeedom(url);      delay(1500); }}  void setup() {    pinMode(altPos, OUTPUT);  pinMode(altNeg, OUTPUT);  pinMode(BpMode, INPUT);  pinMode(LED_BUILTIN, OUTPUT);// Initialize the LED_BUILTIN pin as an output  Serial.begin(115200);  Serial.println("Loading");  delay(10);  Serial.setDebugOutput(true);    Serial.println("Connecting Wifi...");   WiFi.begin(ssid, password);   //Initialise la connection Wifi  while (WiFi.status() != WL_CONNECTED) {    delay(500);    Serial.print(".");  }    Serial.println("");  Serial.println("WiFi connected");  Serial.println("IP address: ");  Serial.print(WiFi.localIP());  delay(10);  server.on("/gpio", updateMode);  server.begin();   //Initialise le server client}void confort() {    //Mode confort : Pas de signal    digitalWrite(altPos, LOW);    digitalWrite(altNeg, LOW);    delay(10);}void eco() {    //Mode Eco : Pleine alternance    digitalWrite(altPos, HIGH);    digitalWrite(altNeg, HIGH);    delay(10);}void arret() {    //Mode Arrêt : Alternance Positive    digitalWrite(altPos, HIGH);    digitalWrite(altNeg, LOW);    delay(10); }void horsGel() {    //Mode Hors Gel : Alternance negative    digitalWrite(altPos, LOW);    digitalWrite(altNeg, HIGH);    delay(10); }void confortMoins1(){   //Mode confort - 1 : Rien pendant 4'57'' puis Pleine alternance pendant 3 ''    previousMillis = millis();    //Initialize the buffer    int i = 1;    //Loop 4'57        while(i == 1){     currentMillis = millis();     digitalWrite(altPos, LOW);     digitalWrite(altNeg, LOW);     delay(20);     Serial.println("4 minutes 57");     bpModeState = digitalRead(BpMode);     unsigned long calcul;     calcul = time1 - (currentMillis - previousMillis);     Serial.print("temps restant = ");     Serial.println (calcul);        // If an order is available, then exit the loop          if (ModeAuto != "2") {        i = 0;        delay(10);        }             // Time condition             if (currentMillis - previousMillis >= time1) {      previousMillis =millis();      // Loop 3''            int j = 1;      while(j == 1) {            bpModeState = digitalRead(BpMode);             // If an order is available, then exit the loop                          if (ModeAuto != "2") {            j = 0;            delay(10);            }           currentMillis = millis();        digitalWrite(altPos, HIGH);        digitalWrite(altNeg, HIGH);        Serial.println("3 secondes");        delay(20);        // Time condition                if (currentMillis - previousMillis >= time3) {          i = 0;          j = 0;        }      }     }    } }void confortMoins2(){   //Mode confort - 2 : Rien pendant 4'53'' puis Pleine alternance pendant 7 ''    previousMillis = millis();    //Initialize the buffer    int i = 1;    //Loop 4'53        while(i == 1){     currentMillis = millis();     digitalWrite(altPos, LOW);     digitalWrite(altNeg, LOW);     delay(20);     Serial.println("4 minutes 53");     bpModeState = digitalRead(BpMode);     unsigned long calcul;     calcul = time2 - (currentMillis - previousMillis);     Serial.print("temps restant = ");     Serial.println (calcul);        // If an order is available, then exit the loop          if (ModeAuto != "3") {        i = 0;        delay(10);        }             // Time condition             if (currentMillis - previousMillis >= time2) {      previousMillis =millis();      // Loop 7''            int j = 1;      while(j == 1) {             // If an order is available, then exit the loop             bpModeState = digitalRead(BpMode);             if (ModeAuto != "3") {            j = 0;            delay(10);            }           currentMillis = millis();        digitalWrite(altPos, HIGH);        digitalWrite(altNeg, HIGH);        Serial.println("7 secondes");        delay(20);        // Time condition                if (currentMillis - previousMillis >= time4) {          i = 0;          j = 0;        }      }     }    }}void loop() {  /*Action du bouton -> changement de mode,* mode 0 Auto (requette HTTP)* mode 1 confort* mode 2 confort -1* mode 3 confort -2* mode 4 Eco* mode 5 Hors Gel* mode 6 Arrêt*/ server.handleClient();    bpModeState = digitalRead(BpMode); delay(10);    if (bpModeState == 1) {   mOde++;   if (mOde > 6) {       mOde = 0;    }  Serial.print("Bouton Mode appuyé. Mode selectionné : ");  Serial.println(mOde);  set = 1;  delay(500); }    if (set == 1){         for (int i = 0; i < 10; i++) {        digitalWrite(LED_BUILTIN, LOW);        delay(175);        digitalWrite(LED_BUILTIN, HIGH);        delay(175);    }    set = 0;    }    digitalWrite(LED_BUILTIN, LOW);    Serial.println("Mode Auto activé");   /* if(Serial.available()>0) {    // get incoming byte:    answer = Serial.read();    delay(10);    Serial.print("answer = ");    Serial.println(answer);    }*/   if (ModeAuto == "1") {      currentMillishttp = millis();      if (currentMillishttp - previousMillishttp > watchdogHttp){        SendData();        delay(10);        previousMillishttp = millis();        delay(10);      }      Serial.println("Mode 1");            confort();    }    else if (ModeAuto == "2"){            currentMillishttp = millis();      if (currentMillishttp - previousMillishttp > watchdogHttp){        SendData();        delay(10);        previousMillishttp = millis();        delay(10);      }      Serial.println("Mode 2");      confortMoins1();    }    else if (ModeAuto == "3"){           if (currentMillishttp - previousMillishttp > watchdogHttp){        SendData();        delay(10);        previousMillishttp = millis();        delay(10);      }      Serial.println("Mode 3");      confortMoins2();    }    else if (ModeAuto == "4"){            currentMillishttp = millis();      if (currentMillishttp - previousMillishttp > watchdogHttp){        SendData();        delay(10);        previousMillishttp = millis();        delay(10);      }      Serial.println("Mode 4");      eco();    }    else if (ModeAuto == "5"){            currentMillishttp = millis();      if (currentMillishttp - previousMillishttp > watchdogHttp){        SendData();        delay(10);        previousMillishttp = millis();        delay(10);      }      Serial.println("Mode 5");      horsGel();    }    else if (ModeAuto == "6"){            currentMillishttp = millis();      if (currentMillishttp - previousMillishttp > watchdogHttp){        SendData();        delay(10);        previousMillishttp = millis();        delay(10);      }      Serial.println("Mode 6");      arret();       }       //unsigned long currentMillishttp = millis();}