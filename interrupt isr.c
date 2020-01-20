#include "config.h"
#include <stdio.h>
#include "PCF8583Read.h"
#include "hexToDec.h"
#include "byteToBin.h"

void interrupt isr (void)
{
   
    uchar  resultSecs, readSecs;
    uchar secMsb, secLsb, numSec = 0;
    uchar minMsb, minLsb = 0;
    uchar  resultMins, readMins;
    uchar  resultHours, readHours;
    uchar hrsLsb, hrsMsb = 0;
    uchar resultDate, readDate;
    uchar dateLsb, dateMsb = 0;
    uchar resultMonth, readMonth;
    uchar monthLsb, monthMsb = 0;
    uchar  resultWkday, readWkday;
    uchar  resultYr, readYr = 0;
    uchar yrLsb, yrMsb = 0;
    
    if(IOCAFbits.IOCAF3 == 1)
    {
        //printf("IOC interrupt!\n");
        
        /* read time and date from PCF8583*/
        resultSecs = PCF8583Read(0xa0,SECS);
        readSecs = hexToDec(resultSecs);       // convert secs from BCD to decimal
        //printf("secs %d\t", readSecs);
         
        resultMins = PCF8583Read(0xa0,MINS);
        readMins = hexToDec(resultMins);
       // printf("mins %d\t", readMins);
        
        resultHours = PCF8583Read(0xa0,HRS);
        readHours = hexToDec(resultHours);
        //printf("hours %d\t", readHours);
        
        resultDate = PCF8583Read(0xa0, YRDATE);
        readDate =  hexToDec(resultDate & 0x3f);
        //printf("date %d\t", readDate);
        
        resultMonth = PCF8583Read(0xa0, DAYMTHS);
        readMonth = hexToDec(resultMonth & 0x1f);
        //printf("month %d\t", readMonth);
        
        resultYr = PCF8583Read(0xa0, YRDATE);
        readYr = ((resultYr & 0xc0)>>6) + year;
       
        printf("year %d\n", readYr);
        
        /*write time and date to LCD*/
        
        secLsb = readSecs%10;       // write secs on LCD
        secMsb = readSecs/10;
        byteToBin(0,0x92);
        byteToBin(1, secMsb + 0x30);
        byteToBin(1, secLsb + 0x30);
        
        byteToBin(0,0x91);
        byteToBin(1, 0x3a);
         
        minLsb = readMins%10;       // write mins on LCD
        minMsb = readMins/10;
        byteToBin(0,0x8f);
        byteToBin(1, minMsb + 0x30);
        byteToBin(1, minLsb + 0x30);
        
        byteToBin(0,0x8e);          // colon
        byteToBin(1, 0x3a);
        
        hrsLsb = readHours%10;       // write hours on LCD
        hrsMsb = readHours/10;
        byteToBin(0,0x8c);
        byteToBin(1, hrsMsb + 0x30);
        byteToBin(1, hrsLsb + 0x30);
        
        dateLsb = readDate%10;       // write date on LCD
        dateMsb = readDate/10;
        byteToBin(0,0x80);
        byteToBin(1, dateMsb + 0x30);
        byteToBin(1, dateLsb + 0x30);
        
        byteToBin(0,0x82);              // fwd slash
        byteToBin(1, 0x2f);
        
        monthLsb = readMonth%10;       // write month on LCD
        monthMsb = readMonth/10;
        byteToBin(0,0x83);              
        byteToBin(1, monthMsb + 0x30);
        byteToBin(1, monthLsb + 0x30);
        
        byteToBin(0,0x85);              // fwd slash
        byteToBin(1, 0x2f);
        
        yrLsb = readYr%10;       // write year on LCD
        yrMsb = readYr/10;
        byteToBin(0,0x86);              
        byteToBin(1, yrMsb + 0x30);
        byteToBin(1, yrLsb + 0x30);
        
        
    }
    IOCAFbits.IOCAF3 = 0;
    INTCONbits.IOCIF = 0;
}
