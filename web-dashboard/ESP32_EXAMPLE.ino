/* 
  CONTOH CODE ESP32 UNTUK PENYIRAMAN OTOMATIS
  
  Kirim data sensor ke dashboard via HTTP POST
  Terima perintah dari server via polling
  
  Requirements:
  - Arduino IDE atau PlatformIO
  - ESP32 Board
  - Libraries:
    - WiFi.h (built-in)
    - HTTPClient.h (built-in)
    - ArduinoJson.h (install via Library Manager)
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ===== WiFi Config =====
const char* SSID = "YOUR_SSID";
const char* PASSWORD = "YOUR_PASSWORD";
const char* SERVER = "http://192.168.x.x:3000";

// ===== Pins =====
const int SENSOR1 = 36;  // Soil moisture 1
const int SENSOR2 = 39;  // Soil moisture 2
const int SENSOR3 = 34;  // Soil moisture 3
const int SENSOR4 = 35;  // Soil moisture 4
const int SENSOR5 = 33;  // Soil moisture 5
const int PUMP_RELAY = 26;    // Water pump
const int VALVE_RELAY = 27;   // Solenoid valve
const int LED_STATUS = 25;    // Status LED

// ===== Variables =====
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 5000;  // Send every 5 seconds

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=== Penyiraman Otomatis ESP32 ===");
  Serial.println("Version 1.0");
  
  // Initialize pins
  pinMode(PUMP_RELAY, OUTPUT);
  pinMode(VALVE_RELAY, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  digitalWrite(PUMP_RELAY, LOW);
  digitalWrite(VALVE_RELAY, LOW);
  digitalWrite(LED_STATUS, LOW);
  
  // ADC setup
  analogSetAttenuation(ADC_11db);
  
  // Connect WiFi
  connectToWiFi();
}

void loop() {
  // Reconnect WiFi if needed
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    connectToWiFi();
    return;
  }
  
  // Send data every UPDATE_INTERVAL
  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    sendSensorData();
  }
  
  delay(100);
}

// ===== WiFi Connection =====
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_STATUS, HIGH);
  } else {
    Serial.println("\n✗ WiFi Failed!");
    digitalWrite(LED_STATUS, LOW);
  }
}

// ===== Read Sensors =====
int readSoilMoisture(int pin) {
  int raw = analogRead(pin);
  
  // Calibration (adjust based on your sensor)
  int dryValue = 4095;   // Completely dry
  int wetValue = 1500;   // Completely wet
  
  int moisture = map(raw, dryValue, wetValue, 0, 100);
  return constrain(moisture, 0, 100);
}

// ===== Send Data to Server =====
void sendSensorData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }
  
  HTTPClient http;
  String url = String(SERVER) + "/api/esp32-data";
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  // Create JSON
  DynamicJsonDocument doc(512);
  
  // Read sensors
  JsonArray sensors = doc.createNestedArray("sensors");
  sensors[0]["moisture"] = readSoilMoisture(SENSOR1);
  sensors[0]["temperature"] = 25.5;  // Placeholder
  
  sensors[1]["moisture"] = readSoilMoisture(SENSOR2);
  sensors[1]["temperature"] = 25.5;
  
  sensors[2]["moisture"] = readSoilMoisture(SENSOR3);
  sensors[2]["temperature"] = 25.5;
  
  sensors[3]["moisture"] = readSoilMoisture(SENSOR4);
  sensors[3]["temperature"] = 25.5;
  
  sensors[4]["moisture"] = readSoilMoisture(SENSOR5);
  sensors[4]["temperature"] = 25.5;
  
  doc["pump"] = digitalRead(PUMP_RELAY) == HIGH;
  doc["valve"] = digitalRead(VALVE_RELAY) == HIGH;
  
  // Serialize and send
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("Sending: " + payload);
  
  int code = http.POST(payload);
  if (code == HTTP_CODE_OK) {
    Serial.println("✓ Data sent");
    digitalWrite(LED_STATUS, HIGH);
    delay(100);
    digitalWrite(LED_STATUS, LOW);
  } else {
    Serial.println("✗ Error: " + String(code));
  }
  
  http.end();
}

// ===== Debug Command via Serial =====
void handleSerialCommand() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd == "pump_on") {
      digitalWrite(PUMP_RELAY, HIGH);
      Serial.println("Pump ON");
    }
    else if (cmd == "pump_off") {
      digitalWrite(PUMP_RELAY, LOW);
      Serial.println("Pump OFF");
    }
    else if (cmd == "valve_on") {
      digitalWrite(VALVE_RELAY, HIGH);
      Serial.println("Valve ON");
    }
    else if (cmd == "valve_off") {
      digitalWrite(VALVE_RELAY, LOW);
      Serial.println("Valve OFF");
    }
    else if (cmd == "status") {
      Serial.println("Pump: " + String(digitalRead(PUMP_RELAY) ? "ON" : "OFF"));
      Serial.println("Valve: " + String(digitalRead(VALVE_RELAY) ? "ON" : "OFF"));
    }
    else if (cmd == "send") {
      sendSensorData();
    }
  }
}

/* 
  Serial Commands:
  - pump_on      : Turn on pump
  - pump_off     : Turn off pump
  - valve_on     : Turn on valve
  - valve_off    : Turn off valve
  - status       : Show current status
  - send         : Send sensor data immediately
*/
