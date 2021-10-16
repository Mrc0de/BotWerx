#include <heltec.h>
#include "xbmLogos.h"
#include <Adafruit_PWMServoDriver.h>
// Lora (dont enable without antenna attached)
#define LoraBAND 868E6  //you can set band here directly,e.g. 868E6,915E6
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
Adafruit_PWMServoDriver pwm;

struct servo {
    String Name;
    uint8_t ID;
    int InitialOffSet;
    bool Inverse;
};

servo Servos[]={
    {"FRShoulder",0,0,false}, {"FRLeg",1,0,false},
    {"FLShoulder",2,70,true}, {"FLLeg",3,85,true},
    {"RRShoulder",4,0,false}, {"RRLeg",5,70,false},
    {"RLShoulder",6,90,true}, {"RLLeg",7,45,true},
};

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
            ResetMax2Min(Servos[n]);
        } else {
            // inverse
            Heltec.display -> drawString(n, n*7,  "Inverse");
            ResetMin2Max(Servos[n]);
        }
    };
    Heltec.display -> display();
    delay(60000);
    // how splash
    shortSplash();
}

void loop() {
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "[ TEST MODE ]");
    Heltec.display -> drawString(8, 24, "[ Model "+String(BotModel)+" ]");
    Heltec.display -> display();
    delay(2000);
    Heltec.display -> clear();
    Heltec.display -> setColor(WHITE);
    Heltec.display -> drawXbm(0,0,xbmLogo1_width,xbmLogo1_height,xbmLogo1);
    Heltec.display -> display();
    delay(2000);
}

// Movement Functions
void ResetMax2Min(servo s){
   uint16_t pulseTarget = map(0+s.InitialOffSet,0,180,SERVOMIN,SERVOMAX);
    for (uint16_t pulselen = SERVOMAX; pulselen > pulseTarget; pulselen--) {
        pwm.setPWM(s.ID, 0, pulselen);
    }
    delay(500);
}

void ResetMin2Max(servo s){
    uint16_t pulseTarget = map(178-s.InitialOffSet,0,180,SERVOMIN,SERVOMAX);
    for (uint16_t pulselen = SERVOMIN; pulselen < pulseTarget; pulselen++) {
        pwm.setPWM(s.ID, 0, pulselen);
    }
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
    int speedDelay=100*speedMultiplier;
    for(int l=0;l<iter;l++){

    }
}