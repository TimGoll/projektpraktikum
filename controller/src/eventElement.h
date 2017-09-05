#ifndef EVENTELEMENT_H
#define EVENTELEMENT_H

namespace control {
    typedef struct MFC_eventElementStruct {
        float value;
        uint32_t time;
    } eventElement_mfc;

    typedef struct Valve_eventElementStruct {
        int8_t value;
        uint32_t time;
    } eventElement_valve;

}

#endif
