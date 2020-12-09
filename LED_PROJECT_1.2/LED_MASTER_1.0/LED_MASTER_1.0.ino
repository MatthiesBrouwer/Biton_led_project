#include <Adafruit_NeoPixel.h>
#include <esp_now.h>
#include <WiFi.h>
#include <memory>


//MAC ADDRESS 1: 4C:11:AE:EA:6C:DC
//MAC ADDRESS 2: 4C:11:AE:EA:6C:B8

 
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

typedef struct LedColor{
  int red;
  int green;
  int blue;
  String color_name;
} LedColor;


typedef struct InterfaceState {
    bool add_button = false;//_state = false;
    bool back_button = false;//_state = false;
    bool reset_button = false;//_state = false;
    long int color_knob;
    long int speed_knob;
    long int brightness_knob;
} InterfaceState;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

class ledstrip_station{
private:
  char *station_name;
  uint8_t station_address[6];
  esp_now_peer_info_t peer_info;
  InterfaceState interface_state;
};


class WifiController{
private:
  uint8_t ledstrip_garderobe_address[6] = {0x4C, 0x11, 0xAE, 0xEA, 0x6C, 0xB8};
  InterfaceState &interface_state;
  esp_now_peer_info_t peerInfo;

  int peers_connected = 0;
  
public:
  WifiController(InterfaceState &interface_state);

  void sendInterfaceState();
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
  InterfaceState interface_state;
  WifiController wifi_controller;
  
public:
  LedstripController();
  
  void handleInput();
  void updateLedstrip();
  void updateLedRGB();

  void TESTSTRIP();
  void TESTWIFI();
};

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
//----------------------------------------------


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;



void LedstripController::TESTWIFI(){
  for(;;){
    interface_state.add_button = false;
    interface_state.back_button = true;
    interface_state.reset_button = false;
    interface_state.color_knob = random(0,9);
    interface_state.speed_knob = random(0,1023);
    interface_state.brightness_knob = random(0,1023);

    wifi_controller.sendInterfaceState();

    delay(2000);
  }
  


}


/*
uint8_t broadcastAddress[] = {0x4C, 0x11, 0xAE, 0xEA, 0x6C, 0xB8};
  
typedef struct InterfaceState {
    bool add_button = false;//_state = false;
    bool back_button = false;//_state = false;
    bool reset_button = false;//_state = false;
    //long int color_knob_state;
    //long int speed_knob_state;
    //long int brightness_knob_state;
} InterfaceState;


InterfaceState interface_state;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void LedstripController::TESTWIFI(){
  WiFi.mode(WIFI_STA);

  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  
  
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  //strcpy(myData.a, "THIS IS A CHAR");
  //interface_state.add_button_state = false;
  //interface_state.back_button_state = true;
  //interface_state.reset_button_state = false;
  interface_state.add_button = false;
  interface_state.back_button = true;
  interface_state.reset_button = false;
  //interface_state.color_knob_state = random(0,9);
  //interface_state.speed_knob_state = random(0,1023);
  //interface_state.brightness_knob_state = random(0,1023);
  
  for(;;){
    //interface_state.add_button_state = !interface_state.add_button_state;
    //interface_state.back_button_state = !interface_state.back_button_state;
    //interface_state.reset_button_state = !interface_state.reset_button_state;
    interface_state.add_button = !interface_state.add_button;
    interface_state.back_button = !interface_state.back_button;
    interface_state.reset_button = !interface_state.reset_button;
    //interface_state.color_knob_state = random(0,9);
    //interface_state.speed_knob_state = random(0,1023);
    //interface_state.brightness_knob_state = random(0,1023);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &interface_state, sizeof(interface_state));
    if (result == ESP_OK) {
    Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    delay(2000);

  }
  
}*/
//-----------------------------------------------



void setup() {
  Serial.begin(112500);
  LedstripController ledstrip_controller;
  ledstrip_controller.TESTWIFI();
  //Serial.flush();
  //Serial.println("ENTERED SETUP");
  //digitalRead(ADD_BUTTON);
  //TESTLEDSTRIP();
  //delay(500);
  Serial.println(WiFi.macAddress());
}

void loop() {
  
  //ledstrip_controller.handleInput();
  //ledstrip_controller.updateLedstrip();
  //ledstrip_controller.updateLedRGB();
}
