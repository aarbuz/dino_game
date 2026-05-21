#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int joyYPin = A0;   
const int buzzerPin = 3;  

// --- BITMAPS ---
const unsigned char dino_run1[] PROGMEM = {
  0x03, 0xFE, 0x03, 0xFF, 0x03, 0xFF, 0x03, 0xFF, 0x03, 0xFC, 0x03, 0xF0, 0x07, 0xE0, 0x7F, 0xE0,
  0xFF, 0xF0, 0xFF, 0xF0, 0xFF, 0xE0, 0x7F, 0xE0, 0x3F, 0x80, 0x1E, 0x00, 0x08, 0x00, 0x0C, 0x00
};

const unsigned char dino_run2[] PROGMEM = {
  0x03, 0xFE, 0x03, 0xFF, 0x03, 0xFF, 0x03, 0xFF, 0x03, 0xFC, 0x03, 0xF0, 0x07, 0xE0, 0x7F, 0xE0,
  0xFF, 0xF0, 0xFF, 0xF0, 0xFF, 0xE0, 0x7F, 0xE0, 0x3F, 0x80, 0x1E, 0x00, 0x04, 0x00, 0x06, 0x00
};

const unsigned char dino_duck1[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x3F, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x7F, 0x80, 0x00, 0x3F, 0x80, 0x0F, 0xFF, 0x80, 0x3F, 0xFF, 
  0x80, 0x7F, 0xFF, 0x00, 0x3F, 0xFE, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00
};

const unsigned char dino_duck2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x3F, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x7F, 0x80, 0x00, 0x3F, 0x80, 0x0F, 0xFF, 0x80, 0x3F, 0xFF, 
  0x80, 0x7F, 0xFF, 0x00, 0x3F, 0xFE, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00
};

const unsigned char cactus[] PROGMEM = {
  0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x19, 0x80, 0x19, 0x80, 0x19, 0x8c, 0x19, 0x8c,
  0x19, 0x8c, 0x1f, 0x8c, 0x0f, 0x8c, 0x03, 0xfc, 0x01, 0xf8, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80
};

const unsigned char bird1[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x03, 0x80, 0x07, 0x80, 0x0F, 0xE0,
  0x3F, 0xF8, 0x7F, 0xFC, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bird2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0,
  0x3F, 0xF8, 0x7F, 0xFC, 0x0F, 0x80, 0x07, 0x80, 0x03, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00, 0x00
};

// --- GAME STATE VARIABLES ---
int dinoY = 38;           
float velocityY = 0;        
float gravity = 1.5;          
bool isJumping = false;
bool isDucking = false;
int animationFrame = 0; 

int obstacleX = 128;
int obstacleY = 38;       
int obstacleType = 0; 

int score = 0;
int highScore = 0;
bool isNewRecord = false; 
float gameSpeed = 5.0;
bool gameOver = false;
bool milestonePlayed = false;

int cloudX = 100;
int cloudY = 15;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  // Od razu po włączeniu wymuszamy odcięcie prądu od buzzera
  noTone(buzzerPin);
  digitalWrite(buzzerPin, LOW);
  
  if(!display.begin(0x3C, true)) for(;;); 
  
  randomSeed(analogRead(A1));
  display.setTextColor(SH110X_WHITE); 
  resetGame();
}

// Funkcja pomocnicza: bezpieczne granie dźwięku i odcięcie prądu
void playSafeTone(int freq, int duration) {
  tone(buzzerPin, freq, duration);
  // Pozwalamy programowi iść dalej, ale w głównej pętli loop będziemy gasić buzzer
}

void playJumpSound() { playSafeTone(1200, 30); }
void playDuckSound() { playSafeTone(400, 30); }
void playGameOverSound() { 
  tone(buzzerPin, 200, 150); delay(150); 
  tone(buzzerPin, 100, 300); delay(300);
  noTone(buzzerPin); 
  digitalWrite(buzzerPin, LOW); // Twarde odcięcie zasilania na ekranie śmierci
}
void playMilestoneSound() { 
  tone(buzzerPin, 1500, 100); delay(120); 
  tone(buzzerPin, 2000, 150); delay(150);
  noTone(buzzerPin);
  digitalWrite(buzzerPin, LOW);
}

void resetGame() {
  gameOver = false;
  isNewRecord = false;
  score = 0;
  obstacleX = 128;
  obstacleType = 0;
  gameSpeed = 4.0;
  dinoY = 38;
  velocityY = 0;
  
  // Zabezpieczenie przy restarcie
  noTone(buzzerPin);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  if (gameOver) {
    display.clearDisplay();
    display.drawBitmap(15, 38, dino_run1, 16, 16, SH110X_WHITE); 
    display.drawBitmap(obstacleX, obstacleY, (obstacleType==0)?cactus:bird1, 16, 16, SH110X_WHITE);
    display.drawLine(0, 54, 128, 54, SH110X_WHITE);
    
    display.fillRect(14, 5, 100, 42, SH110X_BLACK); 
    display.drawRect(14, 5, 100, 42, SH110X_WHITE); 
    display.setTextSize(1);
    display.setCursor(35, 10); display.print("GAME OVER");
    display.setCursor(40, 22); display.print("Wynik: "); display.print(score);
    
    if (isNewRecord) {
      display.setCursor(28, 34); 
      display.print("NOWY REKORD!");
    }

    display.display();

    while(analogRead(joyYPin) > 200 && analogRead(joyYPin) < 800) { delay(10); }
    resetGame();
    delay(200); 
    return;
  }

  if (score > highScore) highScore = score;

  if (score > 0 && score % 10 == 0) {
    if (!milestonePlayed) {
      playMilestoneSound();
      milestonePlayed = true;
      gameSpeed += 0.4; 
    }
  } else {
    milestonePlayed = false;
  }

  int joyVal = analogRead(joyYPin);
  
  if (joyVal < 200 && !isJumping && !isDucking) {
    velocityY = -10.5; 
    isJumping = true;
    playJumpSound();
  }
  
  if (joyVal > 800) {
    if (isJumping) {
      velocityY += 3.0; 
    } else {
      if (!isDucking) playDuckSound();
      isDucking = true;
      dinoY = 38; 
    }
  } else {
    isDucking = false;
  }

  if (isJumping) {
    velocityY += gravity;
    dinoY += velocityY;
    if (dinoY > 38) { 
      dinoY = 38;
      velocityY = 0;
      isJumping = false;
    }
  }

  obstacleX -= (int)gameSpeed;
  cloudX -= 1; 
  if (cloudX < -30) { cloudX = 128; cloudY = random(5, 20); }
  
  if (obstacleX < -20) {
    obstacleX = 128;
    score += 1; 
    
    int randOb = random(0, 10);
    if (randOb < 6) {
      obstacleType = 0; 
      obstacleY = 38;
    } else if (randOb < 8) {
      obstacleType = 1; 
      obstacleY = 22; 
    } else {
      obstacleType = 2; 
      obstacleY = 30;
    }
  }

  // --- HITBOXES ---
  int dinoLeft = 18;
  int dinoRight = isDucking ? 34 : 28; 
  int dinoTop = isDucking ? dinoY + 6 : dinoY; 
  int dinoBottom = dinoY + 16;

  int obLeft = obstacleX + 4;
  int obRight = obstacleX + 12;
  int obTop = obstacleY + 4;
  int obBottom = obstacleY + 14;

  if (dinoRight > obLeft && dinoLeft < obRight && dinoBottom > obTop && dinoTop < obBottom) {
    if (score > highScore && score > 0) {
      highScore = score;
      isNewRecord = true;
    }
    gameOver = true;
    playGameOverSound();
  }

  display.clearDisplay();
  
  display.drawLine(0, 54, 128, 54, SH110X_WHITE); 
  for(int i=0; i<5; i++) {
    int dotX = (128 - ((score*5 + i*30) % 128)); 
    display.drawPixel(dotX, 56 + (i%2), SH110X_WHITE);
  }

  display.fillCircle(cloudX, cloudY, 3, SH110X_WHITE);
  display.fillCircle(cloudX+4, cloudY-1, 4, SH110X_WHITE);
  display.fillCircle(cloudX+8, cloudY, 3, SH110X_WHITE);
  display.fillRect(cloudX, cloudY+1, 9, 3, SH110X_WHITE);

  animationFrame = (millis() / 150) % 2; 
  if (isDucking) {
    if (animationFrame == 0) display.drawBitmap(15, dinoY, dino_duck1, 24, 16, SH110X_WHITE);
    else display.drawBitmap(15, dinoY, dino_duck2, 24, 16, SH110X_WHITE);
  } else if (!isJumping && animationFrame == 0) {
    display.drawBitmap(15, dinoY, dino_run1, 16, 16, SH110X_WHITE);
  } else {
    display.drawBitmap(15, dinoY, dino_run2, 16, 16, SH110X_WHITE);
  }
  
  if (obstacleType == 0) {
    display.drawBitmap(obstacleX, obstacleY, cactus, 16, 16, SH110X_WHITE);
  } else {
    if (animationFrame == 0) display.drawBitmap(obstacleX, obstacleY, bird1, 16, 16, SH110X_WHITE);
    else display.drawBitmap(obstacleX, obstacleY, bird2, 16, 16, SH110X_WHITE);
  }

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("HI "); display.print(highScore);
  display.print("  "); display.print(score);

  display.display(); 
  

  digitalWrite(buzzerPin, HIGH); 
  delay(15); 
}