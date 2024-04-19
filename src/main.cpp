#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>

// Temperature sensor setup
#define DHTPIN 4     // What digital pin the DHT11 is connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// MQTT setup
const char* mqtt_server = "Enter your IP address";
WiFiClient espClient;
PubSubClient client(espClient);

// Wi-Fi credentials
const char* ssid = "Enter your SSID";
const char* password = "Enter your password";

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Connect to the MQTT broker with a clean session
    if (client.connect("ESP32Client", NULL, NULL, NULL, 0, false, NULL, true)) {
      Serial.println("connected");
      // Resubscribe or publish as necessary
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("; try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  dht.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

float readTemperature() {
  float tempC = dht.readTemperature();
  if (isnan(tempC)) {
    Serial.println("Failed to read temperature from DHT11");
    return NAN;
  }
  return tempC;
}

float readHumidity(){
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity from DHT11");
    return NAN;
  }
  return humidity;
}

void sendTemperatureAndHumidity(float temperature, float humidity) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  char tempString[8], humString[8];
  dtostrf(temperature, 1, 2, tempString);
  dtostrf(humidity, 1, 2, humString);
  client.publish("home/room/temperature", tempString);
  client.publish("home/room/humidity", humString);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  float temperature = readTemperature();
  float humidity = readHumidity();
  if (!isnan(temperature) && !isnan(humidity)) {
    sendTemperatureAndHumidity(temperature, humidity);
  }
  delay(10000); // Wait for 10 seconds between readings
}
