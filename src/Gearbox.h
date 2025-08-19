#ifndef GEARBOX_H
#define GEARBOX_H

#include <Arduino.h>

class Gearbox {
public:
    Gearbox(int pPin, int pled, int rPin, int rled, int nPin, int nled, int dPin, int dled, int mPin, int mled);

    void begin();
    void update();

private:
    int _pPin, _pled;
    int _rPin, _rled;
    int _nPin, _nled;
    int _dPin, _dled;
    int _mPin, _mled;

    void setGear(char gear);
    void setLeds(char gear);
    void sendPacket(char gear);
    char lastGear = '\0';
};

#endif 