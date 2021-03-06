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
        //Dateiname erstellen
        void setFileName();
        void openFile();
        void closeFile();
        void writeNewLine(char newLine[]);
        void setDate(char dateString[]);
        void setTypes(char types[][16]);
        void setIntervall(int intervall);
        void setAmountMFC(int amount_MFC);
        void setAmountValve(int amount_valve);
        void setParseInputNewLineFunction(uint16_t (*parseInputNewLine) (char[]));
        void setLoadingProgressFunction(void (*setLaodingProgress) (int8_t));
        void readFile(char name[]);
        bool foundSDcard();
        uint8_t listsource(char list[][16]);
    private:
        void createExample();

        //speichere Funktion von parseInput, um Kreisimport zu verhindern.
        uint16_t (*parseInputNewLine) (char[]);
        //speichere Funktion von mainDisplay, um Ladefortschritt anzuzeigen
        void (*setLaodingProgress) (int8_t);

        char newLine[SERIAL_READ_MAX_LINE_SIZE];
        char mfcTypeList[MAX_AMOUNT_MFC][16];
        char dateString[32];
        char fileName[16];
        int intervall;
        uint8_t amount_MFC;
        uint8_t amount_valve;
        bool sd_available;

        //Speichere Datei Objekt, damit es ueber alle Funktionen hinweg funktioniert
        File fileStream;
    };
}

#endif
