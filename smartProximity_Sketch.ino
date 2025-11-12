#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

// WiFi credentials
#define WIFI_SSID "eyeFon"
#define WIFI_PASSWORD "12345678"

// Firebase credentials
#define API_KEY "AIzaSyDHUgL8d7RocaacutTOm1ehDWzrbp9g86M"
#define DATABASE_URL "https://smartproximity-26075-default-rtdb.firebaseio.com"
#define USER_EMAIL "xyz@gmail.com"
#define USER_PASSWORD "12345678"

// MQTT Public Broker (no auth)
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

// Pins
#define TRIG_PIN D0
#define ECHO_PIN D1
#define LED_PIN D3
#define BUZZER_PIN D5

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long lastSend = 0;
int alertThreshold = 50;  // Default alert threshold

// ========== MQTT CALLBACK ==========
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "smartproximity/control/led") {
    digitalWrite(LED_PIN, (message == "on") ? HIGH : LOW);
  } else if (String(topic) == "smartproximity/control/buzzer") {
    (message == "on") ? tone(BUZZER_PIN, 1000) : noTone(BUZZER_PIN);
  } else if (String(topic) == "smartproximity/control/threshold") {
    alertThreshold = message.toInt();
    Serial.print("Threshold updated to: ");
    Serial.println(alertThreshold);
  }
}

// ========== MQTT Setup ==========
void setupMQTT() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      mqttClient.subscribe("smartproximity/control/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

// ========== Distance Measurement ==========
float getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void controlAlerts(float distance, String& alertMsg) {
  if (distance > 0 && distance < alertThreshold) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
    alertMsg = "Object nearby / Alert ON";
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    alertMsg = "Clear / Alert OFF";
  }
}

// ========== Setup ==========
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);

  // MQTT
  setupMQTT();
}

// ========== Loop ==========
void loop() {
  if (!mqttClient.connected()) reconnectMQTT();
  mqttClient.loop();

  if (Firebase.ready() && millis() - lastSend > 3000) {
    lastSend = millis();

    float distance = getDistanceCM();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    String alertMsg;
    controlAlerts(distance, alertMsg);

    // Firebase Upload
    if (!Firebase.RTDB.setFloat(&fbdo, "/distance/cm", distance)) {
      Serial.println("Firebase distance update failed: " + fbdo.errorReason());
    }
    if (!Firebase.RTDB.setString(&fbdo, "/alerts/status", alertMsg)) {
      Serial.println("Firebase alert update failed: " + fbdo.errorReason());
    }

    // MQTT Publish
    mqttClient.publish("smartproximity/distance", String(distance).c_str(), true);
    mqttClient.publish("smartproximity/alert", alertMsg.c_str(), true);
  }
}
