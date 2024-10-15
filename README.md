# Grant Rektora 2024 - Pomiary batymetryczne [KNG Dahlta]
  
Jest to repozytorium związane z projektem realizowanym przez Koło Naukowe Geodetów 'Dahlta' jako jeden z Grantów Rektora 2024 [AGH]. Projekt "Program badawczy HydroBIM – część pierwsza: automatyzacja pomiarów batymetrycznych zbiorników wodnych i szlaków żeglugowych" ma na celu zbudowanie prototypu echosondy, po to by sprawdzić go w pomiarach batymetrycznych zbiorników wodnych.


## Moduły wykorzystane w projekcie:
- [Moduł czytnika kart microSD](https://botland.com.pl/akcesoria-do-kart-pamieci/8230-modul-czytnika-kart-microsd-5904422311278.html?cd=18298825138&ad=&kd=&gad_source=1&gclid=Cj0KCQjw4MSzBhC8ARIsAPFOuyV3e0OKvE2_MWXHbHzuE3z-97jvh5oQhjkQfZgNQd0Qb-kYaUrYY7caAhA6EALw_wcB)

<img src="assets/modul-czytnika-kart-microsd.jpg" width="200" height="200">

- [Moduł odbiornika GNSS GPS DFRobot TEL0157](https://botland.com.pl/moduly-gps/22671-gravity-modul-odbiornika-gnss-gps-beidou-i2cuart-dfrobot-tel0157-6959420923199.html?cd=18298825651&ad=&kd=&gad_source=1&gclid=Cj0KCQjw4MSzBhC8ARIsAPFOuyVK5DkbfYtcIf9Kkr6AhN3Mr3t-RRxih82mm45TOmbRo41qDEjdA6waAuWuEALw_wcB)

<img src="assets/gravity-modul-odbiornika-gnss-gps-beidou-i2cuart-dfrobot-tel0157.jpg" alt="Moduł odbiornika GNSS" width="200" height="200">

- [Moduł ultradźwiękowego czunika odległości JSN-SR04T z sondą wodoodporną](https://botland.com.pl/ultradzwiekowe-czujniki-odleglosci/7266-ultradzwiekowy-czujnik-odleglosci-jsn-sr04t-20-450cm-z-sonda-wodoodporna-5904422310066.html)

<img src="assets/ultradzwiekowy-czujnik-odleglosci-jsn-sr04t-20-450cm-z-sonda-wodoodporna.jpg" alt="Ultradźwiękowy czujnik odległości" width="200" height="200">

## Od czego rozpocząć?

Aby uruchomić program, należy najpierw zainstalować odpowiednie biblioteki:

- **SD** by Arduino, SparkFun v1.2.4
- **DFRobot_GNSS** by DFRobot v1.0.0
- **jsnsr04t** by Overflaw v1.1.0

Wszystko wykonujemy w programie Arduino IDE, w zakładce "Menedżer bibliotek", wpisując powyższe nazwy i klikając "Zainstaluj".

W folderze [**libraries**](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/tree/main/libraries) znajdują się niektóre pliki z tych bibliotek. Najważniejszą rzeczą, którą
musieliśmy zrobić to w pliku **jsnsr04t.h** zmienić wartość METER_CONVERSION_COEFFICIENT na wartość 13, gdyż początkowa wartość 58 była właściwa dla poruszania się dźwięku w powietrzu.
My natomiast używając ultradźwiękowego czujnika w wodzie, musieliśmy podzielić ten współczynnik około 4,5 razy, więc jego wartość powinna wynieść 13.


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

### Przycisk i dioda LED

|   Moduł   |   Arduino  |
|-----------|------------|
|   LED_R   |    PIN 9   |
|   LED_G   |    PIN 8   |
|   BUTTON  |    PIN 7   |

Wszystkie moduły zasilane są napięciem 5V.

W folderze [**getGNSS**](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/tree/main/getGNSS) znajduje się kod dostosowany do uruchomienia modułu GNSS i weryfikacji poprawności połączenia. Zawiera on dodatkowe komentarze, które tłumaczą poszczególne fragmenty kodu.

W folderze [**NonBlockingWrite**](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/tree/main/NonBlockingWrite) znajduje się kod dostosowany do uruchomienia modułu karty SD i weryfikacji poprawności połączenia. Zawiera on dodatkowe komentarze, które tłumaczą poszczególne fragmenty kodu.

W folderze [**ourProject**](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/tree/main/ourProject) znajduje się realizowany projekt, który będzie się opierał na powyższych folderach. 

## Pierwsze Uruchomienie

Po poprawnym podłączeniu komponentów i uruchomieniu płytki, dioda świeci na czerwono do momentu uzyskania rozwiązania pozycji oraz połączenia z odpowiednią liczbą satelitów. Jest kluczowe, aby przeprowadzać pomiary na terenach, gdzie nie ma przeszkód widokowych, takich jak drzewa czy budynki, które mogłyby blokować sygnał satelitarny i wpływać na jego jakość.

Gdy moduł GNSS nawiąże połączenie z satelitami, dioda zmienia kolor na żółty, co sygnalizuje potrzebę naciśnięcia przycisku. Po naciśnięciu przycisku, dioda zmienia się na zieloną i rozpoczyna regularne wykonywanie pomiarów co 2 sekundy.

## Wyniki po pierwszym uruchomieniu

Przykładowy wynik pomiaru znajduje się w folderze **output** jako plik o nazwie *hour_minute_second.csv*.

Plik składa się z następujących danych: *year*, *month*, *day*, *hour*, *minute*, *second*, *latDirection*, *lonDirection*, *latitude*, *longitude*, *latDegree*, *lonDegree*, *depth*, *high*, *starID*, *sog*, *cog*.

Jeżeli wartość *distance* jest ujemna, oznacza to, że pomiar został wykonany dla odległości większej niż dany moduł może wykryć.

## Zamiana rozszerzenia pliku csv na gpx

Aby kontrolować dokładność zapisanych danych pomiarowych, został napisany kod w Pythonie, który konwertuje plik CSV zawierający wyniki pomiarów na format GPX. Plik GPX można następnie otworzyć na stronie takiej jak [gpx.studio](https://gpx.studio/), aby wizualizować trasę, którą wykonano.

Przykładowy plik po konwersji **output.gpx** oraz kod do zamiany rozszerzenia pliku **python_file_csv_to_gpx.py** znajduje się w folderze [**gpx_trajectory**](https://github.com/szymonzarosa/Echosonda-GrantRektora-KNG_Dahlta/tree/main/gpx_trajectory)

## Pomiar referencyjny punktów znajdujących się przy brzegu

Wykonaliśmy pomiar 12 punktów referencyjnych znajdujących się w wodzie a także 2 osnów pomiarowych niedaleko zbiornika wodnego. Pomiar odbiornikiem GNSS trwał dwa razy po 1 sekundzie na każdy punkt referencyjny (w celu obliczenia przewyższenia a co za tym idzie głębokości, na jakiej znajduje się dany punkt). Ustaliliśmy, że nie potrzebujemy aż tak wysokiej dokładności współrzędnych (uzyskane błędy dx, dy, dh mieściły się w okolicach 1 cm. Pomiar osnowy natomiast miał na celu sprawdzenia dokładności wyznaczenia wartości współrzędnych płaskich. 

Pomiar prototypem na 12 punktach referencyjnych trwał po jednej minucie na każdy punkt, natomiast pomiar na punktach osnowy trwał po 4 minuty. Po pomiarze dane te zostały uśrednione i porównane z obserwacjami z odbiornika GNSS. 

## Wyniki pomiaru referencyjnego 



## Pomiar batymetryczny



## Wyniki pomiaru batymetrycznego





## Dodatkowe linki

Polecane poradniki do modułu Czytnika kart microSD:
- [Arduino SD Library Reference](https://www.arduino.cc/reference/en/libraries/sd/)
- [Zapisywanie danych z czujnika na karcie SD w systemie Arduino](https://mikrokontroler.pl/2022/06/15/zapisywanie-danych-z-czujnika-na-karcie-sd-w-systemie-arduino/)

Polecane poradniki do modułu odbiornika GPS:
- [Gravity GNSS Positioning Module](https://wiki.dfrobot.com/SKU_TEL0157_Gravity_GNSS_Positioning_Module)
- [DFRobot GNSS GitHub Repository](https://github.com/DFRobot/DFRobot_GNSS)

Polecane poradniki do modułu Echosondy:
- [Pomiar odległości z wykorzystaniem Arduino i czujnika HC-SR04](https://botland.com.pl/content/144-pomiar-odleglosci-z-wykorzystaniem-arduino-i-czujnika-hc-sr04-lub-us-015)
