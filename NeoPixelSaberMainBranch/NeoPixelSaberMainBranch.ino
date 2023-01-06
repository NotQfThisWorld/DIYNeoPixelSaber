#include <FastLED.h>

const int DATA_PINF = 12;      // Front strip PIN
// Nano: 7
const int DATA_PINB = 13;      // Back strip PIN
// Nano: 8

const int NUM_LEDSF = 24; // Front strip LED_COUNT
// Change 144 on the finished version, 24 on "test NeoPixel".
const int NUM_LEDSB = 144; // Back strip LED_COUNT

const int BRIGHTNESS = 64;

CRGB ledsF[NUM_LEDSF];
CRGB ledsB[NUM_LEDSB];

int DELAYVAL = 10; //Milliseconds between each pixel on the blade lighting up.

const int igniteButton = 2;
// Nano: 2


const int actionButton = 4;
// Nano: 4
const int shockPin = 5;
// Nano: 9

int shockState;
int actionBtnState = 0;
int igniteBtnState = 0;

int CheckSwitch=1;  // variable for reading the current SwitchPin status
int LastCheck=1;  // variable for keeping track of SwitchPin

bool bladeOff = false;

bool tipChange = false;

const int redPin = 11;
// Nano: 3
const int greenPin = 10;
// Nano: 5 
const int bluePin = 9;
// Nano: 6

// Primary color values, for blade
int red = 0;
int green = 0;
int blue = 0;

// Secondary color values, for tipdrag
int redTip = 0;
int greenTip = 0;
int blueTip = 0;

// Third color values, for blaster deflect
int redDef = 0;
int greenDef = 0;
int blueDef = 0;

int colormode = 0;

String color = "black";

void setup() {

  // *** Debugging ***
  Serial.begin(9600);

  pinMode(shockPin, INPUT);

  FastLED.addLeds<NEOPIXEL, DATA_PINF>(ledsF, NUM_LEDSF); // Start Front LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PINB>(ledsB, NUM_LEDSB); // Start Back LEDs
  FastLED.setBrightness(  BRIGHTNESS );  // Set Brightess

  setColor(); // Runs the Set Color program once, to make sure a color is selected before the user powers up his saber.

}

void loop() {

  shockState = digitalRead(shockPin);
  igniteBtnState = digitalRead(igniteButton);
  actionBtnState = digitalRead(actionButton);

  delay(1);
  
  if (bladeOff) {
    if (igniteBtnState == HIGH) { // Checks if the blade should be on. Starts the blade if true
      // Start Blade
      startBlade(red, green, blue);


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
    else if (actionBtnState == HIGH && tipChange) {
    
      Serial.println("TipDrag");
      analogWrite(redPin, redTip);
      analogWrite(greenPin, greenTip);
      analogWrite(bluePin, blueTip);

      tipChange = ! tipChange;
    }

    else if (actionBtnState == LOW && !tipChange) {
    
      Serial.println("TipDragStopped");
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);

      tipChange = ! tipChange;      

    }

    else if (digitalRead(shockPin) == LOW) {
      Serial.println("How Shocking!");
      delay(500);
    }
  }    
}

void startBlade(int red, int green, int blue) {

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
  
  bladeOff = ! bladeOff;
  delay(10);
  
  // *** Debugging ***
  Serial.println("Blade opened");
  
}

void retractBlade() {

  // Turn off end-LED
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);

  for(int i=NUM_LEDSF; i--;) {

      ledsF[i] = CRGB::Black; 
      ledsB[i] = CRGB::Black; 
      FastLED.show();

    delay(DELAYVAL);
  }
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
    redTip = 255;
    greenTip = 0;
    blueTip = 0;
    // Third color
    redDef = 30;
    greenDef = 30;
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
    greenTip = 0;
    blueTip = 255;
    // Third color
    redDef = 255;
    greenDef = 20;
    blueDef = 10;
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
    redDef = 30;
    greenDef = 255;
    blueDef = 30;
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
    greenDef = 140;
    blueDef = 30;

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
    redDef = 70;
    greenDef = 220;
    blueDef = 220;

    color = "AQUA";
    colormode++;
    break;

    case 5: // SILVER BLUE
    // Primary color
    red = 70;
    green = 70;
    blue = 255;
    // Secondary color
    redTip = 200;
    greenTip = 20;
    blueTip = 20;
    // Third color
    redDef = 110;
    greenDef = 110;
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
    redDef = 130;
    greenDef = 40;
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


void tipDrag(int redTip, int greenTip, int blueTip) {

Serial.print("TipDraging with ");
Serial.print(redTip);
Serial.print(" , ");
Serial.print(greenTip);
Serial.print(" , ");
Serial.println(blueTip);

  analogWrite(redPin, redTip
);
  analogWrite(greenPin, greenTip);
  analogWrite(bluePin, blueTip);

}

void blasterDeflect() {

  // *** Debugging ***
  Serial.println("Deflect!");

}