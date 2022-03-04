#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <stdlib.h>
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

float rectangles[30][3];  // 30 empty spots each storing information for one rectangle(x,y,s)
int rectanglesIndex = 0;  // number of rectangles in the screen
float agario[3];  // information of Agario(x,y,s)

int score = 0;

int EASY = 0, MEDIUM = 1, HARD = 2;
int difficulty = 0;

int START = 0, IN_GAME = 1, GAME_OVER = 2, WIN = 3; // states of the game
int gameMode = 0; // START

int MARK = 1, UNMARK = 0; 

// random generator based on clock of Timer2, Timer3, number of rectangles and Agarioä's position
int randomGen(int min, int max){
  int randomNumber = (TMR3 / TMR2) * rectanglesIndex + agario[0] - agario[1];
  return (randomNumber % (max - min + 1)) + min;
}

void changeDifficulty(int diff){
  if(difficulty != diff){
    menuCounter = 0;
    difficulty = diff;
    gameMode = START;
  }
}

// clears screen
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

// sets up screen by cleaning and creating rectangles + Agario
void setupScreen(){
  clearScreen();
  createAgario();  
  rectanglesIndex = 0;
  markRect(10 + (randomGen(0, 117)), 3+ (randomGen(0, 29)), 8+ (randomGen(0, 3)));
  markRect(100 + (randomGen(0, 27)), 10+ (randomGen(0, 22)), 3+ (randomGen(0, 4)));
  markRect(70 + (randomGen(0, 57)), 15+ (randomGen(0, 17)), 2+ (randomGen(0, 4)));
  markRect(10 + (randomGen(0, 117)), 17+ (randomGen(0, 15)), 1+ (randomGen(0, 4)));
  markRect(0 + (randomGen(0, 127)), 20+ (randomGen(0, 12)), 1+ (randomGen(0, 4)));
  //markRect(4 + (randomGen(0, 123)), 7+ (randomGen(0, 25)), 1+ (randomGen(0, 4)));

}

// start mode
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

void setPixel(int x, int y, int mark){
  // invalid x,y are ignored
	if(y<0 || x<0 || x>127 || y>31) 
  {
    return;
  }

  int index = x + ((y/8)*128);  // +128 per page
  int rowInPage = y%8;
  
  int i, newEightBit=0b00000001;
  for(i=0; i<rowInPage; i++){
    newEightBit = newEightBit << 1;
  }

  if(mark){
    newEightBit = ~newEightBit;
    screen[index] = screen[index] & newEightBit;
  }
  else{
    screen[index] = screen[index] | newEightBit;
  }
}

// creates Agario
void createAgario(){
  int i;
  int x = 64, y = 16, s = 5;
  agario[0] = x;
  agario[1] = y;
  agario[2] = s;

  for(i=0; i<s; i++)
    setPixel(x+i, y, MARK);

  for(i=0; i<s; i++)
    setPixel(x, y+i, MARK);

  for(i=0; i<s; i++)
    setPixel(x+s, y+i, MARK);

  for(i=0; i<=s; i++)
    setPixel(x+i, y+s, MARK);
}

// add a new rectangle
void markRect(int x, int y, int s){
  int i,j;
  for(i=x; i < x+s; i++){
    for(j=y; j < y+s; j++){
      setPixel(i,j, MARK);
    }
  }
  rectangles[rectanglesIndex][0] = x;
  rectangles[rectanglesIndex][1] = y;
  rectangles[rectanglesIndex][2] = s;
  rectanglesIndex++;
}

// updates an existing rectangle
void updateRect(int index, int x, int y, int s){
  int i,j;
  for(i=x; i < x+s; i++){
    for(j=y; j < y+s; j++){
      setPixel(i,j, MARK);
    }
  }
  rectangles[index][0] = x;
  rectangles[index][1] = y;
  rectangles[index][2] = s;
}

// removes a rectangle from screen only
void unmarkRect(int index){

  int x = rectangles[index][0];
  int y = rectangles[index][1];
  int s = rectangles[index][2];
  int i,j;
  for(i=x; i < x+s; i++){
    for(j=y; j < y+s; j++){
      setPixel(i,j, UNMARK);
    }
  }
}

//  moves a rectangle to a new position by xOffset and yOffset
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

// appears Agario on screen
void markAgario(int mark){
  int i;
  int x = agario[0];
  int y = agario[1];
  int s = agario[2];
  
  for(i=0; i<s; i++)
    setPixel(x+i, y, mark);

  for(i=0; i<s; i++)
    setPixel(x, y+i, mark);

  for(i=0; i<s; i++)
    setPixel(x+s, y+i, mark);

  for(i=0; i<=s; i++)
    setPixel(x+i, y+s, mark);
}

// moves Agario to a new position
void moveAgario(int xOffset, int yOffset){
  markAgario(UNMARK);
  int newX = agario[0] + xOffset;
  int newY = agario[1] + yOffset;
  int s = agario[2];

  if(newX > 127) newX = -s;
  if(newY > 32) newY = -s;
  if(newX < -s) newX = 127;
  if(newY < -s) newY = 32;

  agario[0] = newX;
  agario[1] = newY;

  markAgario(MARK);
}

// clears scoreboard located at top-left
void clearScoreboard(){
  int i,j;
  for(i=0; i<31; i++){
    for(j=0; j<5; j++){
      setPixel(i,j, UNMARK);
    }
  }
}

// shows the string "score:" on top-left cornenr of the display
void showScoreString(){
  //S
  setPixel(0,0, MARK);
  setPixel(1,0, MARK);
  setPixel(2,0, MARK);
  setPixel(0,1, MARK);
  setPixel(0,2, MARK);
  setPixel(1,2, MARK);
  setPixel(2,2, MARK);
  setPixel(2,3, MARK);
  setPixel(0,4, MARK);
  setPixel(1,4, MARK);
  setPixel(2,4, MARK);

  //C
  setPixel(4,0, MARK);
  setPixel(5,0, MARK);
  setPixel(6,0, MARK);
  setPixel(4,1, MARK);
  setPixel(4,2, MARK);
  setPixel(4,3, MARK);
  setPixel(4,4, MARK);
  setPixel(5,4, MARK);
  setPixel(6,4, MARK);

  //O
  setPixel(8,0, MARK);
  setPixel(9,0, MARK);
  setPixel(10,0, MARK);
  setPixel(8,1, MARK);
  setPixel(10,1, MARK);
  setPixel(8,2, MARK);
  setPixel(10,2, MARK);
  setPixel(8,3, MARK);
  setPixel(10,3, MARK);
  setPixel(8,4, MARK);
  setPixel(9,4, MARK);
  setPixel(10,4, MARK);

  //R
  setPixel(12,0, MARK);
  setPixel(13,0, MARK);
  setPixel(14,0, MARK);
  setPixel(12,1, MARK);
  setPixel(14,1, MARK);
  setPixel(12,2, MARK);
  setPixel(13,2, MARK);
  setPixel(14,2, MARK);
  setPixel(12,3, MARK);
  setPixel(13,3, MARK);
  setPixel(12,4, MARK);
  setPixel(14,4, MARK);
  
  //E
  setPixel(16,0, MARK);
  setPixel(17,0, MARK);
  setPixel(18,0, MARK);
  setPixel(16,1, MARK);
  setPixel(16,2, MARK);
  setPixel(17,2, MARK);
  setPixel(18,2, MARK);
  setPixel(16,3, MARK);
  setPixel(16,4, MARK);
  setPixel(17,4, MARK);
  setPixel(18,4, MARK);

  //:
  setPixel(20,1, MARK);
  setPixel(20,3, MARK);
}

// shows the value of score in a desired position(x,y)
void showDigit(int score, int x, int y){
  switch (score)
  {
  case 0:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(0 + x,1 + y, MARK);
    setPixel(2 + x,1 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(0 + x,3 + y, MARK);
    setPixel(0 + x,4 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    break;
  case 1:
    setPixel(1 + x,0 + y, MARK);
    setPixel(1 + x,1 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(1 + x,3 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    break;
  case 2:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(2 + x,1 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(0 + x,3 + y, MARK);
    setPixel(0 + x,4 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    break;
  case 3:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(2 + x,1 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    setPixel(0 + x,4 + y, MARK);
    break;
  case 4:
    setPixel(0 + x,0 + y, MARK);
    setPixel(0 + x,1 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    setPixel(2 + x,1 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    break;
  case 5:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(0 + x,1 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(0 + x,4 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    break;
  case 6:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(0 + x,1 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(0 + x,3 + y, MARK);
    setPixel(0 + x,4 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    
    break;
  case 7:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(2 + x,1 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    break;
  case 8:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(0 + x,1 + y, MARK);
    setPixel(2 + x,1 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(0 + x,3 + y, MARK);
    setPixel(0 + x,4 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    break;
  case 9:
    setPixel(0 + x,0 + y, MARK);
    setPixel(1 + x,0 + y, MARK);
    setPixel(2 + x,0 + y, MARK);
    setPixel(0 + x,1 + y, MARK);
    setPixel(2 + x,1 + y, MARK);
    setPixel(0 + x,2 + y, MARK);
    setPixel(1 + x,2 + y, MARK);
    setPixel(0 + x,4 + y, MARK);
    setPixel(1 + x,4 + y, MARK);
    setPixel(2 + x,4 + y, MARK);
    setPixel(2 + x,3 + y, MARK);
    setPixel(2 + x,2 + y, MARK);
    break;
  
  default:
    setPixel(0 + x,0 + y, MARK);
    break;
  }
}

// updates the scoreboard with new score
void showScore(int score){
  clearScoreboard();
  showScoreString();
  if(score<10)
    showDigit(score, 23, 0);
  else{
    showDigit(score%10, 28, 0);
    showDigit(score/10, 23, 0);
  }
}

// is called when a rectangle meets Agario. "eat" if Agario>Recatngle otherwise "be fed" = lose
void eatOrBeFed(int index){
  int recS = rectangles[index][2];
  float agarS = agario[2];
  if(recS < agarS){
    //eat
    unmarkRect(index);
    markAgario(UNMARK);

    agario[2] += (recS + 1.0)/agarS; // growth depends on rectangles size
    score++; // +1 on score for each meal!

    if(difficulty != EASY){
      if(index == 0 || index == 1){
        rectangles[index][2] = agario[2] + randomGen(0, 8);
      }
      else {
        rectangles[index][2] = randomGen(0, agario[2]);
      }
    }
    else{
      if(index == 0){
        rectangles[index][2] = agario[2] + randomGen(0, 8);
      }
      else {
        rectangles[index][2] = randomGen(0, agario[2]);
      }
    }

    rectangles[index][0] += randomGen(0, (127-(rectangles[index][0]) ) );
    rectangles[index][1] += randomGen(0, (32-(rectangles[index][1]) ) );

    
    //check win possibility based on Agario's size
    if(agario[2] >= 13){
      gameMode = WIN;
    }

    return;
  }
  else {
    //be fed = game over
    gameMode = GAME_OVER;
  }
}

// check if it is a hit
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
