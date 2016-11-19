//https://brainy-bits.com/tutorials/arduino-joystick-tutorial/
#include <Wire.h>

const int SW_pin = 2; // digital pin connected to switch output

const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output


const int SW_pin2 = 3; // digital pin connected to switch output

const int X_pin2 = 2; // analog pin connected to X output
const int Y_pin2 = 3; // analog pin connected to Y output

char input[5];
int numPlayer = 0;
//2d input 10x10 for each grid
int cursorLocation[2] = {0, 0};
int cursorLastValue= 0;
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


void setup() {
  
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  
  pinMode(SW_pin2, INPUT);
  digitalWrite(SW_pin2, HIGH);
  
  Serial.begin(115200);
  Wire.begin();        // join i2c bus (address optional for master)
}

void loop() {
  //select 1 or 2 player save in a int
  input[0] = '0';
  modeSelect();
  input[0] = '1';
  if (numPlayer == 1) {
    shipmentplacement(numPlayer);
  }
  else {
    shipmentplacement(numPlayer);
    shipmentplacement(numPlayer);
  }
  
  delay(500);

}

/*
 * 
 * input[0] = mode
 * 0 = player select
 * 1 = Player 1 ship placement
 * 2 = Player 2 ship placement
 * 3 = Player 1 missile location
 * 4 = Player 2 missile location
 * 
 */
 
/*
 * input[1]= x-axis
 * 0 center 
 * 1 right
 * 2 left
 * 
 */

/*
 * input[2]= y-axis
 * 0 center
 * 1 down
 * 2 up
 * 
 */
 
/*
 * input[3]= button
 * 1 not press
 * 0 press
 */
 
//input[4]= end of string char

void modeSelect() {
  int button = HIGH;
  while (button == HIGH || numPlayer == 0) {
    input[4] = '\0';
    input[2] = '0';
    int x = analogRead(X_pin);
    //Serial.println(analogRead(X_pin2));
    // right
    if (x > 600) {
      input[1] = '1';
      numPlayer = 1;
    }
    //left
    else if (x < 500) {
      input[1] = '2';
      numPlayer = 2;
    }
    else {
      input[1] = '0';
    }

    
    button = digitalRead(SW_pin);
    if (button == HIGH) {
      input[3] = '0';
    }
    else if (button == LOW) {
      input[3] = '1';
    }
    printCont();
    Wire.beginTransmission(8); // transmit to device #4
    Wire.write(input);       // write five bytes
  
    Wire.endTransmission();    // stop transmitting
    //Serial.println(input);
  }
}


void movement() {
  // not click

  int xdirection = cursorLocation[0];
  int ydirection = cursorLocation[1];

  int button = HIGH;
  //input[0] = '1';
  int x = analogRead(X_pin);

  //move right
  if (x > 600) {
    input[1] = '1';
    if ( cursorLocation[0] < 9) {
      cursorLocation[0]++;
    }
  }

  //move left
  else if (x < 500) {
    input[1] = '2';
    if ( cursorLocation[0] > 0) {
      cursorLocation[0]--;
    }
  }
  // dont move
  else {
    input[1] = '0';
  }

  delay(100);
  int y = analogRead(Y_pin);

  // move down
  if (y > 600) {
    input[2] = '1';
    if ( cursorLocation[1] < 9) {
      cursorLocation[1]++;
    }
  }

  //move up
  else if (y < 500) {
    input[2] = '2';
    if ( cursorLocation[1] > 0) {
      cursorLocation[1]--;
    }
  }

  // dont move
  else {
    input[2] = '0';
  }

  delay(100);

  button = digitalRead(SW_pin);
  if (button == HIGH) {
    input[3] = '1';
  }
  else if (button == LOW) {
    input[3] = '0';
  }
    
  shipMatrix[xdirection][ydirection]= cursorLastValue;
  cursorLastValue = shipMatrix[cursorLocation[0]][cursorLocation[1]];
  shipMatrix[cursorLocation[0]][cursorLocation[1]] = 1;
  //sendinput();
  printboard();
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(input);       // write five bytes
  Wire.endTransmission();    // stop transmitting
}



void shipmentplacement(int player) {
  //Serial.print("Number of Player ");
  //Serial.println(player);
  //printboard();
  int ships[10]={2,2,2,2,3,3,3,4,4,6};
  int shipPlaced = 0;
  while (shipPlaced < 10) {
    movement();
    if(input[3] == '0'){
      if(collisionCheck(cursorLocation[0],cursorLocation[1],ships[shipPlaced],false) != false){
        for(int i =0;i<ships[shipPlaced];i++){
          shipMatrix[cursorLocation[0]+i][cursorLocation[1]] = 2;
        }
        cursorLastValue = 2;
        shipPlaced++;
      }
      
    }
  }
}
// rotation 0 == vertial
// rotation 1 == horziontal
bool collisionCheck(int i,int j,int shipSize,bool rotation){
  // check x-axis boundary
  if(rotation == false){
    if(i+shipSize > 9){
      return false;
    }
    //check collision with other ship
    for(int bound = 0;bound<shipSize;bound++){
      if(shipMatrix[i+bound][j] == 2){
        return false; 
      }
    }
  }
  if(rotation == true){
    if(j+shipSize > 9){
      return false;
    }
    //check collision with other ship
    for(int bound = 0;bound<shipSize;bound++){
      if(shipMatrix[i][j+bound] == 2){
        return false; 
      }
    }
  }

 return true;
  
}
void sendinput() {
  //Serial.println(input);
  
}


//use without mega
void printboard() {
  for (int i = 0; i < 10; i++) {
    for (int y = 0; y < 10; y++) {
      Serial.print(shipMatrix[y][i]);
    }
    Serial.println();
  }
  Serial.println("-------------------------------");
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
  
  Serial.print("\n\n");
  delay(300);
}