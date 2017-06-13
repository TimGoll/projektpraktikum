#include "main_labCom.h"

namespace communication {
    Main_LabCom::Main_LabCom() {
        this->reading = true;
        this->sending = false;
        this->finished = false;

        this->amount_valve = 0;
        this->amount_MFC = 0;

        this->headerLineCounter = 0;

        srl->println('D', "LabCom erstellt.");
        srl->println('L', "ready"); //Sende Startbefehl an LabView
    }
    Main_LabCom::~Main_LabCom() {

    }

    void Main_LabCom::setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl) {
        this->main_mfcCtrl = main_mfcCtrl;
    }

    void Main_LabCom::setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl) {
        this->main_valveCtrl = main_valveCtrl;
    }

    void Main_LabCom::setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom) {
        this->main_boschCom = main_boschCom;
    }


    void Main_LabCom::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    void Main_LabCom::setMainStringBuilderObjectPointer(communication::Main_StringBuilder *main_stringBuilder) {
        this->main_stringBuilder = main_stringBuilder;
    }

    void Main_LabCom::setParseInputObjectPointer(communication::ParseInput *parseInput){
      this->parseInput=parseInput;
    }

    int16_t Main_LabCom::readLine() {
        srl->println('L', "Lese neue Zeile............");

        this->bufferCharIndex = 0; //setze index auf Startposition zurueck
        uint32_t startTime = millis();

        //Lese so lange, bis Zeile vollstaendig empfangen wurde, oder Timeout erreicht wurde (Main-Thread wird blockiert)
        while(millis() < startTime + SERIAL_READ_TIMEOUT) {
            if (srl->available('L') > 0) { //Lese Daten, wenn vorhanden
                this->inDataBuffer[this->bufferCharIndex] = srl->read('L'); //srl->read() gibt einen einzelnen Char zurueck

                //UEBERPRUEFUNG
                if (this->bufferCharIndex == 0) {
                    if (this->inDataBuffer[this->bufferCharIndex] != '<') { //erstes Zeichen muss oeffnender Tag sein
                        if (this->inDataBuffer[this->bufferCharIndex] == '\n') //Sortiere Strings ohne Inhalt aus
                            return -1;

                        srl->println('D', "ERROR - Falscher Zeilenbeginn");
                        //lese String dennoch bis Zum Ende um mehrfache "Falscher Beginn" Meldung zu verhindern
                        char searchForEnd = ' ';
                        while (searchForEnd != '\n') {
                            searchForEnd = srl->read('L');
                        }
                        return ERR_SERIAL_READ_WRONG_LINE_BEGIN;
                    }
                }
                if (this->inDataBuffer[this->bufferCharIndex] == '\n') { //Zeile zuende
                    if (this->inDataBuffer[this->bufferCharIndex -1] != '>') { //vorheriges Zeichen muss schliessender Tag sein
                        srl->println('D', "ERROR - Falsches Zeilenende");
                        return ERR_SERIAL_READ_WRONG_LINE_ENDING;
                    } else { //vollstaendiger String abgeschlossen
                        //entferne Anfangs- und Endzeichen
                        uint16_t i = 0;
                        while(this->inDataBuffer[i+1] != '>') {
                            this->inDataBuffer[i] = this->inDataBuffer[i+1];
                            i++;
                        }
                        this->inDataBuffer[i] = '\0';

                        this->inDataBuffer[this->bufferCharIndex +1] = '\0';
                        srl->println('D', "");
                        srl->print('D', "Eingabestring akzeptiert: ");
                        srl->print('D', this->inDataBuffer);
                        return 1;
                    }
                }

                this->bufferCharIndex++;
                if (this->bufferCharIndex == SERIAL_READ_MAX_LINE_SIZE) {
                    return ERR_SERIAL_READ_MAX_STRING_SIZE;
                    srl->println('D', "ERROR - Eingabestring zu lang");
                }
            }
        }
        srl->print('D', "ERROR - Timeout: ");
        srl->println('D', this->inDataBuffer);
        return ERR_SERIAL_READ_TIMEOUT; //Timeout
    }

/*
    uint16_t Main_LabCom::splitLine() {
        uint16_t currentCharIndex = 0;
        uint16_t endCharIndex = this->bufferCharIndex -1; //da letztes Zeichen '>' ist

        //Schleife, die durch die Ziel-Array-Indexe navigiert
        for (uint16_t arrayIndex = 0; arrayIndex < SERIAL_READ_MAX_BLOCK_AMOUNT; arrayIndex++) {
            //Schleife, die durch die Char-Positionen eines Ziel-Array-Eintrags navigiert
            for (uint16_t arrayCharIndex = 0; arrayCharIndex < SERIAL_READ_MAX_BLOCK_SIZE; arrayCharIndex++) {
                currentCharIndex++; //Wird anfangs um 1 erhoet, Zeichen 1 ist zu ignorieren ('<')

                if (this->inDataBuffer[currentCharIndex] == ',') { //Eintrag zu ende
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    cmn::trim(this->inDataArray[arrayIndex]);
                    break;
                }

                if (currentCharIndex == endCharIndex) { //Zeilenende erreicht
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    cmn::trim(this->inDataArray[arrayIndex]);
                    return arrayIndex +1; //Index und Groesse sind immer um 1 verschieden
                }

                this->inDataArray[arrayIndex][arrayCharIndex] = this->inDataBuffer[currentCharIndex];
            }
        }
        return 0;
    }*/

    void Main_LabCom::start() {
        if (this->parseInput->get_headerLineCounter() == 9) { //erwarte 'start'
            //Aendere Seriellen Modus
            this->reading = false;
            this->sending = true;

            //Fuege 1000ms zum Start hinzu, um Verzoegerungen durch die Startanzeige zu verhindern
            uint32_t startTime = millis() + 1000;

            srl->println('D', startTime);

            //starte MFCs
            this->main_mfcCtrl->start(startTime);

            //starte Ventile
            this->main_valveCtrl->start(startTime);

            //starte Boschsensor
            this->main_boschCom->start(startTime);

            //starte Display
            this->main_display->start(startTime);

            //starte Stringbuilder (und damit SD)
            this->main_stringBuilder->start(startTime);

            srl->print('D', "[Zeit: ");
            srl->print('D', startTime);
            srl->println('D', "] Messung gestartet.");

            //verhindere weitere Start-Eingaben:
            this->parseInput->set_headerLineCounter(10);
        }
    }

    //////////////////// MAINLOOP ////////////////////

    bool Main_LabCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        // Im Lesemodus wird, wenn ein String verfügbar ist (srl->available('L') ), der String
        // vollstaendig eingelesen und, sofern kein Fehler aufgetreten ist, anschließend an
        // den Input Parser weiter gegeben.
        if (this->reading) { //Empfange Messprogramm
            if (srl->available('L') > 0) {
                int16_t errCode = this->readLine();
                if (errCode == 1) {
                    //Uebergebe gelesene Zeile, verarbeite zurueck gegebenen ErrorCode
                    uint16_t parserErrCode = this->parseInput->parseNewLine(this->inDataBuffer);

                    if (parserErrCode >= 1000)
                        this->main_display->throwError(parserErrCode);
                    else if (parserErrCode == 1)
                        this->start();

                } else if (errCode > 1) {
                    //ErrorCode wird auf Display angezeigt
                    this->main_display->throwError(errCode);
                    srl->println('L', errCode); //Sende Errorcode an LabView
                } //else -1: Keine Eingabe vorhanden (leerer String)
            }
        }

        if (this->sending) { //Sende Messwerte parallel zur Messung
            //ueberpruefe kontinuierlich, ob Eventliste noch Inhalt hat
            if (this->main_mfcCtrl->getQueueFinished() && this->main_valveCtrl->getQueueFinished() && !this->finished) {
                this->finished = true;
                this->main_display->bothQueuesFinished();
                this->main_stringBuilder->bothQueuesFinished();
            }
        }

        return true;
    }
}
