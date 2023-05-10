#include <SPI.h>
#include <LoRa.h>
//#include <WiFi.h>
//#include <FirebaseESP32.h>

byte msgCount = 0;
byte NodeTX1 = 0xAA;
byte NodeTX2 = 0xBB;
byte MasterNode = 0xAB;

String SenderNode = "";
String outgoing = "";
int counter = 0;

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

#define ledR 25
#define ledB 26


// Tracks the time since last event fired
unsigned long previousMillis = 0;
unsigned long int previoussecs = 0;
unsigned long int currentsecs = 0;
unsigned long currentMillis = 0;
int interval = 1 ; // updated every 1 second
int Secs = 0;


void setup() {
  pinMode(ledB, OUTPUT);
  pinMode(ledR, OUTPUT);
  Serial.begin(115200);
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
}

void loop() {

  onReceive(LoRa.parsePacket());
  //  // print RSSI of packet
  //  Serial.print("' with RSSI ");
  //  Serial.println(LoRa.packetRssi());
  //  delay(200);
}

void onReceive(int packetSize) {

//  currentMillis = millis();
//  currentsecs = currentMillis / 1000;
//  if ((unsigned long)(currentsecs - previoussecs) >= interval) {
//    Secs = Secs + 1;
//    if ( Secs >= 11 ) Secs = 0;
//
//    if ((Secs >= 1) && (Secs <= 5)) {
//
//    }
//    if ((Secs >= 6 ) && (Secs <= 10)) {
//
//    }
//
//    previoussecs = currentsecs;
//  }

  int led;
  String incoming = "";
  if (packetSize == 0) return ;
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  while (LoRa.available())
    incoming += (char)LoRa.read();
  if (incomingLength != incoming.length()) return;
  if (recipient != MasterNode && sender != NodeTX1) return;
  if (recipient != MasterNode && sender != NodeTX2) return;
  //    else {
  if (sender == NodeTX1) {
    SenderNode = "NodeTX1:";
    led = ledR;
  }
  if (sender == NodeTX2) {
    SenderNode = "NodeTX2:";
    led = ledB;
  }

  //      SenderNode = "NodeTX2:";
  String t = getValue(incoming, ',', 0);
  String h = getValue(incoming, ',', 1);
  String sd = getValue(incoming, ',', 2);
  String sa = getValue(incoming, ',', 3);
  String ld = getValue(incoming, ',', 4);
  String la = getValue(incoming, ',', 5);
  Serial.println(incoming);

  float temp = t.toFloat();
  float hum = h.toFloat();
  int sdVal = sd.toInt();
  int saVal = sa.toInt();
  int ldVal = ld.toInt();
  int laVal = la.toInt();

  Serial.print("temp Node1: "); Serial.println(temp);
  Serial.print("hum Node1: "); Serial.println(hum);
  Serial.print("Soil Digital Node1: "); Serial.println(sdVal);
  Serial.print("Soil Analog Node1: "); Serial.println(saVal);
  Serial.print("Light Digital Node1: "); Serial.println(ldVal);
  Serial.print("Light Analog Node1: "); Serial.println(laVal);
  Serial.println("------------------------------------------");
  digitalWrite(led, 1);
  delay(200);
  digitalWrite(led, 0);
  delay(200);
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
