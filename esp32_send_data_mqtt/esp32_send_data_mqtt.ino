#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// MQTT broker details
const char* mqtt_broker = "192.168.0.141"; // Replace with your server's IP
const int mqtt_port = 1883;
const char* dataTopic = "esp32/data";

// IR sensor pin
const int irPin = 15;
int sensorState = LOW;

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(irPin, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Connect to MQTT broker
  client.setServer(mqtt_broker, mqtt_port);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32Sender")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, state: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  // Publish IR sensor state
  int currentState = digitalRead(irPin);
  if (currentState != sensorState) {
    sensorState = currentState;
    String data = (sensorState == HIGH) ? "Object Detected" : "No Object";
    client.publish(dataTopic, data.c_str());
    Serial.println("Published data: " + data);
  }

  delay(1000);
}
