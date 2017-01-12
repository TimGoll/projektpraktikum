#include "StoreD.h"

namespace storage {
    StoreD::StoreD() {

    }
    StoreD::~StoreD() {

    }
    void StoreD::detFilenumber(int filenumber, char filename){
      filenumber = 1;
      if (filenumber != 1) {
        while (SD.exists(filename)) {
          filenumber++;
        }
      }
    }

    void StoreD::setFilename(){
      sprintf(filename, "file%04d.txt\0",filenumber);
    }


    bool StoreD::start(){
        /*//Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;
        */

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
              sprintf(filename, "file%04d.txt\0",filenumber);
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

          /*//Serial.println("loop called");
          return true;*/
    }
}
