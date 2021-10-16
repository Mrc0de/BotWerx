#include <heltec.h>
#include "xbmLogos.h"
#include <Adafruit_PWMServoDriver.h>
// Lora (dont enable without antenna attached)
#define LoraBAND 868E6  //you can set band here directly,e.g. 868E6,915E6
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)

// redefine SDA and SCL (32,33)
// Arduino\hardware\arduino\avr\variants\mega\pins_arduino.h
const uint8_t SDAPinServosOnly = 32;
const uint8_t SCLPinServosOnly = 33;
Adafruit_PWMServoDriver pwm;
// FRONT
const uint8_t FrontRightShoulderA = 0;
const uint8_t FrontRightShoulderB = 1;
const uint8_t FrontLeftShoulderA = 2;
const uint8_t FrontLeftShoulderB = 3;
// REAR
const uint8_t RearRightShoulderA = 4;
const uint8_t RearRightShoulderB = 5;
const uint8_t RearLeftShoulderA = 6;
const uint8_t RearLeftShoulderB = 7;

// Model Cooper has 8 MG90S servos
const char BotModel[] = "Cooper-1";
const char LastCmd[] = "WALK";
uint8_t tick = 0;

void setup() {
    // Display and LoRa
    Heltec.begin(true /*DisplayEnable Enable*/,false /*LoRa Enable*/,true /*Serial Enable*/, false /*LoRa use PABOOST*/, LoraBAND /*LoRa RF working band*/);
    delay(250);
    // Movement
    InitMovementSystems();
    Heltec.display -> clear();
    Heltec.display -> drawString(8, 32, "[ SERVO ON ]");
    Heltec.display -> display();
    shortSplash();
}

void loop() {
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "[ TEST MODE ]");
    Heltec.display -> drawString(8, 24, "[ Model "+String(BotModel)+" ]");
    Heltec.display -> display();
    walk(10, 1);
    delay(2000);
}


void shortSplash(){
    // Heltec WiFi LoRa 32 V2
    // 128*64 display
    Heltec.display -> clear();
    Heltec.display -> drawString(8, 24, "[ Model "+String(BotModel)+" Starting ]");
    Heltec.display -> display();
    delay(150);
    Heltec.display -> clear();
    Heltec.display -> setColor(WHITE);
    Heltec.display -> drawXbm(0,0,xbmLogo1_width,xbmLogo1_height,xbmLogo1);
    Heltec.display -> display();
    delay(750);
}

void InitMovementSystems(){
  // Pick up Wire for i2c Pins (Master)
  // before using it, replace it with a proper initialized one, correct pins, custom wire
  Wire1.begin(32,33); // I changed them in pins_arduino for heltec model v2
  pwm = Adafruit_PWMServoDriver(0x40,Wire1);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(60);
  delay(10);
  // its aliiiiive
}

// movement presets
void walk(int iter,int speedMultiplier) {
    int speedDelay=100*speedMultiplier;
    for(int l=0;l<iter;l++){

    }
}

// FR
void FrontRightStep(int msDelay) {
    // Leg Comes up 20deg
    uint16_t pulseTarget = map(20,0,180,SERVOMIN,SERVOMAX);
    uint16_t lastTarget = map(0,0,180,SERVOMIN,SERVOMAX);
    for (uint16_t pulselen = lastTarget; pulselen < pulseTarget; pulselen++) {
        pwm.setPWM(FrontRightShoulderB, 0, pulselen);
    }
    delay(msDelay);
}

