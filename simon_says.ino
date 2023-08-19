/*
Simon Says by Tomasz Luber

Arduino to D-Pad (4 switches):
6,7,8,9 - up, down, left, right - GND

Arduino to Start button:
10 - switch - GND

Arduino to LCD:
12 - 4
11 - 6
5,4,3,2 - 11,12,13,14
+5v - 220 ohm resistor - 15
+5v - 2
GND - 1
GND - 5
GND - 16

Arduino to LED:
A0,A1,A2,A3 - 220 ohm resistor - LED - GND

Arduino to wait LED:
A4 - 220 ohm resistor - LED - GND

Arduino to listen LED:
13 - 220 ohm resistor - LED - GND

Contrast potentiometer to LCD:
+5v - pot1
GND - pot3
pot2 - lcd3

*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
long r;
String seq = "";
String check = "";
int score = 0;
int start = 0;
int speed;
String bottomRow = "";
String udlr = "UDLR";

void setup() {
  lcd.begin(16, 2);

  pinMode(6, INPUT_PULLUP);  //switches
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  pinMode(10, INPUT_PULLUP);  //start button

  pinMode(A6, INPUT); //speed pot in

  pinMode(A0, OUTPUT);  //LED pins
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A4, OUTPUT);  //wait LED
  pinMode(13, OUTPUT);  //listen LED

  randomSeed(analogRead(A5));

  lcd.setCursor(0, 0);
  lcd.write("                ");
  lcd.setCursor(0, 0);
  lcd.write(" press to start ");
  lcd.setCursor(0, 1);
  lcd.write("                ");
  lcd.setCursor(0, 1);
  lcd.write("  SIMON   SAYS  ");
}

void loop() {
  if (start == 1) {
    wScore();
    wSpeed();
    ledSeq();
    for (int i = 0; i < seq.length(); i++) {
      listen();
      checker();
    }
    check = "";
    if (seq.length() > 0) {
      score += 1;
    }
  } else {
    while (start == 0) {
      isStart();
    }
  }
}

void isStart() {
  if (digitalRead(10) == LOW) {
    start = 1;
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.write("                ");
    lcd.setCursor(0, 0);
    lcd.write("Score:  Speed:  ");
  }
}

void ledSeq() {
  r = random(4);
  seq += udlr[r];
  for (int i = 0; i < seq.length(); i++) {
    if (seq[i] == 'U') {
      digitalWrite(A0, HIGH);
      delay(speed);
      digitalWrite(A0, LOW);
      delay(speed);
    } else if (seq[i] == 'D') {
      digitalWrite(A1, HIGH);
      delay(speed);
      digitalWrite(A1, LOW);
      delay(speed);
    } else if (seq[i] == 'L') {
      digitalWrite(A2, HIGH);
      delay(speed);
      digitalWrite(A2, LOW);
      delay(speed);
    } else if (seq[i] == 'R') {
      digitalWrite(A3, HIGH);
      delay(speed);
      digitalWrite(A3, LOW);
      delay(speed);
    } else {
      reset();
    }
  }
}

void listen() {
  int upButton = digitalRead(6);
  int downButton = digitalRead(7);
  int leftButton = digitalRead(8);
  int rightButton = digitalRead(9);
  digitalWrite(13, HIGH);
  if (((upButton == LOW) ^ (downButton == LOW) ^ (leftButton == LOW) ^ (rightButton == LOW)) && ((upButton == LOW) || (downButton == LOW) || (leftButton == LOW) || (rightButton == LOW))) {
    listenCheck();
  } else {
    listen();
  }
}

void listenCheck() { //checks which button is pressed and adds corresponding char to check sequence
  int upButton = digitalRead(6);
  int downButton = digitalRead(7);
  int leftButton = digitalRead(8);
  int rightButton = digitalRead(9);
  if (upButton == LOW) {
    check += "U";
  } else if (downButton == LOW) {
    check += "D";
  } else if (leftButton == LOW) {
    check += "L";
  } else if (rightButton == LOW) {
    check += "R";
  }
  digitalWrite(13, LOW);
  digitalWrite(A4, HIGH);
  delay(500);
  digitalWrite(A4, LOW);
}

void gameOver() {
  lcd.setCursor(0, 0);
  lcd.print("  GAME    OVER  ");
  lcd.setCursor(0, 1);
  lcd.print("       :(       ");
  delay(3000);
  reset();
}

void reset() {
  seq = "";
  check = "";
  score = 0;
  start = 0;
  setup();
}

void wScore() {
  lcd.setCursor(0, 1);
  lcd.print("        ");
  lcd.setCursor(0, 1);
  lcd.print(String(score));
}

void wSpeed() {
  int speedVal = analogRead(A6);
  if (speedVal <= 250) {
    speed = 250;
  } else if ((speedVal > 250)&&(speedVal <= 500)) {
    speed = 500;
  } else if ((speedVal > 500)&&(speedVal <= 750)) {
    speed = 750;
  } else if ((speedVal > 750)&&(speedVal <= 1023)) {
    speed = 1000;
  } else {
    speed = 500;
  }
  lcd.setCursor(8, 1);
  lcd.print("        ");
  lcd.setCursor(8, 1);
  lcd.print(speed);
}

void checker() {
  for (int i = 0; i < check.length(); i++) {
    if (check[i] != seq[i]) {
      gameOver();
    }
  }
}