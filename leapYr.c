#include "config.h"
uchar leapYr(void)
{
   // uchar offset = 0;
    
    if(year%4== 0 && year > 23)
        {
              offset = offset+4 ;      
        }
    else{
            offset = 0;
        }
    return offset;
}
