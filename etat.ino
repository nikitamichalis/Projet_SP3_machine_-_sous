#include <Servo.h>
Servo monServo;

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define MAX_NUM_TRIES 3

int PositionDuServo = 0;
const byte pinButton1 = 6;
const byte pinButton2 = 13;
const byte pinButton3 = 8;
int pinLampe = 9;
int ButtonValue1 = 0;
int ButtonValue2 = 0;
int ButtonValue3 = 0;
int nbRandom1;
int nbRandom2;
int nbRandom3;
bool ref1 = LOW, ref2 = LOW, ref3 = LOW;
int turn_counter = 0;

enum
{
  REPOS,
  ETAT_0,
  ETAT_1,
  ETAT_2,
  ETAT_3,
} etatCourant;

void mettreAuRepos()
{
  digitalWrite(pinLampe, LOW);
  ButtonValue1 = 0;
  ButtonValue2 = 0;
  ButtonValue3 = 0;
  etatCourant = REPOS;
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinButton1, INPUT_PULLUP);
  pinMode(pinButton2, INPUT_PULLUP);
  pinMode(pinButton3, INPUT_PULLUP);
  pinMode(pinLampe, OUTPUT);
  monServo.attach(10);
  monServo.write(PositionDuServo);
  randomSeed(analogRead(A0));
  nbRandom1 = 0;
  nbRandom2 = 0;
  nbRandom3 = 0;

  mettreAuRepos();
}

void loop()
{
  switch (etatCourant)
  {
  case REPOS:
    ButtonValue1 = digitalRead(pinButton1);
    if (digitalRead(pinButton1) != HIGH)
    {
      for (PositionDuServo = 15; PositionDuServo <= 180; PositionDuServo++)
      {
        monServo.write(PositionDuServo);
        delay(15);
      }
      for (PositionDuServo = 180; PositionDuServo >= 15; PositionDuServo--)
      {
        monServo.write(PositionDuServo);
        delay(15);
      }

      for (int i = 0; i < 1; i++)
      {
        nbRandom1 = random(1, 10);
        Serial.println(nbRandom1);
        delay(1000);
        lcd.print(nbRandom1);
      }
      etatCourant = ETAT_1;
    }
    break;

  case ETAT_1:
    ButtonValue2 = digitalRead(pinButton2);

    if (digitalRead(pinButton2) != HIGH)
    {
      for (int i = 0; i < 1; i++)
      {
        nbRandom2 = random(1, 10);
        Serial.println(nbRandom2);
        delay(1000);
        lcd.print(nbRandom2);
      }
      etatCourant = ETAT_2;
    }
    break;

  case ETAT_2:
    ButtonValue3 = digitalRead(pinButton3);
    if (digitalRead(pinButton3) != HIGH)
    {
      for (int i = 0; i < 1; i++)
      {
        nbRandom3 = random(1, 10);
        Serial.println(nbRandom3);
        delay(1000);
        lcd.print(nbRandom3);
        lcd.print(" ");
      }
      etatCourant = ETAT_3;
    }
    break;

  case ETAT_3:
    ref1 = nbRandom1;
    ref2 = nbRandom2;
    ref3 = nbRandom3;
    turn_counter++;
    if (ref1 != 0 && ref2 != 0 && ref3 != 0)
    { 
      if (nbRandom1 == nbRandom2 && nbRandom1 == nbRandom3)
      {
        lcd.setCursor(1, 2);
        lcd.print("WINNER");
        digitalWrite(pinLampe, HIGH);
        turn_counter =3;
      }
      else{
        if (turn_counter >= MAX_NUM_TRIES){
          lcd.setCursor(1, 2);
          lcd.print("GAME OVER");
          digitalWrite(pinLampe, HIGH);
        }
      }
    }
    if (turn_counter >= MAX_NUM_TRIES)
    {
      // 3rd turn => Game finished
      turn_counter=0;
      mettreAuRepos();
      etatCourant = ETAT_0;
    }
    else
    {
      // Means that you are in first or second turn
      ButtonValue1 = 0;
      ButtonValue2 = 0;
      ButtonValue3 = 0;
      etatCourant = REPOS;
    }
    break;
  case ETAT_0:
    // Waiting for all buttons to be pressed together
    while (!(digitalRead(pinButton1) != HIGH && digitalRead(pinButton1) != HIGH && digitalRead(pinButton3) != HIGH))
    {
      ;
    }
    etatCourant = REPOS;
  }
}
