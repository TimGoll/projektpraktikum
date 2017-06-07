#include "StoreD.h"

namespace storage {
    //TODO: liefere Dateinamen in einem Verzeichnis (Programme)
    //TODO: lese Datei und liefere Inhalt

    StoreD::StoreD() {
      SD.begin();
    }

    StoreD::~StoreD() {

    }

    void StoreD::setNewLine(char newLine[]){
      strcpy(newLine, this->newLine);
    }
    void StoreD::setDate(char dateString[]){
       strcpy(dateString, this->dateString);
    }
    void StoreD::setIntervall(int intervall){
      this->intervall = intervall;
    }
    void StoreD::setAmountMFC(int amountMFC){
      this->amountMFC = amountMFC;
    }
    void StoreD::setAmountValve(int amountValve){
      this->amountValve = amountValve;
    }

    void StoreD::setParseInputObjectPointer(communication::ParseInput *parseInput){
      this->parseInput=parseInput;
    }

    void StoreD::setFilename(){
      sprintf(filename, "Messung_%s_#%03d.txt", dateString, filenumber); //-> "Messung_01.01.17_#1.txt"
    }

    void StoreD::detFilenumber(int filenumber, char filename[]){
      filenumber = 1;
      setFilename();
      while (SD.exists(filename)) {
        filenumber++;
        setFilename();
    }
    }

    uint8_t listsource(char list[][16]) {
    dir = SD.open("/programs/");
    uint8_t counter=0;
    while (true) {
        File entry =  dir.openNextFile();
        if (! entry) {
          // keine Dateien mehr
          break;
        }
        if (!entry.isDirectory()) {
          list[counter] = entry.name();
        }
        entry.close();
        counter++;
      }
    dir.close();
    return = counter;
    }

    void StoreD::readFile(char name[]) {
      char filename[32]="/programs/";
      strcat(filename, name);
      File file=SD.open(filename);
      while (file.available()>0){
        uint16_t counter= 0;
        char line[SERIAL_READ_MAX_LINE_SIZE];
        while (true){
          char newchar= file.read();
          if (newchar='\n') {
            line[counter]='\0';
            break;
          }
          line[counter]=newchar;
          counter++;
        }
        this->parseInput->parseNewLine(line);
      }
      file.close();
    }


    void StoreD::openFile(){
        //TODO: auf Display anzeigen, dass gespeichert wird
        if (restart==false) { //restart standardmäßig auf false
          detFilenumber(filenumber, filename);
          setFilename();
        }
        myFile = SD.open(filename, FILE_WRITE);


        //HEADER schreiben
          myFile.print("Messung am "); myFile.println(dateString);

          sprintf(buffer, "%d", intervall);
          myFile.print("Messintervall: "); myFile.println(buffer);

          sprintf(buffer, "%d", amountMFC);
          myFile.print("Anzahl MFCs: "); myFile.println(buffer);

          sprintf(buffer, "%d", amountValve);
          myFile.print("Anzahl Ventile: "); myFile.println(buffer);


          for (int i = 0; i < amountMFC; i++) {
              if (i < (amountMFC-1)){
                sprintf(buffer, "MFC%d: ", i);
                myFile.print(buffer);
                //myFile.print(???.getType_MFC); //MFC-Typen bisher nicht zugreifbar, da noch nicht existent (???)
                myFile.print(", ");
              }
          }
          myFile.println(""); //leere Zeile Abstand
    }

    void StoreD::closeFile(){
          myFile.close();
          filenumber++;
          setFilename();
          restart = true;
    }

    void StoreD::writeNewLine(){

        //Sofern neue Zeile Dateigröße übersteigern würde, beginne neue Datei
        if ((MAX_SD_FILE_SIZE - myFile.size() > SERIAL_READ_MAX_LINE_SIZE)) {
          myFile.println(newLine);
        }
        else {
          myFile.close();
          openFile(); //neue Datei wird geöffnet, dabei wird Dateiname bestimmt und der Header der Datei geschrieben; danach werden wieder die Messwerte abgespeichert
          myFile.println(newLine);
      }
  }
}
