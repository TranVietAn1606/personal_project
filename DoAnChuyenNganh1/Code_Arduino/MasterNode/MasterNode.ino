#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

byte msgCount = 0;
byte NodeTX1 = 0xAA;
byte NodeTX2 = 0xBB;
byte MasterNode = 0xAB;

String SenderNode = "";
String outgoing = "";
String incoming = "";
int counter = 0;

int txPower = 14; // from 0 to 20, default is 14
int spreadingFactor = 9; // from 7 to 12, default is 12
long signalBandwidth = 125E3; // 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3,41.7E3,62.5E3,125E3,250E3,500e3, default is 125E3
int codingRateDenominator = 5; // Numerator is 4, and denominator from 5 to 8, default is 5
int preambleLength = 8; // from 2 to 20, default is 8
String payload = "hello"; // you can change the payload

//ESP32
#define SS 15
#define RST 4
#define DI0 2
#define BAND 433E6
#define WIFI_SSID "NotNetw"
#define WIFI_PASSWORD "abc123123"
#define FIREBASE_HOST "loragreenhouse-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "W0058uW3nOLweW9zGkS8PGW3yOf7p552xzDt7FOu"
FirebaseData firebaseData;
void setup() {
  Serial.begin(115200);
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
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
}

void onReceive(int packetSize) {
  if (packetSize == 0) return ;
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address

  if ( sender == 0xAA ) {                // address Node Sender
    SenderNode = "NodeTX1:";
    byte incomingMsgId = LoRa.read();     // incoming msg ID
    byte incomingLength = LoRa.read();    // incoming msg length
    Serial.println(SenderNode);
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

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

    Firebase.setFloat(firebaseData, "Node1/Sensor/DHT11/temperature", temp);
    Firebase.setFloat(firebaseData, "Node1/Sensor/DHT11/humidity", hum);
    Firebase.setInt(firebaseData, "Node1/Sensor/Soid/Analog/", saVal);
    Firebase.setInt(firebaseData, "Node1/Sensor/Soid/Digital/", sdVal);
    Firebase.setInt(firebaseData, "Node1/Sensor/Light/Analog/", laVal);
    Firebase.setInt(firebaseData, "Node1/Sensor/Light/Digital/", ldVal);

    Firebase.setString(firebaseData, "Node1/Device/Pump/", "ON");


    Serial.print("temp Node1: "); Serial.println(temp);
    Serial.print("hum Node1: "); Serial.println(hum);
    Serial.print("Soil Digital Node1: "); Serial.println(sdVal);
    Serial.print("Soil Analog Node1: "); Serial.println(saVal);
    Serial.print("Light Digital Node1: "); Serial.println(ldVal);
    Serial.print("Light Analog Node1: "); Serial.println(laVal);
    Serial.println("------------------------------------------");
    incoming = "";

  }
  if ( sender == 0xBB ) {                // address Node Sender
    SenderNode = "NodeTX2:";
    byte incomingMsgId = LoRa.read();     // incoming msg ID
    byte incomingLength = LoRa.read();    // incoming msg length
    Serial.println(SenderNode);
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

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


    Firebase.setFloat(firebaseData, "Node2/Sensor/DHT11/temperature", temp);
    Firebase.setFloat(firebaseData, "Node2/Sensor/DHT11/humidity", hum);
    Firebase.setInt(firebaseData, "Node2/Sensor/Soid/Analog/", saVal);
    Firebase.setInt(firebaseData, "Node2/Sensor/Soid/Digital/", sdVal);
    Firebase.setInt(firebaseData, "Node2/Sensor/Light/Analog/", laVal);
    Firebase.setInt(firebaseData, "Node2/Sensor/Light/Digital/", ldVal);

    Firebase.setString(firebaseData, "Node1/Device/Pump/", "ON");
    Serial.print("temp Node2: "); Serial.println(temp);
    Serial.print("hum Node2: "); Serial.println(hum);
    Serial.print("Soil Digital Node2: "); Serial.println(sdVal);
    Serial.print("Soil Analog Node2: "); Serial.println(saVal);
    Serial.print("Light Digital Node2: "); Serial.println(ldVal);
    Serial.print("Light Analog Node2: "); Serial.println(laVal);
    Serial.println("------------------------------------------");
    incoming = "";
  }
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
