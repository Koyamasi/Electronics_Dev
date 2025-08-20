// #include "Gearbox.h"

// Gearbox::Gearbox(int pPin, int pled, int rPin, int rled, int nPin, int nled, int dPin, int dled, int mPin, int mled)
//     : _pPin(pPin), _pled(pled), _rPin(rPin), _rled(rled), _nPin(nPin), _nled(nled),
//       _dPin(dPin), _dled(dled), _mPin(mPin), _mled(mled) {}

// void Gearbox::init() 
// {
//     pinMode(_pPin, INPUT_PULLUP);
//     pinMode(_rPin, INPUT_PULLUP);
//     pinMode(_nPin, INPUT_PULLUP);
//     pinMode(_dPin, INPUT_PULLUP);
//     pinMode(_mPin, INPUT_PULLUP);

//     pinMode(_pled, OUTPUT);
//     pinMode(_rled, OUTPUT);
//     pinMode(_nled, OUTPUT);
//     pinMode(_dled, OUTPUT);
//     pinMode(_mled, OUTPUT);

//     setGear('P'); // Default to 'P' on startup
// }

// void Gearbox::update() 
// {
//     if (digitalRead(_pPin) == LOW) setGear('P');
//     else if (digitalRead(_rPin) == LOW) setGear('R');
//     else if (digitalRead(_nPin) == LOW) setGear('N');
//     else if (digitalRead(_dPin) == LOW) setGear('D');
//     else if (digitalRead(_mPin) == LOW) setGear('M');
// }

// void Gearbox::setGear(char gear) 
// {
//     if (gear != lastGear) {
//         delay(500); // Debounce
//         setLeds(gear);
//         sendPacket(gear);
//         lastGear = gear;
//     }
// }

// void Gearbox::setLeds(char gear) 
// {
//     digitalWrite(_pled, gear == 'P' ? HIGH : LOW);
//     digitalWrite(_rled, gear == 'R' ? HIGH : LOW);
//     digitalWrite(_nled, gear == 'N' ? HIGH : LOW);
//     digitalWrite(_dled, gear == 'D' ? HIGH : LOW);
//     digitalWrite(_mled, gear == 'M' ? HIGH : LOW);
// }

// void Gearbox::sendPacket(char gear) 
// {
//     byte packet[2] = { (byte)gear, '\n' };
//     Serial.write(packet, 2);
// } 