
#include <Servo.h> 
#include "commands.h"

#define EYE_CENTER_L 20
#define EYE_CENTER_R 20
#define EYE_MAX_OFFS 20

#define MOUTH_CLOSED 8
#define MOUTH_OPEN 45

#define MOUTH_PIN 6
#define EYE_PIN_L 5
#define EYE_PIN_R 9
#define NECK_PIN 4

#define RED_PIN A3
#define GREEN_PIN A2
#define BLUE_PIN A1

#define NECK_DEFAULT 90

Servo eyeL; 
Servo eyeR;
Servo mouth;
Servo neck;
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
  
  neck.attach(NECK_PIN);
  eyeL.attach(EYE_PIN_L);
  eyeR.attach(EYE_PIN_R);
  mouth.attach(MOUTH_PIN);
  
  neck.write(NECK_DEFAULT);
  eyeL.write(EYE_CENTER_L);
  eyeR.write(EYE_CENTER_R);
  mouth.write(MOUTH_CLOSED);
  
  setup_RF("extra");
  
  
} 

void look_to(int angle) {
  // -90 is full left, 90 is full right
  int pos_l = map(angle, -90, 90, EYE_CENTER_L-EYE_MAX_OFFS, EYE_CENTER_L+EYE_MAX_OFFS); 
  int pos_r = map(angle, -90, 90, EYE_CENTER_R-EYE_MAX_OFFS, EYE_CENTER_R+EYE_MAX_OFFS);
  
  eyeL.write(pos_l);
  eyeR.write(pos_r);
}
 
 
void mouthact() {
  mouth.write(MOUTH_OPEN);
  delay(100);
  mouth.write(MOUTH_CLOSED);
  delay(100); 
}
 
void rf_loop() {
  rf_wait_for_cmd();
  switch (rf_cmd_type()) {
   case CMD_FET1:
      Serial.print("FET1 ");
      Serial.println(rf_cmd_val());
      look_to(rf_cmd_val() - 128);
      break;
    case CMD_FET2:
      Serial.print("FET2 ");
      Serial.println(rf_cmd_val());
      neck.write(NECK_DEFAULT + (rf_cmd_val() - 128));
      break;
    case CMD_FET3:
      Serial.print("FET3 ");
      Serial.println(rf_cmd_val());
      mouth.write(rf_cmd_val() ? MOUTH_OPEN : MOUTH_CLOSED);
      break;
    case CMD_RELAY:
      Serial.print("RELAY ");
      Serial.println(rf_cmd_val());
      digitalWrite(RED_PIN, !digitalRead(RED_PIN));
      break;
    default:
      Serial.println("INVALID_CMD"); 
  }
}
 
void eyeColor(int r) {
  digitalWrite(RED_PIN, r != 0);
  digitalWrite(GREEN_PIN, r != 1);
  digitalWrite(BLUE_PIN, r != 2);
}
 
#define LONG_DELAY 1000
void loop() { 
  
  int z = random(10);
  if (z < 5) { //Change eye color and move eyes
    int r = random(3);
    eyeColor(0);
    
    look_to(random(180) - 90);
  } else {
    z = random(10);
    if (z < 1) {
      eyeColor(random(2) + 1);
      look_to(random(180) - 90);
    }
  }
  
  if (random(20) == 1) {
    neck.write(NECK_DEFAULT + random(100) - 50); 
  }
  
  if (random(50) == 1) {
    for (int i = 0; i < 5; i++) {
      mouthact();
    }  
  }
  
  delay(LONG_DELAY);
}
  /*
  neck.write(NECK_DEFAULT - 70);
  look_to(-90);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(RED_PIN, LOW);
  delay(1000);
  
  neck.write(NECK_DEFAULT + 70);
  look_to(90);
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
  delay(1000);
  
  neck.write(NECK_DEFAULT);
  look_to(0);
  digitalWrite(BLUE_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  delay(1000);
  
  for (int i = 0; i < 5; i++) {
    mouthact();
  }  
  */

