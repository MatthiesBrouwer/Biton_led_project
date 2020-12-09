void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

WifiController::WifiController(){
  //Initialize the ESP as wifi station
  WiFi.mode(WIFI_STA);

  //Initialize the ESP_NOW library
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //Register a Send function to be called when sending data
  esp_now_register_send_cb(OnDataSent);
}

bool WifiController::addPeer(esp_now_peer_info_t& peer_info){
  if (esp_now_add_peer(&peer_info) != ESP_OK){
    Serial.println("Failed to add peer");
    return false;
  }
  Serial.print("PEER ADDED: ");
  Serial.print(peer_info.peer_addr[0], HEX); Serial.print(", ");
  Serial.print(peer_info.peer_addr[1], HEX); Serial.print(", ");
  Serial.print(peer_info.peer_addr[2], HEX); Serial.print(", ");
  Serial.print(peer_info.peer_addr[3], HEX); Serial.print(", ");
  Serial.print(peer_info.peer_addr[4], HEX); Serial.print(", ");
  Serial.println(peer_info.peer_addr[5], HEX); 
  return true;
}

bool WifiController::sendData(uint8_t* data_holder,  
                              uint8_t network_address[6]){
  Serial.println("SENDING: ");
  Serial.print("ADDRESS:  ");Serial.print(network_address[0], HEX); Serial.print(", ");
  Serial.print(network_address[1], HEX); Serial.print(", ");
  Serial.print(network_address[2], HEX); Serial.print(", ");
  Serial.print(network_address[3], HEX); Serial.print(", ");
  Serial.print(network_address[4], HEX); Serial.print(", ");
  Serial.println(network_address[5], HEX);
  Serial.print("DATA:     ");Serial.println((char*)data_holder);
  Serial.print("SIZE:     ");Serial.println(sizeof(data_holder), DEC);
  Serial.println();
  
  if(!esp_now_is_peer_exist(network_address)){
    Serial.println("PEER NOT REGISTERED");
    return false;
  }

  esp_err_t result = esp_now_send(network_address, 
                                 data_holder, 
                                  sizeof(data_holder));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  
}
