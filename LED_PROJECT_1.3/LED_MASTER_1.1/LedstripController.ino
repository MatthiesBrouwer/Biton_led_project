

void MasterController::MainRun(){
  InterfaceState interface_state_holder;
  for(;;){
    interface_state_holder = input_handler.getInterfaceState();
    if(!(station_storage.getStation(0)->interface_state == interface_state_holder)){
      station_storage.getStation(0)->interface_state = interface_state_holder;
      wifi_controller.sendData((uint8_t*)&station_storage.getStation(0)->interface_state,
                             station_storage.getStation(0)->station_address);
    }

    delay(100);
  }
}
