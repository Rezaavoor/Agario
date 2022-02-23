#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <stdlib.h>
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
//#include "time.h"


int rectangles[30][3];
int rectanglesIndex = 0;
int agario[3] = { 64, 16, 5};


/*int randomGen(int min, int max){
  //srand(time(0));
  return (rand() % (max - min + 1)) + min;
}*/

void createAgario(){
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

void setupScreen(){

  display_string(0, "");
  display_string(1, "");
  display_string(2, "");
  display_string(3, "");
  display_update();

  int i = 0;
  for(i = 0; i<512;i++){
    screen[i] = 255;
  }
  createAgario();  
}

void clearScreen(){
  int i = 0;
  for(i = 0; i<512;i++){
    screen[i] = 255;
  }
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

  if(newX > 127) newX = 0;
  if(newY > 32) newY = 0;
  if(newX < 0) newX = 127;
  if(newY < 0) newY = 32;

  agario[0] = newX;
  agario[1] = newY;

  markAgario();
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

  if(newX > 127) newX = 0;
  if(newY > 32) newY = 0;
  if(newX < 0) newX = 127;
  if(newY < 0) newY = 32;
 
  rectangles[rectIndex][0] = newX;
  rectangles[rectIndex][1] = newY;

  markRect(newX, newY, s);
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
            eatOrBeat(index);
            return 0;
          }
        }

      }
    }
  }
}

void eatOrBeat(int index){
  int recS = rectangles[index][2];
  int agarS = agario[2];
  if(recS < agarS){
    unmarkRect(index);
    unmarkAgario();
    rectangles[index][2] = 0;
    agario[2] += 1;
    return 0;
  }
}