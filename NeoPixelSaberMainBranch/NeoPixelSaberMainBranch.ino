#include <FastLED.h>

// Led Strip Pins
  const int DATA_PINF = 12;      // Front strip PIN // Nano: 7
  const int DATA_PINB = 13;      // Back strip PIN  // Nano: 8

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

// Int for which color is chosen
  int colormode;

// Random number for where to put the Blaster Deflect
  int randomNumber;

// How big the blasterdeflect area should be
  int blastSize = 5;

// Starting positions for blaster deflect
  const int maxStartPos = NUM_LEDSF - blastSize + 5;
  const int minStartPos = 5;

// String for which color is selected
  String color;

void setup() {

  // *** Debugging ***
    Serial.begin(9600);
  
  // Defining shockPin as INPUT  
    pinMode(shockPin, INPUT);

  // Start leds and set brightness
    FastLED.addLeds<NEOPIXEL, DATA_PINF>(ledsF, NUM_LEDSF); // Start Front LEDs
    FastLED.addLeds<NEOPIXEL, DATA_PINB>(ledsB, NUM_LEDSB); // Start Back LEDs
    FastLED.setBrightness(  BRIGHTNESS );  // Set Brightess

  // limit powerdraw to what the arduino/powersupply can supply
    FastLED.setMaxPowerInVoltsAndMilliamps(5,1000); // (Volts,Millamps);

  setColor(); // Runs the Set Color program once, to make sure a color is selected before the user powers up his saber.

}

void loop() {
  
  // State values determined by value comming from buttons/shock sensor
    shockState = digitalRead(shockPin);
    igniteBtnState = digitalRead(igniteButton);
    actionBtnState = digitalRead(actionButton);

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
  }    
}

void startBlade(int red, int green, int blue) { // Startanimation for blade

  for(int i=0; i<NUM_LEDSF; i++) { // Starts each pixel with the set color, with DELAYVAL between each

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

  // Turn off end-LED
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);

  
  for(int i=NUM_LEDSF; i--;) {  // Turns off all leds, one by one

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
    // DELAYVAL Change
      DELAYVAL = 15;  // Changes Delay between pixels ligthing up, to make a more "dramatic" blade opening/retracting

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
    // DELAYVAL Change
      DELAYVAL = 10;  // Changes Delay back to normal

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
      red = 220;
      green = 0;
      blue = 255;
    // Secondary color
      redTip = 255;
      greenTip = 0;
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

  // *** Debugging ***
    Serial.println(randomNumber);

  // Set colors to the deflect values in selected area
  for(int j=randomNumber; j<randomNumber + blastSize; j++ ) {
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
    for(int j=randomNumber; j<randomNumber + blastSize; j++ ) {
      ledsF[j].setRGB( redFade, greenFade, blueFade);
      ledsB[j].setRGB( redFade, greenFade, blueFade);
      FastLED.show();
    }
    delay(0);
  }  
}
