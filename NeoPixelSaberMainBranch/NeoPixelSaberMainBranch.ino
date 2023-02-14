#include <FastLED.h>

// Led Strip Pins
  const int DATA_PINF = 12;      // Front strip PIN // Nano: 10
  const int DATA_PINB = 13;      // Back strip PIN  // Nano: 11

// Number of Leds
  const int NUM_LEDSF = 24; // Front strip LED_COUNT  // Change to (final led count) on the finished version, 24 on "test NeoPixel".
  const int NUM_LEDSB = 24; // Back strip LED_COUNT

// Brigthness
  const int BRIGHTNESS = 100;

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
  bool pulseDown = true; // Boolean to check if the blade is pulsing down or not
  bool lightningOn = false; // Boolean to check if the the lightning effect is playing or not.

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

  // Secondary color values, for MeltTip
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

  // Fifth color values, for Lightning Effect
    int redLightning = 50;
    int greenLightning = 50;
    int blueLightning = 255;
    
  // Sixth color values
    int redTemp;
    int greenTemp;
    int blueTemp;
    // Using a secondary set of variables for some effects, since for some or other weird reason, the FastLED.setRGB command sometimes alter the color variables.

  

// Int for which color is chosen
  int modeCase;

// Where to put the effects
  int randomEffectStartPos;

// How many lightning impacts?
  int lightningCount;
  const int minLightningCount = 3;
  const int maxLightningCount = 5;

// Sizes
  // How big the blasterdeflect area should be
    const int blastSize = 5;
  // Tip size
    const int tipSize = 6; // Should be an even number.
  // Lightning size
    int lightningSize;
    const int minLightningSize = 4;
    const int maxLightningSize = 7;

// How fast the fade effect should be
  int fadeStep;
  int defFadeStep = 20; // For blaster fade effect

// Starting positions for blaster deflect
  const int fromEgdePos = 5;
  const int minStartPos = fromEgdePos;
  const int maxStartPos = NUM_LEDSF - blastSize - fromEgdePos;
  int maxStartPos2 = NUM_LEDSF - maxLightningSize - fromEgdePos;

// String for which color is selected
  String modeName;

// pulsinging Light Variables
  int pulsingLowest;
  int pulsingStep;
  int pulsingLowestTemp; // Temporary values for when the effect needs to be changed for a little time
  int pulsingStepTemp;  // ------||------
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
    if (igniteBtnState == HIGH && shockState == LOW) { //  Starts the blade if the ignite button is pressed, or if the shocksensor is disturbed (enables stab ignition)

      // Start Blade
        startBlade(red, green, blue);


    }
    else if (actionBtnState == HIGH) { // Runs the setMode function if the action button is pressed.

      // Cycle to another color
        setMode();

    }  
  }
  else { // If blade is on...
    if (igniteBtnState == HIGH && shockState == LOW) { // Retracts the blade if the ignite button is pressed.

      // Retract Blade
        retractBlade();

    }

    else if (shockState == LOW && !(igniteBtnState == HIGH) && !(actionBtnState == HIGH)) { // Checks if the knock switch has been disturbed. Runs blasterDeflect function if true.

      // Blaster Deflect Effect
        blasterDeflect();

    }

    else if (actionBtnState == HIGH && tipChange && !(shockState == LOW) && !lightningOn) { // Checks if actionbutton is pressed, and tipchange is true. Turns on tip if both true
    
      // Melttip on
        meltTip(true);
      
    }
    else if (actionBtnState == LOW && !tipChange && !(shockState == LOW)) { // Checks if actionbutton is not pressed, and if tipchange is false. Turns of tip if both are false
    
      // Melttip off
        meltTip(false);

    }
    else if (actionBtnState == HIGH && igniteBtnState == HIGH && !lightningOn) {

      // Run the lightningEffect
        lightningEffect(true);

    }
    else if (actionBtnState == LOW && lightningOn) {

      // Stop the lightningEffect
        lightningEffect(false);

    }

    else {
      // Pulsing Light
        pulsingAnimation();
    }        
  }  
}

void startBlade(int red, int green, int blue) { // Startanimation for blade

  // Turn off red indicator, to turn both leds on (see line 39)
    digitalWrite(redIndi, 0);

  for(int i=0; i<NUM_LEDSF; i++) { // Starts each pixel with the set color, with DELAYVAL between each

    ledsF[i].setRGB( red, green, blue);
    ledsB[i].setRGB( red, green, blue);
    FastLED.show();

    delay(DELAYVAL); // Pause before next pass through loop
    
    // pulsingAnimation(); // Play pulsingAnimation, for more vareity
  }
  
  // Blade is now On
    bladeOff = ! bladeOff; 
  delay(10);
  
  // Turn on green indicator
    digitalWrite(greenIndi, HIGH);

  // *** Debugging ***
    Serial.println("Blade opened");
  
}

void retractBlade() { // Retract animation for blade

  // Turn off green indicator, to turn both Leds on (see line 39)
    digitalWrite(greenIndi, 0);
  
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

  // Write on to green indicator to turn both off (see line 39);
    digitalWrite(greenIndi, 1);
  
  // Defaults. Can and Will be changed in some switch cases.
    // Delay and pulse defaults
      DELAYVAL = 10;  // Sets DELAYVAL to 10 (default). Can be changed in switch statement.
      pulsingStep = 20; // Sets pulsingStep to 20 (default)(this does not mean that it does not pulsing). Can be changed in switch statement.
      pulseAmount = 2; // Sets pulseAmount to 2 (default). Can be changed in switch statement
    // Color defaults
      // Secondary color
        redTip = 200;
        greenTip = 30;
        blueTip = 2;
  
  switch (modeCase) {
    case 0: // BLUE
      // Primary color
        red = 0;
        green = 0;
        blue = 255;

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

      // Third color
        redDef = 255;
        greenDef = 240;
        blueDef = 240;
      // DELAYVAL Change
        DELAYVAL = 20;  // Changes delayval to make a more dramatic blade opening/retracting

      // pulsing Intensity Change
        pulsingStep = 25;
        pulseAmount = 8;

      modeName = "RED";
      modeCase++;
    break;

    case 2: // GREEN
      // Primary color
        red = 0;
        green = 255;
        blue = 0;

      // Third color
        redDef = 200;
        greenDef = 255;
        blueDef = 200;

      // pulsing Intensity Change
        pulsingStep = 10;

      modeName = "GREEN";
      modeCase++;
    break;

    case 3: // GOLD
      // Primary color
        red = 255;
        green = 100;
        blue = 0;

      // Third color
        redDef = 255;
        greenDef = 200;
        blueDef = 180;

      modeName = "GOLD";
      modeCase++;
    break;

    case 4: // ORANGE
      // Primary color
        red = 200;
        green = 10;
        blue = 0;

      // Third color
        redDef = 255;
        greenDef = 200;
        blueDef = 100;
      // DELAYVAL Change
        DELAYVAL = 30;  // Changes delayval to make a more dramatic blade opening/retracting

      // Pulsing Intensity Change
        pulsingStep = 30;
        pulseAmount = 5;      

        modeName = "ORANGE";
        modeCase ++;
    break;

    case 5: // AQUA
      // Primary color
        red = 0;
        green = 200;
        blue = 200;

      // Third color
        redDef = 240;
        greenDef = 255;
        blueDef = 255;

      modeName = "AQUA";
      modeCase++;
    break;

    case 6: // Magenta
      // Primary color
        red = 255;
        green = 0;
        blue = 255;

      // Third color
        redDef = 240;
        greenDef = 230;
        blueDef = 255;

      modeName = "Magenta";
      modeCase++;
    break;

    case 7: // PURPLE
      // Primary color
        red = 100;
        green = 0;
        blue = 255;

      // Third color
        redDef = 255;
        greenDef = 200;
        blueDef = 255;
      // DELAYVAL Change
        DELAYVAL = 20;  // Changes delayval to make a more dramatic blade opening/retracting

      // Pulsing Intensity Change
        pulsingStep = 15;
        pulseAmount = 1;
      
      modeName = "PURPLE";
      modeCase ++;
    break;

    case 8: // WHITE
      // Primary color
        red = 140;
        green = 90;
        blue = 90;

      // Third color
        redDef = 50;
        greenDef = 0;
        blueDef = 80;
      // DELAYVAL Change
        DELAYVAL = 30;  // Changes delayval to make a more dramatic blade opening/retracting

      // Pulsing Intensity Change
        pulsingStep = 0;
        pulseAmount = 0;

      modeName = "WHITE";
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
    randomEffectStartPos = random(minStartPos, maxStartPos);

  // *** Debugging ***
    Serial.println(randomEffectStartPos);

  // Set colors to the deflect values in selected area
  for(int j=randomEffectStartPos; j<randomEffectStartPos + blastSize; j++ ) {
    ledsF[j].setRGB( redDef, greenDef, blueDef);
    ledsB[j].setRGB( redDef, greenDef, blueDef);
    FastLED.show();
  }
  delay(100);
  while (!(redFade == red) || !(greenFade == green) || !(blueFade == blue)){ // While "color"Fade isn't the same as the original color:

    fadeValues(defFadeStep); // Get the fade values

    // Write the new fade colors to the deflect area    
    for(int j=randomEffectStartPos; j<randomEffectStartPos + blastSize; j++ ) {
      ledsF[j].setRGB( redFade, greenFade, blueFade);
      ledsB[j].setRGB( redFade, greenFade, blueFade);
      FastLED.show();

      pulsingAnimation(); // Play pulsinganimation, for more vareity
    }
  }
}

void meltTip(bool on) { // Changes the tipleds to the tipmelt color if called with true, changes them back if called with false.

  if (on) {// Changes the tip to Tipmelt color.

    // Defines the new tip colors to be used.
      redTemp = redTip;
      greenTemp = greenTip;
      blueTemp = blueTip;

    // *** Debugging ***
      Serial.println("MeltTip");

      for(int j=NUM_LEDSB; j>NUM_LEDSB - tipSize/2; j--) { // Turns on the first half of the tipmelt leds in the tipmelt color.
        ledsF[j].setRGB( redTemp, greenTemp, blueTemp);
        ledsB[j].setRGB( redTemp, greenTemp, blueTemp);
      }
      for(int j=NUM_LEDSB-tipSize/2; j>NUM_LEDSB - tipSize; j--) { // Turns on the second half of the tipmelt leds in a slightly brighter color.
        ledsF[j].setRGB( redTemp - 30, greenTemp - 5, blueTemp + 20);
        ledsB[j].setRGB( redTemp - 30, greenTemp - 5, blueTemp + 20);
      }

    // Tip is now On
      tipChange = ! tipChange;
      } 
  else { // Changes the tip back to the original color
    
    // Defines the new tip colors to be used.
      redTemp = red;
      greenTemp = green;
      blueTemp = blue;

    // *** Debugging ***
      Serial.println("MeltTipStopped");

      for(int j=NUM_LEDSB; j>NUM_LEDSB - tipSize; j--) {
        ledsF[j].setRGB( redTemp, greenTemp, blueTemp);
        ledsB[j].setRGB( redTemp, greenTemp, blueTemp);
      }

    // Tip is now Off
      tipChange = ! tipChange;      
    
     FastLED.show(); // Send the updated pixels to the blade.
  }
}
void lightningEffect(bool on) {

    redTemp = red;
    greenTemp = green;
    blueTemp = blue;

  // Turn off the MeltTip effect, since it will be running since we press the actionBtnState
    meltTip(false);
        
  if (on) {
    
    pulsingLowestTemp = pulsingLowest;
    pulsingStepTemp = pulsingStep;

    // How many spots the lightning hits
      lightningCount = random(minLightningCount - maxLightningCount);

    for (int i=0; i<lightningCount; i++ ) {
      randomEffectStartPos = random(minStartPos, maxStartPos2); // Where the effect starts
      lightningSize = random(minLightningSize-maxLightningSize); // How big the effect is

      for(int i=randomEffectStartPos; i<randomEffectStartPos + lightningSize; i++ ) { // Write one lightning spot
        ledsF[i].setRGB( redLightning, greenLightning, blueLightning);
        ledsB[i].setRGB( redLightning, greenLightning, blueLightning);
        
      }
    }
    FastLED.show();

    // Change Pulse values, for a more unstable effect
      pulsingStep = 5;
      pulsingLowest = 5;

    // Lightning Effect is now ON
      lightningOn = true;
    
    // *** Debugging ***
      Serial.println("UNLIMITED!!! POWAAAA!!!");
      
  }
  else {

    for(int i=0; i<NUM_LEDSF; i++) { // Resets the blade to original color
      ledsF[i].setRGB( redTemp, greenTemp, blueTemp);
      ledsB[i].setRGB( redTemp, greenTemp, blueTemp);
    }
    FastLED.show();

    // Change Pulse values back to normal
      pulsingStep = pulsingStepTemp;
      pulsingLowest = pulsingLowestTemp;

    // Lightning Effect is now OFF
      lightningOn = false;

    // *** Debugging ***
      Serial.println("***falls down reactorshaft***");
  }
  // *** Debugging ***
    Serial.println(redTemp);
    Serial.println(greenTemp);
    Serial.println(blueTemp);
    Serial.println(BRIGHTNESS);


}
void pulsingAnimation() { // Pulsing animation for variation

  pulsingLowest = BRIGHTNESS - pulsingStep; // pulsingLowest is BRIGHTNESS - pulsingStep. pulsingStep is set in setMode program.

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
    //Serial.println(pulsingBrightness);
  
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
