//#include <DHT.h>
//
//#define dht_pin 3
//#define light_pin_d 5
//#define light_pin_a A4
//#define soil_pin_d 4
//#define soil_pin_a A5
//#define DHTTYPE DHT11
//DHT dht(dht_pin, DHTTYPE);
//
//
//void setup() {
////  pinMode(light_pin_d, INPUT);
////  pinMode(light_pin_a, INPUT);
////  pinMode(soil_pin_d, INPUT);
////  pinMode(soil_pin_a, INPUT);
//
//  dht.begin();
//  Serial.begin(9600);
//  Serial.println("Start: ");
//}
//
//void loop() {
//  float t = dht.readTemperature();
//  float h = dht.readHumidity();
//  int sd = digitalRead(soil_pin_d);
//  int sa = analogRead(soil_pin_a);
//  int ld = digitalRead(light_pin_d);
//  int la = analogRead(light_pin_a);
//  Serial.print("Nhiet do: ");
//  Serial.print(t);
//  Serial.print("\tdo am: ");
//  Serial.print(h);
//  Serial.print("\tsd: ");
//  Serial.print(sd);
//  Serial.print("\tsa: ");
//  Serial.print(sa);
//  Serial.print("\tld: ");
//  Serial.print(ld);
//  Serial.print("\tla: ");
//  Serial.println(la);
//  delay(500);
//
//}

//String mes = "23.766,12.7,0,126,1";
String mes = "23.766,12.7,0";
void setup() {
  Serial.begin(9600);
}

void loop() {
  String a = getValue(mes, ',', 0);
  String b = getValue(mes, ',', 1);
  String c = getValue(mes, ',', 2);
  //  String d = getValue(mes, ',', 3);
  //  String e = getValue(mes, ',', 4);

  Serial.println(mes);
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  //  Serial.println(d);
  //  Serial.println(e);
  Serial.println("----------------------");
  mes = "";
  delay(1000);
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
}
