#ifndef MAIN_MFCCTRL_H
#define MAIN_MFCCTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "config.h"
#include "mfcCtrl.h"
#include "main_display.h"

#include "mfcCom_buerkert.h"
#include "mfcCom_mks.h"

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
        void createMFC(uint16_t amount);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Adressen.
        //Adressen werden weiter an alle MFC-Objekte gegeben
        void setAddresses(char addresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Typen
        //Typen werden weiter an alle MFC-Objekte gegeben.
        void setTypes(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Wird von LabCom aufgerufen und enthält Eventdaten. Wichtig ist hier, dass
        //dieser Aufruf immer nur fuer EIN MFC ist, daher ist die ID von Noeten
        void setEvent(uint16_t mfcID, uint16_t value, uint32_t time);
        //setzt die 'ready'-Variable der MFCs auf true. Außerdem wird der Nullpunkt der Steuerung gesetzt
        void start(uint32_t startTime);
        //Gebe Adresse des Displayobjektes an die einzelnen MFCs, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Diese Funktion wird vom StringBuilder aufgerufen und sammelt die Daten der MFCs
        //um sie in den Ausgabestring zu schreiben
        //Die Funktion fragt die einzelnen MFCs nach ihren Werten ab
        void getMfcValueList(uint16_t mfcValueList[]);
        //gibt die Anzahl an MFCs zurueck
        uint16_t getAmount_MFC();
        //wird von Main_LabCom kontinuierlich abgefragt, ob die queue abgeschlossen ist.
        bool getQueueFinished();
        //schreibt Typ eines MFCs (ID dessen angeben) in gegebenes Array; bleibt leer,
        //wenn ID nicht existent ist
        void getMfcType(uint16_t id, char type[]);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int16_t amount_MFC;
        bool mfc_continue_next_loop[MAX_AMOUNT_MFC];
        int16_t amount_of_finished_mfcs;
        bool queueFinished;

        control::MfcCtrl *mfc_list[MAX_AMOUNT_MFC]; //Hier werden die Adressen der MFC-Objekte gespeichert

        io::Main_Display *main_display;
    };
}

#endif
