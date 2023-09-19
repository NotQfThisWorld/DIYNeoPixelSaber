#include <FastLED.h>

// Led Strip Pins
  const int DATA_PINF = 10, DATA_PINB = 11; 

// Sound Variables
  const int speakerPin = 5;
  int hum = 40, deflectSound = 400, igniteRetract = 200;

// Number of Leds
  const int NUM_LEDSF = 117, NUM_LEDSB = 117;

// Brigthness
  const int BRIGHTNESS = 150;

// Strip Objects  
  CRGB ledsF[NUM_LEDSF];
  CRGB ledsB[NUM_LEDSB];

// Delayvalue
  int DELAYVAL = 10; //Milliseconds between each pixel on the blade lighting up.

// SENSORS AND BUTTON PINS
  const int igniteButton = 2, actionButton = 4;

// State values for different sensor readings (either HIGH or LOW)
  int actionBtnState, igniteBtnState;

// BOOLEANS
  bool bladeOff = true; // Boolean to check if blade is on or not
  bool tipChange = false; // Boolean to check if tip is different color or not
  bool pulseDown = true; // Boolean to check if the blade is pulsing down or not

// Power-Button-Indicator LED Pins (if you use them.)
  const int redIndi = 8, greenIndi = 7;

// Note about the indicator LEDS:
  /* For some or other weird reason, the green and red input are switched (marked wrong I guess)
     But, they also behave quite weird too. When for example red is on, if I write a On signal to
     green, both leds turns off. If I write a off signal to red, while red is on, the green led
     turns on. Please feel free to change the code if you do not enconter the same problem. */

// COLOR VALUES

  // Primary color values, for blade
    int red, green, blue;

  // Secondary color values, for MeltTip
    int redTip, greenTip, blueTip;

  // Third color values, for blaster deflect
    int redDef, greenDef, blueDef;

  // Forth color values, for fade effect
    int redFade, greenFade, blueFade;

  // Fifth color values, for tip in meltTip()
    int tipRed, tipGreen, tipBlue;
    // Using a secondary set of variables for this, since for some or other weird reason, the FastLED.setRGB command would alter the *color*Tip variables.

// Int for which color is chosen
  int modeCase;

// Where to put the effects
  int randomEffectStartPos;

// Sizes
  // How big the blasterdeflect area should be
    const int blastSize = 25;
  // Tip size
    const int tipSize = 10; // Should be an even number.

// How fast the fade effect should be
  int defFadeStep = 20; // For blaster fade effect

// Starting positions for blaster deflect
  const int fromEgdePos = 5;
  const int minStartPos = fromEgdePos;
  const int maxStartPos = NUM_LEDSF - blastSize - fromEgdePos;

// String for which color is selected
  String modeName;

// pulsinging Light Variables
  int pulseLowest, pulseLow, pulsingBrightness, pulseAmount, pulseFreq;

// Millis Variables
  unsigned long msStart = 0;
  unsigned long msPrevPulseDown = 0, msPrevPulseUp = 0; // To check how long it is since it last pulsed
  unsigned long msPrevModeChange = 0, msModeInterval = 500; // To check how long it is since it last changed mode
  unsigned long msPrevBlaster = 0, msBlasterInterval = 500; // To check how long it is since it last played blaster deflect effect.
  unsigned long msPrevBtnLight = 0; // To check how long it is since the button light changed.

void setup() {

  // PulsingBrightness should be the same as BRIGTHNESS when the blade starts up...
    pulsingBrightness = BRIGHTNESS;  

  // *** Debugging ***
    Serial.begin(115200);

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

  // limit powerdraw of the strips to what the batteries can supply
    FastLED.setMaxPowerInVoltsAndMilliamps(5,2000); // (Volts,Millamps);

  setMode(); // Runs the setMode program once, to make sure a color is selected before the user powers up their saber.

  // Turn on Red indicator, to show that the lightsaber is fully powered on and ready to use
    digitalWrite(redIndi, 1);
    
}

void loop() {

  msStart = millis();
  // State values determined by value comming from buttons/shock sensor
    igniteBtnState = digitalRead(igniteButton);
    actionBtnState = digitalRead(actionButton);

  if (bladeOff) { // If blade off...
    if (igniteBtnState == HIGH) { //  Starts the blade if the ignite button is pressed, or if the shocksensor is disturbed (enables stab ignition)

      // Start Blade
        
        //modeCase = modeCase - 1;
        //setMode(); // Sets the colors once more, because of weird bug that causes it to reset after retracting blade.
        igniteBlade(red, green, blue);


    }
    else if (actionBtnState == HIGH && igniteBtnState == LOW && (msStart-msPrevModeChange > msModeInterval)) { // Runs the setMode function if the action button is pressed.

      // Cycle to another color/mode
        setMode();
        
        msPrevModeChange = msStart;



    }
    else if (actionBtnState == HIGH && igniteBtnState == HIGH) {

      // Some bonus feature maybe?
      
    }
    else if (msStart-msPrevBtnLight > 200){

    // Turn on led indicator after color is chosen
    digitalWrite(greenIndi, 0); 
    msPrevBtnLight = msStart;

    } 
  }
  else { // If blade is on...
    if (igniteBtnState == HIGH && actionBtnState == LOW) { // Retracts the blade if the ignite button is pressed.

      // Retract Blade
        retractBlade();

    }

    else if (actionBtnState == HIGH && igniteBtnState == LOW && tipChange && (msStart-msPrevBlaster > msBlasterInterval)) { // Checks if the knock switch has been disturbed. Runs blasterDeflect function if true.

      // Blaster Deflect Effect
        blasterDeflect();
      
      msPrevBlaster = msStart;

    }

    else if (actionBtnState == HIGH && tipChange && igniteBtnState == HIGH) { // Checks if actionbutton is pressed, and tipchange is true. Turns on tip if both true
    
      // Melttip on
        meltTip(true);
      
    }
    else if (actionBtnState == LOW && !tipChange && igniteBtnState == LOW) { // Checks if actionbutton is not pressed, and if tipchange is false. Turns of tip if both are false
    
      // Melttip off
        meltTip(false);

    }
    
    if (msStart-msPrevPulseDown > pulseFreq){
      
      pulsingAnimation();

      msPrevPulseDown = msStart;
    }   
  }  
}

void igniteBlade(int red, int green, int blue) { // Startanimation for blade

  tone(speakerPin, igniteRetract);

  // Set Brightess a little bit higher, for a bit of a "boom effect"
    FastLED.setBrightness(  BRIGHTNESS + 20);  

  // Turn off red indicator, to turn both leds on (see line 39)
    digitalWrite(redIndi, 0);
  
  int lastLoop = 0;

  for(int i=0; i<11; i++) { // Starts each pixel with the set color, with DELAYVAL between each
    for(int j=lastLoop; j<lastLoop + 10; j++){
    ledsF[j].setRGB( red, green, blue);
    ledsB[j].setRGB( red, green, blue);
    }

    FastLED.show();

    delay(DELAYVAL); // Pause before next pass through loop

    lastLoop = lastLoop + 10;
  } 

  for(int i=NUM_LEDSF-7; i<NUM_LEDSF; i++){

    ledsF[i].setRGB( red, green, blue);
    ledsB[i].setRGB( red, green, blue);
  }
    FastLED.show();

  // Blade is now On
    bladeOff = ! bladeOff; 
  delay(10);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Turn on green indicator
    digitalWrite(greenIndi, HIGH);

  // *** Debugging ***
    Serial.println("Blade opened");
  
    tone(speakerPin, hum);
  
}

void retractBlade() { // Retract animation for blade

  tone(speakerPin, igniteRetract, 200);

  // Setting Brightness back to default here, so the blade wont be on its "darker" end of its pulsing sequence.
    FastLED.setBrightness(BRIGHTNESS); 

  // Turn off green indicator, to turn both Leds on (see line 39)
    digitalWrite(greenIndi, 0);

  int lastLoop = NUM_LEDSF;

  for(int i=0; i<11; i++) {
    for(int j=lastLoop; j>lastLoop - 10; j--){
    ledsF[j] = CRGB::Black;
    ledsB[j] = CRGB::Black;
    }
    lastLoop = lastLoop - 10;
    FastLED.show();
    delay(DELAYVAL); // Pause before next pass through loop
  } 

  for(int i=7; i>-1; i--){
    ledsF[i] = CRGB::Black; 
    ledsB[i] = CRGB::Black;
  }
    FastLED.show();


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
      DELAYVAL = 15;  // How fast the blade ignites/retracts.
      pulseLow = 40; // The lowest brightness the pulsing can have during its cycle.
      pulseAmount = 20; // How much the brightness changes with each cycle.
      pulseFreq = 5; // How often the pulse cycle should start.

  switch (modeCase) {
    case 0: // BLUE
      // Primary color
        red = 0;
        green = 0;
        blue = 255;
      // Secondary color
        redTip = 255;
        greenTip = 0;
        blueTip = 100;
      // Third color
        redDef = 200;
        greenDef = 200;
        blueDef = 255;

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
        greenTip = 50;
        blueTip = 0;
      // Third color
        redDef = 255;
        greenDef = 240;
        blueDef = 240;
      // DELAYVAL Change
        DELAYVAL = 20;  // Changes delayval to make a more dramatic blade opening/retracting

      // Pulsing Intensity Change
        pulseLow = 130;
        pulseAmount = 40;
        pulseFreq = 5;

      modeName = "RED";
      modeCase++;
    break;

    case 2: // GREEN
      // Primary color
        red = 0;
        green = 255;
        blue = 0;
      // Secondary color
        redTip = 0;
        greenTip = 170;
        blueTip = 200;
      // Third color
        redDef = 200;
        greenDef = 255;
        blueDef = 200;

      // Pulsing Intensity Change
        pulseLow = 32;
        pulseAmount = 8;
        pulseFreq = 20;

      modeName = "GREEN";
      modeCase++;
    break;

    case 3: // GOLD
      // Primary color
        red = 255;
        green = 90;
        blue = 0;
      // Secondary color
        redTip = 200;
        greenTip = 40;
        blueTip = 0;
      // Third color
        redDef = 255;
        greenDef = 200;
        blueDef = 180;

      // Pulsing Intensity Change
        pulseLow = 60;
        pulseAmount = 10;
        pulseFreq = 5;

      modeName = "GOLD";
      modeCase++;
    break;

    case 4: // ORANGE
      // Primary color
        red = 200;
        green = 10;
        blue = 0;
      // Secondary color
        redTip = 200;
        greenTip = 50;
        blueTip = 0;
      // Third color
        redDef = 255;
        greenDef = 200;
        blueDef = 100;
      // DELAYVAL Change
        DELAYVAL = 20;  // Changes delayval to make a more dramatic blade opening/retracting

      // Pulsing Intensity Change
        pulseLow = 100;
        pulseAmount = 50;  
        pulseFreq = 5;    

        modeName = "ORANGE";
        modeCase ++;
    break;

    case 5: // AQUA
      // Primary color
        red = 0;
        green = 200;
        blue = 200;
      // Secondary color
        redTip = 200;
        greenTip = 20;
        blueTip = 100;
      // Third color
        redDef = 240;
        greenDef = 255;
        blueDef = 255;
      
      // Pulsing Intensity Change
        pulseLow = 60;
        pulseAmount = 15;
        pulseFreq = 10;

        modeName = "AQUA";
        modeCase++;
    break;

    case 6: // Magenta
      // Primary color
        red = 255;
        green = 0;
        blue = 100;
      // Secondary color
        redTip = 255;
        greenTip = 0;
        blueTip = 255;
      // Third color
        redDef = 240;
        greenDef = 230;
        blueDef = 255;

      // Pulsing Intensity Change
        pulseLow = 80;
        pulseAmount = 20;
        pulseFreq = 10;

      modeName = "Magenta";
      modeCase++;
    break;

    case 7: // PURPLE
      // Primary color
        red = 200;
        green = 0;
        blue = 255;
      // Secondary color
        redTip = 255;
        greenTip = 0;
        blueTip = 100;
      // Third color
        redDef = 255;
        greenDef = 200;
        blueDef = 255;
      // DELAYVAL Change
        DELAYVAL = 18;  // Changes delayval to make a more dramatic blade opening/retracting

      // Pulsing Intensity Change
        pulseLow = 80;
        pulseAmount = 20;
        pulseFreq = 5;
      
      modeName = "PURPLE";
      modeCase ++;
    break;

    case 8: // WHITE
      // Primary color
        red = 160;
        green = 90;
        blue = 90;
      // Secondary color
        redTip = 240;
        greenTip = 0;
        blueTip = 255;
      // Third color
        redDef = 240;
        greenDef = 0;
        blueDef = 255;
      // DELAYVAL Change
        DELAYVAL = 18;  // Changes delayval to make a more dramatic blade opening/retracting

      // Pulsing Intensity Change
        pulseLow = 0;
        pulseAmount = 0;

      modeName = "WHITE";
      modeCase ++;
    break;

    case 9:
      modeCase = 0;
      setMode();
    break;

  }
  
  // *** Debugging ***
    Serial.print("Selected Mode: ");
    Serial.println(modeName);

}

void blasterDeflect() { // Blaster Deflect Effect

  FastLED.setBrightness(BRIGHTNESS); // Setting Brightness back to default here, so the blade wont be on its "darker" end of its pulsing sequence.

  tone(speakerPin, deflectSound, 100);

  // Forth color values, for fade effect
    redFade = redDef;
    greenFade = greenDef;
    blueFade = blueDef;

  // Chooses a random position for the deflect animation
    randomEffectStartPos = random(minStartPos, maxStartPos);

  // *** Debugging ***
    //Serial.println(randomEffectStartPos);

  // Set colors to the deflect values in selected area
  for(int j=randomEffectStartPos; j<randomEffectStartPos + blastSize; j++ ) {
    ledsF[j].setRGB( redDef, greenDef, blueDef);
    ledsB[j].setRGB( redDef, greenDef, blueDef);
  }
  FastLED.show();
  delay(50);
  while (!(redFade == red) || !(greenFade == green) || !(blueFade == blue)){ // While "color"Fade isn't the same as the original color:

    fadeValues(defFadeStep); // Get the fade values

    // Write the new fade colors to the deflect area    
    for(int j=randomEffectStartPos; j<randomEffectStartPos + blastSize; j++ ) {
      ledsF[j].setRGB( redFade, greenFade, blueFade);
      ledsB[j].setRGB( redFade, greenFade, blueFade);
      

      
    }
    FastLED.show();

    delay(5);

    tone(speakerPin, hum);
  }
}

void meltTip(bool on) { // Changes the tipleds to the tipmelt color if called with true, changes them back if called with false.

  if (on) {// Changes the tip to Tipmelt color.

    // Defines the new tip colors to be used.
      tipRed = redTip;
      tipGreen = greenTip;
      tipBlue = blueTip;

    // *** Debugging ***
      Serial.println("MeltTip");

      for(int j=NUM_LEDSB-1; j>NUM_LEDSB - tipSize; j--) { // Turns on the first half of the tipmelt leds in the tipmelt color.
        ledsF[j].setRGB( tipRed, tipGreen, tipBlue);
        ledsB[j].setRGB( tipRed, tipGreen, tipBlue);
      }

    // Tip is now On
      tipChange = ! tipChange;
      } 
  else { // Changes the tip back to the original color
    
    // Defines the new tip colors to be used.
      tipRed = red;
      tipGreen = green;
      tipBlue = blue;

    // *** Debugging ***
      Serial.println("MeltTipStopped");

      for(int j=NUM_LEDSB; j>NUM_LEDSB - tipSize; j--) {
        ledsF[j].setRGB( tipRed, tipGreen, tipBlue);
        ledsB[j].setRGB( tipRed, tipGreen, tipBlue);
      }

    // Tip is now Off
      tipChange = ! tipChange;      
    
     FastLED.show(); // Send the updated pixels to the blade.
  }
}

void pulsingAnimation() { // Pulsing animation for variation

  pulseLowest = BRIGHTNESS - pulseLow; // pulseLowest is BRIGHTNESS - pulseLow. pulseLow is set in setMode program.

  if (pulseDown) {

    pulsingBrightness = pulsingBrightness - pulseAmount;

    if (pulsingBrightness < pulseLowest) {
      pulseDown = ! pulseDown;
    }
  }
  else {

    pulsingBrightness = pulsingBrightness + pulseAmount;

    if (pulsingBrightness >= BRIGHTNESS) {
      pulseDown = ! pulseDown;
    }
  }

  // Send updated brightness
    FastLED.setBrightness(pulsingBrightness);
    FastLED.show();
  
  // *** Debugging ***
  // Serial.println(pulsingBrightness);
  
}

void fadeValues(int fadestep) { // Changes *color*fade values

  if (redFade > red){
    redFade = redFade - fadestep; // Remove fadestep from redFade value
  }
  if (greenFade > green){
    greenFade = greenFade - fadestep; // Remove fadestep from greenFade value
  }
  if (blueFade > blue){
    blueFade = blueFade - fadestep; // Remove fadestep from blueFade value
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
