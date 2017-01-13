#include "StoreD.h"

namespace storage {
    StoreD::StoreD() {

    }

    StoreD::~StoreD() {

    }

    /*
    void Main_LabCom::setMainStringBuilderObjectPointer(communication::Main_StringBuilder *main_stringBuilder) {
        this->main_stringBuilder = main_stringBuilder;
    }
    */

    void StoreD::setFilename(){
      sprintf(filename, "file%04d.txt\0",filenumber);
    }

    void StoreD::detFilenumber(int filenumber, char filename[]){
      filenumber = 1;
      setFilename();
      while (SD.exists(filename)) {
        filenumber++;
      }
    }

    void StoreD::start(unsigned long startTime){
        //TODO: auf Display anzeigen, dass gespeichert wird
        SD.begin();
        if (restart==false) {
          detFilenumber(filenumber, filename);
          setFilename();
        }
        myFile = SD.open(filename, FILE_WRITE);
    }

    void StoreD::finish(unsigned long startTime){
          myFile.close();
          filenumber++;
          setFilename();
          restart = true;
    }

    void StoreD::setNewLine(char newLine[]){
        //Sofern neue Zeile Dateigröße übersteigern würde, beginne neue Datei
        if ((MAX_SD_FILE_SIZE - myFile.size() > SERIAL_READ_MAX_LINE_SIZE)) {
          myFile.println(/*hier Daten, communication::Main_StringBuilder *newLine[] ?? */) ;
        }
        else {
          myFile.close();
          filenumber++;
          setFilename();
          myFile = SD.open(filename, FILE_WRITE);
          myFile.println(/*hier Daten, communication::Main_StringBuilder *newLine[] ?? */);
        }
    }
}
