#include <ezBuzzer.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

const int DATA_PINF = 12;      // Front strip PIN
// Nano: 7
const int DATA_PINB = 13;      // Back strip PIN
// Nano: 8

const int NUM_LEDSF = 24; // Front strip LED_COUNT
// Change 144 on the finished version, 24 on "test NeoPixel".
const int NUM_LEDSB = 144; // Back strip LED_COUNT

// Brigthness
const int BRIGHTNESS = 10;
// Change to higher value when done testing. MUST NOT BE TO HIGH! (Draws more current)

CRGB ledsF[NUM_LEDSF];
CRGB ledsB[NUM_LEDSB];

int DELAYVAL = 10; //Milliseconds between each pixel on the blade lighting up.

const int igniteButton = 2;
// Nano: 2
const int actionButton = 4;
// Nano: 4
const int shockPin = 5;
// Nano: 9

// State values for different sensor readings (either HIGH or LOW)
int shockState;
int actionBtnState;
int igniteBtnState;

int CheckSwitch=1;  // variable for reading the current SwitchPin status
int LastCheck=1;  // variable for keeping track of SwitchPin

// Boolean to check if blade is on or not
bool bladeOff = true;

// Boolean to check if tip is different color or not
bool tipChange = false;

const int redPin = 11;
// Nano: 3
const int greenPin = 10;
// Nano: 5 
const int bluePin = 9;
// Nano: 6

// Primary color values, for blade
int red;
int green;
int blue;

// Secondary color values, for tipdrag
int redTip;
int greenTip;
int blueTip;

// Third color values, for blaster deflect
int redDef;
int greenDef;
int blueDef;

// Int for which color is chosen
int colormode;

// Random number for where to put the Blaster Deflect
int randomNumber;

// Starting positions for blaster deflect
const int maxStartPos = NUM_LEDSF - 7;
const int minStartPos = 5;

// String for which color is selected
String color;

// SOUND EFFECT SETUP

const int buzPin = 6;

ezBuzzer buzzer(buzPin); // ezBuzzer object attached to buzPin

// Pitch values for idle on animation
int pitchAni1;
int pitchAni2;
int pitchAni3;
int pitchAni4;
int pitchAni5;

// NoteDurations for Animated Sound effects
int dur1;
int dur2;
int dur3;
int dur4;
int dur5;

// Max/Min values for durations and pitches

int minDur = 1;
int maxDur = 8;

int minPitch = 100;
int maxPitch = 1500;

// Ignition animtaion
int onAniSound[] = {




  
};

int noteDurOn [] = {




};


// Retract animation
int offAniSound [] = {




};

int noteDurOff [] = {




};

int onLength;
int offLength;
int idleLength;

// END OFF SOUND EFFECT SETUP


void setup() {

  // *** Debugging ***
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, DATA_PINF>(ledsF, NUM_LEDSF); // Start Front LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PINB>(ledsB, NUM_LEDSB); // Start Back LEDs
  FastLED.setBrightness(  BRIGHTNESS );  // Set Brightess

  setColor(); // Runs the Set Color program once, to make sure a color is selected before the user powers up his saber.
  
  // limit powerdraw to what the arduino/powersupply can supply
  FastLED.setMaxPowerInVoltsAndMilliamps(5,1000); // (Volts,Millamps);
  
  onLength = sizeof(noteDurOn) / sizeof(int);
  offLength = sizeof(noteDurOff) / sizeof(int);

}

void loop() {
  
  buzzer.loop();
  
  // State values determined by value comming from buttons/shock sensor
  shockState = digitalRead(shockPin);
  igniteBtnState = digitalRead(igniteButton);
  actionBtnState = digitalRead(actionButton);

  if (bladeOff) {
    if (igniteBtnState == HIGH) { // Checks if the blade should be on. Starts the blade if true
      // Start Blade
      igniteBlade(red, green, blue);


    }
    else if (actionBtnState == HIGH) {

      // Cycle to another color
      setColor();

    }  
  }
  else {
    if (igniteBtnState == LOW) { // Checks if the blade should be on. Retracts the blade if false

      // Retract Blade
      retractBlade();

    }
    else if (actionBtnState == HIGH && tipChange) { // Checks if actionbutton is pressed, and tipchange is true. Turns on tip if both true

      // *** Debugging ***
      Serial.println("TipDrag");

      // Writes tipdrag colors to tip
      analogWrite(redPin, redTip);
      analogWrite(greenPin, greenTip);
      analogWrite(bluePin, blueTip);

      // Tip is now On
      tipChange = ! tipChange;

      
    }
    else if (actionBtnState == LOW && !tipChange) { // Checks if actionbutton is not pressed, and if tipchange is false. Turns of tip if both are false

      // *** Debugging ***
      Serial.println("TipDragStopped");

      // Writes tip colors back to normal
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);

      // Tip is now Off
      tipChange = ! tipChange;

    }
    if (shockState == LOW) { // Checks if the knock switch has been disturbed. Runs blasterDeflect function if true.

      blasterDeflect();

    }  
    if (buzzer.getState() == BUZZER_IDLE) {
        
        idleSoundAnimation();
    }
  }    
}

void igniteBlade(int red, int green, int blue) { // Startanimation for blade

  for(int i=0; i<NUM_LEDSF; i++) { // For each pixel...

    ledsF[i].setRGB( red, green, blue);
    ledsB[i].setRGB( red, green, blue);
    FastLED.show();

    delay(DELAYVAL); // Pause before next pass through loop
  }
  

  // Turn on end-LED
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
  
  // Blade is now On
  bladeOff = ! bladeOff; 
  delay(10);
  
  // *** Debugging ***
  Serial.println("Blade opened");
  
}

void retractBlade() { // Retract animation for blade

if (buzzer.getState() != BUZZER_IDLE) {
      buzzer.stop() ; // stop
    }

  // Turn off end-LED
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);

  // Turns off all leds, one by one
  for(int i=NUM_LEDSF; i--;) {

    ledsF[i] = CRGB::Black; 
    ledsB[i] = CRGB::Black; 
    FastLED.show();

    delay(DELAYVAL);
  }

  // Blade is now Off
  bladeOff = ! bladeOff;

  delay(10);

  // *** Debugging ***
  Serial.println("Blade retracted");
  
}

void setColor() {

  switch (colormode) {
    case 0: // BLUE
    // Primary color
    red = 0;
    green = 0;
    blue = 255;
    // Secondary color
    redTip = 200;
    greenTip = 0;
    blueTip = 200;
    // Third color
    redDef = 200;
    greenDef = 200;
    blueDef = 255;

    color = "BLUE";
    colormode++;
    break;

    case 1: // RED
    // Primary color
    red = 255;
    green = 0;
    blue = 0;
    // Secondary color
    redTip = 255;
    greenTip = 30;
    blueTip = 2;
    // Third color
    redDef = 255;
    greenDef = 150;
    blueDef = 150;
    // Changes Delay, to make a more "dramatic" blade opening/retracting
    DELAYVAL = 15;

    color = "RED";
    colormode++;
    break;

    case 2: // GREEN
    // Primary color
    red = 0;
    green = 255;
    blue = 0;
    // Secondary color
    redTip = 255;
    greenTip = 30;
    blueTip = 0;
    // Third color
    redDef = 200;
    greenDef = 255;
    blueDef = 200;
    // Changes Delay back to normal
    DELAYVAL = 10;

    color = "GREEN";
    colormode++;
    break;

    case 3: // GOLD
    // Primary color
    red = 255;
    green = 100;
    blue = 0;
    // Secondary color
    redTip = 0;
    greenTip = 255;
    blueTip = 255;
    // Third color
    redDef = 255;
    greenDef = 200;
    blueDef = 180;

    color = "GOLD";
    colormode++;
    break;

    case 4: // AQUA
    // Primary color
    red = 0;
    green = 200;
    blue = 200;
    // Secondary color
    redTip = 255;
    greenTip = 100;
    blueTip = 0;
    // Third color
    redDef = 240;
    greenDef = 255; //220;
    blueDef = 255; //220;

    color = "AQUA";
    colormode++;
    break;

    case 5: // SILVER BLUE
    // Primary color
    red = 50;
    green = 50;
    blue = 255;
    // Secondary color
    redTip = 200;
    greenTip = 20;
    blueTip = 20;
    // Third color
    redDef = 240;
    greenDef = 240;
    blueDef = 255;

    color = "SILVER BLUE";
    colormode++;
    break;

    case 6: // PURPLE
    // Primary color
    red = 100;
    green = 0;
    blue = 255;
    // Secondary color
    redTip = 255;
    greenTip = 0;
    blueTip = 0;
    // Third color
    redDef = 255;
    greenDef = 200;
    blueDef = 255;

    color = "PURPLE";
    colormode = 0;
    break;

  }
  delay(500);

  // *** Debugging ***
  Serial.print("Color is now: ");
  Serial.println(color);
}

void blasterDeflect() { // Blaster Deflect Effect

  // Forth color values, for fade effect
  int redFade = redDef;
  int greenFade = greenDef;
  int blueFade = blueDef;

  // Chooses a random position for the deflect animation
  randomNumber = random(minStartPos, maxStartPos);
  Serial.println(randomNumber);
  Serial.println("How Shocking!");

  for(int j=randomNumber; j<randomNumber + 5; j++ ) {
    ledsF[j].setRGB( redDef, greenDef, blueDef);
    ledsB[j].setRGB( redDef, greenDef, blueDef);
    FastLED.show();
    delay(0);
  }
  delay(100);
  while (!(redFade == red) || !(greenFade == green) || !(blueFade == blue)){ // While "color"Fade isn't the same as the original color:

    if (redFade > red){
      redFade = redFade - 10; // Remove 10 from redFade value
    }
    if (greenFade > green){
      greenFade = greenFade - 10; // Remove 10 from greenFade value
    }
    if (blueFade > blue){
      blueFade = blueFade - 10; // Remove 10 from blueFade value
    }
    
    // If removed to much, reset color to the original color
    if (redFade < red) { 
      redFade = red;
    }
    if (greenFade < green){
      greenFade = green;      
    }
    if (blueFade < blue){
      blueFade = blue;
    }

    // Write the new fade colors to the deflect area    
    for(int j=randomNumber; j<randomNumber + 5; j++ ) {
    ledsF[j].setRGB( redFade, greenFade, blueFade);
    ledsB[j].setRGB( redFade, greenFade, blueFade);
    FastLED.show();
    }
    delay(0);
  }
}

void idleSoundAnimation(){

  dur1 = random(minDur, maxDur);
  dur2 = random(minDur, maxDur);
  dur3 = random(minDur, maxDur);
  dur4 = random(minDur, maxDur);
  dur5 = random(minDur, maxDur);

  pitchAni1 = random(minPitch, maxPitch);
  pitchAni2 = random(minPitch, maxPitch);
  pitchAni3 = random(minPitch, maxPitch);
  pitchAni4 = random(minPitch, maxPitch);
  pitchAni5 = random(minPitch, maxPitch);

  int noteDurIdle[] = {
    dur1, dur2, dur3, dur4, dur5
  };

  int idleSound[] = {
    pitchAni1, pitchAni2, pitchAni3, pitchAni4, pitchAni5,

  };
  
  idleLength = sizeof(noteDurIdle) / sizeof(int);

  buzzer.playMelody(idleSound, noteDurIdle, idleLength);
}
