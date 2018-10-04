/* mbed BufferSerial Library
 * Copyright (c) 2013 KentaShimizu
 * Version 0.1 (May 18, 2013)
 * Released under the MIT License: http://mbed.org/license/mit
 */

#ifndef _IG_BUFFERSERIAL_20130518
#define _IG_BUFFERSERIAL_20130518

#include "mbed.h"

class BufferSerial : public Serial {
private:
protected:
    int _size;
    int _present;
    int _last;
    unsigned char* _buf;
    void _setup(const int& size);
    int _getShift(const int& value);
    void _irq(void);
public:
    BufferSerial(PinName tx , PinName rx);
    BufferSerial(PinName tx , PinName rx , const int& bufferSize);
    BufferSerial(PinName tx , PinName rx , const int& bufferSize , const char* name);
    virtual ~BufferSerial();
    virtual int getc(void);
    virtual int unreadable(void);
    virtual int readable(void);
};

#endif
