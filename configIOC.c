#include "config.h"
void configIOC(void)
{
    INTCONbits.GIE = 0;         // enable global interrupts
    INTCONbits.IOCIE = 1;       // enable IOC module
    IOCAPbits.IOCAP3 = 1;       // Interrupt-on-Change enabled on  pin 3  for a positive going edge. IOCAF3 bit and IOCIF flag will
                                // be set upon detecting a positive edge.
    IOCAFbits.IOCAF3 = 0;       // clear interrupt flag for RA3 pin 3 
    INTCONbits.IOCIF = 0;       // clear IOC interrupt flag
}
