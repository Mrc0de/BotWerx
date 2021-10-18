#include <heltec.h>
#include "xbmLogos.h"
#include <Adafruit_PWMServoDriver.h>
// Lora (dont enable without antenna attached)
#define LoraBAND 868E6  //you can set band here directly,e.g. 868E6,915E6
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
Adafruit_PWMServoDriver pwm;
#define FRLeg 1
#define FLLeg 3
#define RRLeg 5
#define RLLeg 7

struct servo {
    String Name;
    uint8_t ID;
    int InitialOffSet;
    bool Inverse;
    int LastDegPos;
};

servo Servos[]={
    {"FRShoulder",0,0,false,0}, {"FRLeg",1,0,false,0},
    {"FLShoulder",2,70,true,0}, {"FLLeg",3,85,true,0},
    {"RRShoulder",4,0,false,0}, {"RRLeg",5,70,false,0},
    {"RLShoulder",6,90,true,0}, {"RLLeg",7,45,true,0},
}; // these are dependent on how well you calibrate the physical servo horns to zero, mine was sloppy

// Model Cooper has 8 MG90S servos
const char BotModel[] = "Cooper-1";
const char LastCmd[] = "WALK";
uint8_t tick = 0;

void setup() {
    // Display and LoRa
    Heltec.begin(true /*DisplayEnable Enable*/,false /*LoRa Enable*/,true /*Serial Enable*/, false /*LoRa use PABOOST*/, LoraBAND /*LoRa RF working band*/);
    delay(250);
    // Setup Servo Handling
    // Start Servos
    InitMovementSystems();
    Heltec.display -> clear();
    // reset all servos
    for (int n=0;n<8;n++) {
        if (!Servos[n].Inverse) {
            // not inverse
            Heltec.display -> drawString(n, n*7,  Servos[n].Name);
            Heltec.display -> display();
            ResetMax2Min(Servos[n]);
        } else {
            // inverse
            Heltec.display -> drawString(n, n*7,  "Inverse");
            Heltec.display -> display();
            ResetMin2Max(Servos[n]);
        }
    };
    delay(5000);
    // how splash
    shortSplash();
}

void loop() {
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "[ TEST MODE ]");
    Heltec.display -> drawString(8, 24, "[ Model "+String(BotModel)+" ]");
    Heltec.display -> display();
    Heltec.display -> clear();
    Heltec.display -> setColor(WHITE);
    Heltec.display -> drawXbm(0,0,xbmLogo1_width,xbmLogo1_height,xbmLogo1);
    Heltec.display -> display();
    walk(10,1);
}

// Movement Functions
void ResetMax2Min(servo s){
   uint16_t pulseTarget = map(5+s.InitialOffSet,0,180,SERVOMIN,SERVOMAX);
    for (uint16_t pulselen = SERVOMAX; pulselen > pulseTarget; pulselen--) {
        pwm.setPWM(s.ID, 0, pulselen);
    }
    // update pos
    s.LastDegPos = 5+s.InitialOffSet;
    delay(500);
}

void ResetMin2Max(servo s){
    uint16_t pulseTarget = map(175-s.InitialOffSet,0,180,SERVOMIN,SERVOMAX);
    for (uint16_t pulselen = SERVOMIN; pulselen < pulseTarget; pulselen++) {
        pwm.setPWM(s.ID, 0, pulselen);
    }
    // update pos
    s.LastDegPos = 175-s.InitialOffSet;
    delay(500);
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
  pwm.setPWMFreq(50);
  delay(1000);
  // its aliiiiive
}


// movement presets
void walk(int iter,int speedMultiplier) {
    int speedDelay=150*speedMultiplier; // 400ms x multiplier
    for(int l=0;l<iter;l++){
        legOut(30,Servos[FRLeg],speedDelay);
        legOut(110,Servos[RRLeg],speedDelay);
        legIn(0,Servos[FRLeg],speedDelay);
        legIn(70,Servos[RRLeg],speedDelay);
        delay(speedDelay);
    }
}

void legOut(int degrees,servo s,int time){
    //  Degrees is the actual desired angle, not an offset
    if ( !s.Inverse ) {
        // normal
        uint16_t pulseTarget = map(degrees,0,180,SERVOMIN,SERVOMAX);
        uint16_t oldTarget = map(s.LastDegPos,0,180,SERVOMIN,SERVOMAX);
        for (uint16_t pulselen = SERVOMIN; pulselen < pulseTarget; pulselen++) {
            pwm.setPWM(s.ID, 0, pulselen);
        }
        // update pos
        s.LastDegPos = degrees;
        delay(time);
    } else {
        // inverse

    }
}

void legIn(int degrees,servo s,int time){
    //  Degrees is the actual desired angle, not an offset
    if ( !s.Inverse ) {
        // normal
        uint16_t pulseTarget = map(degrees,0,180,SERVOMIN,SERVOMAX);
        uint16_t oldTarget = map(s.LastDegPos,0,180,SERVOMIN,SERVOMAX);
        for (uint16_t pulselen = SERVOMAX/2; pulselen > pulseTarget; pulselen--) {
            pwm.setPWM(s.ID, 0, pulselen);
        }
        // update pos
        s.LastDegPos = degrees;
        delay(time);
    } else {
        // inverse

    }
}