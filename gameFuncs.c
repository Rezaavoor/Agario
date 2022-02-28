#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <stdlib.h>
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
//#include "time.h"


float rectangles[30][3];
int rectanglesIndex = 0;
float agario[3];
int score = 0;

int START = 0, IN_GAME = 1, GAME_OVER = 2, WIN = 3;
int gameMode = 0; // START

int randomGen(int min, int max){
  int randomNumber = (TMR3 / TMR2) * rectanglesIndex + agario[0] - agario[1];
  return (randomNumber % (max - min + 1)) + min;
}
void clearScreen(){
  display_string(0, "");
  display_string(1, "");
  display_string(2, "");
  display_string(3, "");
  display_update();

  int i = 0;
  for(i = 0; i<512;i++){
    screen[i] = 255;
  }
}

void setupScreen(){
  clearScreen();
  createAgario();  
  rectanglesIndex = 0;
  markRect(10 + (randomGen(0, 117)), 3+ (randomGen(0, 29)), 8+ (randomGen(0, 3)));
  markRect(25 + (randomGen(0, 102)), 10+ (randomGen(0, 22)), 8+ (randomGen(0, 4)));
  markRect(15 + (randomGen(0, 113)), 15+ (randomGen(0, 17)), 2+ (randomGen(0, 4)));
  markRect(10 + (randomGen(0, 117)), 17+ (randomGen(0, 15)), 1+ (randomGen(0, 4)));
  markRect(0 + (randomGen(0, 127)), 20+ (randomGen(0, 12)), 1+ (randomGen(0, 4)));
  //markRect(4 + (randomGen(0, 123)), 7+ (randomGen(0, 25)), 1+ (randomGen(0, 4)));

}

void start(){
  clearScreen();
  display_string(1, "   Agario!!!!");
  display_update();
  delay(1800);
  display_string(1, "ARE YOU READY???");
  display_update();
  delay(1500);
  display_string(1, "        3");
  display_update();
  delay(1000);
  display_string(1, "        2");
  display_update();
  delay(1000);
  display_string(1, "        1");
  display_update();
  delay(1000);

  gameMode = IN_GAME;
  setupScreen();

}

void createAgario(){
  int i;
  int x = 64, y = 16, s = 5;
  agario[0] = x;
  agario[1] = y;
  agario[2] = s;

  for(i=0; i<s; i++)
    markPixel(x+i, y);

  for(i=0; i<s; i++)
    markPixel(x, y+i);

  for(i=0; i<s; i++)
    markPixel(x+s, y+i);

  for(i=0; i<=s; i++)
    markPixel(x+i, y+s);
}


void markRect(int x, int y, int s){
  int i,j;
  for(i=x; i < x+s; i++){
    for(j=y; j < y+s; j++){
      markPixel(i,j);
    }
  }
  rectangles[rectanglesIndex][0] = x;
  rectangles[rectanglesIndex][1] = y;
  rectangles[rectanglesIndex][2] = s;
  rectanglesIndex++;
}
void updateRect(int index, int x, int y, int s){
  int i,j;
  for(i=x; i < x+s; i++){
    for(j=y; j < y+s; j++){
      markPixel(i,j);
    }
  }
  rectangles[index][0] = x;
  rectangles[index][1] = y;
  rectangles[index][2] = s;
}

void unmarkRect(int index){

  int x = rectangles[index][0];
  int y = rectangles[index][1];
  int s = rectangles[index][2];
  int i,j;
  for(i=x; i < x+s; i++){
    for(j=y; j < y+s; j++){
      unmarkPixel(i,j);
    }
  }
}

void moveRect(int rectIndex, int xOffset, int yOffset){
  unmarkRect(rectIndex);
  int x = rectangles[rectIndex][0];
  int y = rectangles[rectIndex][1];
  int s = rectangles[rectIndex][2];

  int newX = x+xOffset;
  int newY = y+yOffset;

  if(newX > 127) newX = -s;
  if(newY > 32) newY = -s;
  if(newX < -s) newX = 127;
  if(newY < -s) newY = 32;
 
  rectangles[rectIndex][0] = newX;
  rectangles[rectIndex][1] = newY;

  updateRect(rectIndex, newX, newY, s);
}

void markAgario(){
  int i;
  int x = agario[0];
  int y = agario[1];
  int s = agario[2];
  
  for(i=0; i<s; i++)
    markPixel(x+i, y);

  for(i=0; i<s; i++)
    markPixel(x, y+i);

  for(i=0; i<s; i++)
    markPixel(x+s, y+i);

  for(i=0; i<=s; i++)
    markPixel(x+i, y+s);
}

void unmarkAgario(){
  int i;
  int x = agario[0];
  int y = agario[1];
  int s = agario[2];

  for(i=0; i<s; i++)
    unmarkPixel(x+i, y);

  for(i=0; i<s; i++)
    unmarkPixel(x, y+i);

  for(i=0; i<s; i++)
    unmarkPixel(x+s, y+i);

  for(i=0; i<=s; i++)
    unmarkPixel(x+i, y+s);
}

void moveAgario(int xOffset, int yOffset){
  unmarkAgario();
  int newX = agario[0] + xOffset;
  int newY = agario[1] + yOffset;
  int s = agario[2];

  if(newX > 127) newX = -s;
  if(newY > 32) newY = -s;
  if(newX < -s) newX = 127;
  if(newY < -s) newY = 32;

  agario[0] = newX;
  agario[1] = newY;

  markAgario();
}
void clearScoreboard(){
  int i,j;
  for(i=0; i<29; i++){
    for(j=0; j<5; j++){
      unmarkPixel(i,j);
    }
  }
}


 
void markPixel (int x, int y){
	if(y<0 | x<0){
		x= -1;
		y=-1;
	}
	if(x>127 | y > 32){
		x= -1;
		y=-1;
	}
    if(y>= 8 && y<16){
        y=y-8;
        x = x +128;
        if(x<129 | x>257){
            x= -1;
        }
    }
    if(y>= 16 && y<24){
        y=y-16;
        x = x +256;
        if(x<257 | x>384){
            x= -1;
        }
    }
    if(y>= 24 && y<32){
        y=y-24;
        x = x +384;
        if(x<384 | x>512){
            x= -1;
        }
    }
    if(y==0){
        int write = ~1;
        screen[x] = screen[x] & write;
    }
    else {
        int k = 1;
        int l;

        for(l=1; l<8; l++){
            k *= 2;
            if(y==l){
                    int write = ~k;
                    screen[x] = screen[x] & write;
            }
        }
    }
}
void unmarkPixel (int x, int y){
	if(y<0 | x<0){
		x= -1;
		y=-1;
	}
	if(x>127 | y > 32){
		x= -1;
		y=-1;
	}
    if(y>= 8 && y<16){
        y=y-8;
        x = x +128;
        if(x<129 | x>257){
            x= -1;
        }
    }
    if(y>= 16 && y<24){
        y=y-16;
        x = x +256;
        if(x<257 | x>384){
            x= -1;
        }
    }
    if(y>= 24 && y<32){
        y=y-24;
        x = x +384;
        if(x<384 | x>512){
            x= -1;
        }
    }
    if(y==0){
        int write = 1; //11111110 
        screen[x] = screen[x] | write;
    }
    else {
        int k = 1;
        int l;

        for(l=1; l<8; l++){
            k *= 2;
            if(y==l){
                    int write = k;
                    screen[x] = screen[x] | write;
            }
        }
    }
}

void showScoreString(){
  //S
  markPixel(0,0);
  markPixel(1,0);
  markPixel(2,0);
  markPixel(0,1);
  markPixel(0,2);
  markPixel(1,2);
  markPixel(2,2);
  markPixel(2,3);
  markPixel(0,4);
  markPixel(1,4);
  markPixel(2,4);

  //C
  markPixel(4,0);
  markPixel(5,0);
  markPixel(6,0);
  markPixel(4,1);
  markPixel(4,2);
  markPixel(4,3);
  markPixel(4,4);
  markPixel(5,4);
  markPixel(6,4);

  //O
  markPixel(8,0);
  markPixel(9,0);
  markPixel(10,0);
  markPixel(8,1);
  markPixel(10,1);
  markPixel(8,2);
  markPixel(10,2);
  markPixel(8,3);
  markPixel(10,3);
  markPixel(8,4);
  markPixel(9,4);
  markPixel(10,4);

  //R
  markPixel(12,0);
  markPixel(13,0);
  markPixel(14,0);
  markPixel(12,1);
  markPixel(14,1);
  markPixel(12,2);
  markPixel(13,2);
  markPixel(14,2);
  markPixel(12,3);
  markPixel(13,3);
  markPixel(12,4);
  markPixel(14,4);
  
  //E
  markPixel(16,0);
  markPixel(17,0);
  markPixel(18,0);
  markPixel(16,1);
  markPixel(16,2);
  markPixel(17,2);
  markPixel(18,2);
  markPixel(16,3);
  markPixel(16,4);
  markPixel(17,4);
  markPixel(18,4);

  //:
  markPixel(20,1);
  markPixel(20,3);
}
void showDigit(int score, int x, int y){
  switch (score)
  {
  case 0:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(0 + x,1 + y);
    markPixel(2 + x,1 + y);
    markPixel(0 + x,2 + y);
    markPixel(0 + x,3 + y);
    markPixel(0 + x,4 + y);
    markPixel(1 + x,4 + y);
    markPixel(2 + x,4 + y);
    markPixel(2 + x,3 + y);
    markPixel(2 + x,2 + y);
    break;
  case 1:
    markPixel(1 + x,0 + y);
    markPixel(1 + x,1 + y);
    markPixel(1 + x,2 + y);
    markPixel(1 + x,3 + y);
    markPixel(1 + x,4 + y);
    break;
  case 2:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(2 + x,1 + y);
    markPixel(2 + x,2 + y);
    markPixel(1 + x,2 + y);
    markPixel(0 + x,2 + y);
    markPixel(0 + x,3 + y);
    markPixel(0 + x,4 + y);
    markPixel(1 + x,4 + y);
    markPixel(2 + x,4 + y);
    break;
  case 3:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(2 + x,1 + y);
    markPixel(2 + x,2 + y);
    markPixel(1 + x,2 + y);
    markPixel(0 + x,2 + y);
    markPixel(2 + x,3 + y);
    markPixel(2 + x,4 + y);
    markPixel(1 + x,4 + y);
    markPixel(0 + x,4 + y);
    break;
  case 4:
    markPixel(0 + x,0 + y);
    markPixel(0 + x,1 + y);
    markPixel(0 + x,2 + y);
    markPixel(1 + x,2 + y);
    markPixel(2 + x,2 + y);
    markPixel(2 + x,1 + y);
    markPixel(2 + x,0 + y);
    markPixel(2 + x,3 + y);
    markPixel(2 + x,4 + y);
    break;
  case 5:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(0 + x,1 + y);
    markPixel(0 + x,2 + y);
    markPixel(1 + x,2 + y);
    markPixel(2 + x,2 + y);
    markPixel(2 + x,3 + y);
    markPixel(0 + x,4 + y);
    markPixel(1 + x,4 + y);
    markPixel(2 + x,4 + y);
    break;
  case 6:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(0 + x,1 + y);
    markPixel(0 + x,2 + y);
    markPixel(1 + x,2 + y);
    markPixel(2 + x,2 + y);
    markPixel(2 + x,3 + y);
    markPixel(0 + x,3 + y);
    markPixel(0 + x,4 + y);
    markPixel(1 + x,4 + y);
    markPixel(2 + x,4 + y);
    
    break;
  case 7:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(2 + x,1 + y);
    markPixel(2 + x,2 + y);
    markPixel(2 + x,3 + y);
    markPixel(2 + x,4 + y);
    break;
  case 8:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(0 + x,1 + y);
    markPixel(2 + x,1 + y);
    markPixel(0 + x,2 + y);
    markPixel(1 + x,2 + y);
    markPixel(0 + x,3 + y);
    markPixel(0 + x,4 + y);
    markPixel(1 + x,4 + y);
    markPixel(2 + x,4 + y);
    markPixel(2 + x,3 + y);
    markPixel(2 + x,2 + y);
    break;
  case 9:
    markPixel(0 + x,0 + y);
    markPixel(1 + x,0 + y);
    markPixel(2 + x,0 + y);
    markPixel(0 + x,1 + y);
    markPixel(2 + x,1 + y);
    markPixel(0 + x,2 + y);
    markPixel(1 + x,2 + y);
    markPixel(0 + x,4 + y);
    markPixel(1 + x,4 + y);
    markPixel(2 + x,4 + y);
    markPixel(2 + x,3 + y);
    markPixel(2 + x,2 + y);
    break;
  
  default:
    markPixel(0 + x,0 + y);
    break;
  }
}

void showScore(int score){
  clearScoreboard();
  showScoreString();
  if(score<10)
    showDigit(score, 23, 0);
  else{
    showDigit(score%10, 27, 0);
    showDigit(score/10, 23, 0);
  }
}
void eatOrBeFed(int index){
  int recS = rectangles[index][2];
  float agarS = agario[2];
  if(recS < agarS){
    //eat
    unmarkRect(index);
    unmarkAgario();
    if(index == 0 || index == 1){
      rectangles[index][2] = agario[2] + randomGen(0, 5);
    }
    else {
      rectangles[index][2] += randomGen(0, (10-(rectangles[index][2]) ) );
    }
    rectangles[index][0] += randomGen(0, (127-(rectangles[index][0]) ) );
    rectangles[index][1] += randomGen(0, (32-(rectangles[index][1]) ) );

    agario[2] += (recS + 1.0)/agarS; // growth depends on rectangles size
    score++;
    
    //check win possibility
    if(agario[2] >= 10){
      gameMode = WIN;
    }

    return;
  }
  else {
    //be fed = game over
    gameMode = GAME_OVER;
  }
}

void checkHit(){
  int x1 = agario[0],
      x2 = agario[0]+ agario[2], 
      y1 = agario[1], 
      y2 = agario[1] + agario[2];
  
  int index,i,j;
  for(index = 0; index < rectanglesIndex; index++){
    int x = rectangles[index][0];
    int y = rectangles[index][1];
    int s = rectangles[index][2];
    for(i=x; i < (x+s); i++){
      for(j=y; j < (y+s); j++){

        if(i > x1 && i < x2){
          if(j > y1 && j < y2){
            //HIT
            eatOrBeFed(index);
            return;
          }
        }

      }
    }
  }
}
