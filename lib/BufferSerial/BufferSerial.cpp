#include "BufferSerial.h"

BufferSerial :: BufferSerial( PinName tx , PinName rx ) : Serial( tx , rx ) {
    _setup( 1 );
}

BufferSerial :: BufferSerial( PinName tx , PinName rx , const int& bufferSize ) : Serial( tx , rx ) {
    _setup( bufferSize );
}

BufferSerial :: BufferSerial( PinName tx , PinName rx , const int& bufferSize , const char* name ) : Serial( tx , rx , name ) {
    _setup( bufferSize );
}

BufferSerial :: ~BufferSerial() {
    delete[] _buf;
}

void BufferSerial :: _setup( const int& bufferSize ) {
    if ( bufferSize > 1 ){
        _buf = new unsigned char[ bufferSize ];
        _size = bufferSize - 1;
    }else{
        _buf = new unsigned char[ 2 ];
        _size = 1;
    }
    _present = 0;
    _last = 0;
    attach( this, & BufferSerial :: _irq );
}

int BufferSerial :: _getShift( const int& value ) {
    return value ? ( value - 1 ) : _size ;
}

void BufferSerial :: _irq( void ) {
    int n = _getShift( _last );
    _buf[ n ] = (unsigned char)( Serial :: getc() );
    _last = n;
}

int BufferSerial :: unreadable( void ) {
    return ( _present == _last );
}

int BufferSerial :: readable( void ) {
    return ! unreadable();
}

int BufferSerial :: getc( void ) {
    if ( unreadable() ){
        return -1;
    }else{
        _present = _getShift( _present );
        return (int)_buf[ _present ];
    }
}
