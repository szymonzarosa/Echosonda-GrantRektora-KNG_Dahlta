"""
Script: CSV to GPX Converter
Description: 
    Converts raw bathymetric data (CSV format) collected by the Arduino system 
    into standard GPX format compatible with GIS software and web visualizers (e.g., gpx.studio).

Input CSV Structure:
    year, month, day, hour, minute, second, latDirection, lonDirection, 
    latitude, longitude, latDegree, lonDegree, distance, high, starID, sog, cog

Usage:
    Run the script in the terminal: python python_file_csv_to_gpx.py
    Ensure the source CSV file is in the same directory.
"""

import csv
import xml.etree.ElementTree as ET
import os


# Change desktop path here to use this code
desktop_path = r'C:\Users\USERNAME\Desktop'


csv_files = [f for f in os.listdir(desktop_path) if f.endswith('.CSV')]
print("Available CSV files on desktop:")
for idx, file in enumerate(csv_files):
    print(f"{idx + 1}. {file}")


file_idx = int(input("Choose number of file you want to change to GPX: ")) - 1
input_csv_file = os.path.join(desktop_path, csv_files[file_idx])


output_gpx_file = os.path.join('output.gpx')


column_names = ['year', 'month', 'day', 'hour', 'minute', 'second', 'latDirection', 'lonDirection',
                'latitude', 'longitude', 'latDegree', 'lonDegree', 'depth', 'high', 'starID',
                'sog', 'cog']


data = {col: [] for col in column_names}

with open(input_csv_file, 'r') as csvfile:
    reader = csv.reader(csvfile, delimiter=';')
    next(reader)
    next(reader)

    for row in reader:
        for col, value in zip(column_names, row):
            data[col].append(value)


gpx = ET.Element('gpx', version="1.1", creator="MyGPXCreator")
trk = ET.SubElement(gpx, 'trk')
trkseg = ET.SubElement(trk, 'trkseg')


for lat_deg, lon_deg in zip(data['latDegree'], data['lonDegree']):
    trkpt = ET.SubElement(trkseg, 'trkpt', lat=str(lat_deg), lon=str(lon_deg))


tree = ET.ElementTree(gpx)
tree.write(output_gpx_file, encoding='utf-8', xml_declaration=True)


modified_csv_file = os.path.join('modified_' + csv_files[file_idx])
with open(input_csv_file, 'r') as original, open(modified_csv_file, 'w', newline='') as modified:
    reader = csv.reader(original)
    writer = csv.writer(modified)
    writer.writerow([])
    writer.writerow(column_names)
    next(reader)
    next(reader)
    for row in reader:
        writer.writerow(row)

print(f"GPS file has been saved as: {output_gpx_file}")


os.remove(modified_csv_file)
print(f"Modified CSV file has been removed.")
