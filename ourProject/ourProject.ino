// Wczytujemy odpowiednie biblioteki do obsługi karty SD, moduły GNSS oraz echosondy
#include <SD.h>
#include "DFRobot_GNSS.h"
#include <jsnsr04t.h>

// Inicjalizujemy piny dla modułu gnss
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

// Definiujemy piny dla modułu echosondy
#define ECHO_PIN 3
#define TRIGGER_PIN 2
JsnSr04T ultrasonicSensor(ECHO_PIN, TRIGGER_PIN, LOG_LEVEL_VERBOSE);

// Inicjalizujemy zmienną na nazwę pliku
char filename[50];

void setup() {
  // Uruchamiamy port szeregowy, aby wyświetlać komunikaty
  Serial.begin(9600);
  while (!Serial);

  // Inicjalizujemy moduł do komunikacji z modułem GNSS
  while(!gnss.begin()){
    Serial.println("No GNSS Module!");
    delay(1000);
  }
  gnss.enablePower();
  gnss.setGnss(eGPS);
  gnss.setRgbOn();
  Serial.println("GNSS Module initialised");

  // Inicjalizyjemy obiekt do komunikacji z echosondą
  ultrasonicSensor.begin(Serial);
  Serial.println("Sensor Module initialised");

  // Inicjalizujemy moduł do komunikacji z kartą SD
  while (!SD.begin()) {
    Serial.println("Card failed, or not present");
    delay(1000);
  }
  Serial.println("SD Card Module initialised");

  // Sprawdzamy czy moduł GNSS połączył się z którąś satelitą, 
  // sprawdzając czy lat i lon nie są zerami
  sLonLat_t lat = gnss.getLat();
  sLonLat_t lon = gnss.getLon();
  int latitude = lat.latitude;
  int lonitude = lon.lonitude;

  while (latitude == 0 && lonitude == 0){
    Serial.println("No Satelits in view!");
    delay(1000);
    lat = gnss.getLat();
    lon = gnss.getLon();
    latitude = lat.latitude;
    lonitude = lon.lonitude;
  }

  // TODO - potencjalnie można dodać przełącznik do Arduino i uruchamiać nagrywanie danych po przełączeniu
  // Wystarczy podpiąć go do jednego pina i czekać w pętli while aż będzie miał on stan wysoki

  // Tworzymy nowy plik csv o dacie wczytanej z modułu GNSS
  sTim_t utc = gnss.getUTC();
  int hour = utc.hour;
  int minute = utc.minute;
  int second = utc.second;
  sprintf(filename, "%d_%d_%d.csv", hour, minute, second);
  File csvFile = SD.open(filename, FILE_WRITE);

  if (!csvFile) {
    Serial.print("error opening - need to reset Arduino");
    Serial.println(filename);
    while (1);
  }
  
  Serial.println("Created file on SD Card!");

  csvFile.println();
  csvFile.println("year\; month\; day\; hour\; minute\; second\; latDirection\; lonDirection\; latitude\; lonitude\; latDegree\; lonDegree\; distance");
  csvFile.close();
}

void loop() {
  // Zczytujemy dane z modułu GNSS
  sTim_t utc = gnss.getUTC();
  sTim_t date = gnss.getDate();
  sLonLat_t lat = gnss.getLat();
  sLonLat_t lon = gnss.getLon();
  // double high = gnss.getAlt();
  // uint8_t starUserd = gnss.getNumSatUsed();
  // double sog = gnss.getSog();
  // double cog = gnss.getCog();

  // Zczytujemy dane z modułu echosondy
  int dist = ultrasonicSensor.readDistance();

  // Tworzymy wiadomość do zapisania na karcie dla formatu CSV
  String dataString = "";
  dataString += date.year;
  dataString += "\; ";
  dataString += date.month;
  dataString += "\; ";
  dataString += date.date;
  dataString += "\; ";
  dataString += utc.hour;
  dataString += "\; ";
  dataString += utc.minute;
  dataString += "\ ;";
  dataString += utc.second;
  dataString += "\ ;";
  dataString += (char)lat.latDirection;
  dataString += "\ ;";
  dataString += (char)lon.lonDirection;
  dataString += "\ ;";
  dataString += lat.latitude;
  dataString += "\ ;";
  dataString += lon.lonitude;
  dataString += "\ ;";
  dataString += lat.latitudeDegree;
  dataString += "\ ;";
  dataString += lon.lonitudeDegree;
  dataString += "\ ;";
  dataString += dist;

  // Otwieramy plik na karcie SD i zapisujemy dane
  File csvFile = SD.open(filename, FILE_WRITE);
  csvFile.println(dataString);
  csvFile.close();
}
