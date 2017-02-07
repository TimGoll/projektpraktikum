#include "StoreD.h"

namespace storage {
    StoreD::StoreD() {

    }

    StoreD::~StoreD() {

    }

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

    void StoreD::openFile(){
        //TODO: auf Display anzeigen, dass gespeichert wird
        SD.begin();
        if (restart==false) {
          detFilenumber(filenumber, filename);
          setFilename();
        }
        myFile = SD.open(filename, FILE_WRITE);
        /*
        myFile.print("Messung am "); myFile.println(/*getDate);
        myFile.print("Messintervall: "); myFile.println(/*getIntervall);
        control::Main_MfcCtrl MMC;
        myFile.print("Anzahl MFCs: "); myFile.println(/*inttochar(MMC.getAmount_MFC));
        control::Main_ValveCtrl MVC;
        myFile.print("Anzahl Ventile: "); myFile.println(/*inttochar(MVC.getAmount_valve));
        for (int i = 0; i < MMC.getAmount_MFC(); i++) {
            myFile.print("MFC%d: ", i); myFile.print(/*getType_MFC);
            if i < (MMC.getAmount_MFC()-1){
              myFile.print(" ,");
            }
        }
        myFile.println("");
        */
    }

    void StoreD::closeFile(){
          myFile.close();
          filenumber++;
          setFilename();
          restart = true;
    }

    void StoreD::writeNewLine(char newLine[]){
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
