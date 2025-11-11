# SMART IRRIGATION SYSTEM - COMPLETE SETUP GUIDE
## WiFi-Based with Weather Integration

---

## TABLE OF CONTENTS
1. [Prerequisites](#prerequisites)
2. [Hardware Assembly](#hardware-assembly)
3. [Software Installation](#software-installation)
4. [Configuration Setup](#configuration-setup)
5. [Cloud Platform Setup](#cloud-platform-setup)
6. [Testing & Calibration](#testing--calibration)
7. [Troubleshooting](#troubleshooting)

---

## PREREQUISITES

### Hardware Required
- NodeMCU ESP8266 microcontroller
- Capacitive soil moisture sensor
- DHT11 temperature & humidity sensor
- 5V single-channel relay module
- 12V DC water pump
- 12V 2A power adapter
- Breadboard and jumper wires
- USB cable for programming (Micro-USB)

### Software Required
- Arduino IDE v1.8.19 or newer
- ESP8266 board support package
- DHT sensor library (Adafruit)
- ThingSpeak library
- Blynk library

### Accounts Required
- ThingSpeak account (free at thingspeak.com)
- Blynk account (free at blynk.cloud)
- WiFi network with 2.4GHz band

### Internet Connection
- WiFi network for ESP8266
- Internet access for cloud platforms

---

## HARDWARE ASSEMBLY

### Step 1: Prepare Components

1. Inspect all components for damage
2. Test power supplies with multimeter
   - 12V adapter should read 12V DC
   - USB adapter should read 5V DC
3. Check sensor pin connectors
4. Verify all cables are not frayed

### Step 2: Breadboard Layout

```
BREADBOARD SETUP:

Power Rails (Left side):
├─ Red rail: 5V from USB
├─ Black rail: GND (common)
└─ Secondary red rail (middle): 3.3V from ESP8266

Component Placement:
├─ ESP8266: Top-left area
├─ DHT11: Top-right area
├─ Relay: Bottom area
└─ Soil Sensor: Along the side
```

### Step 3: Connect Soil Moisture Sensor

```
Capacitive Soil Moisture Sensor:
┌─────────────────────┐
│  Sensor Module      │
├─────────────────────┤
│ GND ──→ Breadboard GND
│ VCC ──→ Breadboard 5V
│ A0  ──→ ESP8266 A0
└─────────────────────┘
```

**Connection Checklist:**
- [ ] Sensor GND connected to breadboard GND
- [ ] Sensor VCC connected to breadboard +5V
- [ ] Sensor A0 connected to ESP8266 A0
- [ ] All connections are secure

### Step 4: Connect DHT11 Temperature Sensor

```
DHT11 Sensor:
┌─────────────────────┐
│  DHT11 Module       │
├─────────────────────┤
│ GND ──→ Breadboard GND
│ VCC ──→ Breadboard 5V
│ DATA ──→ ESP8266 D4
└─────────────────────┘

Optional: Add 10kΩ pull-up resistor
between DATA and VCC
```

**Connection Checklist:**
- [ ] DHT11 GND connected to breadboard GND
- [ ] DHT11 VCC connected to breadboard +5V
- [ ] DHT11 DATA connected to ESP8266 D4
- [ ] Pull-up resistor installed (if using)

### Step 5: Connect Relay Module

```
5V Relay Module:
┌─────────────────────────┐
│  Relay Module (SRD)     │
├─────────────────────────┤
│ GND ──→ Breadboard GND
│ VCC ──→ Breadboard 5V
│ IN  ──→ ESP8266 D1
│ COM ──→ +12V Supply
│ NO  ──→ Pump Positive
│ NC  ──→ (Not used)
└─────────────────────────┘
```

**Connection Checklist:**
- [ ] Relay GND connected to breadboard GND
- [ ] Relay VCC connected to breadboard +5V
- [ ] Relay IN connected to ESP8266 D1
- [ ] Relay COM connected to 12V power supply positive
- [ ] Pump positive wire connected to relay NO (Normally Open)

### Step 6: Connect Water Pump

```
Water Pump Wiring:
┌──────────────────┐
│   Water Pump     │
├──────────────────┤
│ Pump + ──→ Relay NO
│ Pump - ──→ 12V GND
└──────────────────┘
```

**Connection Checklist:**
- [ ] Pump positive connected to relay NO
- [ ] Pump negative connected to power supply GND
- [ ] Pump tubing is connected to water source
- [ ] Tubing is routed to plant/garden area

### Step 7: Power Supply Configuration

```
Power Distribution:
┌────────────────────────────────────────┐
│ 12V Adapter (2A)                       │
├────────────────────────────────────────┤
│                                        │
│ +12V ──→ Relay COM                     │
│         ──→ USB Adapter IN             │
│                                        │
│ GND  ──→ Common Ground Node            │
│         ├─→ Breadboard GND             │
│         ├─→ Relay GND                  │
│         ├─→ Pump GND                   │
│         └─→ USB Adapter GND            │
└────────────────────────────────────────┘

┌────────────────────────────────────────┐
│ USB Adapter (5V)                       │
├────────────────────────────────────────┤
│                                        │
│ +5V ──→ Breadboard Red Rail            │
│                                        │
│ GND ──→ Breadboard Black Rail          │
└────────────────────────────────────────┘
```

### Step 8: Verify Complete Circuit

**Pre-Power Checklist:**
- [ ] All connections double-checked
- [ ] No loose wires crossing other components
- [ ] Power supplies not connected yet
- [ ] Pump is NOT connected to water yet
- [ ] All sensors seated properly

**Power-Up Test:**
1. Connect USB adapter (5V) first
   - ESP8266 blue LED should blink
   - No smoke or unusual behavior
2. Connect 12V adapter
   - Relay indicator LED should be off (pump inactive)
   - No excessive heat from components
3. If any problems, disconnect immediately and check connections

---

## SOFTWARE INSTALLATION

### Step 1: Install Arduino IDE

1. Visit https://www.arduino.cc/en/software
2. Download Arduino IDE 1.8.19 or newer
3. Install for your operating system
4. Launch Arduino IDE

### Step 2: Add ESP8266 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Find "Additional Board Manager URLs"
4. Paste this URL:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
5. Click OK
6. Go to **Tools → Board → Boards Manager**
7. Search for "esp8266"
8. Install "esp8266 by ESP8266 Community" (latest version)
9. Wait for installation to complete

### Step 3: Install Required Libraries

1. Go to **Sketch → Include Library → Manage Libraries**
2. Search for "DHT" and install:
   - **DHT sensor library by Adafruit**
3. Search for "ThingSpeak" and install:
   - **ThingSpeak by MathWorks**
4. Search for "Blynk" and install:
   - **Blynk by Blynk**
5. Click Close

### Step 4: Configure Arduino IDE for ESP8266

1. Go to **Tools** menu and set:
   - **Board:** NodeMCU 1.0 (ESP-12E Module)
   - **Upload Speed:** 115200
   - **CPU Frequency:** 80 MHz
   - **Flash Size:** 4M (1M SPIFFS)
   - **Port:** COM3 (or your port number)

2. To find your port:
   - Connect NodeMCU via USB
   - Go to Device Manager (Windows) or System Report (Mac)
   - Look for "CH340" or "UART" device
   - Note the COM port number

### Step 5: Download Project Files

1. Download all project files to a folder:
   ```
   smart_irrigation_wifi_weather.ino  (main code)
   config.h                           (configuration)
   ```
2. Place both files in the same Arduino sketch folder

---

## CONFIGURATION SETUP

### Step 1: Set Up ThingSpeak Channel

**On ThingSpeak website:**

1. Go to https://thingspeak.com
2. Create a free account or sign in
3. Click "Channels" in top menu
4. Click "New Channel"
5. Fill in:
   - **Name:** "Smart Irrigation System"
   - **Description:** "WiFi-based irrigation with weather"
   - Check all field boxes
6. Set Field names:
   - **Field 1:** "Soil Moisture"
   - **Field 2:** "Temperature"
   - **Field 3:** "Humidity"
   - **Field 4:** "Pump Status"
   - **Field 5:** "Adjusted Threshold"
   - **Field 6:** "System Mode"
7. Click "Save Channel"
8. Go to "API Keys" tab
9. Copy your "Write API Key" (long string)
10. Copy your "Channel ID" (numeric ID)

### Step 2: Set Up Blynk App

**Mobile phone:**

1. Download Blynk app (iOS/Android)
2. Create account or sign in
3. Create new project:
   - **Device:** NodeMCU
   - **Connection Type:** WiFi
4. Receive "Auth Token" via email (or view in Settings)
5. Copy your Auth Token

**In Blynk app, create widgets:**

1. Add **Gauge** widget:
   - Name: "Soil Moisture"
   - Pin: V0
   - Min: 0, Max: 1023

2. Add **Gauge** widget:
   - Name: "Temperature"
   - Pin: V1
   - Min: 10, Max: 50, Unit: °C

3. Add **Gauge** widget:
   - Name: "Humidity"
   - Pin: V2
   - Min: 0, Max: 100, Unit: %

4. Add **LED** widget:
   - Name: "Pump Status"
   - Pin: V3

5. Add **Value Display** widget:
   - Name: "Threshold"
   - Pin: V4

6. Add **Switch** widget:
   - Name: "Manual Mode"
   - Pin: V5
   - Color: Orange

7. Add **Button** widget:
   - Name: "Pump On/Off"
   - Pin: V6
   - Color: Green

### Step 3: Configure Arduino Code

1. Open `smart_irrigation_wifi_weather.ino` in Arduino IDE
2. Find and replace these values:

```cpp
// Line ~20-30: WiFi
const char* ssid = "YOUR_WIFI_SSID";           // Your WiFi name
const char* password = "YOUR_WIFI_PASSWORD";   // Your WiFi password

// Line ~35-40: ThingSpeak
unsigned long channelID = YOUR_CHANNEL_ID;     // From ThingSpeak
const char* writeAPIKey = "YOUR_THINGSPEAK_WRITE_KEY";  // From ThingSpeak

// Line ~43-44: Blynk
char blynkAuth[] = "YOUR_BLYNK_AUTH_TOKEN";   // From Blynk app
```

### Step 4: Adjust Configuration File

1. Open `config.h` in Arduino IDE
2. Modify these essential settings:

```cpp
// Line ~9-10: WiFi (same as in .ino file)
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Line ~17-20: ThingSpeak
#define THINGSPEAK_CHANNEL_ID 12345678
#define THINGSPEAK_WRITE_API_KEY "XXXXXXXXXX"

// Line ~28: Blynk
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

// Line ~40-45: Soil thresholds (based on your soil type)
#define DRY_THRESHOLD 500              // For loam soil
#define WET_THRESHOLD 300
```

### Step 5: Upload Code to ESP8266

1. Connect NodeMCU to computer via USB
2. In Arduino IDE, click **Upload** button (→)
3. Wait for compilation and upload
   - Should see: "Leaving... Hard resetting via RTS pin..."
4. Open **Tools → Serial Monitor**
5. Set baud rate to **115200**
6. Should see startup messages

---

## CLOUD PLATFORM SETUP

### ThingSpeak Dashboard

After code uploads successfully:

1. Go to your ThingSpeak channel
2. Wait 60 seconds for first data
3. Go to **Channel Settings**
4. Enable visualization:
   - **Field 1 Chart:** "Soil Moisture vs. Time"
   - **Field 2 Chart:** "Temperature vs. Time"
   - **Field 3 Chart:** "Humidity vs. Time"

### Blynk Dashboard

1. Open Blynk app
2. Go to your project
3. Press **Play** button to activate
4. Widgets should start showing live data
5. Adjust widget refresh rate:
   - Settings → Blynk timer → 5000ms (recommended)

---

## TESTING & CALIBRATION

### Phase 1: Dry Run Testing (No Water)

1. **Sensor Reading Validation:**
   - Open Serial Monitor (115200 baud)
   - Observe soil moisture readings
   - Manually measure when sensor is:
     - In dry air: Should read 700-900
     - In water: Should read 100-300
   - Adjust DRY_THRESHOLD and WET_THRESHOLD if needed

2. **Relay Test:**
   - Manually lower DRY_THRESHOLD to 400 temporarily
   - Touch sensor probe with wet fingers
   - Relay should click (pump should turn on)
   - Remove wet finger
   - Relay should click again (pump should turn off)
   - Restore DRY_THRESHOLD to original value

3. **Cloud Connection Test:**
   - Check ThingSpeak channel
   - Verify data appears every 60 seconds
   - Check Blynk app widgets update
   - Verify timestamps are current

### Phase 2: Moisture Calibration

1. **Prepare calibration points:**
   - Dry soil reading
   - Wet soil reading (after watering)
   - Calculate 50% point (midway)

2. **Set thresholds:**
   ```
   DRY_THRESHOLD = (Dry reading + 50% point) / 2
   WET_THRESHOLD = (Wet reading + 50% point) / 2
   ```

3. **Test thresholds:**
   - Let soil dry until pump activates
   - Observe actual moisture level
   - Adjust if activation point is too early/late
   - Repeat until satisfactory

### Phase 3: Pump Operation Test

1. **Fill water reservoir**
2. **Route pump outlet to garden/bucket**
3. **Activate pump manually:**
   - In Blynk: Enable "Manual Mode"
   - Click "Pump On/Off" button
   - Pump should start
   - Water should flow
   - Click button again to stop
4. **Test automatic operation:**
   - Disable "Manual Mode"
   - Let soil dry naturally
   - Pump should activate automatically
   - Monitor water level in reservoir
   - Ensure proper drainage

### Phase 4: Weather Response Testing

1. **Test temperature adjustment:**
   - Manually reduce temperature reading (in code, for testing)
   - Observe threshold change in Serial Monitor
   - Verify Blynk shows updated threshold

2. **Test humidity adjustment:**
   - Place sensor in humid location
   - Observe threshold changes
   - Verify high humidity reduces watering

3. **Test rain detection:**
   - Simulate high humidity (cover sensor or place in bathroom)
   - Pump should NOT activate even if soil is dry
   - System should resume normal operation when humidity drops

---

## TROUBLESHOOTING

### WiFi Connection Issues

**Problem:** ESP8266 cannot connect to WiFi

**Solutions:**
1. Verify SSID and password are correct (case-sensitive)
2. Ensure router broadcasts 2.4GHz (ESP8266 doesn't support 5GHz)
3. Check WiFi signal strength at location (try moving closer)
4. Restart router and ESP8266
5. Check Serial Monitor for error messages
6. Try connecting with only numbers/letters in WiFi password

**Error Message:** "WiFi: Failed to connect"
→ Check credentials, restart router

---

### Sensor Reading Problems

**Problem:** Soil moisture always reads 0 or 1023

**Solutions:**
1. Check sensor cable connections
2. Verify sensor power (should be 5V stable)
3. Test sensor analog reading:
   ```cpp
   // Add to setup():
   pinMode(SOIL_SENSOR_PIN, INPUT);
   ```
   Open Serial Monitor and observe raw readings
4. Dry soil should read ~700-900
5. Wet soil (in water) should read ~100-300
6. If outside this range, sensor may be faulty

**Error Message:** "DHT11 read error!"
→ Check DHT sensor wiring, verify power

---

### Pump Not Activating

**Problem:** Pump doesn't turn on even when soil is dry

**Solutions:**
1. Check relay connections:
   - Relay IN pin connected to D1?
   - Relay COM connected to +12V?
   - Pump positive connected to relay NO?
2. Test relay manually:
   ```cpp
   // Temporary test code in setup()
   digitalWrite(RELAY_PIN, HIGH);  // Should hear click
   delay(1000);
   digitalWrite(RELAY_PIN, LOW);   // Should hear click
   ```
3. Check pump power:
   - Multimeter test: 12V between COM and NO
   - Voltage present when relay IN is HIGH?
4. Verify moisture threshold:
   - Is DRY_THRESHOLD less than current reading?
   - Check Serial Monitor for "soil is dry" message

---

### Cloud Connection Problems

**Problem:** Data not appearing in ThingSpeak/Blynk

**Solutions:**
1. Verify WiFi is connected
2. Check API keys are correct:
   - ThingSpeak Write API Key (not Read key)
   - Blynk Auth Token (full string, no spaces)
3. Check channel ID is numeric
4. Wait 60+ seconds for first update
5. Monitor Serial Monitor for error messages
6. Try manually triggering ThingSpeak update

**Error Code 404:** Wrong channel ID or API key
**Error Code 500:** Server issue, try again later

---

### Excessive Watering

**Problem:** System waters too frequently

**Solutions:**
1. Increase DRY_THRESHOLD (higher = less frequent watering)
2. Decrease WET_THRESHOLD (lower = stops watering sooner)
3. Increase PUMP_COOLDOWN_TIME (longer wait between cycles)
4. Check for leaks in system
5. Verify soil sensor placement (should be near plant roots)

---

### Insufficient Watering

**Problem:** Soil still dry even after irrigation

**Solutions:**
1. Decrease DRY_THRESHOLD (lower = waters sooner)
2. Increase PUMP_MAX_RUNTIME (longer pump operation time)
3. Check pump flow rate:
   - Is water actually flowing?
   - Check for kinked tubing
   - Verify reservoir has water
4. Increase temperature/humidity threshold adjustments
5. Check for water leaks

---

### Performance Issues

**Problem:** System unresponsive or crashes

**Solutions:**
1. Check ESP8266 power supply (should be stable 5V)
2. Reduce SENSOR_READ_INTERVAL (less frequent reads)
3. Reduce CLOUD_UPDATE_INTERVAL (less frequent uploads)
4. Remove unnecessary widgets from Blynk app
5. Reset ESP8266:
   - Hold Reset button for 5 seconds
   - Reboot system
6. Reflash firmware if corrupted

---

## SAFETY REMINDERS

⚠️ **Always follow these precautions:**

- [ ] Disconnect power before modifying circuit
- [ ] Never touch relay contacts when powered
- [ ] Keep pump and electronics away from direct water
- [ ] Use waterproof connectors for outdoor wiring
- [ ] Ensure proper grounding of all components
- [ ] Test pump with water before leaving unattended
- [ ] Check system daily during first week of operation
- [ ] Set pump max runtime to prevent overflow
- [ ] Have manual pump shutoff available

---

## NEXT STEPS

1. **Monitor System:** Check for 24-48 hours
2. **Fine-tune:** Adjust thresholds based on observations
3. **Add Features:** Integrate multiple zones, sensors
4. **Optimize:** Reduce water usage over time
5. **Maintain:** Clean sensor monthly, check connections

---

## SUPPORT & RESOURCES

- Arduino Forum: forum.arduino.cc
- ThingSpeak Community: community.thingspeak.com
- Blynk Community: community.blynk.cc
- ESP8266 Resources: github.com/esp8266/Arduino

---

**Version:** 1.0
**Last Updated:** November 2025
**Status:** Complete & Tested

