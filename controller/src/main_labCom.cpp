#include "main_labCom.h"

namespace communication {
    Main_LabCom::Main_LabCom() {
        this->reading = true;
        this->sending = false;
        this->finished = false;

        this->mfc_finished = false;
        this->valve_finished = false;

        this->amount_valve = 0;
        this->amount_MFC = 0;

        this->headerLineCounter = 0;

        srl->println('D', "LabCom erstellt.");
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

    uint16_t Main_LabCom::readLine() {
        this->bufferCharIndex = 0; //setze index auf Startposition zurueck
        uint32_t startTime = millis();

        //Lese so lange, bis Zeile vollstaendig empfangen wurde, oder Timeout erreicht wurde (Main-Thread wird blockiert)
        while(millis() < startTime + SERIAL_READ_TIMEOUT) {
            if (srl->available('L') > 0) { //Lese Daten, wenn vorhanden
                this->inDataBuffer[this->bufferCharIndex] = srl->read('L'); //srl->read() gibt einen einzelnen Char zurueck

                //UEBERPRUEFUNG
                if (this->bufferCharIndex == 0) {
                    if (this->inDataBuffer[this->bufferCharIndex] != '<') { //erstes Zeichen muss oeffnender Tag sein
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

            srl->println('L', "start"); //Sende 'Befehl ok' an LabView

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
                uint16_t errCode = this->readLine();
                if (errCode == 1) {
                    //Uebergebe gelesene Zeile, verarbeite zurueck gegebenen ErrorCode
                    cmn::trim(this->inDataBuffer);
                    if (this->inDataBuffer[0] != '\n') {
                        uint16_t parserErrCode = this->parseInput->parseNewLine(this->inDataBuffer);

                        if (parserErrCode >= 1000)
                            this->main_display->throwError(parserErrCode);
                    }

                } else if (errCode > 1) {
                    //ErrorCode wird auf Display angezeigt
                    this->main_display->throwError(errCode);
                    srl->println('L', errCode); //Sende Errorcode an LabView
                } //else -1: Keine Eingabe vorhanden (leerer String)
            }
        }

        if (this->sending) {
            // ueberpruefe kontinuierlich, ob Eventliste noch Inhalt hat
            // Diese Ueberpruefung ist dafuer da, damit der Fehler, dass sich komplett lere Eventilisten
            // resetten nicht auftritt.
            if (!this->mfc_finished)
                this->mfc_finished = this->main_mfcCtrl->getQueueFinished();
            if (!this->valve_finished)
                this->valve_finished = this->main_valveCtrl->getQueueFinished();

            if (this->mfc_finished && this->valve_finished && !this->finished) {
                this->finished = true;
                this->main_display->bothQueuesFinished();
                this->main_stringBuilder->bothQueuesFinished();
            }
        }

        return true;
    }
}
