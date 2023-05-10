#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

byte MasterNode = 0xAB;
byte Node1 = 0xAA;
byte Node2 = 0xBB;

String SenderNode = "";
String incoming = "";
byte msgCount = 0;

// Tracks the time since last event fired
unsigned long int previousMillis = 0;
unsigned long int previoussecs = 0;
unsigned long int currentsecs = 0;
unsigned long int currentMillis = 0;
int interval = 1 ; // updated every 1 second
int Secs = 0;

#define ledR 25
#define ledB 26


int txPower = 14; // from 0 to 20, default is 14
int spreadingFactor = 9; // from 7 to 12, default is 12
long signalBandwidth = 125E3; // 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3,41.7E3,62.5E3,125E3,250E3,500e3, default is 125E3
int codingRateDenominator = 5; // Numerator is 4, and denominator from 5 to 8, default is 5
int preambleLength = 8; // from 2 to 20, default is 8
String payload = "hello"; // you can change the payload

//#define WIFI_SSID "NotNetw"
//#define WIFI_PASSWORD "abc123123"
//ESP32
#define SS 15
#define RST 4
#define DI0 2
#define BAND 433E6

#define WIFI_SSID "NotNetw"
#define WIFI_PASSWORD "abc123123"

//#define FIREBASE_HOST "apploramogre-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH "acKPsX5aYpvXjXvXk9Bn2H769O32zaxgUQNMcveVV"
#define DATABASE_URL "apploramogre-default-rtdb.firebaseio.com"
#define API_KEY "AIzaSyDFA7_eMjZpkGSaTDduGsrhLTGQLc3jSa8"
#define USER_EMAIL "tranvietan2001@gmail.com"
#define USER_PASSWORD "abc123123"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
//FirebaseData firebaseData;

void setup() {
  pinMode(ledR, OUTPUT);
  pinMode(ledB, OUTPUT);
  Serial.begin(115200);                   // initialize serial

  while (!Serial);
  Serial.println("LoRa Receiver");
  Serial.print("SetFrequency : ");
  Serial.print(BAND);
  Serial.println("Hz");
  Serial.print("SetSpreadingFactor : SF");
  Serial.println(spreadingFactor);
  SPI.begin();
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setTxPower(txPower, 1);
  LoRa.setSpreadingFactor(spreadingFactor);
  LoRa.setSignalBandwidth(signalBandwidth);
  LoRa.setCodingRate4(codingRateDenominator);
  LoRa.setPreambleLength(preambleLength);

  // Serial.println("LoRa init succeeded.");
  connectWF();
//  connectFB();
}

void loop() {
  currentMillis = millis();
  currentsecs = currentMillis / 1000;
  if ((unsigned long)(currentsecs - previoussecs) >= interval) {
    Secs = Secs + 1;
    if ( Secs >= 21 )
    {
      Secs = 0;
    }
    if ((Secs >= 1) && (Secs <= 10)) {
      String message = "34";
      sendMessage(message, MasterNode, Node1);
    }
    if ((Secs >= 11 ) && (Secs <= 20)) {
      String message = "55";
      sendMessage(message, MasterNode, Node2);
    }
    previoussecs = currentsecs;
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());

}


void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(otherNode);              // add destination address
  LoRa.write(MasterNode);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  int led;
  String nodeName = "";
  if (packetSize == 0) return ;
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address

  if ( sender == Node1 ) {                // address Node Sender
    SenderNode = "NodeTX1:";
    led = ledR;
    nodeName = "Node1";
  }
  if ( sender == Node2 ) {                // address Node Sender
    SenderNode = "NodeTX2:";
    led = ledB;
    nodeName = "Node2";
  }

  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    //Serial.println("error: message length does not match length");
    ;
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode) {
    // Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
  Serial.println(nodeName);
  Serial.println(SenderNode);
  Serial.println(incoming);
  String t = getValue(incoming, ',', 0);
  String h = getValue(incoming, ',', 1);
  String sd = getValue(incoming, ',', 2);
  String sa = getValue(incoming, ',', 3);
  String ld = getValue(incoming, ',', 4);
  String la = getValue(incoming, ',', 5);

  float temp = t.toFloat();
  float hum = h.toFloat();
  int sdVal = sd.toInt();
  int saVal = sa.toInt();
  int ldVal = ld.toInt();
  int laVal = la.toInt();
  Serial.println("------------------------");
  ledBlink(led);
  sendDataFirebase(nodeName, temp, hum, saVal, sdVal, laVal, ldVal);
}



String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex [] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void sendDataFirebase(String nodeName, float temp, float hum, int sa, int sd, int la, int ld) {
  //  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //  Firebase.reconnectWiFi(true);
  int Rssi = LoRa.packetRssi();
  connectFB();
  //  Serial.println(Firebase.ready());
  if (Firebase.ready()) {
    Serial.printf("Set Temperature... %s\n", Firebase.setFloat(fbdo, "Node/" + nodeName + "/temperature", temp) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set humidity... %s\n", Firebase.setInt(fbdo, "Node/" + nodeName + "/humidity", hum) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set soilanalog... %s\n", Firebase.setInt(fbdo, "Node/" + nodeName + "/soilAnalog", sa) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set soilDigital... %s\n", Firebase.setInt(fbdo, "Node/" + nodeName + "/soilDigital", sd) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set lightAnalog... %s\n", Firebase.setInt(fbdo, "Node/" + nodeName + "/lightAnalog", la) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set lightDigital... %s\n", Firebase.setInt(fbdo, "Node/" + nodeName + "/lightDigital", ld) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set RSSI... %s\n", Firebase.setInt(fbdo, "Node/" + nodeName + "/RSSI", Rssi) ? "ok" : fbdo.errorReason().c_str());

    //    Firebase.setFloat(firebaseData, "Node/" + nodeName + "/temperature", temp);
    //    Firebase.setFloat(firebaseData, "Node/" + nodeName + "/humidity", hum);
    //    Firebase.setInt(firebaseData, "Node/" + nodeName + "/soilAnalog", sa);
    //    Firebase.setInt(firebaseData, "Node/" + nodeName + "/soilDigital", sd);
    //    Firebase.setInt(firebaseData, "Node/" + nodeName + "/lightAnalog", la);
    //    Firebase.setInt(firebaseData, "Node/" + nodeName + "/lightDigital", ld);

    FirebaseJson json;
    json.add("temperature", temp);
    json.add("humidity", hum);
    json.add("lightAnalog", la);
    json.add("lightDigital", ld);
    json.add("soilAnalog", sa);
    json.add("soilDigital", sd);
    json.add("RSSI", Rssi);

    Serial.println("------");
    Serial.printf("Push json... %s\n", Firebase.pushJSON(fbdo, "/NodeLoRa/" + nodeName + "/", json) ? "ok" : fbdo.errorReason().c_str());
    //    json.set("temperature", temp);
    //    json.set("humidity", hum);
    //    json.set("lightAnalog", la);
    //    json.set("lightDigital", ld);
    //    json.set("soilAnalog", sa);
    //    json.set("soilDigital", sd);
    //    json.set("RSSI", Rssi);

    json.set("temperature", temp);
    json.set("humidity", hum);
    json.set("light", la);
    json.set("soil", sa);
    json.set("rssi", Rssi);
    Serial.printf("Update json... %s\n\n", Firebase.updateNode(fbdo, String("/NodeLoRa/" + nodeName + "/" + fbdo.pushName()), json) ? "ok" : fbdo.errorReason().c_str());

  }
  //  Firebase.setFloat(firebaseData, "Node/" + nodeName + "/temperature", temp);
  //  Firebase.setFloat(firebaseData, "Node/" + nodeName + "/humidity", hum);
  //  Firebase.setInt(firebaseData, "Node/" + nodeName + "/soilAnalog", sa);
  //  Firebase.setInt(firebaseData, "Node/" + nodeName + "/soilDigital", sd);
  //  Firebase.setInt(firebaseData, "Node/" + nodeName + "/lightAnalog", la);
  //  Firebase.setInt(firebaseData, "Node/" + nodeName + "/lightDigital", ld);
}

void ledBlink(int led) {
  digitalWrite(led, 1);
  delay(200);
  digitalWrite(led, 0);
  delay(200);
}

void connectWF() {
  Serial.println("[WIFI] Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectFB() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
}
