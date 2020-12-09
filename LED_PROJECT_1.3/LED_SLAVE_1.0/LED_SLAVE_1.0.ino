#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <esp_now.h>
#include <memory>

#ifdef __AVR__
  #include <avr/power.h>
#endif


#define MAX_COLORS  20
#define MAX_SPEED 40

#define LEDSTRIP_PIN 21
#define LED_AMOUNT 20



struct LedColor{
  int red;
  int green;
  int blue;
  //uint32_t color;
  String color_name;
};

typedef struct InterfaceState {
    bool add_button_state = false;
    bool back_button_state = false;
    bool reset_button_state = false;
    long int color_knob_state = false;
    long int speed_knob_state = false;
    long int brightness_knob_state = false;

    InterfaceState & operator=(const InterfaceState& new_interface) {
      add_button_state = new_interface.add_button_state;
      add_button_state = new_interface.back_button_state;
      add_button_state = new_interface.reset_button_state;
      add_button_state = new_interface.color_knob_state;
      add_button_state = new_interface.speed_knob_state;
      add_button_state = new_interface.brightness_knob_state;
      return *this;
    }

    bool Compare(InterfaceState compare_interface){
      if(add_button_state != compare_interface.add_button_state){
        return 0;
      }
      else if(back_button_state != compare_interface.back_button_state){
        return 0;
      }
      else if(reset_button_state != compare_interface.reset_button_state){
        return 0;
      }
      else if(color_knob_state != compare_interface.color_knob_state){
        return 0;
      }
      else if(speed_knob_state != compare_interface.speed_knob_state){
        return 0;
      }
      else if(brightness_knob_state != compare_interface.brightness_knob_state){
        return 0;
      }
      return 1;
      
    }

    bool operator==(InterfaceState compare_interface){
      return Compare(compare_interface);
    }
} InterfaceState;


InterfaceState received_interface;
bool update_ready = false;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&received_interface, incomingData, sizeof(received_interface));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("ADD: ");
  Serial.println(received_interface.add_button_state);
  Serial.print("BACK: ");
  Serial.println(received_interface.back_button_state);
  Serial.print("RESET: ");
  Serial.println(received_interface.reset_button_state);
  Serial.print("COLOR: ");
  Serial.println(received_interface.color_knob_state);
  Serial.print("SPEED: ");
  Serial.println(received_interface.speed_knob_state);
  Serial.print("BRIGHTNESS: ");
  Serial.println(received_interface.brightness_knob_state);
  Serial.println();

  update_ready = true;
}


class LedstripController{
private:
  LedColor color_values[10];
  int active_colors[MAX_COLORS];
  
  int color_amount = 0;
  int wheel_pos = 0;
  bool active = false;
  

  InterfaceState interface_state;

  Adafruit_NeoPixel ledstrip;
  
public:
  LedstripController();

  void handleInput();
  void updateLedstrip();

  void TESTSTRIP();
  void TESTWIFI();
};


//--------------------------------------------------------


/*
void LedstripController::TESTWIFI(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println(WiFi.macAddress());
  
  
  
}*/

//------------------------------------------



void setup() {
  Serial.begin(112500);
  
  //Serial.flush();
  //Serial.println("ENTERED SETUP");
  //digitalRead(ADD_BUTTON);
  //ledstrip_controller.TESTWIFI();
  delay(500);
}

//LedstripController ledstrip_controller;
void loop() {
  LedstripController ledstrip_controller;
  for(;;){
    ledstrip_controller.handleInput();
  }
  //ledstrip_controller.initWifi();
  //ledstrip_controller.handleInput();
  //ledstrip_controller.updateLedstrip();
  //ledstrip_controller.updateLedRGB();
  //ledstrip_controller.TESTSTRIP();
  //delay(10);
  //Serial.flush();
  //Serial.println("ENTERED SETUP");
  
}



/*
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
    delay(1000);
    //strip.setPixelColor(1, 0, 255, 0);
    //strip.show();
    //delay(100);
    strip.setPixelColor(1, 255, 0, 0);
    strip.show();
    delay(100);
  }
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
  Serial.println("ENTERED TESTING");
  strip.setPixelColor(1, 0, 255, 0);
  for(;;){
  
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
*/
