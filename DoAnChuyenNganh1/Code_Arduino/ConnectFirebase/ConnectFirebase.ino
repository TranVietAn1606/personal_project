#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "NotNetw"
#define WIFI_PASSWORD "abc123123"

#define FIREBASE_HOST "loramoan-9bb7e-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "a8kT7KYSV0tuydE11UaUUNM9s4M8uWuZW9nzyM9R"
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

}

void loop() {

  //Node1
  Firebase.setString(firebaseData, "Node1/temperature", "23.00");
  Firebase.setString(firebaseData, "Node1/humidity", "75.00");
  Firebase.setString(firebaseData, "Node1/soid_anl", "960");
  Firebase.setString(firebaseData, "Node1/soid_dig", "1");
  Firebase.setString(firebaseData, "Node1/light_anl", "234");
  Firebase.setString(firebaseData, "Node1/light_dig", "0");


  //Node2
  Firebase.setString(firebaseData, "Node2/temperature", "23.00");
  Firebase.setString(firebaseData, "Node2/humidity", "75.00");
  Firebase.setString(firebaseData, "Node2/soid_anl", "960");
  Firebase.setString(firebaseData, "Node2/soid_dig", "1");
  Firebase.setString(firebaseData, "Node2/light_anl", "234");
  Firebase.setString(firebaseData, "Node2/light_dig", "0");

  String node1 = "Node1";
  String node2 = "Node2";
  String s = "/temperation";
  String path1 = node1+s;
  String path2 = node2+s;
  Serial.println(path1);
  Serial.println(path2);

}
