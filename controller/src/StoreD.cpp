#include "StoreD.h"

namespace storage {
    StoreD::StoreD() {

    }

    StoreD::~StoreD() {

    }

    void StoreD::setFilename(){
      //??? sprintf(filename, "Messung_%s_#%04d.txt\0", this->dateString, filenumber); //-> "Messung_01.01.17_#1.txt"
    }

    void StoreD::detFilenumber(int filenumber, char filename[]){
      filenumber = 1;
      setFilename();
      while (SD.exists(filename)) {
        filenumber++;
        setFilename();
      }
    }

    void StoreD::openFile(){
        //TODO: auf Display anzeigen, dass gespeichert wird
        SD.begin();
        if (restart==false) { //restart standardmäßig auf false
          detFilenumber(filenumber, filename);
          setFilename();
        }
        myFile = SD.open(filename, FILE_WRITE);

        /*
        //HEADER schreiben
          myFile.print("Messung am "); myFile.println(this->dateString);

          //hier "besser/lieber" überall this->main_something->intervall/getAmount_MFC()/getAmount_valve() ???
          communication::Main_StringBuilder MSB;
          sprintf(buffer, "%d", MSB.intervall);
          myFile.print("Messintervall: "); myFile.println(buffer);

          control::Main_MfcCtrl MMC;
          sprintf(buffer, "%d", MMC.getAmount_MFC());
          myFile.print("Anzahl MFCs: "); myFile.println(buffer);

          control::Main_ValveCtrl MVC;
          sprintf(buffer, "%d", MVC.getAmount_valve()));
          myFile.print("Anzahl Ventile: "); myFile.println(buffer);

          //MFC-Typen bisher nicht zugreifbar, da noch nicht existent (???)
          /*
          for (int i = 0; i < MMC.getAmount_MFC(); i++) {
              myFile.print("MFC%d: ", i); myFile.print(???.getType_MFC);
              if i < (MMC.getAmount_MFC()-1){
                myFile.print(" ,");
              }
          }
          */
          myFile.println(""); //leere Zeile Abstand
    }

    void StoreD::closeFile(){
          myFile.close();
          filenumber++;
          setFilename();
          restart = true;
    }

    void StoreD::writeNewLine(char newLine[]){
      /*
        communication::Main_StringBuilder MSB; //zweites MSB deklarieren ???
        //Sofern neue Zeile Dateigröße übersteigern würde, beginne neue Datei
        if ((MAX_SD_FILE_SIZE - myFile.size() > SERIAL_READ_MAX_LINE_SIZE)) {
          myFile.println(MSB.newLine[]);
        }
        else {
          myFile.close();
          openFile(); //neue Datei wird geöffnet, dabei wird Dateiname bestimmt und der Header der Datei geschrieben; danach werden wieder die Messwerte abgespeichert
          myFile.println(MSB.newLine[]);
        }
      */
    }
}
