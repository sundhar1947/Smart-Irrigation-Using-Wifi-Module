/*
 * SMART IRRIGATION SYSTEM - CONFIGURATION FILE
 * WiFi-based with Weather Integration
 * 
 * This file contains all user-configurable parameters
 * Modify these settings according to your environment
 */

// ============================================================
// SECTION 1: WiFi CONFIGURATION
// ============================================================
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// WiFi connection retry settings
#define WIFI_CONNECT_TIMEOUT 20        // Connection attempt timeout (seconds)
#define WIFI_RECONNECT_INTERVAL 10000  // Time between reconnection attempts (ms)

// ============================================================
// SECTION 2: THINGSPEAK CLOUD CONFIGURATION
// ============================================================
#define THINGSPEAK_CHANNEL_ID 12345678              // Your channel ID
#define THINGSPEAK_WRITE_API_KEY "XXXXXXXXXX"       // Your write API key
#define THINGSPEAK_READ_API_KEY "YYYYYYYYYY"        // Your read API key
#define THINGSPEAK_UPDATE_INTERVAL 60000             // Update interval (ms)

// ThingSpeak Channel Fields:
// Field 1: Soil Moisture (0-1023)
// Field 2: Temperature (°C)
// Field 3: Humidity (%)
// Field 4: Pump Status (0=OFF, 1=ON)
// Field 5: Adjusted Threshold
// Field 6: Mode (0=AUTO, 1=MANUAL)

// ============================================================
// SECTION 3: BLYNK APP CONFIGURATION
// ============================================================
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
#define BLYNK_SERVER "blynk-cloud.com"
#define BLYNK_PORT 80

// Blynk Virtual Pins Mapping:
// V0 - Soil Moisture (Gauge)
// V1 - Temperature (Gauge)
// V2 - Humidity (Gauge)
// V3 - Pump Status (LED)
// V4 - Adjusted Threshold (Value Display)
// V5 - Manual Mode Toggle (Switch)
// V6 - Manual Pump Control (Button)
// V7 - System Reset (Button)

// ============================================================
// SECTION 4: SOIL MOISTURE THRESHOLDS (0-1023 scale)
// ============================================================

// Base thresholds for average soil conditions
#define DRY_THRESHOLD 500              // Start irrigation when > this value
#define WET_THRESHOLD 300              // Stop irrigation when < this value

// Soil type specific adjustments:
// Sandy soil:  DRY = 400,  WET = 250
// Loam soil:   DRY = 500,  WET = 300
// Clay soil:   DRY = 600,  WET = 350

// Apply these for known soil type:
// Uncomment your soil type
// #define SOIL_TYPE_SANDY
#define SOIL_TYPE_LOAM
// #define SOIL_TYPE_CLAY

#ifdef SOIL_TYPE_SANDY
  #define BASE_DRY_THRESHOLD 400
  #define BASE_WET_THRESHOLD 250
#elif defined SOIL_TYPE_LOAM
  #define BASE_DRY_THRESHOLD 500
  #define BASE_WET_THRESHOLD 300
#elif defined SOIL_TYPE_CLAY
  #define BASE_DRY_THRESHOLD 600
  #define BASE_WET_THRESHOLD 350
#endif

// ============================================================
// SECTION 5: WEATHER-BASED THRESHOLD ADJUSTMENT
// ============================================================

// Temperature ranges (°C)
#define TEMP_HIGH 35.0                 // High temp threshold
#define TEMP_LOW 15.0                  // Low temp threshold
#define TEMP_OPTIMAL_LOW 20.0
#define TEMP_OPTIMAL_HIGH 30.0

// Temperature adjustment amounts
#define TEMP_HIGH_ADJUSTMENT -100      // Increase watering (lower threshold)
#define TEMP_LOW_ADJUSTMENT +100       // Reduce watering (raise threshold)

// Humidity ranges (%)
#define HUMIDITY_HIGH 80.0             // High humidity threshold
#define HUMIDITY_LOW 30.0              // Low humidity threshold

// Humidity adjustment amounts
#define HUMIDITY_HIGH_ADJUSTMENT +50   // Reduce watering
#define HUMIDITY_LOW_ADJUSTMENT -50    // Increase watering

// Rain detection using humidity
#define RAIN_THRESHOLD_HUMIDITY 85.0   // Humidity > this indicates possible rain
#define SKIP_IRRIGATION_IN_RAIN true   // Set to false to ignore rain detection

// ============================================================
// SECTION 6: PUMP SAFETY SETTINGS
// ============================================================

// Pump runtime limits
#define PUMP_MAX_RUNTIME 600000        // Maximum pump runtime per cycle (ms)
                                       // 600000 = 10 minutes

#define PUMP_COOLDOWN_TIME 300000      // Cooldown period after pump stops (ms)
                                       // 300000 = 5 minutes

// Prevents pump from immediately restarting after stopping

// ============================================================
// SECTION 7: SENSOR READING INTERVALS
// ============================================================

#define SENSOR_READ_INTERVAL 2000      // Read sensors every 2 seconds (ms)
#define CLOUD_UPDATE_INTERVAL 60000    // Send to cloud every 60 seconds (ms)

// Note: More frequent readings = higher CPU usage and faster battery drain
// Recommended: 2000ms for sensor reading, 60000ms for cloud updates

// ============================================================
// SECTION 8: CROP-SPECIFIC SETTINGS
// ============================================================

// Select your primary crop type
// This adjusts thresholds and behaviors for optimal growth
// Uncomment only ONE crop type

// #define CROP_TOMATO
// #define CROP_LETTUCE
// #define CROP_CORN
// #define CROP_COTTON
// #define CROP_GRASS
// #define CROP_ORNAMENTAL

// Crop Management Allowable Depletion (MAD) - at what point to irrigate
#ifdef CROP_TOMATO
  // Tomatoes are sensitive to water stress
  #define CROP_MAD 50                  // Irrigate at 50% depletion
  #define CROP_WATER_NEED "High"
#elif defined CROP_LETTUCE
  // Lettuce needs consistent moisture
  #define CROP_MAD 40
  #define CROP_WATER_NEED "High"
#elif defined CROP_CORN
  // Corn is moderately tolerant
  #define CROP_MAD 60
  #define CROP_WATER_NEED "Medium"
#elif defined CROP_COTTON
  // Cotton is drought tolerant
  #define CROP_MAD 70
  #define CROP_WATER_NEED "Low"
#elif defined CROP_GRASS
  // Grass for lawns
  #define CROP_MAD 60
  #define CROP_WATER_NEED "Medium"
#elif defined CROP_ORNAMENTAL
  // Ornamental plants (flowers, decorative)
  #define CROP_MAD 65
  #define CROP_WATER_NEED "Medium"
#endif

// ============================================================
// SECTION 9: NOTIFICATION SETTINGS
// ============================================================

// Enable/disable notifications
#define SEND_PUMP_ON_NOTIFICATION true      // Alert when pump starts
#define SEND_PUMP_OFF_NOTIFICATION true     // Alert when pump stops
#define SEND_LOW_MOISTURE_ALERT true        // Alert for low moisture
#define SEND_HIGH_HUMIDITY_ALERT false      // Alert for high humidity
#define SEND_TEMPERATURE_ALERT false        // Alert for extreme temperatures

// Notification cooldown (prevent spam)
#define NOTIFICATION_COOLDOWN 300000        // Don't send same alert within 5 min

// ============================================================
// SECTION 10: DEBUG & LOGGING
// ============================================================

// Enable serial debug output
#define DEBUG_MODE true                // Set to false to reduce serial output
#define BAUD_RATE 115200               // Serial communication speed

// Debug message levels:
// 0 = Minimal (errors only)
// 1 = Normal (important events)
// 2 = Verbose (all events and sensor readings)
#define DEBUG_LEVEL 1

// ============================================================
// SECTION 11: SYSTEM BEHAVIOR
// ============================================================

// Auto-resume settings
#define AUTO_RESUME_AFTER_POWER_LOSS true   // Resume operation after power failure
#define STARTUP_DELAY 5000                   // Wait after boot before starting (ms)

// Failsafe settings
#define ENABLE_FAILSAFE true                // Enable safety mechanisms
#define FAILSAFE_STOP_PUMP_INTERVAL 1800000 // Stop pump every 30 min in failsafe mode

// Maintenance mode
#define MAINTENANCE_MODE false              // Set to true to disable automatic irrigation

// ============================================================
// SECTION 12: TIME ZONE & SCHEDULING (Future Use)
// ============================================================

#define TIMEZONE_OFFSET 5.5                 // UTC+5:30 for India
#define DAYLIGHT_SAVING_TIME false          // DST enabled?

// Scheduled watering times (HHMM format, 24-hour)
#define SCHEDULE_ENABLED false
#define MORNING_WATER_TIME "0600"           // 6:00 AM
#define EVENING_WATER_TIME "1800"           // 6:00 PM
#define WATERING_DURATION 15                // Minutes to water

// ============================================================
// SECTION 13: SENSOR CALIBRATION CONSTANTS
// ============================================================

// Soil Moisture Sensor calibration
// Adjust these based on your sensor's actual min/max readings
#define SOIL_SENSOR_MIN_RAW 200             // Raw value when fully submerged
#define SOIL_SENSOR_MAX_RAW 900             // Raw value when completely dry

// DHT11 temperature offset (in case sensor reads high/low)
#define TEMPERATURE_OFFSET 0.0              // Add or subtract from reading
#define HUMIDITY_OFFSET 0.0

// ============================================================
// SECTION 14: NETWORK & CONNECTIVITY
// ============================================================

// HTTP timeout for cloud requests (ms)
#define HTTP_TIMEOUT 10000

// Maximum WiFi connection attempts before alert
#define WIFI_MAX_ATTEMPTS 5

// Enable OTA (Over-The-Air) updates
#define ENABLE_OTA false
#define OTA_PORT 8266

// ============================================================
// SECTION 15: PIN ASSIGNMENTS (Advanced)
// ============================================================

// DO NOT change unless using different hardware
#define SOIL_SENSOR_PIN A0              // Analog pin for soil moisture
#define DHT_PIN D4                      // Digital pin for DHT11
#define RELAY_PIN D1                    // Digital pin for relay control
#define STATUS_LED_PIN D8               // Optional status LED
#define ERROR_LED_PIN D0                // Optional error LED

// ============================================================
// END OF CONFIGURATION FILE
// ============================================================

/*
 * HOW TO USE THIS FILE:
 * 
 * 1. ESSENTIAL - Must modify before upload:
 *    - WIFI_SSID & WIFI_PASSWORD: Your home WiFi credentials
 *    - THINGSPEAK_CHANNEL_ID & THINGSPEAK_WRITE_API_KEY
 *    - BLYNK_AUTH_TOKEN
 *    - DRY_THRESHOLD & WET_THRESHOLD (based on your soil type)
 * 
 * 2. RECOMMENDED - Adjust for your environment:
 *    - TEMP_HIGH & TEMP_LOW thresholds
 *    - HUMIDITY_HIGH & HUMIDITY_LOW thresholds
 *    - PUMP_MAX_RUNTIME & PUMP_COOLDOWN_TIME
 *    - CROP_TYPE selection
 * 
 * 3. OPTIONAL - Fine-tuning:
 *    - DEBUG_LEVEL for troubleshooting
 *    - Notification settings
 *    - Sensor calibration constants
 * 
 * Tips:
 * - Start with default values and adjust incrementally
 * - Monitor the system for 24-48 hours before final adjustments
 * - Use Serial Monitor (115200 baud) to observe system behavior
 * - Check ThingSpeak/Blynk dashboards for data validation
 */
