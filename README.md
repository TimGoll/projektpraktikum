# Dokumentation des Projektpraktikums
Projektpraktikum (Mikrokontrollerpraktikum + Messtechnikpraktikum) Winter- + Sommersemester 2016/17 am LMT

_Tim Goll, Matthias Baltes, Matthias Jost, Markus Herrmann-Wicklmayr_

## Aktuelle Demonstrationsvideos:
[Demovideo 1 (03.12.2016)](https://www.youtube.com/watch?v=2sz4_vMzZEc) <br>
[Demovideo 2 (04.06.2017)](https://www.youtube.com/watch?v=daKc67VaTdY)

## Verwendete Bibliotheken:
1. [**mThread**](http://www.kwartzlab.ca/2010/09/arduino-multi-threading-librar/): <br>
 Erstellt Pseudothreads auf dem Board, die nacheinander ausgeführt werden. Jeder Thread hat seine eigene ```loop()```. <br>
 Threads werden hinzugefügt mittels ```main_thread_list -> add_thread(CLASSNAME)```, anschließend laufen sie unbegrenzt weiter. **Hinweis:** Es sind scheinbar maximal nur 10 Threads möglich.

2. [**newdel**](https://github.com/jlamothe/newdel): <br>
 Fügt die Keywörter ```new``` und ```delete``` hinzu. Wird für mThread benötigt und musste leicht angepasst werden, damit sie auf neueren Arduino-Boards funktioniert.

3. [**QueueList**](http://playground.arduino.cc/Code/QueueList): <br>
 Fügt verkettete Listen hinzu, welche beliebige Datentypen speichern können. **Hinweis:** es ist nicht möglich Pointer in ihnen zu speichern.

4. [**Adafruit BME280**](https://github.com/adafruit/Adafruit_BME280_Library) und [**Adafruit Sensor**](https://github.com/adafruit/Adafruit_Sensor): <br>
  Ansteuerung des Boschsensors (Die Version im Repo ist leicht verändert).

## Programmablauf:
1. Programm startet nach Öffnen der Seriellen Verbindung (Reset erfolgt automatisch auf dem Arduino; zu schauen, wie dies auf dem Teensy zu erreichen ist, eventuell Reset-Pin vom USB<->UART-Chip abgreifen (sofern vorhanden)?)
2. Programm empfängt Daten (werden überprüft) und startet nach einem Tasterdruck/"Start"-Befehl
3. Alternativ kommt man, bevor man eine Übertragung gestartet hat, mit einem Tastendruck in ein Menü, von wo aus man Programme, die auf der SD-Karte gespeichert sind, auswählen und starten kann.
4. Arbeitet Eventlisten ab, gibt Debug-Informationen aus, sofern über Schalter aktiviert, während der Laufzeit Einstellbar
5. Im Idle-Modus nach Vollständiger Abarbeitung aller Events; nach Abschluss des Programms leuchtet eine LED und das Display färbt sich rot

## Übertragungsprotokoll (LabView -> Microcontroller):
Die Übertragung erfolgt Zeilenweise in einem möglichst Übertragungssicheren Format. Als Anfangs und Endzeichen dienen zur Überprüfung der Vollständigkeit jeweils ein öffnender und schließender Tag. Alle Zeitangaben sind in Millisekunden, es gibt nur ganzzahliger Integerwerte. Die IDs beginnen bei 0.

**Zeilenweise Betrachtung der Übertragung**:

1. ```<Anzahl MFC, Anzahl Ventile>```
2. ```<Adresse MFC 0, Adresse MFC 1, ...>```
3. ```<Typ MFC 0, Typ MFC 1, ...>```
4. ```<Ventilplatinenadresse1, ...>``` Hexadezimale Ventilplatinenadressen
5. ```<Platinen-ID Ventil-Pin-ID, Platinen-ID Ventil-Pin-ID, ...>```
6. ```<Messintervall>```
7. ```<Datum + Zeit>``` Aktuelles Datum & Uhrzeit. Wird im Header der Messung gespeichert, relativ dazu wird gemessen
8. ```<begin>``` Ende des Headers, Beginn mit der Eventübertragung
9. ```<MFC oder Ventil, ID, Wert, Zeit>``` Setze Events. Hierbei müssen die Events je MFC/Ventil zeitlich sortiert sein, um eine einfachere Verarbeitung zu gewährleisten. Untereinander dürfen die Events jedoch vertauscht sein. (Zeit von MFC2 darf vor MFC1 sein, auch bei späterer Übertragung. Jedoch darf Zeit von MFC1 nicht vor der Zeit von MFC1 sein)
10. ```<end>``` Ende der Eventübertragung, warte auf Start der Messung
11. ```<start>``` Nicht zwingend notwendig, kann auch händisch per Taster gestartet werden

**Beispiel**:

```
<6,9>
<adresse0,adress1,adresse2,adresse3,adresse4,adresse5>
<typ0,typ1,typ2,typ3,typ4,typ5>
<0x20>
<0 0, 0 1, 0 2, 0 3, 0 4, 0 6, 0 9, 0 12, 0 15>
<10>
<08.02.2017-13:03:29>
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

Über den LabView Port wird nach erfolgreicher Initialisierung des Boards ein _"ready"_ gesendet. Wurde ein Befehl korrekt erkannt und erfolgreich verarbeitet wird ein _"ok"_ gesendet, ansonsten kommt ein Errorcode.

## Serielle Hardware
Die Verbindung zwischen dem Teensy und dem PC über Serielle Verbindung wird mittels dem IC **CH340G** realisiert. Dieser funktioniert Plug and Play unter Windows (Windows 10 getestet) und lässt sich auch [mit einfachen Treibern](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver) unter MacOS zum Laufen bringen.

## SD Karte
Das SD System des Teensy scheint auf DOS zu beruhen, zumindest sind maximale Dateinamen 4 Zeichen lang (Großbuchstaben), Dateinamen, die länger als diese acht Zeichen sind, werden automatisch gekürzt. Beim Speichern in eine Datei ist es jedoch wichtig, dass der Dateiname maximal 8 Zeichen hat, ansonsten tritt ein Fehler auf.

**Namensschema:** ```YYMMDDXX```<br>
Datum in Zweierschreibweise, zwei X für eine fortlaufende Nummerierung. Es sind maximal 100 (0..99) Messungen an einem Tag möglich.

Beim Speichern auf der SD-Karte wird eine Datei mit einem Dateinamen, der das Datum und die Messungsnummer (falls mehrere Messungen pro Tag) enthält, erzeugt. Im gleichen Zuge wird in der txt-Datei ein Header in Klartext erstellt. Dieser enthält wesentliche Informationen wie die Startuhrzeit der Messungen, die Anzahl der MFC's und Ventile, sowie die Typen der MFC's. Nach dem Header folgt eine Leerzeile und dann die Messdaten in kodierter Form.


## I2C
I2C wird von uns als Hauptkommunikationsbus verwendet. Folgende Adressen sind belegt (In der Klammer stehen die alternativen Adressen):
- Display: 0x38 (--)
- BME280: 0x76 (0x77)
- IO Expander (Ventilplatine, PCA9555): 0x20 (0x21 .. 0x27)

## BME280
Der Boschsensor wird innerhalb der Klasse ```main_boschCom``` verwaltet. Dort wird ein bme-Objekt erstellt. Dieses Objekt wird geliefert von einer leicht angepassten Adafruit-Bibliothek. (Diese könnte man noch weiter reduzieren, da der ganze SPI Bereich eigentlich nicht gebraucht wird)

**Anschluss des Sensors**
Folgende Pinbelegung ergab sich nach einigem Durchmessen mit einem Multimeter. Bisher erwies sich dieses Layout als korrekt.
```
        +----\_/----+
    VDD |o         o|                     VDD und VDDIO   : 3V3
  VDDIO |o         o|                     GND             : 0V
    GND |o         o|                     SDO (Adresspin) : 0V (0x76), 3V3 (0x77)
    SDO |o         o|                     CSB             : 3V3
    SDA |o         o|                     SDA / SCL       : I2C Bus (3V3 level)
    SCL |o         o|
    CSB |o   |_|   o|
        |o         o|
        |o         o|
        |o         o|
        |o         o| (SCL)
        |o         o| (SDA)
        |o         o|
        |o         o|
        |           |
        +-----------+
```
Den Sensor auslesen kann man in der besagten ```main_boschCom``` mittels folgenden Befehlen:
```cpp
this->bme280->readTemperature();
this->bme280->readPressure();
this->bme280->readHumidity();
```

## MFCs
Bei unserem Aufbau verwenden wir zwei verschiedene Typen von MFCs. **Bürkert** und **MKS** sind die beiden Hersteller, dazu gibt es noch alte analoge MFCs, die von einer anderen Projektgruppe digitalisiert wurden. Diese basieren dadurch auch auf dem **Bürkert** Protokoll. <br>
Von uns angesteuert werden alle MFCs von ihrer jeweiligen Klasse, je nach Typ bekommen sie bei der Initialisierung jedoch ein anderes Kommunikationsobjekt übergeben. Dieses Objekt führt verschiedene Befehle aus und gibt gegebenefalls Daten an die MFC Klasse zurück.

### Bürkert [[Handbuch]](https://www.buerkert.de/de/Media/plm/MAN/MA/MA8006-Supplem.MFC-DE-DE.pdf)
Hier die für uns interessanten Informationen rausgeschrieben, siehe Seite 8ff.
```
Präampel:       2..20 mal 0xFF          // Synchronisierung des Datenverkehrs
Startzeichen:   Master->Slave 0x02      // WAS?
                Slave->Master 0x06      // WAS? Definiert dies, ob eine Antwort erwartet wird?
Adresse:        BIT 7:    1             // unser Master ist Primär
                BIT 6:    0             // kein Burstmodus Hier
                BIT 5..0: 0..32         // Pollingadresse
Befehl:         8-bit Zahl
                ohne 31, 127, 254, 255
Bytezählwert:   Anzahl der Antwortbytes + Datenbytes
Antwortcode:    Nur für Slave->Master (2 Byte groß)
Daten:          Hängen von Befehl ab
Checksumme:     8-bit XOR Kombination von Startzeichen bis Daten
```
Befehle:
```
0x03 - ReadCurrentAndFourDynamicVariables (S.14)
0x92 - ExtSetpoint (S.18)
```
Flusseinheit in Prozent. Maximal 40,976sccm.

### MKS [[Handbuch]](https://www.johnmorrisgroup.com/Content/Attachments/120115/MF1-man.pdf)
"Human Readable Protocol" siehe Seite 73ff, übertragung alles in Ascii Chars.
```
Präambel:       @ (Synchronisierung des Datenverkehrs)
Adresse:        2 Byte, 00 bis 99?
Befehl:         1-stelliger Befehlscode (S.74)
Wert:           1.00000 (Beispielhaft, aber diese Bytegröße)
Terminierung:   \r (Carriage Return)
```
Befehle:
```
F  - Lese aktuellen Fluss (S.74)
S  - Setze Fluss (S.74)
```
Fluss: 0..500 sccm (S.16)

Flusseinheit in sccm (cm^3/min), 100% ensprechen 400k
### allgemein:
- Fehlermeldungen werden weitergereicht an Display und LabView
- Baudrate ist 9600
- **Wir nehmen sccm als Standardeinheit für beide Typen!** Für Bürkert wird der Wert auf dem Board umgerechnet

## Ventile
Die Ventilsteuerung befindet sich auf einer eigenen Platine. Das hat für uns die beiden Vorteile, dass zum Einen Kurzschlüsse auf der Ventilplatine keinen Schaden an der Hauptsteuerung verursachen können und zum Anderen es uns möglich ist "beliebig" viele Ventile mit einem Microcontroller zu steuern.

Theoretisch sind bis zu 8 Ventilplatinen mit je 16 Ventilen nötig, also insgesamt 128 Stück. Auf der Platine befindet sich ein dreipoliger DIP-Switch zum Einstellen der Adresse (Adressraum: 0x20..0x27). Die Adressen sind bei Programmstart per LabView an den Controller zu übertragen. Jede der acht Platinen hat 16 Pins (0 bis 15), die auch bei der Initailisierung so angegeben werden müssen (```<Platinen-ID Pin-ID, ...>```), intern haben die Ventile jedoch einen andere fortlaufende Numerierung.

Die softwareseitige Ansteuerung wird mittels einer kleinen von uns geschriebenen Bibiliothek namens ```pca9555``` (Name des I2C ICs) realisiert. Angelehnt an die Syntax von der Arduino-Umgebung gibt es auch hier eine ```digitalWrite(pinNumber)```-Funktion, der Einfachheit halber haben wir jedoch auf weitere Funktionen verzichtet und bei Porgammstart werden automatisch alle Pins als Ausgang definiert.

## Display:
Das Display ist via I2C mit dem Board verbunden, die Hintergrundbeleuchtung funktioniert mittels 3 PWM Anschlüssen für je eine Grundfarbe. Daraus können beliebige Hintergrundfarben gemischt werden. <br>
Je nach Meldungstyp ist die Hintergrundfarbe unterschiedlich. Folgende Typen gibt es:

- **Unkritische Fehler**: Error Code Level 1000; Dies sind Fehler, die beispielsweise bei der Kommunikation auftreten, aber keinen Programmabbruch benötigen. Sie werden auf dem Display für 2 Sekunden angezeigt. Farbe: _gelb_
- **Kritische Fehler**: Error Code Level 5000; Diese Fehler führen zu einem Absturz des Programms und werden auf dem Display angezeigt bis der Fehler behoben wurde. Farbe: _rot_

Standardmäßig werden auf dem Display aktuelle Daten zum Programmstatus angezeigt.

**Beispielanzeige**:
```
STANDARD:                      ERRORBEISPIEL:                 ENDANZEIGE:
+----------------------+       +----------------------+       +----------------------+
|          #M:04 #V:07 |       |      ERROR 1001      |       |          #M:04 #V:07 |
|                      |       |                      |       |                      |
| LAUFZEIT:00:01:34:17 |       |       Falscher       |       |    ABGESCHLOSSEN     |
| V03-0001-00:01:16:22 |       |     Zeilenbeginn     |       |     02:05:24:34      |
+----------------------+       +----------------------+       +----------------------+
Hintergrund: weiß              Hintergrund: organge/rot       Hintergrund: grün
```

- **Standardanzeige:** <br>
 Zeile 1: Anzahl MFCs und Anzahl Ventile <br>
 Zeile 2: xxx <br>
 Zeile 3: Laufzeit der Software, relativ zum Messstart <br>
 Zeile 4: Letztes Event, Typ-ID-Wert-Zeit
- **Erroranzeige:** <br>
 Zeile 1: Error ID <br>
 Zeile 2: xxx <br>
 Zeile 3+4: Kurzbeschreibung des aufgetretenen Fehlers
- **Endanzeige:** <br>
 Zeigt durchgängig diesen Text an, lässt sich nur zurücksetzen durch Reset des Boards

## Errormeldungen:
Bei allen Fehlermeldungen im 1000er Bereich wird das Programm weiterhin ausgeführt, es wird jedoch eine Wiederholung der entsprechenden Zeile erwartet, daher sind diese Codes LabView-Seitig abzufangen.

### 1000:
**Maximale Input-String Länge überschritten.** In der _config.h_ wird eine Größe definiert, die pro Zeile gesendet werden darf. Diese Länge darf nicht überschritten werden.

### 1001:
**Falscher Zeilenbeginn.** Die Zeile muss mit einem öffnenden Tag ```<``` begonnen werden, damit sie als gültig akzeptiert wird. Dies dient zur Vollständigkeitsüberprüfung.

### 1002:
**Falsches Zeilenende.** Die Zeile muss mit einem schließenden Tag ```>``` beendet werden, damit sie als gültig akzeptiert wird. Dabei darf man jedoch nicht vergessen, dass die Stringeingabe mit einem Zeilenumbruch ```\n``` als Vollständig markiert wird. Dies dient zur Vollständigkeitsüberprüfung.

### 1003:
**Lese-Timeout überschritten.** In der _config.h_ wird eine maximale Lesezeit pro String definiert. Wird diese Zeit überschritten, wird das Lesen des Strings an dieser Stelle abgebrochen.

### 1004:
**Falsche Anzahl an Argumenten.** Bei der Übertragung von LabView an den Controller wird immer überprüft, ob die Anzahl an eingetroffenen Elementen der Anzahl an erwarteten Elementen entspricht.

### 5000:
**Zufriff auf nicht definierte MFC/Ventil ID.** Trifft dieser Fall ein, dann wird eine irreversible Errormeldung geworfen, die nur duch einen Programmneustart behoben werden kann. Man sollte in diesem Fall seine Eingaben darauf überprüfen, ob in den Events nur auf vorher definierte MFCs/Ventile zugegriffen wird.

## Programmaufbau:
### Hauptdatei:
1. **Controller.ino**: [[ino]](../master/controller/controller.ino) <br>
 Hier werden die Hauptobjekte erstellt und verwaltet. Außerdem werden an dieser Stellte die Threads gestartet und später auch wieder gestoppt. Vor dem Start der Threads werden noch Adressen zwischen den einzelnen Objekten ausgetauscht (beispielsweise die der Queues zum späteren Datenaustausch).

### Hauptklassen:
Aus allen Klassen mit einem "main" im Namen wird immer nur **ein** Objekt abgeleitet. Außerdem besitzen sie eine ```loop()```-Funktion, da die Klasse in Pseudothreads ausgeführt wird.

1. **main_labCom** [[cpp]](../master/controller/src/main_labCom.cpp) [[h]](../master/controller/src/main_labCom.h): <br>
 Quasi Hauptklasse des Programms, verwaltet IN/OUT mit LabView

2. **main_boschCom** [[cpp]](../master/controller/src/main_boschCom.cpp) [[h]](../master/controller/src/main_boschCom.h):
3. **main_stringBuilder** [[cpp]](../master/controller/src/main_stringBuilder.cpp) [[h]](../master/controller/src/main_stringBuilder.h): <br>
 Diese Klasse sammelt sich per Abfragen alle Daten zusammen und baut im Messtakt daraus Strings, welche an LabCom und StoreD weiter gegeben werden. Diese Klasse erzeugt und verwaltet StoreD.

4. **main_mfcCtrl** [[cpp]](../master/controller/src/main_mfcCtrl.cpp) [[h]](../master/controller/src/main_mfcCtrl.h): <br>
 Verwaltet alle mfcCtrl Objekte

5. **main_valveCtrl** [[cpp]](../master/controller/src/main_valveCtrl.cpp) [[h]](../master/controller/src/main_valveCtrl.h): <br>
 Verwaltet alle valveCtrl Objekte

6. **main_display** [[cpp]](../master/controller/src/main_display.cpp) [[h]](../master/controller/src/main_display.h):

### Nebenklassen:
1. **mfcCtrl** [[cpp]](../master/controller/src/mfcCtrl.cpp) [[h]](../master/controller/src/mfcCtrl.h):
2. **valveCtrl** [[cpp]](../master/controller/src/valveCtrl.cpp) [[h]](../master/controller/src/valveCtrl.h):
3. **mfcCom** [[cpp]](../master/controller/src/mfcCom.cpp) [[h]](../master/controller/src/mfcCom.h):
4. **mfcCom_buerkert** [[cpp]](../master/controller/src/mfcCom_buerkert.cpp) [[h]](../master/controller/src/mfcCom_buerkert.h):
5. **mfcCom_mks** [[cpp]](../master/controller/src/mfcCom_nks.cpp) [[h]](../master/controller/src/mfcCom_mks.h):
6. **StoreD** [[cpp]](../master/controller/src/StoreD.cpp) [[h]](../master/controller/src/StoreD.h): <br>
 Sorgt dafür, dass Daten auf der SD Karte gespeichert werden.
7. **parseInput** [[cpp]](../master/controller/src/parseInput.cpp) [[h]](../master/controller/src/parseInput.h): <br>

### Eigene Bibliotheken
1. **serialCommunication** [[cpp]](../master/controller/src/ownlibs/serialCommunication.cpp) [[h]](../master/controller/src/ownlibs/serialCommunication.h):
 Zerlegt den ankommenden String und verteilt die Daten an alle Steuerungsobjekte.
2. **inputHandler** [[cpp]](../master/controller/src/ownlibs/inputHandler.cpp) [[h]](../master/controller/src/ownlibs/inputHandler.h): <br>
 Kümmert sich um Tastereingaben, ruft Callbackfunktionen auf. Entprellt und erkennt Flanken.
3. **lcd_I2C** [[cpp]](../master/controller/src/ownlibs/lcd_I2C.cpp) [[h]](../master/controller/src/ownlibs/lcd_I2C.h): <br>
 Steuert das Display an
4. **pca9555** [[cpp]](../master/controller/src/ownlibs/pca9555.cpp) [[h]](../master/controller/src/ownlibs/pca9555.h): <br>
 Steuert die Ventilplatine via I2C an

### Sonstige:
1. **common** [[cpp]](../master/controller/src/ownlibs/common.cpp) [[h]](../master/controller/src/ownlibs/common.h): <br>
 Standardfunktionen, die Überall gebraucht werden (```trim()```, ```getTimeString(time, timeString)```, ...)

2. **config** [[h]](../master/controller/src/config.h): <br>
 Einstellmöglichkeiten diverser Parameter.

3. **eventElement** [[h]](../master/controller/src/eventElement.h): <br>
 Event-Struct, welches von MFCs und Ventilen verwendet wird.

3. **errors** [[cpp]](../master/controller/src/errors.cpp) [[h]](../master/controller/src/errors.h): <br>
 CharArray, in welchem die Displayausgaben der einzelnen Errormeldungen gespeichert sind.

## Testskript [[py]](../master/serial_connector_script/serial_connection.py)
Derzeit gibt es ein kleines Testskript zum Test der Steuerungssoftware auf dem Board. Bei der Ausführung ist zu beachten, dass keine andere Serielle Verbindung geöffnet sein darf (Arduino-Debug-Monitor, ...).

Zur Ausführung müssen ```Python 2.7``` und ```pySerial``` installiert sein und bestenfalls die PATH-Variable für die Konsole gesetzt sein. Da das Programm hardgecoded ist, muss der Port in Zeile 10 angepasst werden. Der Port ist beispielsweise in der Arduino IDE sehr einfach einzusehen.

Ist alles vorbereitet wird das Skript mit ```python serial_connection.py``` ausgeführt, insofern man sich im Verzeichnis dieser Datei befindet. In der Datei kann in einem Array die Übertragung definiert werden.

[[Einrichtung von Python (Windows)]](https://learn.adafruit.com/arduino-lesson-17-email-sending-movement-detector/installing-python-and-pyserial)

## LabView:
@Matthias Baltes: TODO!


## Elektronik:
### Pinbelegung
#### Teensy
Untenstehend eine Zeichnung des Teensyboards mit allen Anschlüssen und unserer aktuellen Belegung.
```
                                         U
                                +------| S |------+
                                |o GND |_B_| VIN o|
                  Labview - RX1 |o 00        GND o|
                  LabView - TX1 |o 01        3V3 o|
                                |o 02         23 o|
                                |o 03         22 o|
                                |o 04         21 o|
                                |o 05         20 o| EINGANG - Debug-Schalter
            Debug-LED - Ausgang |o 06         19 o| I2C - SCL0
                    Debug - RX3 |o 07         18 o| I2C - SDA0
                    Debug - TX3 |o 08         17 o| 
                 Uart-MKS - RX2 |o 09         16 o| EINGANG - OK-Taster
                 Uart-MKS - TX2 |o 10         15 o| EINGANG - Runter-Taster
                                |o 11         14 o| EINGANG - Hoch-Tater
                                |o 12         13 o| Abgeschlossen-LED - AUSGANG
                                |o 3V3       GND o|
                                |o 24            o|
                                |o 25            o|
           Enable MKS - AUSGANG |o 26         39 o|
       Enable Bürkert - AUSGANG |o 27         38 o|
                                |o 28         37 o| PWM - Displaybeleuchtung blau
                                |o 29         36 o| PWM - Displaybeleuchtung grün
                                |o 30  _____  35 o| PWM - Displaybeleuchtung rot
             Uart-Bürkert - RX4 |o 31 |  S  | 34 o|
             Uart-Bürkert - TX4 |o 32 |  D  | 33 o|
                                +-----------------+
```
#### Display
PIN 		Belegung
```
        +---+
  PWM_1 |o  |18                     
  PWM_2 |o  |                     
  PWM_3 |o  |                     
 5V_LED |o  |                     
  I/O_3 |o  |                     
  I/O_2 |o  |
  I/O_1 |o  |
  I/O_0 |o  |
        |o  |
        |o  |
        |o  | 
        |o  | 
  I/O_4 |o  |
  I/O_5 |o  |
  I/O_6 |o  |
   Poti |o  |
V_IN_5V |o  |
    GND |o  |1
        +---+
```
### Hardware
1. Schaltkizze: [[link]](../master/electronic/Schalplan.pdf)
2. Verwendete Bauteile auf dem Haupboard:
 * **Teensy 3.6** Board [[link]](http://www.pjrc.com/teensy/) <br>
  Vorderseite [[link]](http://www.pjrc.com/teensy/beta/card9a_rev1_print.pdf) <br>
  Rückseite [[link]](https://www.pjrc.com/teensy/card9b_rev1.pdf)
 * **20x4 LCD Display (Buntes Backlight)** [[link]](https://www.adafruit.com/product/499) <br>
  RGB-LCD Pin Header [[link]](https://cdn-shop.adafruit.com/datasheets/WH2004A-CFH-JT%23.pdf) <br>
  I2C-Chip für Display (PCF8574) [[link]](http://www.sunfounder.com/wiki/images/1/18/PCF8574T_datasheet.pdf)
3. Weitere Dinge:
 * **Wandlung "TTL" (3V3) auf RS-485** (Teensy auf RS485 für die MFCs) [[link]](https://datasheets.maximintegrated.com/en/ds/MAX14775E-MAX14776E.pdf) <br>
 Seite 3 Logic Inputs, 3V3 funktioniert
 Seite 2 Logic Output, Vcc-0,2V
 DE High für enable
 Dieser IC kann sowohl 5V als auch 3V3 als High Pegel erkennen. Dadurch werden Pegelwandler eingespart.
 * **Anschließen der Ventile** [[link]](http://www.nxp.com/documents/data_sheet/PCA9555.pdf) <br>
  Werden über I2C angesteuert (16bit I2C GPIO-Expander dient als Erweiterung) <br>
  Jede Ventilplatine hat einen PCA9555DB mit 8 möglichen Adressen. Dadurch sind bis zu 8 Ventilplatinen an einem Teensy möglich<br>
  Dieser IC läuft auch auf 3V3, lässt sich also OHNE Pegelwandler betreiben<br>
  Hängt mit allen anderen I2C Geräten an Bus 0
 * **Ventilansteuerung** [[link]](http://www.infineon.com/dgdl/Infineon-BTS555-DS-v01_00-en.pdf?fileId=db3a30432ba3fa6f012bd3dfdd0b3b65) <br>

 * **UART <-> USB** <br>
  Integrierte Lösung: http://www.ebay.de/itm/252960740317?_trksid=p2057872.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT
#### MFC
### Bürkert
S.4 Datenblatt für die Anschlüsse am D-Sub Stecker

### MKS
S.68 Datenblatt für die Anschlüsse am D-Sub Stecker

#### Ventile
Die Steuersignale aus dem I/O Expander werden durch einen Operationsverstärker (einer pro 4 Ventile) von den 24V Signalen getrennt.
Ein Operationsverstärker kann 4 Transistoren unabhängig schalten welche die Ventile mit der 24V verbinden.


#### USB Anschlüsse
USB1 (näher am Teensy) : LabView über RX1, TX1
USB2: Debug über RX3, TX3

#### LED Anzeigen
Über den USB Anschlüssen ist die Power-LED
Über dem Teensy Bord, neben dem Debug Schalter, ist die Debug LED.

#### PCF8547

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

 https://github.com/AlDanial/cloc v 1.70  T=0.50 s (72.0 files/s, 6490.0 lines/s)
 -------------------------------------------------------------------------------
 Language                     files          blank        comment           code
 -------------------------------------------------------------------------------
 C++                             17            424            235           1564
 C/C++ Header                    19            158            175            689
 -------------------------------------------------------------------------------
 SUM:                            36            582            410           2253
 -------------------------------------------------------------------------------
 ```
