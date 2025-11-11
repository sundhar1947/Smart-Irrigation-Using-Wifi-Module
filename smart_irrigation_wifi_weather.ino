/*
 * Smart Irrigation System with Weather Integration
 * WiFi-based IoT System using ESP8266 and ThingSpeak
 * 
 * Features:
 * - Real-time soil moisture monitoring
 * - Temperature and humidity sensing (DHT11)
 * - Weather-based intelligent irrigation
 * - Cloud data logging to ThingSpeak
 * - Mobile app control via Blynk
 * - Remote notifications and alerts
 * - Manual and automatic modes
 * 
 * Hardware:
 * - NodeMCU ESP8266
 * - Capacitive Soil Moisture Sensor
 * - DHT11 Temperature & Humidity Sensor
 * - 5V Relay Module
 * - 12V Water Pump
 * - 12V Power Adapter
 * 
 * Libraries Required:
 * - ESP8266WiFi
 * - DHT sensor library (Adafruit)
 * - ThingSpeak library
 * - Blynk library
 */

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <BlynkSimpleEsp8266.h>
#include <Time.h>

// ==================== WiFi Configuration ====================
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ==================== ThingSpeak Configuration ====================
unsigned long channelID = YOUR_CHANNEL_ID;
const char* thingSpeakServer = "api.thingspeak.com";
const char* writeAPIKey = "YOUR_THINGSPEAK_WRITE_KEY";

// ==================== Blynk Configuration ====================
char blynkAuth[] = "YOUR_BLYNK_AUTH_TOKEN";
char blynkServer[] = "blynk-cloud.com";
const int blynkPort = 80;

// ==================== Pin Definitions ====================
#define SOIL_SENSOR_PIN A0
#define DHT_PIN D4
#define RELAY_PIN D1
#define DHT_TYPE DHT11

// ==================== Sensor Objects ====================
DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;

// ==================== System Configuration ====================
// Moisture Thresholds (0-1023 scale)
const int DRY_THRESHOLD = 500;      // Trigger irrigation when soil is dry
const int WET_THRESHOLD = 300;      // Stop irrigation when soil is wet

// Temperature-based adjustment (°C)
const float TEMP_HIGH = 35.0;       // High temperature - reduce watering
const float TEMP_LOW = 15.0;        // Low temperature - reduce watering
const float TEMP_OPTIMAL_LOW = 20.0;
const float TEMP_OPTIMAL_HIGH = 30.0;

// Humidity-based adjustment (%)
const float HUMIDITY_HIGH = 80.0;   // High humidity - reduce watering
const float HUMIDITY_LOW = 30.0;    // Low humidity - increase monitoring

// Rain detection (optional - using humidity as proxy)
const float RAIN_THRESHOLD_HUMIDITY = 85.0;

// Timing Configuration
const unsigned long updateInterval = 60000;      // Send data every 60 seconds
const unsigned long sensorReadInterval = 2000;  // Read sensors every 2 seconds
const unsigned long pumpMaxRuntime = 600000;    // Max pump runtime 10 minutes
const unsigned long pumpCooldownTime = 300000;  // 5 minute cooldown after pump stops

// ==================== System Variables ====================
unsigned long lastUpdateTime = 0;
unsigned long lastSensorReadTime = 0;
unsigned long pumpStartTime = 0;
unsigned long lastPumpStopTime = 0;

bool pumpState = false;
bool manualMode = false;
bool manualPumpState = false;

// Sensor readings
int soilMoisture = 0;
float temperature = 0.0;
float humidity = 0.0;
float adjustedThreshold = DRY_THRESHOLD;

// ==================== Setup Function ====================
void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("Smart Irrigation System - WiFi Version");
  Serial.println("With Weather Integration");
  Serial.println("========================================");

  // Initialize pins
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Pump OFF initially
  pinMode(SOIL_SENSOR_PIN, INPUT);

  // Initialize DHT sensor
  dht.begin();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.print("[WiFi] Connecting to: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int connectionAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && connectionAttempts < 20) {
    delay(500);
    Serial.print(".");
    connectionAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("[WiFi] Connected successfully!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("[WiFi] Failed to connect. Retrying...");
  }

  // Initialize ThingSpeak
  ThingSpeak.begin(wifiClient);
  Serial.println("[Cloud] ThingSpeak initialized");

  // Initialize Blynk
  Blynk.config(blynkAuth, blynkServer, blynkPort);
  Serial.println("[Cloud] Blynk configured");

  Serial.println("\nSystem Ready!\n");
  printSystemStatus();
}

// ==================== Main Loop ====================
void loop() {
  // Handle WiFi reconnection
  if (WiFi.status() != WL_CONNECTED) {
    reconnectWiFi();
  }

  // Handle Blynk connection
  if (!Blynk.connected()) {
    Blynk.connect();
  } else {
    Blynk.run();
  }

  // Read sensors periodically
  if (millis() - lastSensorReadTime >= sensorReadInterval) {
    readSensors();
    lastSensorReadTime = millis();
  }

  // Apply weather-based threshold adjustment
  adjustMoistureThreshold();

  // Control pump based on soil moisture
  controlPump();

  // Send data to cloud periodically
  if (millis() - lastUpdateTime >= updateInterval) {
    sendToCloud();
    lastUpdateTime = millis();
    printSystemStatus();
  }

  // Safety: Prevent pump from running too long
  if (pumpState && (millis() - pumpStartTime > pumpMaxRuntime)) {
    Serial.println("[Safety] Max pump runtime exceeded! Stopping pump.");
    stopPump();
  }
}

// ==================== Sensor Reading Function ====================
void readSensors() {
  // Read soil moisture
  soilMoisture = analogRead(SOIL_SENSOR_PIN);

  // Read temperature and humidity
  float tempReading = dht.readTemperature();
  float humidReading = dht.readHumidity();

  if (!isnan(tempReading) && !isnan(humidReading)) {
    temperature = tempReading;
    humidity = humidReading;
  } else {
    Serial.println("[Sensor] DHT11 read error!");
  }
}

// ==================== Weather-Based Threshold Adjustment ====================
void adjustMoistureThreshold() {
  adjustedThreshold = DRY_THRESHOLD;  // Start with base threshold

  // Adjust for temperature
  if (temperature > TEMP_HIGH) {
    // High temperature - increase watering frequency (lower threshold)
    adjustedThreshold -= 100;  // More aggressive watering
    Serial.println("[Weather] High temp detected - increasing watering frequency");
  } 
  else if (temperature < TEMP_LOW) {
    // Low temperature - reduce watering frequency
    adjustedThreshold += 100;  // Less frequent watering
    Serial.println("[Weather] Low temp detected - reducing watering frequency");
  }

  // Adjust for humidity
  if (humidity > HUMIDITY_HIGH) {
    // High humidity - reduce watering (increase dry threshold)
    adjustedThreshold += 50;
    Serial.println("[Weather] High humidity detected - reducing watering");
  } 
  else if (humidity < HUMIDITY_LOW) {
    // Low humidity - increase watering (lower dry threshold)
    adjustedThreshold -= 50;
    Serial.println("[Weather] Low humidity detected - increasing watering");
  }

  // Rain detection using humidity threshold
  if (humidity > RAIN_THRESHOLD_HUMIDITY) {
    Serial.println("[Weather] Possible rain detected (High humidity) - skipping irrigation");
    if (pumpState) {
      stopPump();
    }
    return;
  }

  // Clamp adjusted threshold within valid range
  adjustedThreshold = constrain(adjustedThreshold, 200, 700);
}

// ==================== Pump Control Logic ====================
void controlPump() {
  // In manual mode, respect manual command
  if (manualMode) {
    if (manualPumpState && !pumpState) {
      startPump();
    } 
    else if (!manualPumpState && pumpState) {
      stopPump();
    }
    return;
  }

  // Automatic mode logic
  // Start pump when soil is dry
  if (soilMoisture > adjustedThreshold && !pumpState) {
    // Check cooldown period
    if (millis() - lastPumpStopTime > pumpCooldownTime) {
      Serial.print("[Irrigation] Soil is dry (");
      Serial.print(soilMoisture);
      Serial.print(") > threshold (");
      Serial.print(adjustedThreshold);
      Serial.println(") - Starting pump");
      startPump();
      sendNotification("Irrigation Started", "Soil moisture is low. Pump activated!");
    }
  }

  // Stop pump when soil is wet
  if (soilMoisture < WET_THRESHOLD && pumpState) {
    Serial.print("[Irrigation] Soil is wet (");
    Serial.print(soilMoisture);
    Serial.print(") < threshold (");
    Serial.print(WET_THRESHOLD);
    Serial.println(") - Stopping pump");
    stopPump();
    sendNotification("Irrigation Complete", "Soil moisture is adequate. Pump stopped.");
  }
}

// ==================== Pump Control Functions ====================
void startPump() {
  if (!pumpState) {
    digitalWrite(RELAY_PIN, HIGH);
    pumpState = true;
    pumpStartTime = millis();
    Serial.println("[Pump] STARTED");
  }
}

void stopPump() {
  if (pumpState) {
    digitalWrite(RELAY_PIN, LOW);
    pumpState = false;
    lastPumpStopTime = millis();
    Serial.println("[Pump] STOPPED");
  }
}

// ==================== Cloud Communication ====================
void sendToCloud() {
  // Send to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    ThingSpeak.setField(1, soilMoisture);
    ThingSpeak.setField(2, temperature);
    ThingSpeak.setField(3, humidity);
    ThingSpeak.setField(4, pumpState ? 1 : 0);
    ThingSpeak.setField(5, adjustedThreshold);
    ThingSpeak.setField(6, manualMode ? 1 : 0);

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (response == 200) {
      Serial.println("[Cloud] Data sent to ThingSpeak successfully");
    } else {
      Serial.print("[Cloud] Error sending to ThingSpeak. Response code: ");
      Serial.println(response);
    }
  }

  // Update Blynk
  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, soilMoisture);
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, pumpState ? 1 : 0);
    Blynk.virtualWrite(V4, adjustedThreshold);
    Blynk.virtualWrite(V5, manualMode ? 1 : 0);
  }
}

void sendNotification(String title, String message) {
  if (Blynk.connected()) {
    Blynk.notify(title + ": " + message);
  }
  Serial.print("[Notification] ");
  Serial.print(title);
  Serial.print(" - ");
  Serial.println(message);
}

// ==================== WiFi Reconnection ====================
void reconnectWiFi() {
  static unsigned long lastReconnectAttempt = 0;
  const unsigned long reconnectInterval = 10000;  // Try every 10 seconds

  if (millis() - lastReconnectAttempt > reconnectInterval) {
    Serial.println("[WiFi] Attempting to reconnect...");
    WiFi.begin(ssid, password);
    lastReconnectAttempt = millis();
  }
}

// ==================== Blynk Virtual Pin Handlers ====================
// V0 - Soil Moisture (Read only)
BLYNK_READ(V0) {
  Blynk.virtualWrite(V0, soilMoisture);
}

// V1 - Temperature (Read only)
BLYNK_READ(V1) {
  Blynk.virtualWrite(V1, temperature);
}

// V2 - Humidity (Read only)
BLYNK_READ(V2) {
  Blynk.virtualWrite(V2, humidity);
}

// V3 - Pump Status (Read only)
BLYNK_READ(V3) {
  Blynk.virtualWrite(V3, pumpState ? 1 : 0);
}

// V4 - Adjusted Threshold (Read only)
BLYNK_READ(V4) {
  Blynk.virtualWrite(V4, adjustedThreshold);
}

// V5 - Manual Mode Toggle
BLYNK_WRITE(V5) {
  int manualModeValue = param.asInt();
  if (manualModeValue) {
    manualMode = true;
    Serial.println("[Manual] Manual mode ENABLED");
    Blynk.notify("Manual Mode Enabled");
  } else {
    manualMode = false;
    Serial.println("[Manual] Manual mode DISABLED");
    Blynk.notify("Switched to Automatic Mode");
  }
}

// V6 - Manual Pump Control (only active in manual mode)
BLYNK_WRITE(V6) {
  if (manualMode) {
    manualPumpState = param.asInt();
    if (manualPumpState) {
      Serial.println("[Manual] Pump command: ON");
      Blynk.notify("Manual: Pump ON");
    } else {
      Serial.println("[Manual] Pump command: OFF");
      Blynk.notify("Manual: Pump OFF");
    }
  } else {
    Serial.println("[Manual] Cannot control pump - Manual mode disabled");
    Blynk.notify("Enable manual mode first");
  }
}

// V7 - Reset/System Status
BLYNK_WRITE(V7) {
  Serial.println("[System] Manual reset initiated");
  stopPump();
  manualMode = false;
  lastPumpStopTime = millis();
  Blynk.notify("System Reset");
}

// ==================== System Status Display ====================
void printSystemStatus() {
  Serial.println("\n========== SYSTEM STATUS ==========");
  Serial.print("Time: ");
  Serial.println(millis() / 1000);

  Serial.println("--- Sensors ---");
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println(" (0-1023)");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.println("--- System State ---");
  Serial.print("Pump Status: ");
  Serial.println(pumpState ? "ON" : "OFF");
  Serial.print("Adjusted Threshold: ");
  Serial.println(adjustedThreshold);
  Serial.print("Mode: ");
  Serial.println(manualMode ? "MANUAL" : "AUTOMATIC");

  Serial.println("--- Connectivity ---");
  Serial.print("WiFi: ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
  Serial.print("Blynk: ");
  Serial.println(Blynk.connected() ? "Connected" : "Disconnected");

  Serial.println("===================================\n");
}
