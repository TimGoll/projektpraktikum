#ifndef MAIN_MFCCTRL_H
#define MAIN_MFCCTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "config.h"
#include "mfcCtrl.h"
#include "main_display.h"

namespace control {
    // an die MFCs werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class Main_MfcCtrl : public Thread {
    public:
        //Defaultconstructor
        Main_MfcCtrl();
        //Destructor
        ~Main_MfcCtrl();
        //Funktion, die von LabCom aufgerufen wird. Sie erstellt MFC-Objekte
        void createMFC(int amount);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Adressen.
        //Adressen werden weiter an alle MFC-Objekte gegeben
        void setAdresses(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Typen
        //Typen werden weiter an alle MFC-Objekte gegeben.
        void setTypes(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Wird von LabCom aufgerufen und enthält Eventdaten. Wichtig ist hier, dass
        //dieser Aufruf immer nur fuer EIN MFC ist, daher ist die ID von Noeten
        void setEvent(int mfcID, int value, unsigned long time);
        //setzt die 'ready'-Variable der MFCs auf true. Außerdem wird der Nullpunkt der Steuerung gesetzt
        void start(unsigned long startTime);
        //Gebe Adresse des Displayobjektes an die einzelnen MFCs, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Diese Funktion wird vom StringBuilder aufgerufen und sammelt die Daten der MFCs
        //um sie in den Ausgabestring zu schreiben
        //Die Funktion fragt die einzelnen MFCs nach ihren Werten ab
        void getMfcValueList(int mfcValueList[]);
        int getAmount_MFC();
        //wird von Main_LabCom kontinuierlich abgefragt, ob die queue abgeschlossen ist.
        bool getQueueFinished();
        //schreibt Typ eines MFCs (ID dessen angeben) in gegebenes Array; bleibt leer,
        //wenn ID nicht existent ist
        void getMfcType(int id, char type[]);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int amount_MFC;
        control::MfcCtrl *mfc_list[MAX_AMOUNT_MFC]; //Hier werden die Adressen der MFC-Objekte gespeichert
        bool mfc_continue_next_loop[MAX_AMOUNT_MFC];
        int amount_of_finished_mfcs;
        bool queueFinished;

        io::Main_Display *main_display;
    };
}

#endif
