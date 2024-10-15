clc,clear

A='pomiar 1.txt';
f1=fopen(A,'r');
dane = textscan(f1,'%s %s');
fclose(f1);

X = dane{1};
Y = dane{2};

f = fopen("mapa 1.txt","wt");

fprintf(f,"<?xml version=""1.0"" encoding=""UTF-8""?><gpx creator=""Szymon Myśliński"">\n");
fprintf(f,"<trk>\n");
fprintf(f,"    <trkseg>\n");

i=0;
for i=1:length(X)
    East = X{i};
    North = Y{i};
    fprintf(f,'    <trkpt ');
    fprintf(f,'lat="%s" lon="%s">\n', East, North);
    fprintf(f,'    </trkpt>\n');
end

fprintf(f,"  </trkseg>\n");

fprintf(f,"</trk>\n");
fprintf(f,"</gpx>");

fclose(f);