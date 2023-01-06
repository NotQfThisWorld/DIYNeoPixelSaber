#include <Adafruit_NeoPixel.h>

#define PINF 12      // Front strip PIN
// Nano: 7
#define PINB 13      // Back strip PIN
// Nano: 8

#define LED_COUNTF 24 // Front strip LED_COUNT
// Change 144 on the finished version, 24 on "test NeoPixel".

#define LED_COUNTB 144 // Back strip LED_COUNT

Adafruit_NeoPixel frontStrip(LED_COUNTF, PINF, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel backStrip(LED_COUNTB, PINB, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 20 // Milliseconds between each pixel on the blade lighting up.

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
  
  frontStrip.begin();
  backStrip.begin();
  frontStrip.show();
  backStrip.show();
  frontStrip.setBrightness(50);
  backStrip.setBrightness(50);

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

  for(int i=0; i<LED_COUNTF; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    frontStrip.setPixelColor(i, frontStrip.Color(red, green, blue));
    backStrip.setPixelColor(i, backStrip.Color(red, green, blue));

    frontStrip.show();   // Send the updated pixel colors to the hardware.
    backStrip.show();

    
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

    frontStrip.setPixelColor(i, frontStrip.Color(0, 0, 0));
    backStrip.setPixelColor(i, backStrip.Color(0, 0, 0));

    frontStrip.show();   // Send the updated pixel colors to the hardware.
    backStrip.show();

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
    red = 255;
    green = 255;
    blue = 255;
    Serial.println("Please Select Your Color");
    break;
    case 1:
    red = 0;
    green = 0;
    blue = 255;
    Serial.println("BLUE");
    break;
    case 2:
    red = 255;
    green = 0;
    blue = 0;
    Serial.println("RED");
    break;
    case 3:
    red = 0;
    green = 255;
    blue = 0;
    Serial.println("GREEN");
    break;
    case 4:
    red = 255;
    green = 100;
    blue = 0;
    Serial.println("Gold");
    break;
    case 5:
    red = 255;
    green = 0;
    blue = 255;
    Serial.println("PURPLE");
    colormode = 0;
    Serial.println("Looping back");
    break;
  }
  colormode++;
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