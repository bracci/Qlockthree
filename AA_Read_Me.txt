Read.me!

Aussfuehrliche Anleitung:
http://www.christians-bastel-laden.de/firmwareupload.html

Kurze Anleitung:

Damit sich die Firmware kompilieren laesst, benoetigt man folgende Librarys im Libraries-Ordner:

... die NeoPixel-Library: [url]https://github.com/adafruit/Adafruit_NeoPixel[/url] (der Download-Button ist rechts "Download ZIP")
... die DotStar-Library: [url]https://github.com/adafruit/Adafruit_DotStar[/url] (der Download-Button ist rechts "Download ZIP")
... die LPD8806-Library: [url]https://github.com/adafruit/LPD8806[/url] (der Download-Button ist rechts "Download ZIP")
... die MAX7219-Library: [url]https://github.com/wayoda/LedControl[/url] (der Download-Button ist rechts "Download ZIP")

P.S.: Leider hat die LPD8806-Library von Adafruit einen Bug, der das erfolgreiche Kompilieren im Moment verhindert.
Ich habe den Bug gemeldet, weiss aber nicht, wann er gefixt wird. Solange muss man es halt selber tun. Dazu...
... oeffnet man die Datei "LPD8806.h" aus dem Ordner "Sketchbook-Folder->libraries->LPD8806" und fuegt vor der Zeile

class LPD8806 {

die beiden Zeilen
#ifndef __LPD8806__H__
#define __LPD8806__H__

ein. Am Ende fuegt man die Zeile

#endif

ein.

Die Klasse sieht dann etwas eingedampft so aus:

[... Anfangscode...]
#ifndef __LPD8806__H__
#define __LPD8806__H__

class LPD8806 {
[... weiterer Code...]
};

#endif
