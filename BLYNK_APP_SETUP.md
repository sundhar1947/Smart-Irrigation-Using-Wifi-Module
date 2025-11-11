# BLYNK APP SETUP GUIDE
## Smart Irrigation System

---

## OVERVIEW

Blynk is a mobile app that allows you to:
- Monitor real-time sensor data (moisture, temperature, humidity)
- Control the pump manually from anywhere
- Switch between automatic and manual modes
- Receive instant notifications
- View historical graphs

---

## INSTALLATION

### Step 1: Download Blynk App

**iOS (iPhone/iPad):**
1. Open App Store
2. Search for "Blynk - IoT for Arduino"
3. Tap "Get" and authenticate with Face ID/Apple ID
4. Wait for installation

**Android:**
1. Open Google Play Store
2. Search for "Blynk - IoT for Arduino"
3. Tap "Install"
4. Wait for installation

### Step 2: Create Blynk Account

1. Open Blynk app
2. Tap "Create New Account"
3. Enter your email address
4. Create a strong password
5. Tap "Sign Up"
6. Check email for verification link
7. Verify your email
8. Return to app and log in

---

## PROJECT CREATION

### Step 1: Create New Project

1. Open Blynk app (logged in)
2. Tap "+" button (create new)
3. Select project name:
   - "Smart Irrigation System"
4. Choose device type:
   - **Device:** NodeMCU
   - **Connection:** WiFi
5. Choose template: "Blank"
6. Tap "Create"
7. **Receive Auth Token via email** - Save this!

### Step 2: Copy Auth Token

Your Auth Token looks like: `abc123def456ghi789`

1. Check your email for Auth Token
2. Copy the full token (no spaces)
3. Paste into Arduino code:
   ```cpp
   char blynkAuth[] = "abc123def456ghi789";
   ```
4. Save and upload code to ESP8266

---

## WIDGET SETUP

### Virtual Pin Mapping

```
V0 = Soil Moisture (Input)
V1 = Temperature (Input)
V2 = Humidity (Input)
V3 = Pump Status (Input)
V4 = Threshold Value (Input)
V5 = Manual Mode Toggle (Output)
V6 = Pump Control Button (Output)
V7 = System Reset (Output)
```

### Widget 1: Soil Moisture Gauge

**Purpose:** Display real-time soil moisture level

1. Tap "+" button to add widget
2. Search and select **Gauge**
3. Tap to configure:
   - **Name:** "Soil Moisture"
   - **Pin:** V0 (Virtual pin 0)
   - **Min:** 0
   - **Max:** 1023
   - **Color:** Blue
   - **Refresh Rate:** 5 seconds
4. Tap "OK" to save

**What it shows:**
- Green (0-300): Soil wet, no watering needed
- Yellow (300-500): Moisture adequate
- Red (500-1023): Soil dry, watering needed

### Widget 2: Temperature Gauge

**Purpose:** Display current temperature

1. Tap "+" to add widget
2. Select **Gauge**
3. Configure:
   - **Name:** "Temperature"
   - **Pin:** V1
   - **Min:** 10
   - **Max:** 50
   - **Unit:** °C
   - **Color:** Orange
   - **Refresh Rate:** 10 seconds
4. Save

**Thresholds:**
- Blue (10-15°C): Cold - watering reduced
- Green (15-30°C): Optimal - normal watering
- Red (30-50°C): Hot - watering increased

### Widget 3: Humidity Gauge

**Purpose:** Display air humidity

1. Tap "+" to add widget
2. Select **Gauge**
3. Configure:
   - **Name:** "Humidity"
   - **Pin:** V2
   - **Min:** 0
   - **Max:** 100
   - **Unit:** %
   - **Color:** Cyan
   - **Refresh Rate:** 10 seconds
4. Save

**Indicators:**
- 0-30%: Low humidity - increase watering
- 30-80%: Normal - standard operation
- 80-100%: High humidity/Rain - skip watering

### Widget 4: Pump Status LED

**Purpose:** Show if pump is ON or OFF

1. Tap "+" to add widget
2. Select **LED**
3. Configure:
   - **Name:** "Pump Status"
   - **Pin:** V3
   - **ON Color:** Red
   - **OFF Color:** Gray
   - **Refresh Rate:** 2 seconds
4. Save

**LED States:**
- Red: Pump is running
- Gray: Pump is stopped

### Widget 5: Adjusted Threshold Value

**Purpose:** Display current moisture threshold

1. Tap "+" to add widget
2. Select **Value Display** (or similar)
3. Configure:
   - **Name:** "Current Threshold"
   - **Pin:** V4
   - **Refresh Rate:** 10 seconds
4. Save

**Shows:**
- Current DRY_THRESHOLD after weather adjustments
- Changes based on temperature and humidity

### Widget 6: Manual Mode Toggle Switch

**Purpose:** Switch between automatic and manual control

1. Tap "+" to add widget
2. Select **Switch**
3. Configure:
   - **Name:** "Manual Mode"
   - **Pin:** V5
   - **ON Value:** 1
   - **OFF Value:** 0
   - **ON Color:** Orange
   - **OFF Color:** Green
   - **Switch Type:** "Switch" (not momentary)
4. Save

**Usage:**
- OFF (Green): Automatic mode - system controls pump
- ON (Orange): Manual mode - you control pump
- When manual mode is ON, you can click pump button

### Widget 7: Pump Control Button

**Purpose:** Manually turn pump ON/OFF (only in manual mode)

1. Tap "+" to add widget
2. Select **Button**
3. Configure:
   - **Name:** "Pump Control"
   - **Pin:** V6
   - **ON Text:** "PUMP ON"
   - **OFF Text:** "PUMP OFF"
   - **ON Value:** 1
   - **OFF Value:** 0
   - **Color:** Green (when ON)
   - **Switch Type:** "Switch" (toggle)
4. Save

**Usage:**
1. Enable "Manual Mode" toggle first
2. Click this button to turn pump ON/OFF
3. Button only works when manual mode is enabled
4. System sends notification when pump state changes

### Widget 8: System Reset Button

**Purpose:** Emergency pump stop and system reset

1. Tap "+" to add widget
2. Select **Button**
3. Configure:
   - **Name:** "Emergency Stop"
   - **Pin:** V7
   - **ON Text:** "STOP SYSTEM"
   - **Color:** Red
   - **Switch Type:** "Momentary" (not toggle)
4. Save

**Usage:**
- Press and hold to trigger emergency stop
- Immediately stops pump
- Disables manual mode
- Returns to automatic mode

---

## DASHBOARD LAYOUT

### Recommended Arrangement

```
┌──────────────────────────────────────┐
│  Smart Irrigation System             │
├──────────────────────────────────────┤
│                                      │
│  [Soil Moisture]  [Temperature]      │  (Top Row)
│       Gauge            Gauge         │
│                                      │
│  [Humidity]  [Pump Status]           │  (Middle Row)
│    Gauge          LED                │
│                                      │
│  [Current Threshold Display]         │  (3rd Row)
│                                      │
│  [Manual Mode]  [Pump Control]       │  (4th Row)
│    Toggle         Button             │
│                                      │
│  [Emergency Stop]                    │  (Bottom)
│     Button (RED)                     │
│                                      │
└──────────────────────────────────────┘
```

---

## NOTIFICATION SETUP

### Enable Notifications

1. On Blynk app, tap **Settings** icon
2. Enable **Notifications**
3. Allow app permissions if prompted
4. Select notification methods:
   - Push notifications ✓
   - Email notifications ✓
   - In-app notifications ✓

### Notification Events

System sends notifications for:

| Event | Message |
|-------|---------|
| Pump Starts | "Irrigation Started - Soil moisture is low" |
| Pump Stops | "Irrigation Complete - Soil moisture adequate" |
| Manual Mode On | "Manual Mode Enabled" |
| Manual Mode Off | "Switched to Automatic Mode" |
| Weather Alert | "High humidity detected - reducing watering" |
| System Error | "Connection lost - Check WiFi" |
| Manual Control | "Pump turned ON/OFF manually" |

---

## USING THE APP

### Daily Monitoring

1. Open Blynk app
2. Check soil moisture gauge:
   - Green/Yellow: OK
   - Red: Needs attention
3. Monitor temperature:
   - System auto-adjusts watering
4. Check humidity:
   - High humidity skips watering
5. Verify pump status LED

### Manual Watering

1. Tap "Manual Mode" toggle to ON
2. Tap "Pump Control" button
3. Pump starts immediately
4. See water flowing to garden
5. Tap button again to stop
6. To return to automatic: Toggle "Manual Mode" to OFF

### Emergency Stop

1. Tap "Emergency Stop" button (RED) if needed
2. Pump stops immediately
3. System resets to automatic mode
4. Check for issues before resuming

---

## TROUBLESHOOTING

### App Won't Connect to System

**Problem:** "Device Offline" message

**Solutions:**
1. Check WiFi connection on phone
2. Verify ESP8266 is powered and WiFi LED blinks
3. Verify WiFi SSID/password in Arduino code
4. Restart Blynk app
5. Restart ESP8266 (power cycle)
6. Check Auth Token is copied correctly

### Widgets Not Updating

**Problem:** Gauges show old values or "--"

**Solutions:**
1. Verify WiFi is connected (check Serial Monitor)
2. Check Blynk is connected:
   - App should show "Online"
3. Wait 10-15 seconds for first data
4. Reduce refresh rates (2-5 seconds for faster updates)
5. Check Arduino code error messages

### Pump Doesn't Respond to Button

**Problem:** Button press has no effect

**Solutions:**
1. Ensure "Manual Mode" toggle is ON (orange)
2. Check virtual pin V6 is selected for button
3. Verify pump relay connections
4. Try "Emergency Stop" - if this works, issue is elsewhere
5. Check Arduino Serial Monitor for errors

### Notifications Not Received

**Problem:** No alerts or push notifications

**Solutions:**
1. Enable notifications in Blynk Settings
2. Check phone notification permissions
3. Verify WiFi connection is stable
4. Wait for events to trigger:
   - Manually activate pump
   - Watch for notification
5. Check in-app notification bell icon

---

## ADVANCED FEATURES

### Create Automation (Rules)

**Coming in future versions:**
- Create rules like "If moisture > 600, notify me"
- Schedule automatic watering times
- Set alerts for temperature extremes

### Export Data

1. In Blynk: Data → Export
2. Choose date range
3. Export as CSV/PDF
4. Analyze in spreadsheet

### Share Access

1. Settings → Share
2. Invite other users by email
3. Grant read or control permissions
4. Others can view/control from their Blynk app

---

## TIPS & BEST PRACTICES

✓ **Do:**
- Check app daily for first week
- Adjust thresholds if watering too much/little
- Keep WiFi password secure
- Backup your Auth Token somewhere safe
- Update Blynk app when new versions available
- Monitor water usage trends

✗ **Don't:**
- Share your Auth Token publicly
- Leave manual mode ON permanently
- Ignore high humidity warnings
- Run pump more than 10 minutes continuously
- Block the sensor with water
- Use the system without WiFi connection

---

## DATA ANALYTICS

### Viewing Water Usage

1. In Blynk: History tab
2. View graphs for:
   - Soil moisture trends
   - Temperature variations
   - Pump runtime logs
3. Analyze patterns:
   - When does soil dry fastest?
   - Best irrigation times?
   - Water savings compared to manual?

### Optimization

After 2 weeks of operation:
1. Review trends
2. Identify peak watering hours
3. Adjust DRY_THRESHOLD if needed
4. Fine-tune PUMP_COOLDOWN_TIME
5. Compare water usage month-to-month

---

## SECURITY

### Protect Your System

1. **Strong Password:**
   - Use unique, complex Blynk password
   - Don't use same password as WiFi

2. **Auth Token:**
   - Never share your Auth Token
   - Treat like a password
   - Regenerate if compromised

3. **WiFi Security:**
   - Use WPA2 password (not WEP)
   - Hide SSID if possible
   - Change default WiFi password

4. **App Updates:**
   - Keep Blynk app updated
   - Install security patches

---

## CONTACT SUPPORT

**Blynk Support:** support@blynk.io
**Issue Help:** help.blynk.cc

---

**Version:** 1.0
**Last Updated:** November 2025

