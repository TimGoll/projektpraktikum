// Dies ist die Hauptdatei unseres Prpgramms. Der Name muss identisch zum Programmverzeichnis sein.
// Hier werden alle Bibliotheken importiert, Threads gestartet und verwaltet

// INCLUDES
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <QueueList.h> //vielleicht hier nicht gebraucht

//Klassen aus Includes mit einem "main" im Namen werden in dieser Datei erstellt
//und haben einen Thread
#include "src/main_labCom.h"
#include "src/main_mfcCtrl.h"
#include "src/main_valveCtrl.h"
#include "src/main_boschCom.h"
#include "src/main_StoreD.h"
#include "src/mfcCtrl.h"
#include "src/valveCtrl.h"

void setup() {
    // DEBUG
    Serial.begin(SERIAL_BAUDRATE);
  
    // ERSTELLE GEBRAUCHTE OBJEKTE
    communication::Main_LabCom *main_labCom = new communication::Main_LabCom();
    control::Main_MfcCtrl *main_mfcCtrl     = new control::Main_MfcCtrl();

    // TAUSCHE DATEN ZWISCHEN THREADS AUS
    main_labCom->setMainMfcObjectPointer(main_mfcCtrl);

    // STARTE PSEUDOTHREADS
    main_thread_list -> add_thread(main_labCom);
    main_thread_list -> add_thread(main_mfcCtrl);
}
