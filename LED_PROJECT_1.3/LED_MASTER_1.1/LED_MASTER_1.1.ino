#include <Adafruit_NeoPixel.h>
#include <esp_now.h>
#include <WiFi.h>
#include <memory>

//MAC ADDRESS 1: 4C:11:AE:EA:6C:DC
//MAC ADDRESS 2: 4C:11:AE:EA:6C:B8
 
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define MAX_STATIONS  20
#define MAX_SPEED 40

#define BRIGHTNESS_KNOB_PIN 35
#define SPEED_KNOB_PIN 34
#define COLOR_KNOB_PIN 32

#define ADD_BUTTON 33
#define BACK_BUTTON 25
#define RESET_BUTTON 26

/*
 * InterfaceState
 *  This class holds the interface state for a specific station.
 *  The state of a specific interface can be changed
 *  when an actor interacts with the "Theekransje" ledstrips interface
 *  while its pointing to its corresponding station index.
 */
typedef struct InterfaceState {
    bool add_button_state = false;
    bool back_button_state = false;
    bool reset_button_state = false;
    long int color_knob_state = 0;
    long int speed_knob_state = 0;
    long int brightness_knob_state = 0;

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


/*
 * LedstripStation
 *  This struct holds all needed information held by a specific station,
 *  including its name to display, its wifi address, the peer info used
 *  by the wifi controller and its interface state.
 */

typedef struct LedstripStation {
    char *station_name;
    uint8_t station_address[6];
    esp_now_peer_info_t peer_info;
    InterfaceState interface_state;

    LedstripStation(){};
    LedstripStation(char * station_name, uint8_t* Station_address):
      station_name(station_name){
        for(int i = 0; i < 6; i++){
          station_address[i] = Station_address[i];
        }
        memcpy(peer_info.peer_addr, station_address, 6);
        peer_info.channel = 0;
        peer_info.encrypt = false;
        peer_info.ifidx = ESP_IF_WIFI_STA;
        //if (esp_now_add_peer(&peer_info) != ESP_OK){
        //  Serial.println("Failed to add peer");
        //  return;
        //}
      }
}LedstripStation;

/*
 * StationStorage
 *  This class functions as a central container to hold all currently
 *  active ledstrip stations. It's used as a convenient way of storing
 *  and accessing all stations.
 */
class StationStorage{
private:
  LedstripStation* ledstrip_stations[MAX_STATIONS];
  int stations_active = 0;
public:
   StationStorage(){
      for(int i = 0; i < MAX_STATIONS; i++){
        ledstrip_stations[i] = NULL;
      }
    };
  int getActiveStations();
  bool addLedstripStation(uint8_t station_address[6], char* station_name);
  LedstripStation* getStation(uint8_t station_address[6]);
  LedstripStation* getStation(char* station_name);
  LedstripStation* getStation(int index);
  
};


/*
 * WifiController
 *  This class handles all outgoing connections to the slave instances.
 *  a new peer has to be added before data can be send to it.
 */
class WifiController{
public:
  WifiController();
  bool addPeer(esp_now_peer_info_t& peer_info);
  bool sendData(uint8_t* data_holder, uint8_t network_address[6]);
};


class InputHandler{
private:
  unsigned long last_debounce_time = 0;
  unsigned long debounce_delay = 50;
public:
  InterfaceState getInterfaceState();
};


/*
 * MasterController
 *  This class functions as the central controller of the entire
 *  system. It handles all operations involving the cooperation needed
 *  to help the system function. It alsoholds all other controller classes.
 */
class MasterController{
private:
  WifiController wifi_controller;
  StationStorage station_storage;
  InputHandler input_handler;
public:
  MasterController(){
    //ADD THE USED STATIONS BELOW
    //Example:
    //  uint8_t new_station_address[6] = {0x4C, 0x11, 0xAE, 0xEA, 0x6C, 0xB8};
    //  char* new_station_name = "Garderobe";
    //  storage.addLedstripStation(new_station_address, new_station_name);

    uint8_t new_station_address[6] = {0x4C, 0x11, 0xAE, 0xEA, 0x6C, 0xB8};
    char* new_station_name = "Garderobe";
    station_storage.addLedstripStation(new_station_address, new_station_name);
    wifi_controller.addPeer(station_storage.getStation(0)->peer_info);
  }

  void MainRun();

};

void setup(){

  Serial.begin(112500);
  Serial.println("STARTING");

  pinMode(33, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  
  MasterController master_controller;
  master_controller.MainRun();
}

void loop(){
  
}
