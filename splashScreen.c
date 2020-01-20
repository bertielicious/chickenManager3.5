#include "config.h"
#include "byteToBin.h"
void splashScreen(void)
{
    uchar splash[] = {"Chicken Manager 3"};
    
    uchar *chickPtr = &splash[0];
  
    byteToBin(0,0xc2);              // write banner on 2nd line, 2nd space to right
    while (*chickPtr > 0)
    {
        byteToBin(1, *chickPtr);    // send charater data to the LCD
        chickPtr++;
    }
}
