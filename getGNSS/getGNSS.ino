 /*!
  * @file  getGNSS.ino
  * @brief Get gnss simple data
  * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @license The MIT License (MIT)
  * @author ZhixinLiu(zhixin.liu@dfrobot.com)
  * @version V1.0
  * @date 2023-03-07
  * @url https://github.com/dfrobot/DFRobot_GNSS
  */


// Importujemy bibliotekę DFRobot_GNSS - składa się ona z odpowiednio napisanego kodu, który pozwala na komunikację z modułem GNSS
// Dla nas istotne jest jedynie umiejętne korzystanie z funkcji dostępnych w tej bibliotece co jest pokazane w poniższym kodzie

#include "DFRobot_GNSS.h"

// Jeżeli chcemy używać komunikacji I2C to odkomentowujemy poniższą linię (raczej chcemy używać komunikacji UART, więc zakomentowujemy poniższą linię)
// #define I2C_COMMUNICATION  //use I2C for communication, but use the serial port for communication if the line of codes were masked

// KJ: Jeżeli odkomentujemy powyższą linię to poniższe linie zostaną wywołane i moduł ustawi się do komunikacji I2C
#ifdef  I2C_COMMUNICATION
  DFRobot_GNSS_I2C gnss(&Wire ,GNSS_DEVICE_ADDR);
#else

// KJ: poniższe połączenia są zgodne tylko wtedy, gdy używamy komunikacji UART
/* ---------------------------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |   m0  |
 *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |  vcc  |
 *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |  gnd  |
 *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |  tx1  |
 *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |  rx1  |
 * ----------------------------------------------------------------------------------------------------------------------*/

// Poniższe linijki wywołają się w zależności od wykorzystywanego modelu płytki (my używamy UNO, więc wywoła się pierwsza linijka)
/* Baud rate cannot be changed */
  #if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    // Ustawiana jest komunikacja UART na pinach 4 i 5 (zgodnie z tabelką powyżej) oraz z prędkością 9600
    SoftwareSerial mySerial(4, 5);
    DFRobot_GNSS_UART gnss(&mySerial ,9600);
  #elif defined(ESP32)
    DFRobot_GNSS_UART gnss(&Serial1 ,9600 ,/*rx*/D2 ,/*tx*/D3);
  #else
    DFRobot_GNSS_UART gnss(&Serial1 ,9600);
  #endif
#endif

// Funkcja setup() wywołuje się raz po uruchomieniu płytki
void setup()
{

  // Inicjalizacja komunikacji szeregowej z prędkością 115200 (którą uruchamiamy przy użyciu portu szeregowego w Arduino IDE)
  // W poniższej linijce sprawdzamy, czy moduł GNSS jest podłączony do płytki, jeżeli nie, to co sekundę wyświetlamy komunikat "NO Devices !"
  Serial.begin(115200);
  while(!gnss.begin()){
    Serial.println("NO Devices !");
    delay(1000);
  }

  // Jeżeli moduł GNSS jest poprawnie połączony to uruchamiamy zasilanie modułu
  gnss.enablePower();      // Enable gnss power 

  // Wybieramy, jaki system GNSS chcemy używać (system BeiDou jest używany w Chinach, system GLONASS jest używany w Rosji)

/** Set GNSS to be used 
 *   eGPS              use gps
 *   eBeiDou           use beidou
 *   eGPS_BeiDou       use gps + beidou
 *   eGLONASS          use glonass
 *   eGPS_GLONASS      use gps + glonass
 *   eBeiDou_GLONASS   use beidou +glonass
 *   eGPS_BeiDou_GLONASS use gps + beidou + glonass
 */
  gnss.setGnss(eGPS); // KJ: odpowiednio nadpisujemy wartość

  // Możemy wyłączyć lub włączyć diodę RGB na module
  // gnss.setRgbOff();
  gnss.setRgbOn();
  // gnss.disablePower();      // Disable GNSS, the data will not be refreshed after disabling  
}

// Funkcja loop() wywołuje się w nieskończoność po wykonaniu funkcji setup()
void loop()
{
  // Pobieramy dane z modułu GNSS, 
  // Funkcja getUTC() zwraca strukturę sTim_t z danymi czasu (godzina, minuta, sekunda)
  // Funkcja getDate() zwraca strukturę sTim_t z danymi daty (rok, miesiąc, dzień)
  // Funkcja getLat() zwraca strukturę sLonLat_t z danymi szerokości geograficznej
  // Funkcja getLon() zwraca strukturę sLonLat_t z danymi długości geograficznej
  // Funkcja getAlt() zwraca wysokość nad poziomem morza
  // Funkcja getNumSatUsed() zwraca ilość użytych satelitów
  // Funkcja getSog() zwraca prędkość węzłową (podczas poruszania się)
  // Funkcja getCog() zwraca kąt kursu (podczas poruszania się)


  sTim_t utc = gnss.getUTC();
  sTim_t date = gnss.getDate();
  sLonLat_t lat = gnss.getLat();
  sLonLat_t lon = gnss.getLon();
  double high = gnss.getAlt();
  uint8_t starUserd = gnss.getNumSatUsed();
  double sog = gnss.getSog();
  double cog = gnss.getCog();

  // KWyświetlamy pobrane dane na porcie szeregowym
  // Czym różni się funkcja print i printl można sprawdzić w dokumentacji Arduino
  // https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
  // https://www.arduino.cc/reference/en/language/functions/communication/serial/print/

  Serial.println("");
  Serial.print(date.year);
  Serial.print("/");
  Serial.print(date.month);
  Serial.print("/");
  Serial.print(date.date);
  Serial.print("/");
  Serial.print(utc.hour);
  Serial.print(":");
  Serial.print(utc.minute);
  Serial.print(":");
  Serial.print(utc.second);
  Serial.println();
  Serial.println((char)lat.latDirection);
  Serial.println((char)lon.lonDirection);
  
  Serial.print("lat DDMM.MMMMM = ");
  Serial.println(lat.latitude, 5);
  Serial.print(" lon DDDMM.MMMMM = ");
  Serial.println(lon.lonitude, 5);
  Serial.print("lat degree = ");
  Serial.println(lat.latitudeDegree,6);
  Serial.print("lon degree = ");
  Serial.println(lon.lonitudeDegree,6);

  Serial.print("star userd = ");
  Serial.println(starUserd);
  Serial.print("alt high = ");
  Serial.println(high);
  Serial.print("sog =  ");
  Serial.println(sog);
  Serial.print("cog = ");
  Serial.println(cog);
  Serial.print("gnss mode =  ");
  Serial.println(gnss.getGnssMode());

  // Możemy zatrzymać działanie programu na 1 sekundę przed kolejnym odczytem

  // delay(1000);
}
