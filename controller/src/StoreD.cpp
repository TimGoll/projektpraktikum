#include "StoreD.h"

namespace storage {
    StoreD::StoreD() {
        //WICHTIG: "BUILTIN_SDCARD", da dieser SD-Leser 4-bit parallel liesst
        this->sd_available = SD.begin(BUILTIN_SDCARD);
        if (this->sd_available) {
            srl->println('D', "SD Karte gefunden.");
        } else {
            srl->println('D', "SD Karte nicht gefunden.");
        }

        //erstelle Ordner falls nicht vorhanden, ist noetig fuer korrektes Funktionieren des Programms
        if (this->sd_available) {
            SD.mkdir("/PROGRAMS/");
            SD.mkdir("/RESULTS/");
        }

        this->createExample();
    }

    StoreD::~StoreD() {

    }

    void StoreD::setDate(char dateString[]){
        strcpy(this->dateString, dateString);

        this->setFileName();
    }
    void StoreD::setTypes(char types[][16]) {
        for (uint8_t i = 0; i < MAX_AMOUNT_MFC; i++)
            strcpy(this->mfcTypeList[i], types[i]); //kopiere gesamte Liste, da wir Anzahl nicht kennen
    }
    void StoreD::setIntervall(int intervall){
        this->intervall = intervall;
    }
    void StoreD::setAmountMFC(int amount_MFC){
        this->amount_MFC = amount_MFC;
    }
    void StoreD::setAmountValve(int amount_valve){
        this->amount_valve = amount_valve;
    }

    void StoreD::setParseInputNewLineFunction(uint16_t (*parseInputNewLine) (char[])) {
        this->parseInputNewLine=parseInputNewLine;
    }

    void StoreD::setFileName() {
        //setze sechstelliges Datum
        this->fileName[0] = this->dateString[8];
        this->fileName[1] = this->dateString[9];
        this->fileName[2] = this->dateString[3];
        this->fileName[3] = this->dateString[4];
        this->fileName[4] = this->dateString[0];
        this->fileName[5] = this->dateString[1];
        this->fileName[6] = '\0';

        //Erzeuge Dateinummer
        char tmp_path[32];
        char num_buf[4];
        uint8_t number = 0;
        while(true) {
            sprintf(num_buf, "%02d", number);
            strcpy(tmp_path, "/RESULTS/");
            strcat(tmp_path, this->fileName);
            strcat(tmp_path, num_buf);
            strcat(tmp_path, ".TXT");
            if (SD.exists(tmp_path)) {
                number++;
            } else {
                strcat(this->fileName, num_buf);
                strcat(this->fileName, ".TXT");
                break;
            }
        }
    }

    uint8_t StoreD::listsource(char list[][16]) {
        if (this->sd_available) {
            File dir = SD.open("/PROGRAMS/");
            uint8_t counter = 0;
            while (true) {
                File entry = dir.openNextFile();
                if (!entry) { // keine Dateien mehr
                    break;
                }
                if (!entry.isDirectory()) {
                    strcpy(list[counter], entry.name());
                    if (list[counter][0] != '_' && list[counter][0] != '.') //ignoriere Systemdateien
                        counter++;
                }
                entry.close();
            }
            dir.close();
            return counter;
        }
        return 0; //keine SD Karte gefunden
    }

    void StoreD::readFile(char name[]) {
        if (this->sd_available) {
            char filepath[32] = "/PROGRAMS/";
            strcat(filepath, name);
            File file = SD.open(filepath, FILE_READ);
            while (file.available() > 0){
                uint16_t counter = 0;
                char line[SERIAL_READ_MAX_LINE_SIZE];
                while (true){
                    char newchar = file.read();
                    if (newchar == '\n') { //zeile normal zuende
                        line[counter] = '\0';
                        break;
                    } else if (newchar == '#') { //Kommenarteile bis zum Ende ignorieren
                        while(true) { //lese Zeile bis zum Ende aus Stream
                            newchar = file.read();
                            if (newchar == '\n')
                                break;
                        }
                        line[counter] = '\0';
                        break;
                    }
                    line[counter] = newchar;
                    counter++;
                }
                //sortiere leere Zeilen und Kommentare aus
                cmn::trim(line);
                //line[0] != 13: 13 = vertical Tab, leere Zeile
                if (line[0] != '\0' && line[0] != '\n' && line[0] != 13)
                    this->parseInputNewLine(line);
            }
            file.close();
        }
    }


    void StoreD::openFile(){
        if (this->sd_available) {
            char filepath[32] = "/RESULTS/";
            strcat(filepath, this->fileName);
            this->fileStream = SD.open(filepath, FILE_WRITE);

            if (this->fileStream) {
                srl->println('D', " - Datei erstellt");
            } else {
                srl->println('D', " - Fehler beim Datei erstellen!");
                return;
            }

            //HEADER schreiben
            this->fileStream.print("Messung am:     ");   this->fileStream.println(this->dateString);
            this->fileStream.print("Messintervall:  ");   this->fileStream.println(this->intervall);
            this->fileStream.print("Anzahl Ventile: ");   this->fileStream.println(this->amount_valve);
            this->fileStream.print("Anzahl MFCs:    ");   this->fileStream.println(this->amount_MFC);
            this->fileStream.print("Typen der MFCs: ");
            for (uint8_t i = 0; i < this->amount_MFC; i++) {
                this->fileStream.print(this->mfcTypeList[i]);
                this->fileStream.print(" ");
            }
            this->fileStream.println("");this->fileStream.println("");

            this->fileStream.print("Wertsortierung: ");   this->fileStream.println("Laufzeit (ms), MFC Soll, MFC Ist, Ventil Soll, Bosch (Temp /C, Druck /Pa, Feucht /%)");
            this->fileStream.println("");
        }
    }

    void StoreD::closeFile(){
        if (this->sd_available && this->fileStream) {
            this->fileStream.close();
        }
    }

    void StoreD::writeNewLine(char newLine[]){
        if (this->sd_available && this->fileStream) {
            this->fileStream.println(newLine);
        }
    }




    void StoreD::createExample() {
        if (this->sd_available) {
            char filepath[32] = "/PROGRAMS/_EXAMPLE.TXT";
            SD.remove(filepath);
            File file = SD.open(filepath, FILE_WRITE);

            file.println("# DIES IST EINE BEISPIELDATEI.");
            file.println("# Dateinmane duerfen nicht mit '.' oder '_' beginnen, sonst werden sie ignoriert.");
            file.println("# Im Gegensatz zu einer LabView Uebertragung sind hier Kommentare moeglich.");
            file.println("# leere Zeilen und Leerzeichen werden ignoriert.");
            file.println("");
            file.println("4,16 #(MFCs, Ventile) in Dateien werden keine Tags benutzt");
            file.println("01,21,07,14 #(MFC Adressen)");
            file.println("Buerkert,Buerkert,MKS,MKS #(MFC Typen)");
            file.println("0x20 #(Ventilplatinen Adressen)");
            file.println("0 0, 0 1, 0 2, 0 3, 0 4, 0 5, 0 6, 0 7, 0 8, 0 9, 0 10, 0 11, 0 12, 0 13, 0 14, 0 15 #(Ventil Pins)");
            file.println("250 #(Messintervall)");
            file.println("08.02.2017-13:03:29 #(Datumstring)");
            file.println("");
            file.println("begin #Ende Header, uebertrage Eventliste");
            file.println("");
            file.println("M,0,120,1000");
            file.println("V,0,1,1000");
            file.println("V,1,1,750");
            file.println("");
            file.println("end #Uebertragung abgeschlossen");
            file.println("start #Optionaler automatischer Programmstart");

            file.close();
        }
    }
}
