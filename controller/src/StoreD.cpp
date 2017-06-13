#include "StoreD.h"

namespace storage {
    //TODO: liefere Dateinamen in einem Verzeichnis (Programme)
    //TODO: lese Datei und liefere Inhalt

    StoreD::StoreD() {
        //WICHTIG: "BUILTIN_SDCARD", da dieser SD-Leser 4-bit parallel liesst
        this->sd_available = SD.begin(BUILTIN_SDCARD);
        if (this->sd_available) {
            srl->println('D', "SD Karte gefunden.");
        } else {
            srl->println('D', "SD Karte nicht gefunden.");
        }
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
    void StoreD::setAmountMFC(int amount_MFC){
        this->amount_MFC = amount_MFC;
    }
    void StoreD::setAmountValve(int amount_Valve){
        this->amount_Valve = amount_Valve;
    }

    void StoreD::setParseInputNewLineFunction(uint16_t (*parseInputNewLine) (char[])) {
        this->parseInputNewLine=parseInputNewLine;
    }

    void StoreD::setFilename(){
        sprintf(filename, "Messung_%s_#%03d.txt", dateString, filenumber); //-> "Messung_01.01.17_#1.txt"
    }

    void StoreD::detFilenumber(int filenumber, char filename[]){
        if (this->sd_available) {
            filenumber = 1;
            setFilename();
            while (SD.exists(filename)) {
                filenumber++;
                setFilename();
            }
        }
    }

    uint8_t StoreD::listsource(char list[][16]) {
        if (this->sd_available) {
            File dir = SD.open("/programs/");
            uint8_t counter = 0;
            while (true) {
                File entry = dir.openNextFile();
                if (!entry) { // keine Dateien mehr
                    break;
                }
                if (!entry.isDirectory()) {
                    srl->println('D', entry.name());
                    strcpy(list[counter], entry.name());
                }
                entry.close();
                counter++;
            }
            dir.close();
            return counter;
        }
        return 0; //keine SD Karte gefunden
    }

    void StoreD::readFile(char name[]) {
        if (this->sd_available) {
            char filename[32]="/programs/";
            strcat(filename, name);
            File file=SD.open(filename);
            while (file.available()>0){
                uint16_t counter= 0;
                char line[SERIAL_READ_MAX_LINE_SIZE];
                while (true){
                    char newchar= file.read();
                    if (newchar=='\n') {
                        line[counter]='\0';
                        break;
                    }
                    line[counter]=newchar;
                    counter++;
                }
                this->parseInputNewLine(line);
            }
            file.close();
        }
    }


    void StoreD::openFile(){
        if (this->sd_available) {
            if (restart==false) { //restart standardmäßig auf false
                detFilenumber(filenumber, filename);
                setFilename();
            }
            myFile = SD.open(filename, FILE_WRITE);


            //HEADER schreiben
            myFile.print("Messung am "); myFile.println(dateString);

            sprintf(buffer, "%d", intervall);
            myFile.print("Messintervall: "); myFile.println(buffer);

            sprintf(buffer, "%d", amount_MFC);
            myFile.print("Anzahl MFCs: "); myFile.println(buffer);

            sprintf(buffer, "%d", amount_Valve);
            myFile.print("Anzahl Ventile: "); myFile.println(buffer);


            for (int i = 0; i < amount_MFC; i++) {
                if (i < (amount_MFC-1)){
                    sprintf(buffer, "MFC%d: ", i);
                    myFile.print(buffer);
                    //myFile.print(???.getType_MFC); //MFC-Typen bisher nicht zugreifbar, da noch nicht existent (???)
                    myFile.print(", ");
                }
            }
            myFile.println(""); //leere Zeile Abstand
        }
    }

    void StoreD::closeFile(){
        if (this->sd_available) {
            myFile.close();
            filenumber++;
            setFilename();
            restart = true;
        }
    }

    void StoreD::writeNewLine(){
        if (this->sd_available) {
            //Sofern neue Zeile Dateigröße übersteigern würde, beginne neue Datei
            if ((MAX_SD_FILE_SIZE - myFile.size() > SERIAL_READ_MAX_LINE_SIZE)) {
                myFile.println(newLine);
            } else {
                myFile.close();
                openFile(); //neue Datei wird geöffnet, dabei wird Dateiname bestimmt und der Header der Datei geschrieben; danach werden wieder die Messwerte abgespeichert
                myFile.println(newLine);
            }
        }
    }
}
