///////////////////////////////////////////////////////////////////////////////
///// HAUPTDATEI //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Includes mit '< >' sind Bibliotheken, die im Standardverzeichnis gesucht werden,
// mit ' " " ' können es eigenen Dateien in beliebigen Ordnern sein

#include <Arduino.h>

#include "src/boschCom.h"
#include "src/labCom.h"
#include "src/mfcCtrl.h"
#include "src/valveCtrl.h"
#include "src/StoreD.h"

// Diese Datei wird unabhaengig von der Haupt-*.ino kompiliert und ausgefuehrt
// Achtung: Die Datei darf nicht "main.cpp", sowie "controller.ino" heissen

int main () {
    init(); //Arduino_init() - Wird gebraucht fuer millis(), analogRead(), ...

    communication::LabCom test;

    // Ab hier kommt unser Code
    Serial.begin(9600);
    Serial.println(test.return5());
}
