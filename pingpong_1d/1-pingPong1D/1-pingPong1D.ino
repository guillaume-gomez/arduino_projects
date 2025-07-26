#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


byte ballChars[8] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
};

byte racketChar[8] = {
  B11111,
  B11111,
  B11011,
  B11011,
  B11011,
  B11011,
  B11111,
  B11111,
};


byte racketCharClicked[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

int leftSwitch = 8;
int rightSwitch = 9;

const int BALL_NUM = 0;
const int RACKET_NUM = 1;
int RACKET_CLICKED_NUM = 2; 

int indexBall = 6;
int direction = 1;

int player1Score = 0; 
int player2Score = 0; 

unsigned long previousTime = 0;
int interval = 250;

int gameState = 0; // 0 -> game start, 1 => game play, 2 => game over

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2); // set up number of columns and rows

  // init special characters
  lcd.createChar(BALL_NUM, ballChars);
  lcd.createChar(RACKET_NUM, racketChar);
  lcd.createChar(RACKET_CLICKED_NUM, racketCharClicked); 

  // connect the buttons
  pinMode(leftSwitch, INPUT);
  pinMode(rightSwitch, INPUT);
}


void loop() {
  switch(gameState) {
    case 0:
      gameStartLoop();
    break;
    case 1:
      gamePlayLoop();
    break;
    case 2:
      gameOverLoop();
    break;
  }
}

void renderRacket(int switchState) {
  if(switchState == 1)
    {
      lcd.write(byte(RACKET_NUM));
    }
    else
    {
      lcd.write(byte(RACKET_CLICKED_NUM));
    }
}

void computeBallPosition() {

}

void gamePlayLoop() {
  unsigned long currentTime = millis();
  int leftState = digitalRead(leftSwitch);
  int rightState = digitalRead(rightSwitch);
  
  if(currentTime - previousTime > interval  ) {
    previousTime = currentTime;  

    if(leftState == 1 && indexBall == 1)
    {
      direction = 1; 
    }
    
    else if(rightState == 1 && indexBall == 14 )
    {
      direction = -1; 
    }
  
    indexBall = indexBall + direction ;
    if(indexBall > 14 || indexBall < 1) 
    {
      if(indexBall > 14)
      {
        player1Score = player1Score + 1; 
      }
      else if(indexBall < 1)
      {
        player2Score = player2Score + 1; 
      }
      indexBall = 7; 
      direction = -1 * direction;
    }

    // render ball and rackets
    lcd.clear();
    lcd.setCursor(indexBall, 0);
    lcd.write(byte(BALL_NUM)); 

    lcd.setCursor(0, 0);
    renderRacket(leftState);
  
    lcd.setCursor(15, 0);
    renderRacket(rightState);

    //render scores
    lcd.setCursor(0, 1);
    lcd.print(player1Score);
    lcd.setCursor(14,1);
    lcd.print(player2Score); 
  }

  if(player1Score == 10 || player2Score == 10 ) {
    gameState = 2;
  }
}

void gameOverLoop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game over !");
  lcd.setCursor(0,1);
  if(player1Score > player2Score) {
    lcd.print("Player 1 wins");
  } else {
    lcd.print("Player 2 wins");
  }
 
  delay(3000);
  gameState = 0;
}

void gameStartLoop() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Welcome to ");
  lcd.setCursor(2,1);
  lcd.print("Ping Pong 1D"); 

  delay(3000);

  player1Score = player2Score = 0;
  gameState = 1;
}
