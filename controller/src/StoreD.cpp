#include "StoreD.h"

namespace storage {
    Main_StoreD::Main_StoreD() {

    }
    Main_StoreD::~Main_StoreD() {

    }
    void Main_StoreD::detFilenumber(int filenumber, char filename){
      filenumber = 1;
      if (filenumber != 1) {
        while (SD.exists(filename)) {
          filenumber++;
        }
      }
    }

    void Main_StoreD::detDecplaces(int decplaces, int decplaceshelp){
      decplaces = 1;
      decplaceshelp = filenumber;
      while (decplaceshelp > 10) {
        decplaceshelp = decplaceshelp % 10;
        decplaces++;
      }
      filename[9 + decplaces] = "file";// + filenumber + ".txt";
    }

/*    bool Main_StoreD::loop(bool button,char filename, int filenumber, int decplaces, File myFile){
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        SD.begin();

        if (button) { //TODO: setzen von button durch Tastendruck
          //TODO: auf Display anzeigen, dass gespeichert wird
          if (!SD.exists(filename)) {
            myFile = SD.open(filename, FILE_WRITE);
          }
          if ((MAX_SD_FILE_SIZE - myFile.size() > SERIAL_READ_MAX_LINE_SIZE)) {
            myFile.println(hier Daten) ;
          }
          else {
            myFile.close();
            filenumber++;
            filename[] = "file";// + filenumber + ".txt";
            myFile = SD.open(filename, FILE_WRITE);
            myFile.println(hier Daten);
          }
        }
        else {
          if (SD.exists(filename)) { //wenn gespeichert wurde und jetzt nicht mehr, soll die geöffnete Datei geschlossen werden
            myFile.close();
            filenumber++; //auch wenn MAX_SD_FILE_SIZE noch nicht erreicht, beginne neue Datei beim erneuten Starten
          }
          //else: keine laufende Messung && keine geöffnete Datei -> mache nichts
        }

        //Serial.println("loop called");
        return true;
    }*/
}
