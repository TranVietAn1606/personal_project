#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "NotNetw"
#define WIFI_PASSWORD "abc123123"

/* 2. Define the API Key */
//#define API_KEY "AIzaSyDFA7_eMjZpkGSaTDduGsrhLTGQLc3jSa8"
#define API_KEY "AIzaSyAg5wBF5oUxZ_Y97vO63Byr2Fx9OEP33oI"


/* 3. Define the RTDB URL */
//#define DATABASE_URL "apploramogre-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

#define DATABASE_URL "embeddedsystemhieu-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app


/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "abc123@gmail.com"
#define USER_PASSWORD "abc123123"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
FirebaseData firebaseData;

unsigned long sendDataPrevMillis = 0;

int count = 0;
void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  //Or use legacy authenticate method
  //Firebase.begin(DATABASE_URL, "<database secret>");
}

void loop()
{
  float tempA = random(10, 40);
  int humA = random(10, 90);
  int lightA = random(10, 60);
  int soilA = random(10, 99);
  int rssiA = random(1, 20);

  float tempB = random(10.00, 40.00);
  int humB = random(10, 90);
  int lightB = random(10, 60);
  int soilB = random(10, 99);
  int rssiB = random(1, 20);
  //Node1
  Firebase.setFloat(firebaseData, "Node/Node1/temperature", tempA);
  Firebase.setInt(firebaseData, "Node/Node1/humidity", humA);
  Firebase.setInt(firebaseData, "Node/Node1/soilAnalog", soilA);
  Firebase.setString(firebaseData, "Node/Node1/soilDigital", "1");
  Firebase.setInt(firebaseData, "Node/Node1/lightAnalog", lightA);
  Firebase.setString(firebaseData, "Node/Node1/lightDigital", "0");


  //Node2
  Firebase.setFloat(firebaseData, "Node/Node2/temperature", tempB);
  Firebase.setInt(firebaseData, "Node/Node2/humidity", humB);
  Firebase.setInt(firebaseData, "Node/Node2/soilAnalog", soilB);
  Firebase.setString(firebaseData, "Node/Node2/soilDigital", "1");
  Firebase.setInt(firebaseData, "Node/Node2/lightAnalog", lightB);
  Firebase.setString(firebaseData, "Node/Node2/lightDigital", "0");
// fbdo.errorReason().c_str());
//  Serial.println("OK");


  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    ////////////////node1
    //    float tempA = random(10, 40);
    //    int humA = random(10, 90);
    //    int lightA = random(10, 60);
    //    int soilA = random(10, 99);
    //    int rssiA = random(1, 20);

    FirebaseJson jsonA;
    jsonA.add("temperature", tempA);
    jsonA.add("humidity", humA);
    jsonA.add("light", lightA);
    jsonA.add("soil", soilA);
    jsonA.add("rssi", rssiA);

    Serial.printf("Push json... %s\n", Firebase.pushJSON(fbdo, "/NodeLoRa/Node1", jsonA) ? "ok" : fbdo.errorReason().c_str());
    jsonA.set("temperature", tempA);
    jsonA.set("humidity", humA);
    jsonA.set("light", lightA);
    jsonA.set("soil", soilA);
    jsonA.set("rssi", rssiA);
    Serial.printf("Update json... %s\n\n", Firebase.updateNode(fbdo, String("/NodeLoRa/Node1/" + fbdo.pushName()), jsonA) ? "ok" : fbdo.errorReason().c_str());


    /////////////node2
    FirebaseJson jsonB;
    //    float tempB = random(10.00, 40.00);
    //    int humB = random(10, 90);
    //    int lightB = random(10, 60);
    //    int soilB = random(10, 99);
    //    int rssiB = random(1, 20);

    jsonB.add("temperature", tempB);
    jsonB.add("humidity", humB);
    jsonB.add("light", lightB);
    jsonB.add("soil", soilB);
    jsonB.add("rssi", rssiB);

    Serial.printf("Push json... %s\n", Firebase.pushJSON(fbdo, "/NodeLoRa/Node2", jsonB) ? "ok" : fbdo.errorReason().c_str());
    jsonB.set("temperature", tempB);
    jsonB.set("humidity", humB);
    jsonB.set("light", lightB);
    jsonB.set("soil", soilB);
    jsonB.set("rssi", rssiB);
    Serial.printf("Update json... %s\n\n", Firebase.updateNode(fbdo, String("/NodeLoRa/Node2/" + fbdo.pushName()), jsonB) ? "ok" : fbdo.errorReason().c_str());

    count++;
    if (count > 99 ) {
      tempA = humA = lightA = soilA = rssiA = 0;
      tempB = humB = lightB = soilB = rssiB = 0;
    }
  }
}
