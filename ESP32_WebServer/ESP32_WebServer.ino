/*
  ESP32 Web Server - Penyiraman Otomatis Dashboard
  
  Features:
  - Reads capacitive soil moisture sensors (raw ADC → percentage)
  - Serves minimalist fullscreen web dashboard
  - Real-time updates via WebSocket
  - Auto pump/valve control based on moisture levels
  - Accessible from any device on same WiFi network
  - When ESP32 is OFF, dashboard is unreachable
  
  Libraries required (install via Arduino Library Manager):
  - WebSockets by Markus Sattler
  - ArduinoJson by Benoit Blanchon
*/

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "dashboard.h"

// ============ WiFi CONFIG — GANTI DENGAN MILIK ANDA ============
const char* WIFI_SSID = "KOST DIAN";
const char* WIFI_PASS = "qwerty123";

// ============ SENSOR PINS ============
// 3 sensor slots
const int SENSOR_PINS[] = {32, 33, 34};
const int NUM_SENSORS = 3;
const int ACTIVE_SENSORS = 3; // Menggunakan 3 sensor aktif

// ============ ACTUATOR PINS ============
const int PUMP_PIN = 26;
const int VALVE_PIN = 27;

// ============ CALIBRATION ============
// Sesuaikan dengan sensor Anda!
// Cara kalibrasi:
// 1. Sensor di UDARA → catat raw value → DRY_VALUE
// 2. Sensor di AIR → catat raw value → WET_VALUE
const int DRY_VALUE = 4095;
const int WET_VALUE = 1500;

// ============ CONTROL THRESHOLDS ============
// < 30%  = SANGAT KERING → pump ON
// 30-45% = AGAK KERING   → pump ON
// 50-70% = IDEAL          → pump OFF (target)
// > 80%  = TERLALU BASAH  → pump OFF
const int THRESHOLD_ON  = 45;  // Nyalakan pump di bawah ini
const int THRESHOLD_OFF = 50;  // Matikan pump di atas ini

// ============ GLOBALS ============
WebServer server(80);
WebSocketsServer ws(81);

struct SensorData {
  int raw;
  int percent;
  String level;
};

SensorData sensorData[NUM_SENSORS];
bool pumpOn = false;
bool valveOn = false;
String controlMode = "AUTO";
unsigned long lastRead = 0;
const unsigned long READ_INTERVAL = 2000;

// ============ SETUP ============
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Penyiraman Otomatis ESP32 ===");

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(VALVE_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(VALVE_PIN, LOW);

  analogSetAttenuation(ADC_11db);

  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorData[i] = {0, 0, "N/A"};
  }

  connectWiFi();

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server on port 80");

  ws.begin();
  ws.onEvent(wsEvent);
  Serial.println("WebSocket server on port 81");
}

// ============ LOOP ============
void loop() {
  server.handleClient();
  ws.loop();

  if (millis() - lastRead >= READ_INTERVAL) {
    lastRead = millis();
    readSensors();
    if (controlMode == "AUTO") autoControl();
    broadcastData();
  }
}

// ============ WiFi ============
void connectWiFi() {
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("Dashboard: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Failed! Restarting in 5s...");
    delay(5000);
    ESP.restart();
  }
}

// ============ Sensor Reading ============
void readSensors() {
  for (int i = 0; i < ACTIVE_SENSORS; i++) {
    int raw = analogRead(SENSOR_PINS[i]);
    int pct = map(raw, DRY_VALUE, WET_VALUE, 0, 100);
    pct = constrain(pct, 0, 100);

    sensorData[i].raw = raw;
    sensorData[i].percent = pct;
    sensorData[i].level = getLevel(pct);

    Serial.printf("S%d: raw=%d pct=%d%% [%s]\n",
      i + 1, raw, pct, sensorData[i].level.c_str());
  }
}

String getLevel(int pct) {
  if (pct < 30) return "SANGAT KERING";
  if (pct <= 45) return "AGAK KERING";
  if (pct <= 70) return "IDEAL";
  if (pct <= 80) return "BASAH";
  return "TERLALU BASAH";
}

// ============ Auto Control Logic ============
void autoControl() {
  int sum = 0;
  for (int i = 0; i < ACTIVE_SENSORS; i++) {
    sum += sensorData[i].percent;
  }
  int avg = sum / max(ACTIVE_SENSORS, 1);

  if (avg < THRESHOLD_ON && !pumpOn) {
    pumpOn = true;
    valveOn = true;
    digitalWrite(PUMP_PIN, HIGH);
    digitalWrite(VALVE_PIN, HIGH);
    Serial.println("AUTO: Tanah kering → Pump ON, Valve ON");
  } else if (avg >= THRESHOLD_OFF && pumpOn) {
    pumpOn = false;
    valveOn = false;
    digitalWrite(PUMP_PIN, LOW);
    digitalWrite(VALVE_PIN, LOW);
    Serial.println("AUTO: Tanah ideal → Pump OFF, Valve OFF");
  }
}

// ============ Web Server ============
void handleRoot() {
  server.send_P(200, "text/html", DASHBOARD_HTML);
}

// ============ WebSocket Events ============
void wsEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("Client %u connected\n", num);
      broadcastData();
      break;
    case WStype_DISCONNECTED:
      Serial.printf("Client %u disconnected\n", num);
      break;
    case WStype_TEXT:
      handleWsMessage(num, payload, length);
      break;
    default:
      break;
  }
}

void handleWsMessage(uint8_t num, uint8_t* payload, size_t length) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) return;

  String action = doc["action"].as<String>();

  if (action == "setMode") {
    controlMode = doc["mode"].as<String>();
    Serial.println("Mode changed: " + controlMode);
  } else if (action == "setPump" && controlMode == "MANUAL") {
    pumpOn = doc["state"].as<bool>();
    digitalWrite(PUMP_PIN, pumpOn ? HIGH : LOW);
  } else if (action == "setValve" && controlMode == "MANUAL") {
    valveOn = doc["state"].as<bool>();
    digitalWrite(VALVE_PIN, valveOn ? HIGH : LOW);
  } else if (action == "emergency") {
    pumpOn = false;
    valveOn = false;
    digitalWrite(PUMP_PIN, LOW);
    digitalWrite(VALVE_PIN, LOW);
    controlMode = "MANUAL";
    Serial.println("EMERGENCY STOP!");
  }

  broadcastData();
}

// ============ Broadcast Data via WebSocket ============
void broadcastData() {
  JsonDocument doc;

  JsonArray sArr = doc["sensors"].to<JsonArray>();
  for (int i = 0; i < NUM_SENSORS; i++) {
    JsonObject s = sArr.add<JsonObject>();
    s["id"] = i + 1;
    s["active"] = (i < ACTIVE_SENSORS);
    s["raw"] = sensorData[i].raw;
    s["percent"] = sensorData[i].percent;
    s["level"] = sensorData[i].level;
  }

  int sum = 0;
  for (int i = 0; i < ACTIVE_SENSORS; i++) {
    sum += sensorData[i].percent;
  }
  int avg = (ACTIVE_SENSORS > 0) ? sum / ACTIVE_SENSORS : 0;

  doc["average"] = avg;
  doc["avgLevel"] = getLevel(avg);
  doc["pump"] = pumpOn;
  doc["valve"] = valveOn;
  doc["mode"] = controlMode;
  doc["uptime"] = millis() / 1000;
  doc["ip"] = WiFi.localIP().toString();

  String json;
  serializeJson(doc, json);
  ws.broadcastTXT(json);
}
