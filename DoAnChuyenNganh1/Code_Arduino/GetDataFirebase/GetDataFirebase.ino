
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Admin"
#define WIFI_PASSWORD "160601@n"

/* 2. Define the API Key */
#define API_KEY "AIzaSyDFA7_eMjZpkGSaTDduGsrhLTGQLc3jSa8"

#define DATABASE_URL "apploramogre-default-rtdb.firebaseio.com"

#define USER_EMAIL "tranvietan2001@gmail.com"
#define USER_PASSWORD "abc123123"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

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
//  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
//  {
    if(Firebase.ready()){
//    sendDataPrevMillis = millis();
    Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, "/Node/Node1/RSSI") ? String(fbdo.intData()).c_str() : fbdo.errorReason().c_str());
    //    sendDataPrevMillis = millis();
    //
    //    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/test/int", count) ? "ok" : fbdo.errorReason().c_str());
    //
    //    Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, "/test/int") ? String(fbdo.intData()).c_str() : fbdo.errorReason().c_str());
    //
    //    FirebaseJson json;
    //    json.add("value", count);
    //    json.add("temperature", 24);
    //
    //    Serial.printf("Push json... %s\n", Firebase.pushJSON(fbdo, "/test/push", json) ? "ok" : fbdo.errorReason().c_str());
    //
    //    json.set("value", count + 100);
    //    json.set("temperature", 24);
    //    Serial.printf("Update json... %s\n\n", Firebase.updateNode(fbdo, String("/test/push/" + fbdo.pushName()), json) ? "ok" : fbdo.errorReason().c_str());
    //
    //    count++;
  }
}
