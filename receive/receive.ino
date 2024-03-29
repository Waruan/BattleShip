

#include <Wire.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
// If your matrix has the DOUBLE HEADER input, use:
//#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
//#define LAT 9
//#define OE  10
//#define A   A3
//#define B   A2
//#define C   A1
//#define D   A0
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);



char input[6]; //Data from controller
int gameMode = -1; //Save the mode Default at -1 so that in not in any mode
int numberOfPlayer = 1;//Save number of player Default at 1 player 
int ships[10]={2,3,3,4,5};

int whichShip = 0;


int whichShip2 = 0;

int cursorX = 0;// X-axis of Cursor
int cursorY = 0;// Y-axis of Cursor

int cursorX2 = 0;// X-axis of Cursor
int cursorY2 = 0;// Y-axis of Cursor

int mcursorX = 0;// X-axis of Cursor
int mcursorY = 0;// Y-axis of Cursor

int mcursorX2 = 0;// X-axis of Cursor
int mcursorY2 = 0;// Y-axis of Cursor


int lastLocation = 0;
int lastLocation2 = 0;

int mlastLocation = 0;
int mlastLocation2 = 0;

bool rotation1 = false;
bool rotation2 = false;
int shipMatrix[10][10] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


int shipMatrix2[10][10] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int mMatrix1[10][10] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


int mMatrix2[10][10] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void setup() {
  input[5] = '\0';
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);  // start serial for output
  
  matrix.begin();

}

void loop() {

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()


void receiveEvent(int howMany)
{
  //Serial.println("Recieve");
  // loop through all but the last
  int num = 0;
  while(Wire.available() && num < 6) 
  {
     // receive byte as an integer
    char c = Wire.read();
    input[num] = c;
    num++;    
    //Serial.println(x);  // print the integer
    
  }
  
  

  // Number of Player Selection
  if(input[0] == '0'){
    if(gameMode != 0){
      modeSelectLayout();
      gameMode = 0;
    }
    if(input[1] == '1'){
      numberOfPlayers(1);
    }
    else if(input[1] == '2'){
      numberOfPlayers(2);
    }
  }

  // Player 1 ship Placement
  if(input[0] == '1'){
    if(gameMode != 1){
      board();
      gameMode = 1;
    }
    else{
      placeShip(1);
    }
  }
  if(input[0] == '2'){
    if(gameMode != 1){
      board();
      gameMode = 1;
    }
    else{
      placeShip(2);
    }
  }

  if(input[0] == '3'){
    placeMissile(1);
  }
  if(input[0] == '4'){
    placeMissile(2);
  }

  if(input[0] == '5'){
    winnerDisplay();
  }

  
}

void winnerDisplay(){
  if(input[4] = '8'){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  
    matrix.setCursor(3, 0);    // start at top left, with one pixel of spacing
    matrix.setTextSize(1);     // size 1 == 8 pixels high
    matrix.setTextWrap(true); // Don't wrap at end of line - will do ourselves

    matrix.setTextColor(matrix.Color333(0,7,0));
    matrix.println("Winner Player 1");
 
  }
  else if(input[4] = '9'){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
  
    matrix.setCursor(3, 0);    // start at top left, with one pixel of spacing
    matrix.setTextSize(1);     // size 1 == 8 pixels high
    matrix.setTextWrap(true); // Don't wrap at end of line - will do ourselves

    matrix.setTextColor(matrix.Color333(0,7,0));
    matrix.println("Winner Player 2");
 
  }
}

void placeMissile(int player){
  if(player == 1){
    int xdirection = mcursorX;
    int ydirection = mcursorY;
    //matrix.drawRect(18,18,12,12, matrix.Color333(0, 7, 7));
    if(mlastLocation == 0){
      matrix.drawPixel((19+mcursorX),(19+mcursorY),matrix.Color333(0,0,0)); 
      mMatrix1[mcursorX][mcursorY] = 0;
    }
    else if(mlastLocation == 2){
      matrix.drawPixel((19+mcursorX),(19+mcursorY),matrix.Color333(7,0,0)); 
      mMatrix1[mcursorX][mcursorY] = 2;
    }
    else if(mlastLocation == 3){
      matrix.drawPixel((19+mcursorX),(19+mcursorY),matrix.Color333(3,3,3)); 
      mMatrix1[mcursorX][mcursorY] = 3;
    }
    if(input[1] == '1'){
      if(mcursorX < 9){
        mcursorX++;
      }
    }
    else if (input[1] == '2'){
      if(mcursorX > 0){
        mcursorX--;
      }
    }

    if(input[2] == '1'){
      if(mcursorY < 9){
        mcursorY++;
      }
    }
    else if (input[2] == '2'){
      if(mcursorY > 0){
        mcursorY--;
      }
    }

    mMatrix1[xdirection][ydirection]= mlastLocation;
    mlastLocation = mMatrix1[mcursorX][mcursorY];
    matrix.drawPixel((19+mcursorX),(19+mcursorY),matrix.Color333(7,7,7));
    mMatrix1[mcursorX][mcursorY] = 1;
    //hit
    if(input[4] == '4'){
      matrix.drawPixel((19+mcursorX),(19+mcursorY),matrix.Color333(7,0,0));
      mMatrix1[mcursorX][mcursorY] = 2;
      mlastLocation = 2;
    }
    //miss
   
    if(input[4] == '5'){
      matrix.drawPixel((19+mcursorX),(19+mcursorY),matrix.Color333(3,3,3));
      mMatrix1[mcursorX][mcursorY] = 3;
      mlastLocation = 3;
    }
  }
  
  if(player == 2){
    int xdirection = mcursorX2;
    int ydirection = mcursorY2;
    // matrix.drawRect(18,2,12,12, matrix.Color333(7, 7, 0));
    if(mlastLocation2 == 0){
      matrix.drawPixel((19+mcursorX2),(3+mcursorY2),matrix.Color333(0,0,0)); 
      mMatrix2[mcursorX2][mcursorY2] = 0;
    }
    else if(mlastLocation2 == 2){
      matrix.drawPixel((19+mcursorX2),(3+mcursorY2),matrix.Color333(7,0,0)); 
      mMatrix2[mcursorX2][mcursorY2] = 2;
    }
    else if(mlastLocation2 == 3){
      matrix.drawPixel((19+mcursorX2),(3+mcursorY2),matrix.Color333(3,3,3)); 
      mMatrix2[mcursorX2][mcursorY2] = 3;
    }
    if(input[1] == '1'){
      if(mcursorX2 < 9){
        mcursorX2++;
      }
    }
    else if (input[1] == '2'){
      if(mcursorX2 > 0){
        mcursorX2--;
      }
    }

    if(input[2] == '1'){
      if(mcursorY2 < 9){
        mcursorY2++;
      }
    }
    else if (input[2] == '2'){
      if(mcursorY2 > 0){
        mcursorY2--;
      }
    }

    mMatrix2[xdirection][ydirection]= mlastLocation2;
    mlastLocation2 = mMatrix2[mcursorX2][mcursorY2];
    matrix.drawPixel((19+mcursorX2),(3+mcursorY2),matrix.Color333(7,7,7));
    mMatrix2[mcursorX2][mcursorY2] = 1;
    

    if(input[4] == '4'){
      matrix.drawPixel((19+mcursorX2),(3+mcursorY2),matrix.Color333(7,0,0));
      mMatrix2[mcursorX2][mcursorY2] = 2;
      mlastLocation2 = 2;
    }
    if(input[4] == '5'){
      matrix.drawPixel((19+mcursorX2),(3+mcursorY2),matrix.Color333(3,3,3));
      mMatrix2[mcursorX2][mcursorY2] = 3;
      mlastLocation2 = 3;
    }
    
  }
}
void placeShip(int player){
  /*Serial.println(input[4]);
  if(input[4] == '2'){
    matrix.drawPixel((3+cursorX),(19+cursorY),matrix.Color333(0,7,0));      
  }
   else if(input[4] == '1'){
    matrix.drawPixel((3+cursorX),(19+cursorY),matrix.Color333(0,7,0)); 
  }
  else if(input[4] == '0'){
    matrix.drawPixel((3+cursorX),(19+cursorY),matrix.Color333(0,0,0)); 
  }*/
  if(player == 1){
  int xdirection = cursorX;
  int ydirection = cursorY;
  
  if(lastLocation == 0){
    matrix.drawPixel((3+cursorX),(19+cursorY),matrix.Color333(0,0,0)); 
    shipMatrix[cursorX][cursorY] = 0;
  }
  else if(lastLocation == 2){
    matrix.drawPixel((3+cursorX),(19+cursorY),matrix.Color333(0,7,0)); 
    shipMatrix[cursorX][cursorY] = 2;
  }
    if(input[1] == '1'){
      if(cursorX < 9){
        cursorX++;
      }
    }
    else if (input[1] == '2'){
      if(cursorX > 0){
        cursorX--;
      }
    }

    if(input[2] == '1'){
      if(cursorY < 9){
        cursorY++;
      }
    }
    else if (input[2] == '2'){
      if(cursorY > 0){
        cursorY--;
      }
    }

    shipMatrix[xdirection][ydirection]= lastLocation;
    lastLocation = shipMatrix[cursorX][cursorY];
     matrix.drawPixel((3+cursorX),(19+cursorY),matrix.Color333(7,7,7));
     shipMatrix[cursorX][cursorY] = 1;
    if(input[3] == '2'){
      rotation1 = !rotation1;
    }
    if(input[4] == '1'){
      if(whichShip < 5){
        if(rotation1 == false){
          for(int i=0;i<ships[whichShip];i++){
            matrix.drawPixel((3+cursorX+i),(19+cursorY),matrix.Color333(0,7,0));
            shipMatrix[cursorX+i][cursorY] = 2;
            lastLocation = 2;
          }
        }
        else if(rotation1 == true){
          for(int i=0;i<ships[whichShip];i++){
            matrix.drawPixel((3+cursorX),(19+cursorY+i),matrix.Color333(0,7,0));
            shipMatrix[cursorX][cursorY+i] = 2;
            lastLocation = 2;
          }
        }
        whichShip++;
      }
    }
    
  }


  if(player == 2){
  int xdirection = cursorX2;
  int ydirection = cursorY2;
  //matrix.drawRect(2,2,12,12, matrix.Color333(7, 0, 0));
  if(lastLocation2 == 0){
    matrix.drawPixel((3+cursorX2),(3+cursorY2),matrix.Color333(0,0,0)); 
    shipMatrix2[cursorX2][cursorY2] = 0;
  }
  else if(lastLocation2 == 2){
    matrix.drawPixel((3+cursorX2),(3+cursorY2),matrix.Color333(0,7,0)); 
    shipMatrix2[cursorX2][cursorY2] = 2;
  }
    if(input[1] == '1'){
      if(cursorX2 < 9){
        cursorX2++;
      }
    }
    else if (input[1] == '2'){
      if(cursorX2 > 0){
        cursorX2--;
      }
    }

    if(input[2] == '1'){
      if(cursorY2 < 9){
        cursorY2++;
      }
    }
    else if (input[2] == '2'){
      if(cursorY2 > 0){
        cursorY2--;
      }
    }

    shipMatrix2[xdirection][ydirection]= lastLocation2;
    lastLocation2 = shipMatrix2[cursorX2][cursorY2];
     matrix.drawPixel((3+cursorX2),(3+cursorY2),matrix.Color333(7,7,7));
     shipMatrix2[cursorX2][cursorY2] = 1;
    if(input[3] == '2'){
      rotation2 = !rotation2;
    }
    if(input[4] == '1'){
      if(whichShip2 < 5){
        if(rotation2 == false){
          for(int i=0;i<ships[whichShip2];i++){
            matrix.drawPixel((3+cursorX2+i),(3+cursorY2),matrix.Color333(0,7,0));
            shipMatrix2[cursorX2+i][cursorY2] = 2;
            lastLocation2 = 2;
          }
        }
        else if(rotation2 == true){
          for(int i=0;i<ships[whichShip2];i++){
            matrix.drawPixel((3+cursorX2),(3+cursorY2+i),matrix.Color333(0,7,0));
            shipMatrix2[cursorX2][cursorY2+i] = 2;
            lastLocation2 = 2;
          }
        }
        whichShip2++;
      }
    }
    
  }
}



void printCont(){
  Serial.print("Switch:  ");
  Serial.print(input[3]);
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(input[1]);
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(input[2]);
  Serial.print("Mode: ");
  Serial.println(input[0]);
  Serial.print("Extra: ");
  Serial.println(input[4]);
  Serial.print("\n\n");
  
}
void board(){
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  matrix.drawRect(0, 16, 32, 16, matrix.Color333(0, 0, 7));
  matrix.drawRect(0,0, 32, 16, matrix.Color333(7, 0, 0));
  
  //matrix.drawRect(2,2,12,12, matrix.Color333(7, 0, 0));

  matrix.drawRect(2,2,12,12, matrix.Color333(7, 0, 0));
  matrix.drawRect(18,2,12,12, matrix.Color333(7, 7, 0));
  matrix.drawRect(2,18,12,12, matrix.Color333(0, 0, 7));
  matrix.drawRect(18,18,12,12, matrix.Color333(0, 7, 7));
}

void modeSelectLayout(){
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  matrix.setCursor(3, 0);    // start at top left, with one pixel of spacing
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(true); // Don't wrap at end of line - will do ourselves

  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.println("MODE");
  matrix.println();
  matrix.println(" 1 2 ");
  //matrix.drawRect(5,15,9,9, matrix.Color333(7, 0, 0));
}
void modeWord(){
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  matrix.setCursor(3, 0);    // start at top left, with one pixel of spacing
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(true); // Don't wrap at end of line - will do ourselves

  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.println("MODE");
  matrix.println();
  matrix.println(" 1 2 ");
}

//error temp fix
void numberOfPlayers(int mode){
  if(mode == 1 && numberOfPlayer == 1){
    matrix.drawRect(5,15,9,9, matrix.Color333(0, 0, 0));
    matrix.drawRect(16,15,9,9, matrix.Color333(7, 0, 0));
    numberOfPlayer = 2;
  }
  //box over 2
  else if (mode == 2 && numberOfPlayer == 2){
    matrix.drawRect(16,15,9,9, matrix.Color333(0, 0, 0));
    matrix.drawRect(5,15,9,9, matrix.Color333(0, 7, 0));
    numberOfPlayer = 1;
  }
}




