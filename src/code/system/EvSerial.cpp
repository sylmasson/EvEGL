
#include    <EvEGL.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static DummySerial  sEvSerial;

Stream      *EvIn = &sEvSerial;
Stream      *EvOut = &sEvSerial;
Stream      *EvErr = &sEvSerial;
Stream      *EvDbg = &sEvSerial;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         DummySerial::available(void) { return 0; }
int         DummySerial::peek(void) { return -1; }
int         DummySerial::read(void) { return -1; }
size_t      DummySerial::write(uint8_t C) { return 1; }
int         DummySerial::availableForWrite(void) { return 1; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
