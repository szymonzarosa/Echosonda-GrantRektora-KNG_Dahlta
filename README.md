# Grant Rektora 2024 - Pomiary batymetryczne [KNG Dahlta]

Jest to repozytorium związane z projektem realizowanym przez Koło Naukowe Geodetów 'Dahlta' jako jeden z Grantów Rektora 2024 [AGH]. Projekt "Program badawczy HydroBIM – część pierwsza: automatyzacja pomiarów batymetrycznych zbiorników wodnych i szlaków żeglugowych" ma na celu zbudowanie prototypu echosondy, po to by sprawdzić go w pomiarach batymetrycznych zbiorników wodnych.

## Moduły wykorzystane w projekcie:
- Moduł czytnika kart microSD ([link](https://botland.com.pl/akcesoria-do-kart-pamieci/8230-modul-czytnika-kart-microsd-5904422311278.html?cd=18298825138&ad=&kd=&gad_source=1&gclid=Cj0KCQjw4MSzBhC8ARIsAPFOuyV3e0OKvE2_MWXHbHzuE3z-97jvh5oQhjkQfZgNQd0Qb-kYaUrYY7caAhA6EALw_wcB)) 

<img src="![modul-czytnika-kart-microsd](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/assets/173694204/d6dedc90-ff2a-433d-b4b9-6081e24ce8fd)
p" alt="Moduł czytnika kart microSD" width="200" height="200">

- Moduł odbiornika GNSS GPS DFRobot TEL0157 ([link](https://botland.com.pl/moduly-gps/22671-gravity-modul-odbiornika-gnss-gps-beidou-i2cuart-dfrobot-tel0157-6959420923199.html?cd=18298825651&ad=&kd=&gad_source=1&gclid=Cj0KCQjw4MSzBhC8ARIsAPFOuyVK5DkbfYtcIf9Kkr6AhN3Mr3t-RRxih82mm45TOmbRo41qDEjdA6waAuWuEALw_wcB)) 

<img src="![gravity-modul-odbiornika-gnss-gps-beidou-i2cuart-dfrobot-tel0157](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/assets/173694204/552cb718-d298-41c3-9363-cafaa5b204a4)" alt="Moduł odbiornika GNSS" width="200" height="200">

- Moduł ultradźwiękowego czunika odległości JSN-SR04T z sondą wodoodporną ([link](https://botland.com.pl/ultradzwiekowe-czujniki-odleglosci/7266-ultradzwiekowy-czujnik-odleglosci-jsn-sr04t-20-450cm-z-sonda-wodoodporna-5904422310066.html)) 

<img src="![ultradzwiekowy-czujnik-odleglosci-jsn-sr04t-20-450cm-z-sonda-wodoodporna](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/assets/173694204/2a8c9be3-6a25-428c-af80-6ed9cab2a954)
" alt="Ultradźwiękowy czujnik odległości" width="200" height="200">

## Od czego rozpocząć?

Aby uruchomić program, należy najpierw zainstalować odpowiednie biblioteki:

- **SD** by Arduino, SparkFun v1.2.4
- **DFRobot_GNSS** by DFRobot v1.0.0
- **jsnsr04t** by Overflaw v1.1.0

Wszystko wykonujemy w programie Arduino IDE, w zakładce "Menedżer bibliotek", wpisując powyższe nazwy i klikając "Zainstaluj".

Następnie podpinamy odpowiednio przewody modułów do pinów Arduino Uno w następującej kolejności:

### Moduł GNSS

|   Moduł   |  Arduino  |
|-----------|-----------|
|    RX     |   PIN 5   |
|    TX     |   PIN 4   |

### Moduł Echosondy

|   Moduł   |  Arduino  |
|-----------|-----------|
|  ECHO PIN |   PIN 3   |
|  TRIG PIN |   PIN 2   |

### Moduł Karty SD

|   Moduł   |  Arduino  |
|-----------|-----------|
|    CS     |   PIN 10  |
|   MOSI    |   PIN 11  |
|   MISO    |   PIN 12  |
|    SCK    |   PIN 13  |

Wszystkie moduły zasilane są napięciem 5V.

W folderze **getGNSS** znajduje się kod dostosowany do uruchomienia modułu GNSS i weryfikacji poprawności połączenia. Zawiera on dodatkowe komentarze z przypiskiem KJ, które tłumaczą poszczególne fragmenty kodu.

W folderze **NonBlockingWrite** znajduje się kod dostosowany do uruchomienia modułu karty SD i weryfikacji poprawności połączenia. Zawiera on dodatkowe komentarze z przypiskiem KJ, które tłumaczą poszczególne fragmenty kodu.

W folderze **ourProject** znajduje się realizowany projekt, który będzie się opierał na powyższych folderach. 

## Pierwsze Uruchomienie

do napisania ...

## Wyniki

Przykładowy wynik pomiaru znajduje się w folderze **output** jako plik o nazwie *hour_minute_second.csv*. Plik składa się z następujących danych: *year*, *month*, *day*, *hour*, *minute*, *second*, *latDirection*, *lonDirection*, *latitude*, *longitude*, *latDegree*, *lonDegree*, *distance*. Jeżeli wartość *distance* jest ujemna, oznacza to, że pomiar został wykonany dla odległości większej niż dany moduł może wykryć.

## Dodatkowe linki

Polecane poradniki do modułu Czytnika kart microSD:
- https://www.arduino.cc/reference/en/libraries/sd/
- https://mikrokontroler.pl/2022/06/15/zapisywanie-danych-z-czujnika-na-karcie-sd-w-systemie-arduino/

Polecane poradniki do modułu odbiornika GPS:
- https://wiki.dfrobot.com/SKU_TEL0157_Gravity_GNSS_Positioning_Module
- https://github.com/DFRobot/DFRobot_GNSS

Polecane poradniki do modułu Echosondy
- https://botland.com.pl/content/144-pomiar-odleglosci-z-wykorzystaniem-arduino-i-czujnika-hc-sr04-lub-us-015
