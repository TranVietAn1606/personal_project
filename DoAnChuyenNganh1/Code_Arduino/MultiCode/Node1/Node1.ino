#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <DHT.h>

#define dht_pin 3
#define light_pin_d 5
#define light_pin_a A4
#define soil_pin_d 8
#define soil_pin_a A5
#define DHTTYPE DHT11
DHT dht(dht_pin, DHTTYPE);

byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xAB;
byte Node1 = 0xAA;

int txPower = 14; // from 0 to 20, default is 14
int spreadingFactor = 9; // from 7 to 12, default is 12
long signalBandwidth = 125E3; // 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3,41.7E3,62.5E3,125E3,250E3,500e3, default is 125E3 ..băng tầng của tín hiệu
int codingRateDenominator = 5; // Numerator is 4, and denominator from 5 to 8, default is 5
int preambleLength = 8; // from 2 to 20, default is 8
String payload = "";

//Arduino nano
#define SS 10
#define RST 9
#define DI0 2

#define BAND 433E6  // Here you define the frequency carrier //băng tần hoạt động

void setup() {
  Serial.begin(115200);                   // initialize serial
  while (!Serial);
  Serial.println("LoRa Sender");
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

  Serial.println("LoRa init succeeded.");
  dht.begin();
}

void loop() {
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(otherNode);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  if (incomingLength != incoming.length()) {   // check length for error
    // Serial.println("error: message length does not match length");
    ;
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
  Serial.println(incoming);
  int Val = incoming.toInt();
  if (Val == 34)
  {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    delay(10);
    int sd = digitalRead(soil_pin_d);
    delay(10);
    int sa = analogRead(soil_pin_a);
    delay(10);
    int ld = digitalRead(light_pin_d);
    delay(10);
    int la = analogRead(light_pin_a);
    delay(10);

    String message = "";
    message = message + t + "," + h + "," + sd + "," + sa + "," + ld + "," + la;
    sendMessage(message, MasterNode, Node1);
    delay(100);
  }
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



//#include<DHT.h>
//#define dht_pin 3
//#define DHTTYPE DHT11
//DHT dht(dht_pin, DHTTYPE);
//
//void setup() {
//  Serial.begin(115200);
//  dht.begin();
//}
//void loop() {
//  float t = dht.readTemperature();
//  float h = dht.readHumidity();
//  delay(10);
//  Serial.print(t);
//  Serial.print("-");
//  Serial.println(h);
//}
