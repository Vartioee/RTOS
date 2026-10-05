#define RGB_BUILTIN 38 
#define RGB_BRIGHTNESS 50 
 
void setup() { 
} 
 
void loop() { 
  // LED ON 
  neopixelWrite(RGB_BUILTIN, 0, 50, 50);
  delay(1000); 
 
  // LED OFF 
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); 
  delay(1000); 

    // LED ON 
  neopixelWrite(RGB_BUILTIN, 50, 0, 50);
  delay(700); 
 
  // LED OFF 
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); 
  delay(700); 

    // LED ON 
  neopixelWrite(RGB_BUILTIN, 50, 50, 50);
  delay(500); 
 
  // LED OFF 
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); 
  delay(500); 
} 

