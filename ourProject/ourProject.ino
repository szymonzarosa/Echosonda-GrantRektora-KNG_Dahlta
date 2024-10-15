// Wczytujemy odpowiednie biblioteki do obsługi karty SD, moduły GNSS oraz echosondy
#include <SD.h>
#include "DFRobot_GNSS.h"
#include <jsnsr04t.h>

// Inicjalizujemy PINy dla modułu GNSS
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

// Definiujemy PINy dla modułu echosondy
#define ECHO_PIN 3
#define TRIGGER_PIN 2
#define LED_R 9
#define LED_G 8
#define BUTTON 7

JsnSr04T ultrasonicSensor(ECHO_PIN, TRIGGER_PIN, LOG_LEVEL_VERBOSE);

// Inicjalizujemy zmienną na nazwę pliku
char filename[50];

void setup() {
  // Inicjalizujemy LED I przycisk
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(BUTTON,INPUT);

  // Ustawiamy wyjście czerwonego na wysoki a zielonego na niski
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,LOW);

  // Uruchamiamy port szeregowy, aby wyświetlać komunikaty
  Serial.begin(9600);
  while (!Serial);

  // Inicjalizujemy moduł do komunikacji z modułem GNSS
  while(!gnss.begin()){
    Serial.println("Brak modułu GNSS!");
    delay(1000);
  }
  gnss.enablePower();
  gnss.setGnss(eGPS);
  gnss.setRgbOn();
  Serial.println("Inicjalizacja modułu GNSS");

  // Inicjalizujemy obiekt do komunikacji z echosondą
  ultrasonicSensor.begin(Serial);
  Serial.println("Inicjalizacja modułu czujnika");

  // Inicjalizujemy moduł do komunikacji z kartą SD
  while (!SD.begin()) {
    Serial.println("Błąd wczytania karty lub jej brak");
    delay(1000);
  }
  Serial.println("Zainicjalizowano moduł karty SD");
  
  // Sprawdzamy, czy moduł GNSS połączył się z którąś satelitą, 
  // sprawdzając czy lat i lon nie są zerami
  sLonLat_t lat = gnss.getLat();
  sLonLat_t lon = gnss.getLon();
  double latitude = lat.latitude;
  double longitude = lon.lonitude;

  while (latitude == 0 && longitude == 0){
    Serial.println("Brak widocznych satelit!");
    delay(2000);
    lat = gnss.getLat();
    lon = gnss.getLon();
    latitude = lat.latitude;
    longitude = lon.lonitude;
  }

  // Wystarczy podpiąć go do jednego PINa i czekać  
  // w pętli while aż będzie miał on stan wysoki

  // Wszystkie moduły są zainicjalizowane 
  // Czekanie na kliknięcie przycisku
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_G,HIGH);

  while (!digitalRead(BUTTON)) {
    Serial.println("Wciśnij przycisk");
    delay(2000);
  }

  // Inicjalizacja zczytywania danych - LED świeci się na zielono
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);

  // Tworzymy nowy plik .csv o dacie wczytanej z modułu GNSS
  sTim_t utc = gnss.getUTC();
  int hour = utc.hour;
  int minute = utc.minute;
  int second = utc.second;
  sprintf(filename, "%d_%d_%d.csv", hour, minute, second);
  File csvFile = SD.open(filename, FILE_WRITE);

  if (!csvFile) {
    Serial.print("błąd otwarcia - trzeba zresetować Arduino");
    Serial.println(filename);
    while (1);
  }
  
  // Serial.println("Stworzono plik na karcie SD !");

  csvFile.println();
  csvFile.println("year; month; day; hour; minute; second; latDirection; lonDirection; latitude; longitude; latDegree; lonDegree; depth; high; starID; sog; cog");
  csvFile.close();
}

void loop() {
  // Zczytujemy dane z modułu GNSS
  sTim_t utc = gnss.getUTC();
  sTim_t date = gnss.getDate();
  sLonLat_t lat = gnss.getLat();
  sLonLat_t lon = gnss.getLon();
  double high = gnss.getAlt();
  uint8_t starUserd = gnss.getNumSatUsed();
  double sog = gnss.getSog();
  double cog = gnss.getCog();

  // Zczytujemy dane z modułu echosondy
  int dist = ultrasonicSensor.readDistance();

  File csvFile = SD.open(filename, FILE_WRITE);
  // Tworzymy wiadomość do zapisania na karcie dla formatu CSV

  String dataString = "";
  // Zapis roku
  
  dataString += date.year;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Zapis miesiąca
  
  dataString += date.month;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Zapis dnia 
  
  dataString += date.date;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Zapis godziny (UTC +00:00)
  
  dataString += utc.hour;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Zapis minuty
  
  dataString += utc.minute;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Zapis sekundy
  
  dataString += utc.second;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Kierunek szerokości geograficznej
  
  dataString += (char)lat.latDirection;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Kierunek długości geograficznej 
  
  dataString += (char)lon.lonDirection;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  //Szerokość geograficzna - zapis dziesiętny dla stopni
  
  dataString += String(lat.latitude, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Długość geograficzna - zapis dziesiętny dla stopni
  
  dataString += String(lon.lonitude, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Szerokość geograficzna - zapis dziesiętny dla minut
  
  dataString += String(lat.latitudeDegree, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Długość geograficzna - zapis dziesiętny dla minut
  
  dataString += String(lon.lonitudeDegree, 6);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Głębokość pomierzona przez sensor
  
  dataString += dist;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Wysokość modułu GNSS
  
  dataString += String(high,2);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Liczba widzianych satelitów
  
  dataString += starUserd;
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Prędkość poruszania się
  
  dataString += String(sog, 2);
  dataString += "; ";
  csvFile.print(dataString);
  dataString = "";
  // Kąt poruszania się
  
  dataString += String(cog, 2);
  csvFile.println(dataString);
  dataString = "";

  // Debugowanie danych przed zapisem
  // Serial.println(dataString);

  // Otwieramy plik na karcie SD i zapisujemy dane
  // File csvFile = SD.open(filename, FILE_WRITE);
  // csvFile.println(dataString);
  csvFile.close();
}
