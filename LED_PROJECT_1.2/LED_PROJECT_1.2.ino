#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define BRIGHTNESS_KNOB_PIN 3
#define SPEED_KNOB_PIN 2
#define COLOR_KNOB_PIN 1

#define ADD_BUTTON 5
#define BACK_BUTTON 4
#define RESET_BUTTON 3

#define RGBLED_RED_PIN 11
#define RGBLED_GREEN_PIN 10
#define RGBLED_BLUE_PIN 9

#define BUTTON_POWER_PIN 7

#define MAX_COLORS  20
#define MAX_SPEED 40

#define LEDSTRIP_PIN 2
#define LED_AMOUNT 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_AMOUNT, LEDSTRIP_PIN, NEO_GRB + NEO_KHZ800);

struct LedColor{
  int red;
  int green;
  int blue;
  //uint32_t color;
  String color_name;
};



class LedstripController{
private:
  LedColor color_values[10];
  int active_colors[MAX_COLORS];
  
  bool add_button_state = false;
  bool add_button_pressed = false;
  
  bool back_button_state = false;
  bool back_button_pressed = false;

  bool reset_button_state = false;
  bool reset_button_pressed = false;

  int brightness_knob_state = 0;
  int speed_knob_state = 0;
  int color_knob_state = 0;
  
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;
  int color_amount = 0;

  int wheel_pos = 0;

  bool active = false;
  
public:
  LedstripController();
  
  void handleInput();
  void updateLedstrip();
  void updateLedRGB();

  void TESTSTRIP();
};


void TESTKNOBS(){
  Serial.println("ENTERED TEST FUNCTION");
  for(;;){
    int new_led_brightness = map(analogRead(BRIGHTNESS_KNOB_PIN), 0, 1023, 0, 100);
    int new_led_speed = map(analogRead(SPEED_KNOB_PIN), 0, 1023, 0, 100);
    int new_led_color = map(analogRead(COLOR_KNOB_PIN), 0, 1023, 0, 9);
    
    Serial.print(new_led_brightness);Serial.print(",    ");
    Serial.print(new_led_speed);Serial.print(",   ");
    Serial.println(new_led_color);
  }
}

void TESTBUTTONS(){
  Serial.println("ENTERED TEST FUNCTION");
  for(;;){
    bool add_button_state = digitalRead(ADD_BUTTON);
    bool back_button_state = digitalRead(BACK_BUTTON);
    bool reset_button_state = digitalRead(RESET_BUTTON);
    int new_led_brightness = map(analogRead(BRIGHTNESS_KNOB_PIN), 0, 1023, 0, 100);
    int new_led_speed = map(analogRead(SPEED_KNOB_PIN), 0, 1023, 0, 100);
    int new_led_color = map(analogRead(COLOR_KNOB_PIN), 0, 1023, 0, 9);
    
    Serial.print(add_button_state);Serial.print(",    ");
    Serial.print(back_button_state);Serial.print(",   ");
    Serial.print(reset_button_state);Serial.print(",    ");
    Serial.print(new_led_brightness);Serial.print(",   ");
    Serial.print(new_led_speed);Serial.print(",   ");
    Serial.println(new_led_color);
  }
}


void TESTLEDSTRIP(){
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_AMOUNT, LEDSTRIP_PIN, NEO_RGB + NEO_KHZ800);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  uint16_t i, j;
  //strip.setPixelColor(0, 0, 0, 0);
  uint32_t magenta = strip.Color(255, 0, 0);
  uint32_t black = strip.Color(0, 255, 0);
  for(;;){
    //for(int i = 0; i < 40; i++){
      //strip.setPixelColor(i, 255, 0, 0);
      strip.fill(magenta, 0, 150);
      strip.fill(black, 150, 300);
      strip.show();
      
      delay(100);
      strip.fill(black, 0, 150);
      strip.fill(magenta, 150, 300);
      strip.show();
      
      delay(100);/*
      //strip.clear();
      //strip.setPixelColor(i, 0, 0, 0);
      //strip.show();
      //delay(50);
    }
    delay(2000);
    */
    //strip.setPixelColor(1, 0, 255, 255);
    /*strip.setPixelColor(2, 255, 0, 255);
    strip.setPixelColor(3, 255, 255, 0);
    strip.show();
    delay(1000);
    strip.setPixelColor(1, 255, 255, 0);
    strip.setPixelColor(2, 0, 255, 255);
    strip.setPixelColor(3, 255, 0, 255);
    strip.show();
    delay(1000);
    strip.setPixelColor(1, 255, 0, 255);
    strip.setPixelColor(2, 255, 255, 0);
    strip.setPixelColor(3, 0, 255, 255);
    strip.show();
    delay(1000);*/
    //strip.setPixelColor(1, 0, 255, 0);
    //strip.show();
    //delay(100);
    //strip.setPixelColor(1, 255, 0, 0);
    //strip.show();
    //delay(100);
  //}
}}



void LedstripController::TESTSTRIP(){
  color_amount = 9;
  active_colors[0] = 0;
  active_colors[1] = 1;
  active_colors[2] = 2;
  active_colors[3] = 3;
  active_colors[4] = 4;
  active_colors[5] = 5;
  active_colors[6] = 6;
  active_colors[7] = 7;
  active_colors[8] = 8;
  //Serial.println("ENTERED TESTING");
  //strip.setPixelColor(1, 0, 255, 0);
  //for(;;){
  
  float led_position = 0;
  if(color_amount > -1){
    for(int i = 0; i < color_amount; i++){
      for(float j = 0.0; j < LED_AMOUNT / color_amount; j++){
         led_position = j + ((LED_AMOUNT / color_amount) * i) + wheel_pos;
        
         if( led_position > LED_AMOUNT){
          led_position = -1 + (led_position - LED_AMOUNT);
         }
         strip.setPixelColor( led_position , strip.Color(
                                                    color_values[active_colors[i]].red,
                                                    color_values[active_colors[i]].green,
                                                    color_values[active_colors[i]].blue));
         //Serial.print(" - ");Serial.print(j + ((LED_AMOUNT / color_amount) * i));Serial.print(" = "); Serial.println(color_values[active_colors[i]].color_name);
         
      }
    }
    //Serial.println("----------");
    strip.show();
    delay(30);
    wheel_pos++;
    if(wheel_pos > LED_AMOUNT){
      wheel_pos = 0;
    }
  }
}



LedstripController ledstrip_controller;
void setup() {
  Serial.begin(112500);
  //Serial.flush();
  //Serial.println("ENTERED SETUP");
  //digitalRead(ADD_BUTTON);
  //TESTLEDSTRIP();
  delay(500);
}

//LedstripController ledstrip_controller;
void loop() {
  
  ledstrip_controller.handleInput();
  ledstrip_controller.updateLedstrip();
  ledstrip_controller.updateLedRGB();
  //ledstrip_controller.TESTSTRIP();
  //delay(10);
  //Serial.flush();
  //Serial.println("ENTERED SETUP");
}
