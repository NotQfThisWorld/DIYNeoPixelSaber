#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

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


//Adafruit_NeoPixel frontStrip(LED_COUNTF, PINF, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel backStrip(LED_COUNTB, PINB, NEO_GRB + NEO_KHZ800);

int DELAYVAL = 10; //Milliseconds between each pixel on the blade lighting up.

const int igniteButton = 2;
// Nano: 2


const int actionButton = 4;
// Nano: 4

int actionButtonState = 0;
int igniteButtonState = 0;

bool bladeOff = false;

const int redPin = 11;
// Nano: 3
const int greenPin = 10;
// Nano: 5 
const int bluePin = 9;
// Nano: 6

int red = 0;
int green = 0;
int blue = 0;

int colormode = 0;

void setup() {

  // For debugging
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, DATA_PINF>(ledsF, NUM_LEDSF);
  FastLED.addLeds<NEOPIXEL, DATA_PINB>(ledsB, NUM_LEDSB);

  FastLED.setBrightness(  BRIGHTNESS );  
  setColor();

}

void loop() {
  
  igniteButtonState = digitalRead(igniteButton);
  actionButtonState = digitalRead(actionButton);


  if (bladeOff) {
    if (igniteButtonState == HIGH) { // Checks if the blade should be on. Starts the blade if true
      // Start Blade
      startBlade(red, green, blue);


    }
    else if (actionButtonState == HIGH) {

      // Cycle to another color
      setColor();

    }  
  }
  else {
    if (igniteButtonState == LOW) { // Checks if the blade should be on. Retracts the blade if false

      // Retract Blade
      retractBlade();



    }
    else if (actionButtonState == HIGH) {
      // Blaster Deflect Effect
      blasterDeflect();

    }
    else if (actionButtonState == HIGH) {
      // Tip Drag Effect
      tipdrag();
     
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

  for(int i=24; i--;) {

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
    case 0:
    red = 0;
    green = 0;
    blue = 255;
    Serial.println("BLUE");
    colormode++;
    break;
    case 1:
    red = 255;
    green = 0;
    blue = 0;
    DELAYVAL = 15;
    Serial.println("RED");
    colormode++;
    break;
    case 2:
    red = 0;
    green = 255;
    blue = 0;
    DELAYVAL = 10;
    Serial.println("GREEN");
    colormode++;
    break;
    case 3:
    red = 255;
    green = 100;
    blue = 0;
    Serial.println("GOLD");
    colormode++;
    break;
    case 4:
    red = 255;
    green = 0;
    blue = 255;
    Serial.println("PURPLE");
    colormode = 0;
    Serial.println("Looping Back");
    break;
  }
  delay(1000);
}


void tipdrag() {

  // *** Debugging ***
  Serial.println("TipDrag"); 

}

void blasterDeflect() {

  // *** Debugging ***
  Serial.println("Deflect!");

}