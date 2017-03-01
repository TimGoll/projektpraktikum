#ifndef MAIN_STORED_H
#define MAIN_STORED_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <SD.h>


#include "main_valveCtrl.h" //Objekte zum Speichern der Objektpointer
#include "main_mfcCtrl.h"
//#include "main_stringBuilder.h"

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
        void writeNewLine();
        void setNewLine(char newLine[]);
        void setDate(char dateString[]);
        void setIntervall(int intervall);
    private:
        char newLine[2000]; //MAX_LINE_SIZE?? SERIAL_READ_MAX_LINE_SIZE??
        char dateString[16];
        int intervall;
        char filename[50];
        int filenumber;
        int decplaces;
        int decplaceshelp;
        char buffer[16]; //Buffer für Werte vom Messintervall, MFC-Anzahl, Ventil-Anzahl usw.
        File myFile;
        //Programm gestartet -> Messung gestartet -> Messung gestoppt -> Messung gestartet -> restart = true
        bool restart = false;

        control::Main_ValveCtrl *main_valveCtrl;
        control::Main_MfcCtrl *main_mfcCtrl;
        //communication::Main_StringBuilder *main_stringBuilder;
    };
}

#endif
