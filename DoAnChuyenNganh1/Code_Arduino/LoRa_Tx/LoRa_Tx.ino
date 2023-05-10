#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define dht_pin 3
#define light_pin_d 5
#define light_pin_a A4
#define soil_pin_d 8
#define soil_pin_a A5
#define DHTTYPE DHT11
DHT dht(dht_pin, DHTTYPE);

byte msgCount = 0;
byte NodeTX1 = 0xAA;
byte NodeTX2 = 0xBB;
byte MasterNode = 0xAB;

float t = 0;
float h = 0;
int sd = 0;
int sa = 0;
int ld = 0;
int la = 0;

String Message = "";
String Messages = "";

int counter = 0;

// Parameters you can play with :

int txPower = 14; // from 0 to 20, default is 14
int spreadingFactor = 9; // from 7 to 12, default is 12
long signalBandwidth = 125E3; // 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3,41.7E3,62.5E3,125E3,250E3,500e3, default is 125E3 ..băng tầng của tín hiệu
int codingRateDenominator = 5; // Numerator is 4, and denominator from 5 to 8, default is 5
int preambleLength = 8; // from 2 to 20, default is 8
String payload = "hello"; // you can change the payload


////ESP32
//#define SS 15
//#define RST 4
//#define DI0 2

//Arduino nano
#define SS 10
#define RST 9
#define DI0 2

#define BAND 433E6  // Here you define the frequency carrier //băng tần hoạt động

void setup() {
  pinMode(light_pin_d, INPUT);
  pinMode(light_pin_a, INPUT);
  pinMode(soil_pin_d, INPUT);
  pinMode(soil_pin_a, INPUT);

  Serial.begin(115200);
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
  // LoRa.setPolarity(1);
  //LoRa.setFSK();
  dht.begin();
}

void loop() {
  t = dht.readTemperature();
  h = dht.readHumidity();
  delay(10);
  sd = digitalRead(soil_pin_d);
  delay(10);
  sa = analogRead(soil_pin_a);
  delay(10);
  //  Serial.println(sd);
  //  Serial.println(sa);
  ld = digitalRead(light_pin_d);
  delay(10);
  la = analogRead(light_pin_a);
  delay(10);

  //
  Messages = Messages + t + "," + h + "," + sd + "," + sa + "," + ld + "," + la;
  //  //  Messages = Messages + t + "," + h + "," + sd ;
  //
  //  Messages = "Hello! I am NODE 1";

  //  sendMessage(Messages, MasterNode, NodeTX1);
  sendMessage(Messages, MasterNode, NodeTX1);
  delay(100);
  Messages = "";
}

void sendMessage(String Message, byte MasterNodes, byte Nodes) {
  LoRa.beginPacket();             //start packet
  LoRa.write(MasterNodes);        //add destination address
  LoRa.write(Nodes);              //add sender address
  LoRa.write(msgCount);           //add message ID
  LoRa.write(Message.length());   //add payload lenght
  LoRa.print(Message);            //add payload
  LoRa.endPacket();               //finish packet and send payload
  Serial.println("send payload OK!!!");
  msgCount++;                     // increment message IP
}
