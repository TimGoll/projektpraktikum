#ifndef MAIN_STORED_H
#define MAIN_STORED_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <SD.h>

#include "main_valveCtrl.h" //Objekte zum Speichern der Objektpointer
#include "main_mfcCtrl.h"

#include "config.h"

namespace storage {
    class StoreD {
    public:
        //Defaultconstructor
        StoreD();
        //Destructor
        ~StoreD();
        //falls neue Messung gestartet wird und Dateien von voriger Messung auf SD, starte bei chronologisch logischer filenumber
        void detFilenumber(int filenumber, char filename[]);
        //Dateiname erstellen
        void setFilename();
        void openFile();
        void closeFile();
        void writeNewLine(char newLine[]);
    private:
        char filename[16];
        int filenumber;
        int decplaces;
        int decplaceshelp;
        File myFile;
        //Programm gestartet -> Messung gestartet -> Messung gestoppt -> Messung gestartet -> restart = true
        bool restart = false;

        control::Main_ValveCtrl *main_valveCtrl;
        control::Main_MfcCtrl *main_mfcCtrl;
        //communication::Main_StringBuilder *main_stringBuilder;
    };
}

#endif
