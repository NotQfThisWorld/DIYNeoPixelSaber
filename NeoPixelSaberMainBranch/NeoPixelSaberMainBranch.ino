#include <FastLED.h>

// Led Strip Pins
  const int DATA_PINF = 12;      // Front strip PIN // Nano: 10
  const int DATA_PINB = 13;      // Back strip PIN  // Nano: 11

// Number of Leds
  const int NUM_LEDSF = 24; // Front strip LED_COUNT  // Change 144 on the finished version, 24 on "test NeoPixel".
  const int NUM_LEDSB = 144; // Back strip LED_COUNT

// Brigthness
  const int BRIGHTNESS = 10;  // Change to higher value when done testing. MUST NOT BE TO HIGH! (Draws more current)

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
  int colormode;

// RANDOM NUMBERS
  // Random number for where to put the Blaster Deflect
    int randomNumber;

  // Random numbers for flickering
    int randomNumLow;
    int randomNumMid;
    int randomNumHigh;

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
  String color;

// Flickering Light Variables
  int flickerIntensity;
  int flickerBrightness;
  int flickerRange;

void setup() {

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

  setColor(); // Runs the Set Color program once, to make sure a color is selected before the user powers up his saber.

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
    else if (actionBtnState == HIGH) { // Runs the setcolor function if the action button is pressed.

        setColor(); // Cycle to another color

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
    
    // Flickering Light
      flickerAnimation();
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
    
    flickerAnimation(); // Play flickeranimation, for more vareity
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
    
    flickerAnimation(); // Play flickeranimation, for more vareity
  }

  // Blade is now Off
    bladeOff = ! bladeOff;

  delay(10);

  // Turn on red indicator:
    digitalWrite(redIndi, 1); 

  // *** Debugging ***
    Serial.println("Blade retracted");
  
}

void setColor() { // Sets color

  // Write on to green indicator to turn both off (see line 43);
    digitalWrite(greenIndi, 1);
  
  DELAYVAL = 10;  // Sets DELAYVAL to 10 (default). Can be changed in switch statement.
  flickerRange = 0; // Sets flickerRange to 0 (default)(this does not mean that it does not flicker). Can be changed in switch statement.

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
        greenDef = 240;
        blueDef = 240;
      // DELAYVAL Change
        DELAYVAL = 20;  // Changes delayval to make a more dramatic blade opening/retracting

      // Flicker Intensity Change
        flickerRange = 2; // Changes flickerRange since the red blade is kind of more "unstable"

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

      // Flicker Intensity Change
        flickerRange = 1; // Changes flickerRange (don't know why, I just think it might work)

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
        greenDef = 255;
        blueDef = 255;

      color = "AQUA";
      colormode++;
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

      color = "Magenta";
      colormode++;
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

      color = "PURPLE";
      colormode = 0;
    break;

  }
  delay(300);

  // Turn on led indicator after color is chosen
    digitalWrite(greenIndi, 0);
  
  // *** Debugging ***
    Serial.print("Color is now: ");
    Serial.println(color);
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

      flickerAnimation(); // Play flickeranimation, for more vareity
    }
  }
}

void flickerAnimation() { // Flicker animation for variation

  // Random numbers for low, medium and high flicker
    randomNumLow = random(1, 4);
    randomNumMid = random(1, 6);
    randomNumHigh = random(1, 8);

  // Choose flicker intensity range (determined by flickerRange variable, set in setColor() function)
  
    switch (flickerRange) {

      case 0: // Low flicker
        flickerIntensity = random(0, 2);

      // *** Debugging ***
        Serial.println("Low");
                
      break;

      case 1: // Medium flicker
        flickerIntensity = random(0, 3);

      // *** Debugging ***
        Serial.println("Medium");
        
      break;
        
      case 2: // High flicker
        flickerIntensity = random(0, 4);

      // *** Debugging ***
        Serial.println("High");

      break;      

    }

  // Switch to the new randomly selected brightness value

    switch (flickerIntensity) {

      case 0:
        flickerBrightness = BRIGHTNESS;
      break;

      case 1:
        flickerBrightness = BRIGHTNESS + randomNumLow;
      break;

      case 2:
        flickerBrightness = BRIGHTNESS + randomNumLow;
      break;

      case 3:
        flickerBrightness = BRIGHTNESS + randomNumMid;
      break;

      case 4:
        flickerBrightness = BRIGHTNESS + randomNumHigh;
      break;
           
    }
  // Send updated brightness
    FastLED.setBrightness(flickerBrightness);
    FastLED.show();
  
  // *** Debugging ***
    Serial.println(flickerBrightness);
  

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
