///////////////////////////////////////////////////////////////////////////////
///// HAUPTDATEI //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

#include <src/boschCom.cpp>
#include <src/labCom.cpp>
#include <src/mfcCtrl.cpp>
#include <src/valveCtrl.cpp>
#include <src/StoreD.cpp>

// Diese Datei wird unabhaengig von der Haupt-*.ino kompiliert und ausgefuehrt
// Achtung: Die Datei darf nicht "main.cpp", sowie "projekpraktikum.ino" heissen

int main () {
    init(); //Arduino_init() - Wird gebraucht fuer millis(), analogRead(), ...

    // Ab hier kommt unser Code
}
