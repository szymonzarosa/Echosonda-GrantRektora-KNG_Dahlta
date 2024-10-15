/*
  Non-blocking Write

  This example demonstrates how to perform non-blocking writes
  to a file on a SD card. The file will contain the current millis()
  value every 10ms. If the SD card is busy, the data will be buffered
  in order to not block the sketch.

  NOTE: myFile.availableForWrite() will automatically sync the
        file contents as needed. You may lose some unsynced data
        still if myFile.sync() or myFile.close() is not called.

  The circuit:
  - Arduino MKR Zero board
  - micro SD card attached

  This example code is in the public domain.
*/

// Wczytujemy bibliotekę SD, która pozwala na komunikację z kartą SD
#include <SD.h>

// Deklarujemy nazwę pliku, który będzie zapisywany na karcie SD. W naszym przypadku będzie to "demo.txt"
// file name to use for writing
const char filename[] = "demo.txt";

// Deklarujemy obiekt File, który będzie reprezentował plik 
// File object to represent file
File txtFile;

// Deklarujemy zmienną typu String, która będzie przechowywała dane, które chcemy zapisać na karcie SD
// string to buffer output
String buffer;

// Deklarujemy zmienną lastMillis, która będzie przechowywała czas ostatniego zapisu
unsigned long lastMillis = 0;

// Funkcja setup() wywołuje się raz po uruchomieniu płytki
void setup() {
  // Inicjalizacja komunikacji szeregowej z prędkością 9600 (którą uruchamiamy przy użyciu portu szeregowego w Arduino IDE)
  Serial.begin(9600);
  while (!Serial);

  // Rezerwujemy 1kB dla String używanej jako bufor (1024 bajty = 1kB)
  // reserve 1kB for String used as a buffer
  buffer.reserve(1024);

  // Ustawiamy wbudowaną diodę LED na Arduino jako wyjście, która będzie migać podczas zapisywania
  // set LED pin to output, used to blink when writing
  pinMode(LED_BUILTIN, OUTPUT);

  // Inicjalizacja karty SD, jeżeli nie uda się to wyświetlamy komunikat "Card failed, or not present"
  // init the SD card
  if (!SD.begin()) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  // Sprawdzamy czy plik o nazwie filename istnieje, jeżeli tak, to go usuwamy (aby zacząć od pustego pliku)
  // If you want to start from an empty file,
  // uncomment the next line:
  // SD.remove(filename);

  // Sprawdzamy czy plik o nazwie filename istnieje, jeżeli nie to go tworzymy
  // try to open the file for writing
  txtFile = SD.open(filename, FILE_WRITE);
  if (!txtFile) {
    Serial.print("error opening ");
    Serial.println(filename);
    while (1);
  }

  // Dodajemy nową linię oraz tekst "Hello World!" na początek pliku
  // add some new lines to start
  txtFile.println();
  txtFile.println("Hello World!");
}

// Funkcja loop() wywołuje się w nieskończoność po wykonaniu funkcji setup()
void loop() {

  // Sprawdzamy czy minęło więcej niż 10ms od ostatniego zapisu
  // Wykorzystujemy funkcję millis(), która zwraca czas w milisekundach od uruchomienia płytki
  // https://www.arduino.cc/reference/pl/language/functions/time/millis/

  // check if it's been over 10 ms since the last line added
  unsigned long now = millis();
  if ((now - lastMillis) >= 10) {  // every 10 ms

    // Tworzymy nowy tekst, który będzie dodany do bufora
    // add a new line to the buffer
    buffer += "Hello ";
    buffer += now;
    buffer += "\r\n";

    lastMillis = now;
  }

  // Sprawdzamy, czy karta SD jest dostępna do zapisu danych bez blokowania
  // oraz czy dane w buforze są wystarczające dla pełnego rozmiaru bloku
  
  // check if the SD card is available to write data without blocking
  // and if the buffered data is enough for the full chunk size
  unsigned int chunkSize = txtFile.availableForWrite();
  if (chunkSize && buffer.length() >= chunkSize) {

    // Zapisanie do pliku oraz mignięcie diody LED
    // write to file and blink LED
    digitalWrite(LED_BUILTIN, HIGH);
    txtFile.write(buffer.c_str(), chunkSize);
    digitalWrite(LED_BUILTIN, LOW);

    // Usuwamy zapisane dane z bufora, aby można było zapisać nowe dane
    // remove written data from buffer
    buffer.remove(0, chunkSize);
  }
}
