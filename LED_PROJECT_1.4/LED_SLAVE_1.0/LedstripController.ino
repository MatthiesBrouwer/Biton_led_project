LedstripController::LedstripController():
  color_values{
    //{RED, GREEN, BLUE}
    {0, 0, 0, "BLACK"},
    {255, 255, 255, "WHITE"},   //WHITE
    {255, 0, 0, "RED"},         //RED
    {255, 50, 0, "ORANGE"},    //ORANGE
    {255, 255, 0,"YELLOW"},     //YELLOW
    {0, 255, 0, "GREEN"},       //GREEN
    {17, 0, 255, "BLUE"},       //BLUE
    {100, 0, 160, "PURPLE"},    //PURPLE
    {50, 15, 0, "BROWN"},     //BROWN
              
  }{

    //Initialize the ESP as a slave device
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    esp_now_register_recv_cb(OnDataRecv);
    Serial.println(WiFi.macAddress());

    //Initialize the ledstrip as off
    ledstrip = Adafruit_NeoPixel(LED_AMOUNT, LEDSTRIP_PIN, NEO_GRB + NEO_KHZ800);
    for(int i = 0; i < MAX_COLORS; i++){
      active_colors[i] = 0;
    }
    
    ledstrip.begin();
    ledstrip.show();
    ledstrip.fill(ledstrip.Color(0, 0, 0), 0, LED_AMOUNT);
    ledstrip.show();
  }





void LedstripController::handleInput(){
  if(update_ready){
    update_ready = !update_ready;

    
    
  }
    
}
/*
void LedstripController::handleInput(){
  //if(update_ready){
    
  //}
  ledstrip.clear();
  ledstrip.show();
  ledstrip.setPixelColor( 0 , ledstrip.Color(
                                                    color_values[0].red,
                                                    color_values[0].green,
                                                    color_values[0].blue));

  ledstrip.fill(ledstrip.Color(
                                                    color_values[0].red,
                                                    color_values[0].green,
                                                    color_values[0].blue),
                                                    0,
                                                    LED_AMOUNT);
                                                    ledstrip.show();
  Serial.print(color_values[0].red); Serial.print(", ");
  Serial.print(color_values[0].green); Serial.print(", ");
  Serial.println(color_values[0].blue); 
  delay(500);
  ledstrip.fill(ledstrip.Color(
                                                    color_values[1].red,
                                                    color_values[1].green,
                                                    color_values[1].blue),
                                                    0,
                                                    LED_AMOUNT);
                                                    ledstrip.show();
                                                    Serial.print(color_values[1].red); Serial.print(", ");
  Serial.print(color_values[1].green); Serial.print(", ");
  Serial.println(color_values[1].blue);
  delay(500);
  ledstrip.fill(ledstrip.Color(
                                                    color_values[2].red,
                                                    color_values[2].green,
                                                    color_values[2].blue),
                                                    0,
                                                    LED_AMOUNT);
                                                    ledstrip.show();
                                                    Serial.print(color_values[2].red); Serial.print(", ");
  Serial.print(color_values[2].green); Serial.print(", ");
  Serial.println(color_values[2].blue);
  delay(500);
                                                   
}*/





/*LedstripController::LedstripController():
  color_values{
    //{RED, GREEN, BLUE}
    {0, 0, 0, "BLACK"},
    {255, 255, 255, "WHITE"},   //WHITE
    {255, 0, 0, "RED"},         //RED
    {255, 50, 0, "ORANGE"},    //ORANGE
    {255, 255, 0,"YELLOW"},     //YELLOW
    {0, 255, 0, "GREEN"},       //GREEN
    {17, 0, 255, "BLUE"},       //BLUE
    {100, 0, 160, "PURPLE"},    //PURPLE
    {50, 15, 0, "BROWN"},     //BROWN
              
  }{
    pinMode(ADD_BUTTON, INPUT);
    pinMode(BACK_BUTTON, INPUT);
    pinMode(RESET_BUTTON, INPUT);
    pinMode(BUTTON_POWER_PIN, OUTPUT);
    digitalWrite(BUTTON_POWER_PIN, HIGH);
    for(int i = 0; i < MAX_COLORS; i++){
      active_colors[i] = 0;
    }
    
    strip.begin();
    strip.show();
    strip.fill(strip.Color(0, 0, 0), 0, LED_AMOUNT);
    strip.show();
    delay(200);
    
  }

  


void LedstripController::updateLedstrip(){
  int led_position;
  if(color_amount > 0){
    if(!active){
      active = true;
    }
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
        //Serial.print(" - ");Serial.print(j + ((LED_AMOUNT / color_amount) * i));Serial.print(" = "); Serial.println(color_values[active_colors[i]].color); 
        }
      }
      //Serial.println("----------");
      strip.setBrightness(brightness_knob_state);

      if(speed_knob_state > 5 ){
        delay(MAX_SPEED - speed_knob_state);
        wheel_pos++;
        if(wheel_pos > LED_AMOUNT){
          wheel_pos = 0;
        }
      }

      strip.show();
    }
    else if(active){
      active = false;
      strip.fill(strip.Color(0, 0, 0), 0, LED_AMOUNT);
      strip.show();
    }
}


void LedstripController::handleInput(){
  add_button_pressed = digitalRead(ADD_BUTTON);
  back_button_pressed = digitalRead(BACK_BUTTON);
  reset_button_pressed = digitalRead(RESET_BUTTON);

  brightness_knob_state = map(analogRead(BRIGHTNESS_KNOB_PIN), 0, 1023, 0, 100);
  speed_knob_state = map(analogRead(SPEED_KNOB_PIN), 0, 1023, 0, MAX_SPEED);
  color_knob_state = map(analogRead(COLOR_KNOB_PIN), 0, 1023, 0, 8);
  // Serial.println(add_button_pressed);
  if(add_button_pressed && !add_button_state){
    add_button_state = true;
    if(color_amount < MAX_COLORS){
      //Serial.println("ADD PRESSED");
      Serial.print("NEW COLOR:  {");Serial.println(color_knob_state);
      //Serial.print(color_values[color_knob_state].red);Serial.print(", ");
      //Serial.print(color_values[color_knob_state].green);Serial.print(", ");
      //Serial.print(color_values[color_knob_state].blue);Serial.println("}");
      //analogWrite(LED_RED, color_values[color_knob_state].red);
      //analogWrite(LED_GREEN, color_values[color_knob_state].green);
      //analogWrite(LED_BLUE, color_values[color_knob_state].blue);
      
      active_colors[color_amount] = color_knob_state;
      color_amount++;
    }else{
      Serial.println("COLORS FULL!!!");
    }
    Serial.print("{");
    for(int i = 0; i< MAX_COLORS; i++){
      Serial.print(color_values[active_colors[i]].color_name[0]);Serial.print(", ");
    }Serial.println("}");
    Serial.print("COLOR AMOUNT:   ");Serial.println(color_amount);
  }
  else if(!add_button_pressed && add_button_state){
    add_button_state = false;
  }
  
  if(back_button_pressed && !back_button_state ){
    back_button_state = true;
    if(color_amount > 0){
      active_colors[color_amount - 1] = 0;
      color_amount--;
    }
    else{
      Serial.println("NO COLORS ACTIVE!!!");
    }
    Serial.print("{");
    for(int i = 0; i< MAX_COLORS; i++){
      Serial.print(color_values[active_colors[i]].color_name[0]);Serial.print(", ");
    }Serial.println("}");
    Serial.print("COLOR AMOUNT:   ");Serial.println(color_amount);
  }
  else if(!back_button_pressed && back_button_state){
    back_button_state = false;
  }
  
  if(reset_button_pressed && !reset_button_state){
    reset_button_state = true;
    if(color_amount > 0){
      for(int i = 0; i < color_amount; i++){
        active_colors[i] = 0;
      }
      color_amount = 0;
    }
    else{
      Serial.println("NO COLORS ACTIVE - reset");
    }
    Serial.print("{");
    for(int i = 0; i< MAX_COLORS; i++){
      Serial.print(color_values[active_colors[i]].color_name[0]);Serial.print(", ");
    }Serial.println("}");
    Serial.print("COLOR AMOUNT:   ");Serial.println(color_amount);
  }
  else if(!reset_button_pressed && reset_button_state){
    reset_button_state = false;
  }
  
  delay(10);
}*/
