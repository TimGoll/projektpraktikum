// Dies ist die Hauptdatei unseres Prpgramms. Der Name muss identisch zum Programmverzeichnis sein.
// Hier werden alle Bibliotheken importiert, Threads gestartet und verwaltet

// INCLUDES
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <QueueList.h> //vielleicht hier nicht gebraucht

#include "src/boschCom.h"
#include "src/labCom.h"
#include "src/mfcCtrl.h"
#include "src/valveCtrl.h"
#include "src/StoreD.h"

void setup() {
    // ERSTELLE GEBRAUCHTE OBJEKTE
    communication::LabCom *labCom = new communication::LabCom();
    control::MfcCtrl *mfcCtrl     = new control::MfcCtrl(1);

    // STARTE PSEUDOTHREADS
    main_thread_list -> add_thread(labCom);
    main_thread_list -> add_thread(mfcCtrl);

    // DEBUG
    Serial.begin(9600);
}
