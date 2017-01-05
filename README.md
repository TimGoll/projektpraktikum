# Dokumentation des Projektpraktikums
Projektpraktikum (Mikrokontrollerpraktikum + Messtechnikpraktikum) Wintersemester 2016/17 am LMT

_Tim Goll, Matthias Baltes, Matthias Jost, Markus Herrmann-Wicklmayr_

## Aktuelles Demonstrationsvideo:
[Link zu YouTube Video](https://www.youtube.com/watch?v=2sz4_vMzZEc)

## Verwendete Bibliotheken:
1. [**mThread**](http://www.kwartzlab.ca/2010/09/arduino-multi-threading-librar/): <br>
 Erstellt Pseudothreads auf dem Board, die nacheinander ausgeführt werden. Jeder Thread hat seine eigene ```loop()```. <br>
 Threads werden hinzugefügt mittels ```main_thread_list -> add_thread(CLASSNAME)```, anschließend laufen sie unbegrenzt weiter. **Hinweis:** Es sind scheinbar maximal nur 10 Threads möglich.

2. [**newdel**](https://github.com/jlamothe/newdel): <br>
 Fügt die Keywörter ```new``` und ```delete``` hinzu. Wird für mThread benötigt und musste leicht angepasst werden, damit sie auf neueren Arduino-Boards funktioniert.

3. [**QueueList**](http://playground.arduino.cc/Code/QueueList): <br>
 Fügt verkettete Listen hinzu, welche beliebige Datentypen speichern können. **Hinweis:** es ist nicht möglich Pointer in ihnen zu speichern.

## Übertragungsprotokoll (LabView -> Microcontroller):
Die Übertragung erfolgt Zeilenweise in einem möglichst Übertragungssicheren Format. Als Anfangs und Endzeichen dienen zur Überprüfung der Vollständigkeit jeweils ein öffnender und schließender Tag. Alle Zeitangaben sind in Millisekunden, es gibt nur ganzzahliger Integerwerte. Die IDs beginnen bei 0.

**Zeilenweise Betrachtung der Übertragung**:

1. ```<Anzahl MFC, Anzahl Ventile>```
2. ```<Adresse MFC 0, Adresse MFC 1, ...>```
3. ```<Typ MFC 0, Typ MFC 1, ...>```
4. ```<Ventil-Pin-0, Ventil-Pin-1, ...>```
5. ```<Messintervall>```
6. ```<beginn>``` Ende des Headers, Beginn mit der Eventübertragung
7. ```<MFC oder Ventil, ID, Wert, Zeit>``` Setze Events. Hierbei müssen die Events je MFC/Ventil zeitlich sortiert sein, um eine einfachere Verarbeitung zu gewährleisten. Untereinander dürfen die Events jedoch vertauscht sein. (Zeit von MFC2 darf vor MFC1 sein, auch bei späterer Übertragung. Jedoch darf Zeit von MFC1 nicht vor der Zeit von MFC1 sein)
8. ```<end>``` Ende der Eventübertragung, warte auf Start der Messung
9. ```<start>``` Nicht zwigend notwendig, kann auch händisch per Taster gestartet werden

**Beispiel**:

```
<6,9>
<adresse0,adress1,adresse2,adresse3,adresse4,adresse5>
<typ0,typ1,typ2,typ3,typ4,typ5>
<2,3,4,5,6,7,8,9,10>
<10>
<begin>
<M,0,120,1000>
<M,0,240,1500>
<M,1,170,800>
<M,0,150,1700>
...
<V,0,1,1200>
...
<end>
<start>
```

## Serielle Kommunikation:
Drei Ports des Boards werden verwendet. Port 0 dient zur Kommunikation mit LabView (IN/OUT), Port 1 gibt Debug-Nachrichten aus, sofern der Debug-Schalter am Board aktiviert ist. Zur Kommunikation mit den MFCs dient Port 2.

Es gibt eine extra Klasse namens ```serialCommunication```, welche die Kommunikation über die 3 Ports verwaltet. Zu beginn muss in der ```setup()```-Funktion der Hauptino die Klasse initialisiert werden:
```cpp
srl->setSerial(&Serial, &Serial1, &Serial2); //labview / debug / uart
```
Anschließend kann man an beliebiger Stelle im Programm diese Klasse einfach nutzen:
```cpp
srl->print('D', "Hallo Welt!"); //(Typ: L, D, U / Ausgabetext)
srl->println('D', "Hallo Welt mit Zeilenende!"); //Natuerlich gibt es das ganze auch mit Linebreak
```
Der Typ der Ausgabe entscheidet, welcher Port genutzt wird. Hierbei gibt es drei Typen: L, D und U für LabView, Debug und UART. Die Baudrate wird in der **config.h** eingestellt.

Über den LabView Port wird nach Erfolgreicher Initialisierung des Boards ein "ready" gesendet. Wurde Ein Befehl korrekt erkannt und erfolgreich verarbeitet wird ein "ok" gesendet, ansonsten kommt ein Errorcode.

## Serielle Hardware
Die Verbindung zwischen dem Teensy und dem PC über Serielle Verbindung ist daher etwas schwer, da das Board keine eigene Möglichkeit der Kommunikation bietet. Abhilfe schafft jedoch der **Prolific PL2303HX** IC, welcher ein Uart Signal zu einem USB-Signal wandelt und dem PC ein USB-Device simuliert. Dieser Chip ist stanndardmäßig nicht mit Windowsversionen neuer als Windows 8 kompatibel, doch ein [inofizieller Treiber](http://www.ifamilysoftware.com/news37.html) schafft Abhilfe. <br>
Wieso wir diesen Chip dennoch genommen haben? - Ganz einfach, er wird in den meisten käuflich erhältlichen USB<->Uart bauteilen verwendet und somit gibt es auch am meisten Informationen zu diesem.

Die Treiber befinden sich auch in diesem GitHub Repository.

## Display:
Das Display ist via I2C mit dem Board verbunden, die Hintergrundbeleuchtung funktioniert mittels 3 PWM Anschlüssen für je eine Grundfarbe. Daraus können beliebige Hintergrundfarben gemischt werden. <br>
Je nach Meldungstyp ist die Hintergrundfarbe unterschiedlich. Folgende Typen gibt es:

- **Unkritische Fehler**: Error Code Level 1000; Dies sind Fehler, die beispielsweise bei der Kommunikation auftreten, aber keinen Programmabbruch benötigen. Sie werden auf dem Display für 2 Sekunden angezeigt. Farbe: _gelb_
- **Kritische Fehler**: Error Code Level 5000; Diese Fehler führen zu einem Absturz des Programms und werden auf dem Display angezeigt bis der Fehler behoben wurde. Farbe: _rot_

Standardmäßig werden auf dem Display aktuelle Daten zum Programmstatus angezeigt.

**Beispielanzeige**:
```
STANDARD:                      ERRORBEISPIEL:
+----------------------+       +----------------------+
|          #M:04 #V:07 |       |      ERROR 1001      |
|                      |       |                      |
| LAUFZEIT:00:01:34:17 |       |       Falscher       |
| V03-0001-00:01:16:22 |       |     Zeilenbeginn     |
+----------------------+       +----------------------+
```

- **Standardanzeige:** <br>
 Zeile 1: Anzahl MFCs und Anzahl Ventile <br>
 Zeile 2: xxx <br>
 Zeile 3: Laufzeit der Software, relativ zum Messstart <br>
 Zeile 4: Letztes Event, Typ_ID-Wert-Zeit
- **Erroranzeige:** <br>
 Zeile 1: Error ID <br>
 Zeile 2: xxx <br>
 Zeile 3+4: Kurzbeschreibung des augetretenen Fehlers

## Errormeldungen:
Bei allen Fehlermeldungen im 1000er Bereich wird das Programm weiterhin ausgeführt, es wird jedoch eine Wiederholung der entsprechenden Zeile erwartet, daher sind diese Codes LabView-Seitig abzufangen.

### 1000:
**Maximale Input-String-Länge überschritten.** In der _config.h_ wird eine Größe definiert, die pro Zeile gesendet werden darf. Diese Länge darf nicht überschritten werden.

### 1001:
**Falscher Zeilenbeginn.** Die Zeile muss mit einem öffenenden Tag ```<``` begonnen werden, damit sie als gültig akzeptiert wird. Dies dient zur Vollständigkeitsüberprüfung.

### 1002:
**Falsches Zeilenende.** Die Zeile muss mit einem schließenden Tag ```>``` beendet werden, damit sie als gültig akzeptiert wird. Dabei darf man jedoch nicht vergessen, dass die Stringeingabe mit einem Zeilenumbruch ```\n``` als Vollständig markiert wird. Dies dient zur Vollständigkeitsüberprüfung.

### 1003:
**Lese-Timeout überschritten.** In der _config.h_ wird eine maximale Lesezeit pro String definiert. Wird diese Zeit überschritten, wird das Lesen des Strings an dieser Stelle abgebrochen.

### 5000:
**Zufriff auf nicht definierte MFC/Ventil ID.** Trifft dieser Fall ein, dann wird eine irreversible Errormeldung geworfen, die nur duch einen Programmneustart behoben werden kann. Man sollte in diesem Fall seine Eingaben überprüfen, ob in den Events nur auf vorher definierte MFCs/Ventile zugegriffen wird.

## Programmaufbau:
### Hauptdatei:
1. **Controller.ino**: [[ino]](../master/controller/controller.ino) <br>
 Hier werden die Hauptobjekte erstellt und verwaltet. Außerdem werden an dieser Stellte die Threads gestartet und später auch wieder gestoppt. Vor dem Start der Threads werden noch Adressen zwischen den einzelnen Objekten ausgetauscht (beispielsweise die der Queues zum späteren Datenaustausch).

### Hauptklassen:
Aus allen Klassen mit einem "main" im Namen wird immer nur **ein** Objekt abgeleitet. Außerdem besitzen sie eine ```loop()```-Funktion, da die Klasse in Pseudothreads ausgeführt wird.

1. **main_labCom** [[cpp]](../master/controller/src/main_labCom.cpp) [[h]](../master/controller/src/main_labCom.h): <br>
 Quasi Hauptklasse des Programms, verwaltet IN/OUT mit LabView

2. **main_boschCom** [[cpp]](../master/controller/src/main_boschCom.cpp) [[h]](../master/controller/src/main_boschCom.h):
3. **main_stringBuilder** [[cpp]](../master/controller/src/main_stringBuilder.cpp) [[h]](../master/controller/src/main_stringBuilder.h):
 Diese Klasse sammelt sich per Abfragen alle Daten zusammen und baut im Messtakt daraus Strings, welche an LabCom und StoreD weiter gegeben werden.

4. **main_mfcCtrl** [[cpp]](../master/controller/src/main_mfcCtrl.cpp) [[h]](../master/controller/src/main_mfcCtrl.h): <br>
 Verwaltet alle MFC Objekte

5. **main_valveCtrl** [[cpp]](../master/controller/src/main_valveCtrl.cpp) [[h]](../master/controller/src/main_valveCtrl.h): <br>
 Verwaltet alle Ventil Objekte

6. **main_display** [[cpp]](../master/controller/src/main_display.cpp) [[h]](../master/controller/src/main_display.h):

### Nebenklassen:
1. **mfcCtrl** [[cpp]](../master/controller/src/mfcCtrl.cpp) [[h]](../master/controller/src/mfcCtrl.h):
2. **valveCtrl** [[cpp]](../master/controller/src/valveCtrl.cpp) [[h]](../master/controller/src/valveCtrl.h):
3. **main_StoreD** [[cpp]](../master/controller/src/StoreD.cpp) [[h]](../master/controller/src/StoreD.h):
4. **serialCommunication** [[cpp]](../master/controller/src/ownlibs/serialCommunication.cpp) [[h]](../master/controller/src/ownlibs/serialCommunication.h):

### Sonstige:
1. **common** [[cpp]](../master/controller/src/ownlibs/common.cpp) [[h]](../master/controller/src/ownlibs/common.h): <br>
 Standardfunktionen, die Überall gebraucht werden (```trim()```, ```getTimeString(time, timeString)```, ...)

2. **config** [[h]](../master/controller/src/config.h): <br>
 Einstellmöglichkeiten diverster Parameter.

3. **eventElement** [[h]](../master/controller/src/eventElement.h): <br>
 Event-Struct, welches von MFCs und Ventilen verwendet wird.

3. **errors** [[cpp]](../master/controller/src/errors.cpp) [[h]](../master/controller/src/errors.h): <br>
 CharArray, in welchem die Displayausgaben der einzelnen Errormeldungen gespeichert sind.

## Testskript [[py]](../master/serial_connector_script/serial_connection.py)
Derzeit gibt es ein kleines Testskript zum Test der Steuerungssoftware auf dem Board. Bei der Ausführung ist zu beachten, dass keine andere Serielle Verbindung geöffnet sein darf (Arduino-Debug-Monitor, ...).

Zur Ausführung müssen ```Python 2.7``` und ```pySerial``` installiert sein und bestenfalls die PATH-Variable für die Konsole gesetzt sein. Da das Programm hardgecoded ist, muss der Port in Zeile 10 angepasst werden. Der Port ist beispielsweise in der Arduino IDE sehr einfach einzusehen.

Ist alles vorbereitet wird das Skript mit ```python serial_connection.py``` ausgeführt, insofern man sich im Verzeichnis dieser Datei befindet. In der Datei kann in einem Array die Übertragung definiert werden.

[[Einrichtung von Python (Windows)]] (https://learn.adafruit.com/arduino-lesson-17-email-sending-movement-detector/installing-python-and-pyserial)

## LabView:

## Hardware:
### Hauptboard
1. Schaltkizze: [[link]](../master/electronic/Schalplan.pdf)
2. Verwendete Bauteile auf dem Haupboard:
 * **Teensy 3.6** Board [[link]](http://www.pjrc.com/teensy/) <br>
  Vorderseite [[link]](http://www.pjrc.com/teensy/beta/card9a_rev1_print.pdf) <br>
  Rückseite [[link]](https://www.pjrc.com/teensy/card9b_rev1.pdf)
 * **20x4 LCD Display (Buntes Backlight)** [[link]](https://www.adafruit.com/product/499) <br>
  RGB-LCD Pin Header [[link]](https://cdn-shop.adafruit.com/datasheets/WH2004A-CFH-JT%23.pdf) <br>
  I2C-Chip für Display (PCF8574) [[link]](http://www.sunfounder.com/wiki/images/1/18/PCF8574T_datasheet.pdf)


## Sonstiges:
1. **MarkdownGuide** [[link]](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
2. **cloc-1.70.exe** [[link]](https://github.com/AlDanial/cloc)

 Ein kleines Programm zur Ausführung in der Konsole, welches den Softwareumfang darstellt. Wird wie folgt benutzt: ```prompt> cloc-1.70.exe controller/src``` (Auszuführen im Basepath des Projektes, dort liegt auch die exe)

 Ergibt folgendes:
 ```
 prompt> cloc-1.70.exe controller/src
       24 text files.
       24 unique files.
        2 files ignored.

 https://github.com/AlDanial/cloc v 1.70  T=0.50 s (48.0 files/s, 2734.0 lines/s)
 -------------------------------------------------------------------------------
 Language                     files          blank        comment           code
 -------------------------------------------------------------------------------
 C++                             11            185             50            665
 C/C++ Header                    13             72             88            307
 -------------------------------------------------------------------------------
 SUM:                            24            257            138            972
 -------------------------------------------------------------------------------
 ```

## gesammeltes (zu sortieren):
1. Send Email [[link]](http://playground.arduino.cc/Code/Email)
2. Write to SD (kann bei diesem Board anders sein, Dokumentation lesen!) [[link]](https://www.arduino.cc/en/Tutorial/ReadWrite)
3. LabView-Network [[link]](http://www.ni.com/white-paper/2710/de/)
4. Arduino-Network-Communication [[link]](https://github.com/evothings/evothing)
