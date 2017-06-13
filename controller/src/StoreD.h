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
        void writeNewLine();
        void setNewLine(char newLine[]);
        void setDate(char dateString[]);
        void setIntervall(int intervall);
        void setAmountMFC(int amount_MFC);
        void setAmountValve(int amount_Valve);
        void setParseInputNewLineFunction(uint16_t (*parseInputNewLine) (char[]));
        void readFile(char name[]);
        uint8_t listsource(char list[][16]);
    private:
        //speichere Funktion von parseInput, um Kreisimport zu verhindern.
        uint16_t (*parseInputNewLine) (char[]);

        char newLine[2000]; //MAX_LINE_SIZE?? SERIAL_READ_MAX_LINE_SIZE??
        char dateString[16];
        int intervall;
        char filename[50];
        int filenumber;
        int decplaces;
        int decplaceshelp;
        uint8_t amount_MFC;
        uint8_t amount_Valve;
        char buffer[16]; //Buffer für Werte vom Messintervall, MFC-Anzahl, Ventil-Anzahl usw.
        File myFile;
        //Programm gestartet -> Messung gestartet -> Messung gestoppt -> Messung gestartet -> restart = true
        bool restart = false;
        bool sd_available;
    };
}

#endif
