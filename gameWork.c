/* gameWork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   Changed by Reza Hosseini and Mahdi Nazari during February 2022

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int timeoutcount2 = 0; // counter for Timer2
int timeoutcount3 = 0; // counter for Timer3
int counter = 0x00;    // counter that is shown by LEDs
int disableButtons = 1;          // disables buttons when 1 (works like semaphore )
int menuCounter = 0;   // acts as sort of a delay

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

  return;
}

/* This function is called repetitively from the main program */

void labwork( void )
{
  if((IFS(0)>>12) & 1){ // if bit 12 == 1 means Timer3 is timedout
    IFS(0) &= 0xffffefff;   // reset the bit 12
    timeoutcount3++;
    if(timeoutcount3==5){
      int btnData = getbtns();
      int swData = getsw();

      //read switches
      if(swData == 1 || swData == 0){ // EASY
        changeDifficulty(EASY);
      }
      if(swData == 3){ // MEDIUM
        changeDifficulty(MEDIUM);
      }
      if(swData == 7){ // HARD
        changeDifficulty(HARD); 
      }
      

      // read buttons
      if(gameMode == IN_GAME){
        // game mode
        if(btnData & 8){  // 1000
          moveAgario(-2, 0);
        }
        if(btnData & 4){  // 0100
          moveAgario(0, 2);
        }
        if(btnData & 2){  // 0010
          moveAgario(0, -2);
        }
        if(btnData & 1){  // 0001
          moveAgario(2, 0);
        }
      }
      else if(gameMode == GAME_OVER || gameMode == WIN){
        // start over the game
        if(!disableButtons){ // not disableButtons = buttons are enabled and you can change game mode
          if ( btnData != 0){
            gameMode = IN_GAME;
            score = 0;
            setupScreen();
            disableButtons = 1;
            menuCounter = 0;
          }
        }
      }
      timeoutcount3 = 0;
    }
  }

  if((IFS(0)>>8) & 1)  //   bit 8 is being masked,  if bit 8 = 1 means timeout
  {


    IFS(0) &= 0xfffffeff; // 1110 1111 1111 reset the bit 8
    timeoutcount2++;
    if(timeoutcount2==5){
      //LEDs shining
      PORTE = counter;
      counter += 0x1; // +1   
    
      if(gameMode == START){
        start();
      }
      else if(gameMode == IN_GAME){
        checkHit();

        // the default directions are like below, but new rectangles will have random directions
        moveRect(0, 1+difficulty, 1+difficulty);
        moveRect(1, 2+difficulty, 0+difficulty);
        moveRect(2, -1-difficulty, 1+difficulty);
        moveRect(3, 1+difficulty, -1-difficulty);
        moveRect(4, -1-difficulty, -2-difficulty);
        //moveRect(5, 4, 1);
        
        markAgario(MARK);

        showScore(score);
        display_image(0, screen);
      }
      else if(gameMode == GAME_OVER){
        disableButtons = 1;
        if(menuCounter< 2){
          clearScreen();
          display_string(0, "    Game Over");
          display_string(1, "");
          display_update();
          delay(2000);
          display_string(1, "      Score:");
          display_update();
          delay(1000);
          //show score
          if(score<10)
            showDigit(score, 64, 18);
          else{
            showDigit(score%10, 68, 18);
            showDigit(score/10, 63, 18);
          }
          display_image(0, screen);
          delay(1200);
        }
        else{
          display_string(0, "Press any button");
          display_string(1, "  to play again");
          display_update();
          disableButtons = 0;
        }
        menuCounter++;
      }
      else if(gameMode == WIN){
         disableButtons = 1;
        if(menuCounter< 2){
          clearScreen();
          display_string(0, " U R DA WINNER!");
          display_string(1, "");
          display_update();
          delay(2000);
          display_string(1, "      Score:");
          display_update();
          delay(1000);
          //show score
          if(score<10)
            showDigit(score, 64, 18);
          else{
            showDigit(score%10, 68, 18);
            showDigit(score/10, 63, 18);
          }
          display_image(0, screen);
          delay(1200);
        }
        else{
          display_string(0, "Press any button");
          display_string(1, "  to play again");
          display_update();
          disableButtons = 0;
        }
        menuCounter++;
      }
      timeoutcount2 = 0;

    }
  }
}
