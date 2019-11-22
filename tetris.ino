#include <LedControl.h>

#define Bpin_A 3  //left button
#define Bpin_B 6  //right button
#define Bpin_C 10 //down button

boolean contlp = true;//boolean for loop cond.
char matrix[10][20];//display
boolean active = false;

boolean btnAnw = false;
boolean btnAlt = false;
boolean btnBnw = false;
boolean btnBlt = false;
boolean btnCnw = false;
boolean btnClt = false;

short frames = 1;
char tetromino[7][4][4][2] = {
  {//O
    {
      {0, 0}, {1, 0}, {0, -1}, {1, -1}
    },
    {
      {0, 0}, {1, 0}, {0, -1}, {1, -1}
    },
    {
      {0, 0}, {1, 0}, {0, -1}, {1, -1}
    },
    {
      {0, 0}, {1, 0}, {0, -1}, {1, -1}
    }
  },
  {//I
    {//vertical
      {0, 1}, {0, 0}, {0, -1}, {0, -2}
    },
    {//horizontal
      {-2, 0}, {-1, 0}, {0, 0}, {1, 0}
    },
    {//vertical
      {0, 2}, {0, 1}, {0, 0}, {0, -1}
    },
    {//horizontal
      {-2, 0}, {-1, 0}, {0, 0}, {1, 0}
    }
  },
  {//S
    {//horizontal
      {-1,-1}, {0, -1}, {0, 0}, {1, 0}
    },
    {//vertical
      {0, 1}, {0, 0}, {1, 0}, {1, 1}
    },
    {//horizontal
      {-1,-1}, {0, -1}, {0, 0}, {1, 0}
    },
    {//vertical
      {0, 1}, {0, 0}, {1, 0}, {1, 1}
    }
  },
  {//Z
    {//horizontal
      {-1, 1}, {0, 1}, {0, 0}, {-1, 0}
    },
    {//vertical
      {1, 1}, {1, 0}, {0, 0}, {0, -1}
    },
    {//horizontal
      {-1, 1}, {0, 1}, {0, 0}, {-1, 0}
    },
    {//vertical
      {1, 1}, {1, 0}, {0, 0}, {0, -1}
    }
  },
  {//T
    {//inverted
      {-1, 0}, {0, 0}, {1, 0}, {0, 1}
    },
    {//towards right
      {0, 1}, {0, 0}, {1, 0}, {0, -1}
    },
    {//straight
      {-1, 0}, {0, 0}, {1, 0}, {0, -1}
    },
    {//towards left
      {0, 1}, {0, 0}, {-1, 0}, {0, -1}
    }
  },
  {//J
    {//up
      {-1, 1}, {-1, 0}, {0, 0}, {1, 0}
    },
    {//right
      {1, 1}, {0, 1}, {0, 0}, {0, -1}
    },
    {//down
      {1, -1}, {1, 0}, {0, 0}, {-1, 0}
    },
    {//left
      {0, 1}, {0, 0}, {0, -1}, {-1, -1}
    }
  },
  {//L
    {//up
      {1, 1}, {1, 0}, {0, 0}, {-1, 0}
    },
    {//right
      {0, -1}, {0, 0}, {0, -1}, {1, -1}
    },
    {//down
      {1, 0}, {0, 0}, {-1, 0}, {-1, -1}
    },
    {//left
      {0, -1}, {0, 0}, {0, 1}, {-1, 1}
    }
  }
};

byte current;
byte orient = 0;
byte location[2];
short rowCom = 0;

void spawn(){
  active = true;
  location[0] = 6;
  location[1] = 18;
  orient = 0;
  current = random(0, 6);
  paintT();
}

void updateButton(){
  btnAlt = btnAnw;
  btnAnw = digitalRead(Bpin_A);
  btnBlt = btnBnw;
  btnBnw = digitalRead(Bpin_B);
  btnClt = btnCnw;
  btnCnw = digitalRead(Bpin_C);
}

void removeRow(byte row){
  for(;row < 20; row++){
    for(byte x = 0; x < 10; x++){
      matrix[x][row] = matrix[x][row+1];
    }
  }
 
  for(byte x = 0; x < 10; x++){
    matrix[x][19] = 0;
  }
 
}

void removeCompletedRows(){
  for(byte i = noOfCompleteRows(); i > 0; i--){
    removeRow(completedRow());
    rowCom++;
  }
}

void paintT(){
  clrAct();
  for(byte i = 0; i < 4; i++){
    matrix[(location[0] + tetromino[current][orient][i][0])][(location[1] + tetromino[current][orient][i][1])] = (current + 1);
  }
}

boolean btmClr(){
  for(byte x = 0; x < 4; x++){
    if(matrix[(location[0] + tetromino[current][orient][x][0])][((location[1] + tetromino[current][orient][x][0])- 1)] != 0){
      return false;
    }
  }
  return true;
}

boolean rgtClr(){
  for(byte i = 0; i < 4; i++){
    if((location[0] + tetromino[current][orient][i][0]) == 9){
      return false;
    }
  }
  for(byte i = 0; i < 4; i++){
    if(matrix[((location[0] + tetromino[current][orient][i][0]) + 1)][(location[1] + tetromino[current][orient][i][1])] != 0){
      return false;
    }
  }
  return true;
}

void moveLeft(){
  if(lftClr()){
    location[0] = location[0] - 1;
  }
  paintT();
}

void moveRight(){
  if(rgtClr()){
    location[0] = location[0] + 1;
  }
  paintT();
}

byte noOfCompleteRows(){
  byte incompleteRows = 0;
 
  for(byte y = 0; y < 20; y++){
    for(byte x = 0; x < 10; x++){
      if(matrix[x][y] == 0){
        incompleteRows++;
        x = 10;
      }
    }
  }
 
  return (20 - incompleteRows);
 
}

char completedRow(){
  for(char y = 0; y < 20; y++){
    for(char x = 0; x < 10; x++){
      if(matrix[x][y] == 0){
        x = 10;
      }
      if(x == 9){
        return y;
      }
    }
  }
 
  return -1;
 
}

void moveDown(){
  if(btmClr()){
    clrAct();
    location[1] = location[1] - 1;
  } else {
    freeze();
  }
}

boolean lftClr(){
  for(byte i = 0; i < 4; i++){
    if((location[0] + tetromino[current][orient][i][0]) == 0){
      return false;
    }
  }
  for(byte i = 0; i < 4; i++){
    if(matrix[((location[0] + tetromino[current][orient][i][0]) - 1)][(location[1] + tetromino[current][orient][i][1])] != 0){
      return false;
    }
  }
  return true;
}

void clrAct(){
  for(byte y = 0; y < 20; y++){
    for(byte x = 0; x < 10; x++){
      if(matrix[x][y] < 8){
        matrix[x][y] = 0;
      }
    }
  }
}

void freeze(){
  for(byte i = 0; i < 4; i++){
    matrix[(location[0] + tetromino[current][orient][i][0])][(location[1] + tetromino[current][orient][i][1])] = (current + 8);
  }
  active = false;
  removeCompletedRows();
}

void setup(){
  randomSeed(analogRead(A0));
  pinMode(Bpin_A, INPUT);
  pinMode(Bpin_B, INPUT);
  pinMode(Bpin_C, INPUT);
  do{
    if(!active){
      spawn();
    }
    updateButton();
    if(btnAlt && !btnAnw){
      moveLeft();
    } else if(btnBlt && !btnBnw){
      moveRight();
    } else if(btnClt && !btnCnw){
      //change delay NEED WORK!!!!!
    }
   
    if(frames == 0){
      moveDown();
    }
   
    delay(1);
    frames = frames + 1;
    frames = frames % 1000;
  }while(contlp);
  contlp = true;
}

void loop(){}
