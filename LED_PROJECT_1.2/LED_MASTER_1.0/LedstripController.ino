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
              
  },
  wifi_controller{interface_state}{
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

void LedstripController::updateLedRGB(){
  //analogWrite(RGBLED_RED_PIN, color_values[color_knob_state].red);
  //analogWrite(RGBLED_GREEN_PIN, color_values[color_knob_state].green);
 // analogWrite(RGBLED_BLUE_PIN, color_values[color_knob_state].blue);
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
  
  if(add_button_pressed && !add_button_state){
    add_button_state = true;
    if(color_amount < MAX_COLORS){
      Serial.print("NEW COLOR:  {");Serial.println(color_knob_state);
      
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
}
