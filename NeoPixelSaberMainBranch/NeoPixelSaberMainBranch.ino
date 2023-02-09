#include <FastLED.h>

// Led Strip Pins
  const int DATA_PINF = 12;      // Front strip PIN // Nano: 10
  const int DATA_PINB = 13;      // Back strip PIN  // Nano: 11

// Number of Leds
  const int NUM_LEDSF = 24; // Front strip LED_COUNT  // Change 144 on the finished version, 24 on "test NeoPixel".
  const int NUM_LEDSB = 144; // Back strip LED_COUNT

// Brigthness
  const int BRIGHTNESS = 100;  // Change to higher value when done testing. MUST NOT BE TO HIGH! (Draws more current)

CRGB ledsF[NUM_LEDSF];
CRGB ledsB[NUM_LEDSB];

// Delayvalue
  int DELAYVAL = 10; //Milliseconds between each pixel on the blade lighting up.

// SENSORS AND BUTTON PINS
  const int igniteButton = 2;  // Nano: 2
  const int actionButton = 4;  // Nano: 4
  const int shockPin = 5;  // Nano: 9

// State values for different sensor readings (either HIGH or LOW)
  int shockState;
  int actionBtnState;
  int igniteBtnState;

// BOOLEANS
  bool bladeOff = true; // Boolean to check if blade is on or not
  bool tipChange = false; // Boolean to check if tip is different color or not
  bool pulseDown; // Boolean to check if the blade is pulsing down or not

// RGB LED PINS
  const int redPin = 11;  // Nano: 3
  const int greenPin = 10;  // Nano: 5
  const int bluePin = 9;  // Nano: 6

// Power-Button-Indicator LED Pins (if you use them.)
  const int redIndi = 8; // Nano: 8
  const int greenIndi = 7; // Nano: 7

// Note about the indicator LEDS:
  /* For some or other weird reason, the green and red input are switched (marked wrong I guess)
     But, they also behave quite weird too. When for example red is on, if I write a On signal to
     green, both leds turns off. If I write a off signal to red, while red is on, the green led
     turns on. Please feel free to change the code if you do not enconter the same problem. */

// COLOR VALUES

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

  // Forth color values, for fade effect
    int redFade;
    int greenFade;
    int blueFade;

// Int for which color is chosen
  int modeCase;

// Random number for where to put the Blaster Deflect
  int randomNumber;

// How big the blasterdeflect area should be
  int blastSize = 5;

// How fast the fade effect should be
  int fadeStep;
  int defFadeStep = 20; // For blaster fade effect

// Starting positions for blaster deflect
  const int fromEgdePos = 5;
  const int minStartPos = fromEgdePos;
  const int maxStartPos = NUM_LEDSF - blastSize - fromEgdePos;

// String for which color is selected
  String modeName;

// pulsinging Light Variables
  int pulsingLowest;
  int pulseStep;
  int pulsingBrightness;
  int pulseAmount;

void setup() {

  // PulsingBrightness should be the same as BRIGTHNESS when the blade starts up...
    pulsingBrightness = BRIGHTNESS;  

  // *** Debugging ***
    Serial.begin(9600);
  
  // Defining shockPin as INPUT  
    pinMode(shockPin, INPUT);

  // Defining Led Indicator pins as OUTPUTs
    pinMode(redIndi, OUTPUT);
    pinMode(greenIndi, OUTPUT);

  // Start leds and set brightness
    FastLED.addLeds<NEOPIXEL, DATA_PINF>(ledsF, NUM_LEDSF); // Start Front LEDs
    FastLED.addLeds<NEOPIXEL, DATA_PINB>(ledsB, NUM_LEDSB); // Start Back LEDs
    FastLED.setBrightness(  BRIGHTNESS );  // Set Brightess

  // Turn off leds (if they of some or other reason should be on already)
    for(int i=NUM_LEDSF; i--;) {  // Turns off all leds, one by one

      ledsF[i] = CRGB::Black; 
      ledsB[i] = CRGB::Black; 
      FastLED.show();

      delay(10);
    }

  // limit powerdraw to what the arduino/powersupply can supply
    FastLED.setMaxPowerInVoltsAndMilliamps(5,1000); // (Volts,Millamps);

  setMode(); // Runs the setMode program once, to make sure a color is selected before the user powers up his saber.

  // Turn on Red indicator, to show that the lightsaber is powered on and ready to use
    digitalWrite(redIndi, 1);
    
}

void loop() {
  
  // State values determined by value comming from buttons/shock sensor
    shockState = digitalRead(shockPin);
    igniteBtnState = digitalRead(igniteButton);
    actionBtnState = digitalRead(actionButton);

  if (bladeOff) { // If blade off...
    if (igniteBtnState == HIGH) { //  Starts the blade if the ignite button is pressed.

      // Start Blade
        startBlade(red, green, blue);


    }
    else if (actionBtnState == HIGH) { // Runs the setMode function if the action button is pressed.

        setMode(); // Cycle to another color

    }  
  }
  else { // If blade is on...
    if (igniteBtnState == HIGH) { // Retracts the blade if the ignite button is pressed.

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
    else if (shockState == LOW) { // Checks if the knock switch has been disturbed. Runs blasterDeflect function if true.

      blasterDeflect();

    }
    
    // Pulsing Light
      pulsingAnimation();
  }  
}

void startBlade(int red, int green, int blue) { // Startanimation for blade

  // Turn off red indicator, to turn both leds on (see line 43)
    digitalWrite(redIndi, 0);

  for(int i=0; i<NUM_LEDSF; i++) { // Starts each pixel with the set color, with DELAYVAL between each

    ledsF[i].setRGB( red, green, blue);
    ledsB[i].setRGB( red, green, blue);
    FastLED.show();

    delay(DELAYVAL); // Pause before next pass through loop
    
    // pulsingAnimation(); // Play pulsingAnimation, for more vareity
  }
  
  // Turn on end-LED
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  
  // Blade is now On
    bladeOff = ! bladeOff; 
  delay(10);
  
  // Turn on green indicator
    digitalWrite(greenIndi, HIGH);

  // *** Debugging ***
    Serial.println("Blade opened");
  
}

void retractBlade() { // Retract animation for blade

  // Turn off green indicator, to turn both Leds on (see line 43)
    digitalWrite(greenIndi, 0);

  // Turn off end-LED
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);

  
  for(int i=NUM_LEDSF; i--;) {  // Turns off all leds, one by one

      ledsF[i] = CRGB::Black; 
      ledsB[i] = CRGB::Black; 
      FastLED.show();

    delay(DELAYVAL);
    
    // pulsingAnimation(); // Play pulsinganimation, for more vareity
  }

  // Blade is now Off
    bladeOff = ! bladeOff;

  delay(10);

  // Turn on red indicator:
    digitalWrite(redIndi, 1); 

  // *** Debugging ***
    Serial.println("Blade retracted");
  
}

void setMode() { // Sets Mode (colors, effect intensities etc.)

  // Write on to green indicator to turn both off (see line 43);
    digitalWrite(greenIndi, 1);
  
  // Defaults. Can and Will be changed in some switch cases.
    DELAYVAL = 10;  // Sets DELAYVAL to 10 (default). Can be changed in switch statement.
    pulseStep = 20; // Sets pulseStep to 20 (default)(this does not mean that it does not pulsing). Can be changed in switch statement.
    pulseAmount = 2; // Sets pulseAmount to 2 (default). Can be changed in switch statement

  switch (modeCase) {
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

      // pulsing Intensity Change
        pulseAmount = 1;

      modeName = "BLUE";
      modeCase++;
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
        greenDef = 240;
        blueDef = 240;
      // DELAYVAL Change
        DELAYVAL = 20;  // Changes delayval to make a more dramatic blade opening/retracting

      // pulsing Intensity Change
        pulseStep = 25;
        pulseAmount = 8;

      modeName = "RED";
      modeCase++;
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

      // pulsing Intensity Change
        pulseStep = 10;

      modeName = "GREEN";
      modeCase++;
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

      modeName = "GOLD";
      modeCase++;
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
        greenDef = 255;
        blueDef = 255;

      modeName = "AQUA";
      modeCase++;
    break;

    case 5: // Magenta
      // Primary color
        red = 255;
        green = 0;
        blue = 255;
      // Secondary color
        redTip = 255;
        greenTip = 50;
        blueTip = 0;
      // Third color
        redDef = 240;
        greenDef = 230;
        blueDef = 255;

      modeName = "Magenta";
      modeCase++;
    break;

    case 6: // PURPLE
      // Primary color
        red = 100;
        green = 0;
        blue = 255;
      // Secondary color
        redTip = 255;
        greenTip = 50;
        blueTip = 0;
      // Third color
        redDef = 255;
        greenDef = 200;
        blueDef = 255;
      // DELAYVAL Change
        DELAYVAL = 20;  // Changes delayval to make a more dramatic blade opening/retracting

      // pulsing Intensity Change
        pulseStep = 15;
        pulseAmount = 1;
      
      modeName = "PURPLE";
      modeCase = 0;
    break;

  }
  delay(300);

  // Turn on led indicator after color is chosen
    digitalWrite(greenIndi, 0);
  
  // *** Debugging ***
    Serial.print("Selected Mode: ");
    Serial.println(modeName);
}

void blasterDeflect() { // Blaster Deflect Effect

  // Forth color values, for fade effect
    redFade = redDef;
    greenFade = greenDef;
    blueFade = blueDef;

  // Chooses a random position for the deflect animation
    randomNumber = random(minStartPos, maxStartPos);

  // *** Debugging ***
    Serial.println(randomNumber);

  // Set colors to the deflect values in selected area
  for(int j=randomNumber; j<randomNumber + blastSize; j++ ) {
    ledsF[j].setRGB( redDef, greenDef, blueDef);
    ledsB[j].setRGB( redDef, greenDef, blueDef);
    FastLED.show();
  }
  delay(100);
  while (!(redFade == red) || !(greenFade == green) || !(blueFade == blue)){ // While "color"Fade isn't the same as the original color:

    fadeValues(defFadeStep); // Get the fade values

    // Write the new fade colors to the deflect area    
    for(int j=randomNumber; j<randomNumber + blastSize; j++ ) {
      ledsF[j].setRGB( redFade, greenFade, blueFade);
      ledsB[j].setRGB( redFade, greenFade, blueFade);
      FastLED.show();

      pulsingAnimation(); // Play pulsinganimation, for more vareity
    }
  }
}

void pulsingAnimation() { // Pulsing animation for variation

  pulsingLowest = BRIGHTNESS - pulseStep; // pulsingLowest is BRIGHTNESS - pulseStep. PulseStep is set in setMode program.

  if (pulseDown) {
    for (int j=0; j<pulseAmount; j++ ) {
      pulsingBrightness --;
    }
    if (pulsingBrightness < pulsingLowest) {
      pulseDown = ! pulseDown;
    }
  }
  else {
    for (int j=0; j<pulseAmount; j++ ) {
      pulsingBrightness ++;
    }
    if (pulsingBrightness >= BRIGHTNESS) {
      pulseDown = ! pulseDown;
    }
  }

  // Send updated brightness
    FastLED.setBrightness(pulsingBrightness);
    FastLED.show();
  
  // *** Debugging ***
    Serial.println(pulsingBrightness);
  
}

void fadeValues(int fadestep) { // Changes *color*fade values

  if (redFade > red){
    redFade = redFade - fadestep; // Remove 10 from redFade value
  }
  if (greenFade > green){
    greenFade = greenFade - fadestep; // Remove 10 from greenFade value
  }
  if (blueFade > blue){
    blueFade = blueFade - fadestep; // Remove 10 from blueFade value
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
}
