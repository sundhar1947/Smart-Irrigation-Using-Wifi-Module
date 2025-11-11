# THINGSPEAK CLOUD SETUP GUIDE
## Smart Irrigation System

---

## OVERVIEW

ThingSpeak is a cloud platform that:
- Stores your irrigation data permanently
- Creates automatic charts and graphs
- Allows data analysis and visualization
- Provides API for external integrations
- Offers free tier for small projects

---

## GETTING STARTED

### Step 1: Create ThingSpeak Account

1. Go to https://thingspeak.com
2. Click "Get Started Free"
3. Enter your email address
4. Create a password (min 6 characters)
5. Accept Terms of Service
6. Click "Create my account"
7. Check email for verification
8. Click verification link
9. Log in to ThingSpeak

### Step 2: Verify Email

1. Check your email inbox
2. Find email from "ThingSpeak" (noreply@thingspeak.com)
3. Click "Verify Email" link
4. Return to ThingSpeak website
5. You're now ready to create channels

---

## CREATE IRRIGATION CHANNEL

### Step 1: Create New Channel

1. Log in to ThingSpeak
2. Click "Channels" in top menu
3. Click "My Channels"
4. Click "New Channel" button

### Step 2: Configure Channel

**Basic Information:**

| Field | Value |
|-------|-------|
| Name | Smart Irrigation System |
| Description | WiFi-based IoT irrigation with weather data logging |
| Visibility | Public (or Private if you prefer) |

**Field Configuration:**

Enable these fields by checking the boxes:

| Field | Name | Unit | Description |
|-------|------|------|-------------|
| 1 | Soil Moisture | - | Analog reading 0-1023 |
| 2 | Temperature | °C | Sensor reading |
| 3 | Humidity | % | Sensor reading |
| 4 | Pump Status | ON/OFF | 1=ON, 0=OFF |
| 5 | Adjusted Threshold | - | Weather-adjusted threshold |
| 6 | System Mode | AUTO/MANUAL | 0=AUTO, 1=MANUAL |

**Optional Settings:**

- **Elevation:** Your location elevation (helps with atmospheric data)
- **Latitude/Longitude:** Your garden location (for weather integration)
- **Tags:** "irrigation, esp8266, weather"

### Step 3: Save Channel

1. Scroll to bottom
2. Click "Save Channel"
3. You'll see "Channel created successfully"
4. Channel ID will be displayed

### Step 4: Get API Keys

1. Go to your channel
2. Click "API Keys" tab
3. You'll see two keys:

**Write API Key:**
- For uploading data from ESP8266
- Looks like: `XXXXXXXXXXXXXX`
- Copy this into Arduino code

**Read API Key:**
- For retrieving data
- Not needed for this project

```cpp
// Example - Paste into Arduino code:
#define THINGSPEAK_WRITE_API_KEY "XXXXXXXXXXXXXX"
#define THINGSPEAK_CHANNEL_ID 123456789
```

---

## SETUP DATA VISUALIZATION

### Step 1: Configure Field Charts

1. Go to your channel
2. Click "Private View" tab
3. You should see empty chart areas

### Step 2: Edit Field Displays

**For Field 1 (Soil Moisture):**

1. Click "Edit" on Field 1 chart
2. Set:
   - **Chart Title:** "Soil Moisture Over Time"
   - **Y-Axis Label:** "Moisture Level (0-1023)"
   - **Chart Type:** Line chart
   - **Color:** Blue
3. Save

**For Field 2 (Temperature):**

1. Click "Edit" on Field 2 chart
2. Set:
   - **Chart Title:** "Temperature Monitoring"
   - **Y-Axis Label:** "Temperature (°C)"
   - **Chart Type:** Line chart
   - **Color:** Orange
3. Save

**For Field 3 (Humidity):**

1. Click "Edit" on Field 3 chart
2. Set:
   - **Chart Title:** "Humidity Levels"
   - **Y-Axis Label:** "Humidity (%)"
   - **Chart Type:** Area chart
   - **Color:** Cyan
3. Save

### Step 3: Time Range Options

When viewing charts, you can select:
- Last 2 hours
- Last 12 hours
- Last 3 days
- Last 15 days
- Last 60 days
- Custom date range

---

## CONNECT ESP8266 TO THINGSPEAK

### Arduino Code Integration

In your `smart_irrigation_wifi_weather.ino`:

```cpp
// ThingSpeak configuration
unsigned long channelID = YOUR_CHANNEL_ID;           // From channel details
const char* thingSpeakServer = "api.thingspeak.com";
const char* writeAPIKey = "YOUR_THINGSPEAK_WRITE_KEY";

// Initialize ThingSpeak
void setup() {
  ThingSpeak.begin(wifiClient);
}

// Send data function (called every 60 seconds)
void sendToCloud() {
  if (WiFi.status() == WL_CONNECTED) {
    ThingSpeak.setField(1, soilMoisture);
    ThingSpeak.setField(2, temperature);
    ThingSpeak.setField(3, humidity);
    ThingSpeak.setField(4, pumpState ? 1 : 0);
    ThingSpeak.setField(5, adjustedThreshold);
    ThingSpeak.setField(6, manualMode ? 1 : 0);

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (response == 200) {
      Serial.println("Data sent to ThingSpeak successfully");
    }
  }
}
```

### Step-by-Step Upload

1. Copy your Channel ID
2. Copy your Write API Key
3. Paste into Arduino code (see above)
4. Upload to ESP8266
5. Wait 60 seconds for first data point
6. Check ThingSpeak channel for data

---

## MONITOR YOUR DATA

### View Real-Time Data

1. Log in to ThingSpeak
2. Click "Channels" → "My Channels"
3. Select your channel
4. Click "Private View"
5. See live updating charts

### Check Last Entry

1. Click "Data Viewer" tab
2. See raw data entries:
   - Timestamp
   - Field values
   - Source IP (your ESP8266)

### View Statistics

1. Click on any field chart
2. See statistics:
   - Minimum value
   - Maximum value
   - Average value
   - Latest value

---

## ADVANCED FEATURES

### React Application

Create automatic actions based on data:

1. Click "React" tab
2. Click "New React"
3. Set trigger:
   - **Condition:** If Field 1 > 600
   - **Action:** Send email notification
4. Configure action:
   - **Subject:** "Low Soil Moisture Alert"
   - **Body:** "Your garden needs watering!"
5. Save React

### TalkBack Queue

Send commands to your device:

1. Click "TalkBack" tab
2. Create new TalkBack
3. Add commands:
   - "PUMP_ON"
   - "PUMP_OFF"
   - "MANUAL_MODE"
4. Your Arduino can query this and execute commands

### MQTT Integration

Connect other IoT devices:

1. Click "API" tab
2. See MQTT broker details
3. Subscribe to topics:
   - `channels/YOUR_CHANNEL_ID/publish/fields/field1`
4. Integrate with other systems

---

## DATA ANALYSIS

### Export Data

1. Click "Data Viewer" tab
2. Select date range
3. Click "Export"
4. Choose format:
   - CSV (for Excel/Sheets)
   - JSON (for programming)
5. Download file

### Analyze Trends

**Using built-in MATLAB:**

1. Click "Apps" tab
2. Click "MATLAB Analysis"
3. Write scripts to analyze data
4. Visualize results

**Example questions to answer:**
- When does soil dry fastest?
- What's average daily water usage?
- How does temperature affect watering?
- Humidity correlation with watering stops?

---

## TROUBLESHOOTING

### No Data Appearing

**Problem:** ThingSpeak channel shows no data

**Check:**
1. WiFi connection on ESP8266
   - Check Serial Monitor at 115200 baud
   - Should show "WiFi connected"
2. API Key is correct
   - Copy from ThingSpeak again
   - Ensure no spaces
   - Check it's Write API Key, not Read
3. Channel ID is correct
   - Verify numeric ID matches
4. Code is sending data
   - Should see "Data sent to ThingSpeak" in Serial Monitor
5. ThingSpeak server is accessible
   - Try accessing thingspeak.com from computer

**Solutions:**
- Restart ESP8266 (power cycle)
- Re-upload code with correct API key
- Check WiFi SSID/password
- Move ESP8266 closer to router
- Restart router

### Sporadic Data (gaps in graph)

**Problem:** Some data points missing, gaps in chart

**Causes:**
1. WiFi dropout - ESP8266 disconnects temporarily
2. Server issue - ThingSpeak temporarily unreachable
3. Throttling - Too many requests too fast

**Solutions:**
1. Improve WiFi signal
2. Increase time between updates (from 60s to 120s)
3. Add WiFi reconnection logic
4. Check System Status: ThingSpeak → System Status

### Wrong Data Values

**Problem:** Data showing incorrect numbers

**Check:**
1. Sensor connections:
   - Soil moisture readings 0-1023 OK?
   - Temperature readings in reasonable range?
2. Threshold values correct?
3. Pump status boolean correct (0 or 1)?

**Solutions:**
- Calibrate soil moisture sensor
- Check temperature offset setting
- Verify pump relay state

---

## SHARING DATA

### Public Channel

Make your data public:

1. Go to channel settings
2. Set **Visibility:** Public
3. Others can view your channel
4. Share channel URL: `https://thingspeak.com/channels/YOUR_CHANNEL_ID`

### Share with Others

Give read/write access:

1. Go to channel settings
2. Click "Collaborators"
3. Invite users by email
4. Grant permissions:
   - Read: Can view data
   - Write: Can send data
   - Admin: Can modify settings

### Embed in Website

Embed ThingSpeak charts in your website:

1. Click chart
2. Click "Code" button
3. Copy embed code
4. Paste into your website HTML

---

## SECURITY & PRIVACY

### Protect Your Keys

✓ **Do:**
- Keep API keys private
- Use different keys for different users
- Regenerate if compromised
- Don't share channel URL if private

✗ **Don't:**
- Post API keys on GitHub (public)
- Share via email
- Print in documentation
- Hardcode in public code

### Regenerate Keys

If you think your key is compromised:

1. Go to channel
2. Click "API Keys"
3. Click regenerate button
4. Old key stops working
5. Update Arduino code with new key
6. Re-upload to ESP8266

---

## PERFORMANCE TIPS

### Reduce Bandwidth

- Increase update interval (every 120s instead of 60s)
- Send only essential data
- Compress data before sending

### Faster Response

- Decrease update interval (to 30s)
- Use local server (self-hosted)
- Reduce retries on connection failure

### Long-term Storage

ThingSpeak stores data for:
- Free account: 15 days
- Paid account: Much longer
- Export before data expires

---

## INTEGRATIONS

### Send to Google Sheets

Use IFTTT or similar services:

1. Create IFTTT account
2. Set trigger: "ThingSpeak → Field 1 updated"
3. Set action: "Google Sheets → Add row"
4. Data automatically logged to spreadsheet

### Send Email Alerts

1. Use React (see Advanced Features)
2. When Field 1 > 600, send email
3. Configure email address
4. Get notifications automatically

### Slack Notifications

1. Create Slack webhook
2. Use React to trigger
3. Receive messages in Slack channel

---

## FAQ

**Q: How long is data stored?**
A: Free account keeps 15 days, Paid plans keep longer

**Q: What's the update limit?**
A: Free account: 3 million updates/month (~1 per second average)

**Q: Can I set alerts?**
A: Yes, use React application to trigger actions

**Q: How do I backup my data?**
A: Export to CSV regularly or use API

**Q: Can I delete channels?**
A: Yes, go to Settings and delete (data will be lost)

---

## RESOURCES

**ThingSpeak Documentation:**
https://www.mathworks.com/help/thingspeak/

**Arduino ThingSpeak Library:**
https://github.com/mathworks/thingspeak-arduino

**API Reference:**
https://www.mathworks.com/help/thingspeak/rest-api.html

---

**Version:** 1.0
**Last Updated:** November 2025
**Status:** Complete

