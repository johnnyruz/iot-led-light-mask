// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


#define PIN D3
#define RING_PIN D6
#define PIXEL_TYPE WS2812B

#define TEST_LED D7

#define NUM_LEDS 16
#define RING_LEDS 12

#define BRIGHTNESS 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, PIXEL_TYPE);
Adafruit_NeoPixel ring = Adafruit_NeoPixel(RING_LEDS, RING_PIN, PIXEL_TYPE);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


volatile byte isMotion = LOW;
volatile byte isRainbow = LOW;



int stripPos = 0;
int ringPos = 0;
int stripDir = 1;
int rainbowCyclePos;

int larsonSpeedInput = 0;
long larsonDelay = 115;

int redValOut = 255;
int grnValOut = 255;
int bluValOut = 255;

int redVal = 255;
int grnVal = 255;
int bluVal = 255;

//variables for returning current status
String conditions = "TEST!";
float brightness = 255;
int speedInt = 500;
int colorInt = 500;


void setup() {
    
  Particle.variable("conditions", conditions);
  Particle.function("speed",setSpeed);
  Particle.function("color",setColor);
  Particle.function("motion",motionToggle);
  Particle.function("rainbow",rainbowToggle);
  Particle.function("bright", setBright);
  
  
  pinMode(TEST_LED, OUTPUT);

  Serial.begin(9600);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  ring.begin();
  ring.show();
  
  conditions = "FIRST!";
  setConditions();

}

void loop() {

      if(isMotion == HIGH && isRainbow == HIGH){
         
         uint16_t i;

        if(rainbowCyclePos == 256)
          rainbowCyclePos = 0;

        for(i=0; i< strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + rainbowCyclePos) & 255));
        }
        strip.show();
        
        for(i=0; i< ring.numPixels(); i++) {
          ring.setPixelColor(i, Wheel(((i * 256 / ring.numPixels()) + rainbowCyclePos) & 255));
        }
        ring.show();
        
        rainbowCyclePos = rainbowCyclePos + 1;
        delay(larsonDelay / 20);
           
      }
      else if(isMotion == LOW && isRainbow == HIGH){

        uint16_t i;

        for(i=0; i< strip.numPixels(); i++) {
              strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels()) & 255));
        }
        strip.show();

        for(i=0; i< ring.numPixels(); i++) {
              ring.setPixelColor(i, Wheel((i * 256 / ring.numPixels()) & 255));
        }
        ring.show();
        
      }          
      else if(isMotion == HIGH && isRainbow == LOW)
      {
         
         //strips
         for(uint16_t i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0) );
         }
                
         strip.setPixelColor(stripPos, strip.Color(redValOut,grnValOut,bluValOut) );
         strip.setPixelColor(stripPos + 8, strip.Color(redValOut,grnValOut,bluValOut) );
    
         stripPos = stripPos + stripDir;
    
         if(stripPos == strip.numPixels() / 2.0 && stripDir == 1)
         {
             stripDir = -1;
             stripPos = stripPos - 2;
         }
         else if(stripPos == 0 && stripDir == -1)
         {
             stripDir = 1;
         }
    
         //ring 
         for(uint16_t i=0; i<ring.numPixels(); i++) {
            ring.setPixelColor(i, ring.Color(0,0,0) );
         }
    
         ring.setPixelColor(ringPos, ring.Color(redValOut,grnValOut,bluValOut) );
    
         ringPos = ringPos + 1;
    
         if(ringPos == ring.numPixels()){ ringPos = 0; }
    
         strip.show();
         ring.show();
         delay(larsonDelay);
         
      } 
      else if(isMotion == LOW && isRainbow == LOW){
    
        //strips
        
         for(uint16_t i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(redValOut,grnValOut,bluValOut) );
         }         

        //ring 
         for(uint16_t i=0; i<ring.numPixels(); i++) {
            ring.setPixelColor(i, ring.Color(redValOut,grnValOut,bluValOut) );
         }
         

         strip.show();
         ring.show();
         delay(5);
         
      }
      
}
//end loop

void setConditions() {
    
    //char myJSON[255] = "";
    //snprintf(myJSON, 255, "{\"current\":{\"motion\":%d,\"rainbow\":%d,\"speed\":%d,\"color\":%d,\"brightness\":%d}}", int(isMotion), int(isRainbow), int(speedInt), int(colorInt), int(brightness));
    //strcpy(conditions, myJSON);
    
    conditions = "{\"motion\":" + String(int(isMotion)) 
                    + ",\"rainbow\":" + String(int(isRainbow)) 
                    + ",\"speed\":" + String(int(speedInt)) 
                    + ",\"color\":" + String(int(colorInt)) 
                    + ",\"brightness\":" + String(int(brightness)) + "}";
    
}

int motionToggle(String command) {
    
    if (command=="on") {
        isMotion = HIGH;
        setConditions();
        return 1;
    }
    else if (command=="off") {
        isMotion = LOW;
        setConditions();
        return 0;
    }
    else {
        return -1;
    }
    
}

int rainbowToggle(String command) {
    
    if (command=="on") {
        isRainbow = HIGH;
        digitalWrite(TEST_LED, HIGH);
        setConditions();
        return 1;
    }
    else if (command=="off") {
        isRainbow = LOW;
        digitalWrite(TEST_LED, LOW);
        setConditions();
        return 0;
    }
    else {
        return -1;
    }
    
}

int setSpeed(String command) {
    
    int speed = command.toInt();
    speedInt = speed;
    
    if(speed < 1 || speed > 1024){
        return -1;
    }
    
    larsonSpeedInput = 1024 - speed;
    larsonDelay = ((larsonSpeedInput * 150.0) / 1023.0) + 30.0;
      
    setConditions();  
    return speed;
    
}

int setBright(String command) {
    
    float bright = command.toFloat();
    
    if(bright < 1 || bright > 255){
        return -1;
    }
    
    redValOut = float(redVal) * (bright / 255.0);
    grnValOut = float(grnVal) * (bright / 255.0);
    bluValOut = float(bluVal) * (bright / 255.0);
      
    brightness = bright;
    
    setConditions();
    return bright;
    
}

int setColor(String command) {
    
    int colorPotVal = command.toInt();
    colorInt = colorPotVal;
    
    if(colorPotVal < 1 || colorPotVal > 1024){
        return -1;
    }
    
    if (colorPotVal < 341)  // Lowest third of the potentiometer's range (0-340)
      {                  
        colorPotVal = (colorPotVal * 3) / 4; // Normalize to 0-255
    
        redVal = 255 - colorPotVal;  // Red from full to off
        grnVal = colorPotVal;        // Green from off to full
        bluVal = 1;             // Blue off
        
        if(redVal == 0) redVal = 1;
        
      }
      else if (colorPotVal < 682) // Middle third of potentiometer's range (341-681)
      {
        colorPotVal = ( (colorPotVal-341) * 3) / 4; // Normalize to 0-255
    
        redVal = 1;            // Red off
        grnVal = 255 - colorPotVal; // Green from full to off
        bluVal = colorPotVal;       // Blue from off to full
        
        if(grnVal == 0) grnVal = 1;
        
      }
      else  // Upper third of potentiometer"s range (682-1023)
      {
        colorPotVal = ( (colorPotVal-683) * 3) / 4; // Normalize to 0-255
    
        redVal = colorPotVal;       // Red from off to full
        grnVal = 1;            // Green off
        bluVal = 255 - colorPotVal; // Blue from full to off
        
        if(bluVal == 0) bluVal = 1;
        
      }
      
    redValOut = float(redVal) * (brightness / 255.0);
    grnValOut = float(grnVal) * (brightness / 255.0);
    bluValOut = float(bluVal) * (brightness / 255.0);
    
    setConditions();
    return colorInt;
    
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  float rainbowBrightness = (brightness / 255.0);
  int r = 0;
  int g = 0;
  int b = 0;
  
  if(WheelPos < 85) {
    r = (255.0 - float(WheelPos) * 3.0) * rainbowBrightness;
    g = 0;
    b = (float(WheelPos) * 3.0) * rainbowBrightness;
  }
  else if(WheelPos < 170) {
    WheelPos -= 85;
    r = 0;
    g = (float(WheelPos) * 3.0) * rainbowBrightness;
    b = (255.0 - float(WheelPos) * 3.0) * rainbowBrightness;
  }
  else {
    WheelPos -= 170;
    r = (float(WheelPos) * 3.0) * rainbowBrightness;
    g = (255.0 - float(WheelPos) * 3.0) * rainbowBrightness;
    b = 0;
  }
  
  return strip.Color(r, g, b, 0);

}


