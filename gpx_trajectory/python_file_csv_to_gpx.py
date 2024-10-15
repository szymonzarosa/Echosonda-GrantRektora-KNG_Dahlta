import csv
import xml.etree.ElementTree as ET
import os

# Ścieżka do pulpitu
desktop_path = r'C:\Users\48692\Desktop'

# Wyświetlenie plików CSV na pulpicie
csv_files = [f for f in os.listdir(desktop_path) if f.endswith('.CSV')]
print("Dostępne pliki CSV na pulpicie:")
for idx, file in enumerate(csv_files):
    print(f"{idx + 1}. {file}")

# Wybór pliku przez użytkownika
file_idx = int(input("Wybierz numer pliku, który chcesz zamienić na GPX: ")) - 1
input_csv_file = os.path.join(desktop_path, csv_files[file_idx])

# Nazwa pliku GPX do zapisu
output_gpx_file = os.path.join('output.gpx')

# Definicja nazw kolumn
column_names = ['year', 'month', 'day', 'hour', 'minute', 'second', 'latDirection', 'lonDirection',
                'latitude', 'longitude', 'latDegree', 'lonDegree', 'depth', 'high', 'starID',
                'sog', 'cog']

# Odczyt pliku CSV z pominięciem dwóch pierwszych linii
data = {col: [] for col in column_names}

with open(input_csv_file, 'r') as csvfile:
    reader = csv.reader(csvfile, delimiter=';')
    next(reader)  # Pomijamy pierwszą pustą linię
    next(reader)  # Pomijamy linię z nagłówkami

    for row in reader:
        for col, value in zip(column_names, row):
            data[col].append(value)

# Zapis do pliku GPX
gpx = ET.Element('gpx', version="1.1", creator="MyGPXCreator")
trk = ET.SubElement(gpx, 'trk')
trkseg = ET.SubElement(trk, 'trkseg')

for lat_deg, lon_deg in zip(data['latDegree'], data['lonDegree']):
    trkpt = ET.SubElement(trkseg, 'trkpt', lat=str(lat_deg), lon=str(lon_deg))

tree = ET.ElementTree(gpx)
tree.write(output_gpx_file, encoding='utf-8', xml_declaration=True)

# Dodanie pustej linii i linii z nazwami kolumn do pliku CSV
modified_csv_file = os.path.join('modified_' + csv_files[file_idx])
with open(input_csv_file, 'r') as original, open(modified_csv_file, 'w', newline='') as modified:
    reader = csv.reader(original)
    writer = csv.writer(modified)

    # Zapisz pustą linię
    writer.writerow([])

    # Zapisz linię z nazwami kolumn oddzielonymi średnikami
    writer.writerow(column_names)

    # Przepisz pozostałe dane
    next(reader)  # Pomijamy pierwszą pustą linię
    next(reader)  # Pomijamy starą linię z nagłówkami
    for row in reader:
        writer.writerow(row)

print(f"Plik GPX został zapisany jako: {output_gpx_file}")

# Usunięcie zmodyfikowanego pliku CSV
os.remove(modified_csv_file)
print(f"Zmodyfikowany plik CSV został usunięty.")
