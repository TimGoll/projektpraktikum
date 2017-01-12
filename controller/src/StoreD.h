#ifndef MAIN_STORED_H
#define MAIN_STORED_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <SD.h>

#include "config.h"

namespace storage {
    class StoreD {
    public:
        //Defaultconstructor
        StoreD();
        //Destructor
        ~StoreD();
        //falls neue Messung gestartet wird und Dateien von voriger Messung auf SD, starte bei chronologisch logischer filenumber
        void detFilenumber();
        //Dateiname erstellen
        void setFilename();
        void start(unsigned long startTime);
    private:
        bool button;
        //bool button_before;
        char filename[16];
        int filenumber;
        int decplaces;
        int decplaceshelp;
        File myFile;
    };
}

#endif
