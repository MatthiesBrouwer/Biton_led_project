WifiController::WifiController(InterfaceState &interface_state):
  interface_state(interface_state)
  {
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  //esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, ledstrip_garderobe_address, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  Serial.print(peerInfo.peer_addr[0], HEX); Serial.print(", ");
  Serial.print(peerInfo.peer_addr[1], HEX); Serial.print(", ");
  Serial.print(peerInfo.peer_addr[2], HEX); Serial.print(", ");
  Serial.print(peerInfo.peer_addr[3], HEX); Serial.print(", ");
  Serial.print(peerInfo.peer_addr[4], HEX); Serial.print(", ");
  Serial.println(peerInfo.peer_addr[5], HEX); 
  
}

void WifiController::sendInterfaceState(){
  esp_err_t result = esp_now_send(ledstrip_garderobe_address, (uint8_t *) &interface_state, sizeof(interface_state));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
