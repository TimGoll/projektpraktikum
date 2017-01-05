#ifndef MAIN_STORED_H
#define MAIN_STORED_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <SD.h>

namespace storage {
    class Main_StoreD : public Thread {
    public:
        //Defaultconstructor
        Main_StoreD();
        //Destructor
        ~Main_StoreD();
        //falls neue Messung gestartet wird und Dateien von voriger Messung auf SD, starte bei chronologisch logischer filenumber
        void detFilenumber(int filenumber, char filename);
        //bestimme Dezimalstellen von filenumber
        void detDecplaces(int decplaces, int decplaceshelp);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop(bool button,char filename, int filenumber, int decplaces, File myFile);
    private:
        bool button;
        //bool button_before;
        char filename;
        int filenumber;
        int decplaces;
        int decplaceshelp;
        File myFile;
    };
}

#endif
