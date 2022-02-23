/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x4557;
int timeoutcount2 = 0;
int timeoutcount3 = 0;

char textstring[] = "text, more text, and even more text!";

// project stuff



/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */

void labinit( void )
{
  TRISE &= 0xffffff00; //PORTE = output
  TRISF &= 0x2; //  0000 0010 => BTN1 = output
  TRISD |= 0x0FE0; // 0000 1111 1110 0000     bits 5-11 are inputs


  //Timer 2
  T2CON = 0x70; // TCKPS = 111 =>  0111 0000 bit 4-6 are all 1 => prescaling 1:256
  T2CON &= 0xFD; // TCS = 0 => bit 1 is 0 => internal peripheral clock gets used

  PR2 = (80000000/256)/100; // 80MHZ / 256 presacling = 1s    =>     ans/10 = 10ms
  TMR2 = 0; // timer starts at 0

  T2CON |= 0x8000; // ON = 1 => bit 15 is 1 => timer is enabled


  //Timer 3
  T3CON = 0x70; // TCKPS = 111 =>  0111 0000 bit 4-6 are all 1 => prescaling 1:256
  T3CON &= 0xFD; // TCS = 0 => bit 1 is 0 => internal peripheral clock gets used

  PR3 = (80000000/256)/100; // 80MHZ / 256 presacling = 1s    =>     ans/10 = 10ms
  TMR3 = 0; // timer starts at 0

  T3CON |= 0x8000; // ON = 1 => bit 15 is 1 => timer is enabled

  setupScreen();
  markRect(20, 5, 7);
  markRect(100, 15, 1);
  markRect(50, 1, 1);
  //markRect(110, 30, 1);
  //markRect(46, 2, 2);
  //markRect(70, 8, 10);

  createAgario();

  return;
}

/* This function is called repetitively from the main program */

volatile int* portE = (volatile int*) 0xbf886110; //  =PORTE
int counter = 0x00;



int x=5,y=0;
void labwork( void )
{
  
  /*int btnData = getbtns();  // ex: 4 = 100 = BTN4 is pressed
  int swData = getsw();   // ex: 15 = 1111 = all switches are 1 

  if (btnData & 4){
    mytime &= 0x0fff; // ex: myTime = 0x5957 => mytime = 0x0957
    mytime += swData*16*16*16; // ex: swData=4 => firstDigit = 0x4000
  }
  if (btnData & 2){
      mytime &= 0xf0ff; // ex: myTime = 0x5957 => mytime = 0x5057
      mytime += swData*16*16;  // ex: swData=4 => firstDigit = 0x0400
  }
  if(btnData & 1){
        mytime &= 0xff0f; // ex: myTime = 0x5957 => mytime = 0x5907
        mytime += swData*16;  // ex: swData=4 => firstDigit = 0x0040 
  }*/

  if((IFS(0)>>12) & 1){
    IFS(0) &= 0xffffefff;
    timeoutcount3++;
    if(timeoutcount3==5){
      int btnData = getbtns();

      if(btnData & 8){  // 1000
        moveAgario(-1, 0);
      }
      if(btnData & 4){  // 0100
        moveAgario(0, 1);
      }
      if(btnData & 2){  // 0010
        moveAgario(0, -1);
      }
      if(btnData & 1){  // 0001
        moveAgario(1, 0);
      }
      timeoutcount3 = 0;
    }
  }

  if((IFS(0)>>8) & 1)  //   bit 8 is being masked,  if bit 8 = 1 means timeout
  {


    IFS(0) &= 0xfffffeff; // 1110 1111 1111 reset the bit 8
    timeoutcount2++;
    if(timeoutcount2==8){
      //LEDs shining
      *portE = counter;
      counter += 0x1; // +1   
    
      checkHit();

      moveRect(0, -1, 0);
      moveRect(1, 1, 0);
      moveRect(2, 4, 2);
      markAgario();
      display_image(0, screen);

      ////////////////////////////////////////


      timeoutcount2 = 0;

    }
  }
}
