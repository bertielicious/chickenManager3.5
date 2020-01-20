/*
 * File:   main.c
 * Author: Phil Glazzard
 *
 * Created on 19 November 2019, 18:49
 */
/* Software Construction
 1. Problem definition
 * Control of a motorised door to the chicken coup where the door will be:
 a)  Open if the light level is daylight or the clock timer has been 
 * triggered to open the door, or if the manual open button has been pressed.
 b) Closed if the light level is night level or the clock timer has been 
 * triggered to close the door or the manual close button has been 
 * pressed
 2. Display on LCD the date, time, door open time, door close time, timer ON/OFF
 * light sensor ON/ OFF, manual OPEN/ CLOSE ON/ OFF and battery charge level on 
 * an LCD with three button keypad to control all of above. Confirm key presses
 *  via buzzer.
 3. Via blu-tooth connection, transmit all LCD display info to phone app, and 
 * also allow control of all door functions from the app.
 4. Microcontroller to be in sleep mode except when:
 * a) door needs to move from closed to open or open to closed due to timer or 
 * light sensor inputs or manual close/ open button.
 * b) someone presses a keypad button (time out back to sleep mode after 
 * 2 minutes without button activity)
 * (c) blue tooth transmit and receive
 2. Sub systems
 * a) LCD
 * b) uC sleep
 * c) blue tooth
 * d) door
 * e) timer/ clock
 * f) light sensor
 * g) keypad
 * h) solar psu
 * i) motor
 * 
 3. Structs
 * a) LCD
 *  date(day, month, year)
 *  time (hour, minutes, seconds)
 *  door (open time, close time)
 *  timer(on, off)
 *  set time(door open time, door close time)
 *  light sensor (on, off)
 *  light sensor (adjust up time, adjust down time)
 *  manual door button (open, close)
 *  battery charge level display (o% - 100%)
 *  keypad with three buttons (up, down, enter)
 *  confirm key press with buzzer (key pressed, key not pressed)
 * 
 * b) uC sleep
 * if (blue tooth active or button pressed or door needs to open or close (light sensor or timer))
 *    wake up
 *    run required code
 *    else
 *    sleep
 * 
 * c) blu-tooth
 *    transmit
 *    receive
 *    run necessary code
 * 
 * d) door
 *    open 
 *    close
 *    open or close door
 * 
 * e) clock/ timer
 *      check open
 *      check close
 *      open or close door
 * 
 * f) light sensor
 *      check light
 *      check dark
 *      open or close door
 * 
 * g) keypad
 *    check button press
 *       which button pressed
 *       setup screen or exit to main loop
 *       take action or open / close door
 *  
 * 
 * h) solar psu
 *      charge LiPo battery
 *      display battery charge level
 * 
 * i) motor
 *    motor moves clockwise = open door
 *    motor moves anti-clockwise = close door
 * 
 * 
 *                  16f1459
 *                  ---------
 *   +5 Volts    1 |Vdd      | 20 0 Volts
        LCD D6   2 |RA5   RA0| 19   - PUSH BUTTON
 *    motor ACW  3 |RA4   RA1| 18   + PUSH BUTTON
      IOCA3      4 |RA3      | 17  MOTOR DIRECTION
 *  ENT PBUTTON  5 |RC5   RC0| 16  BOTTOM LIMIT SWITCH
 *    RS         6 |RC4   RC1| 15  motor CW
 *    EN         7 |RC3   RC2| 14  TOP LIMIT SWITCH
 *    LCD D4     8 |RC6   RB4| 13  SDA
 *    LCD D5     9 |RC7   RB5| 12  LCD D7
 *    TX        10 |RB7   RB6| 11  SCL
 *                  ---------
 *
 */

#include "config.h"
#include "configOsc.h"
#include "configPorts.h"
#include "configUsart.h"
#include "putch.h"
#include <stdio.h>
#include "configLCD.h"
#include "pulse.h"
#include "nibToBin.h"
#include "byteToBin.h"
#include "configI2c.h"
#include "i2cStart.h"
#include "i2cWrite.h"
#include "i2cRestart.h"
#include "PCF8583Read.h"
#include "PCF8583Write.h"
#include "setupTime.h"
#include "dateInput.h"

#include "clearRow.h"
#include "setupDate.h"
#include "decToHex.h"
#include "hexToDec.h"
#include "configIOC.h"
#include "splashScreen.h"
#include "setTimeDate.h"

void main()
{
    configOsc();        // configure internal clock to run at 16MHz
    configPorts();      // configure PORTS A/ B/ C
    configUsart();      // allow serial comms to PC for debugging
    configLCD();        // 20 x 4 LCD set up for 4 bit operation
    configI2c();        // I2C setup with PCF8583 RTC chip
    configIOC();
    printf("hello!\n"); // test serial port
    
  
    /* define variables for seconds, minutes, hours, date, month and year*/
     uchar seconds, secondsSet,minutes, minutesSet,hours, hoursSet,date, dateSet,wkday, wkdaySet, month, monthSet, yr ,yrSet, numSec, secLsb, secMsb  = 0;
     uchar colon = 0x3a;
     uchar numMin, minLsb, minMsb, numHour, hourLsb, hourMsb = 0;
    /*Initialise time and date*/
    /*secondsSet = 0;
    minutesSet = 0;
    hoursSet = 0;
    dateSet = 0;
    wkdaySet = 0;
    monthSet = 0;
    yrSet = 0;*/
    uchar status = 1;
    splashScreen();     // display "Chicken Manager 3" on LCD
    __delay_ms(2000);
    clearRow(0xc0, 0xd3);
    PCF8583Write(0xa0, CTRLSTAT, 0x80);         //turn counting off until counter settings loaded
    while(status <25)
    {
        status = setTimeDate(status);
        printf("state %d\n", status);
    }
    INTCONbits.GIE = 1;         // enable global interrupts
    PCF8583Write(0xa0, CTRLSTAT, 0x00);         //turn counting ON
    while(1)
    {
      //status = setTimeDate(status);
     // 
       if(UP == 0) 
       {
           RC2 = 1;
       }
       else if(DOWN == 0)
       {
           RC2 = 0;
       }
    }
}