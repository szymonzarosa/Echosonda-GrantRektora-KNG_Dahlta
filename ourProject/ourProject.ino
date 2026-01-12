/*
 * Project: Low-Cost Bathymetric Measurement System
 * Author:  Szymon Zarosa (KNG Dahlta, AGH University)
 * Date:    2024
 * * Description:
 * This sketch controls the autonomous bathymetric unit. It integrates:
 * 1. GNSS Receiver (DFRobot TEL0157) for positioning (UART).
 * 2. Ultrasonic Sensor (JSN-SR04T) for depth measurement (Digital I/O).
 * 3. SD Card Module for data logging (SPI).
 * * Logic Flow:
 * - Initialize modules (SD, GNSS, Sensor).
 * - Wait for stable GNSS FIX (indicated by LED).
 * - Loop: Measure depth -> Read coordinates -> Log to CSV on SD card.
 * * Calibration Note:
 * The speed of sound in water (~1482 m/s) is approx. 4.3x faster than in air.
 * Ensure the JSN-SR04T library conversion coefficient is set to ~13 (instead of default 58).
 */

// Include libraries for SD card, GNSS module, and Echo sounder
#include <SD.h>
#include "DFRobot_GNSS.h"
#include <jsnsr04t.h>

// Initialize pins/objects for GNSS module
#ifdef  I2C_COMMUNICATION
  DFRobot_GNSS_I2C gnss(&Wire ,GNSS_DEVICE_ADDR);
  #else
  #if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    SoftwareSerial mySerial(4, 5);
    DFRobot_GNSS_UART gnss(&mySerial ,9600);
  #elif defined(ESP32)
    DFRobot_GNSS_UART gnss(&Serial1 ,9600 ,/*rx*/D2 ,/*tx*/D3);
  #else
    DFRobot_GNSS_UART gnss(&Serial1 ,9600);
  #endif
#endif

// Define pins for the echo sounder and UI
#define ECHO_PIN 3
#define TRIGGER_PIN 2
#define LED_R 9
#define LED_G 8
#define BUTTON 7

JsnSr04T ultrasonicSensor(ECHO_PIN, TRIGGER_PIN, LOG_LEVEL_VERBOSE);

// Initialize variable for filename
char filename[50];

void setup() {
  // Initialize LEDs and Button
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(BUTTON,INPUT);

  // Set Red LED high and Green LED low (initial state)
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,LOW);

  // Start serial port for debugging
  Serial.begin(9600);
  while (!Serial);

  // Initialize communication with GNSS module
  while(!gnss.begin()){
    Serial.println("GNSS module not found!");
    delay(1000);
  }
  gnss.enablePower();
  gnss.setGnss(eGPS);
  gnss.setRgbOn();
  Serial.println("GNSS module initialized");

  // Initialize communication with the ultrasonic sensor
  ultrasonicSensor.begin(Serial);
  Serial.println("Sensor module initialized");

  // Initialize SD card module
  while (!SD.begin()) {
    Serial.println("SD card read error or missing card");
    delay(1000);
  }
  Serial.println("SD card module initialized");
  
  // Check if GNSS module has a satellite fix 
  // by verifying if lat and lon are not zero
  sLonLat_t lat = gnss.getLat();
  sLonLat_t lon = gnss.getLon();
  double latitude = lat.latitude;
  double longitude = lon.lonitude;

  while (latitude == 0 && longitude == 0){
    Serial.println("No satellites visible!");
    delay(2000);
    lat = gnss.getLat();
    lon = gnss.getLon();
    latitude = lat.latitude;
    longitude = lon.lonitude;
  }

  // All modules are initialized. 
  // Wait for the button to be pressed (Active High logic)
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,HIGH);

  while (!digitalRead(BUTTON)) {
    Serial.println("Press the button");
    delay(2000);
  }

  // Start data recording - LED turns Green
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);

  // Create a new .csv file named after the current UTC time
  sTim_t utc = gnss.getUTC();
  int hour = utc.hour;
  int minute = utc.minute;
  int second = utc.second;
  sprintf(filename, "%d_%d_%d.csv", hour, minute, second);
  File csvFile = SD.open(filename, FILE_WRITE);

  if (!csvFile) {
    Serial.print("Error opening file - please reset Arduino");
    Serial.println(filename);
    while (1);
  }
  
  // Serial.println("File created on SD card!");

  csvFile.println();
  csvFile.println("year; month; day; hour; minute; second; latDirection; lonDirection; latitude; longitude; latDegree; lonDegree; depth; high; starID; sog; cog");
  csvFile.close();
}

void loop() {
  // Read data from GNSS module
  sTim_t utc = gnss.getUTC();
  sTim_t date = gnss.getDate();
  sLonLat_t lat = gnss.getLat();
  sLonLat_t lon = gnss.getLon();
  double high = gnss.getAlt();
  uint8_t starUserd = gnss.getNumSatUsed();
  double sog = gnss.getSog();
  double cog = gnss.getCog();

  // Read data from Echo sounder (Ultrasonic sensor)
  int dist = ultrasonicSensor.readDistance();

  File csvFile = SD.open(filename, FILE_WRITE);
  // Construct the data string for CSV format

  String dataString = "";
  
  // Write Year
  dataString += date.year;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Write Month
  dataString += date.month;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Write Day
  dataString += date.date;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Write Hour (UTC +00:00)
  dataString += utc.hour;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Write Minute
  dataString += utc.minute;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Write Second
  dataString += utc.second;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Latitude Direction
  dataString += (char)lat.latDirection;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Longitude Direction 
  dataString += (char)lon.lonDirection;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Latitude - decimal degrees
  dataString += String(lat.latitude, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Longitude - decimal degrees
  dataString += String(lon.lonitude, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Latitude - degrees (DMS helper)
  dataString += String(lat.latitudeDegree, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Longitude - degrees (DMS helper)
  dataString += String(lon.lonitudeDegree, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Depth measured by sensor
  dataString += dist;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Altitude (GNSS)
  dataString += String(high,2);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Number of satellites used
  dataString += starUserd;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Speed Over Ground (SOG)
  dataString += String(sog, 2);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  
  // Course Over Ground (COG)
  dataString += String(cog, 2);
  csvFile.println(dataString);
  dataString = "";

  // Debug data before writing
  // Serial.println(dataString);

  // Open SD file and write data (Commented out in original)
  // File csvFile = SD.open(filename, FILE_WRITE);
  // csvFile.println(dataString);
  csvFile.close();
}
