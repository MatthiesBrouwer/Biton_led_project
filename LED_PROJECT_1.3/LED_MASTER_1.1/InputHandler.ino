int led_state = 0;


InterfaceState InputHandler::getInterfaceState(){
  InterfaceState interface_state;
  
  interface_state.add_button_state = digitalRead(ADD_BUTTON);
  interface_state.back_button_state = digitalRead(BACK_BUTTON);
  interface_state.reset_button_state = digitalRead(RESET_BUTTON);

  interface_state.color_knob_state = map(analogRead(COLOR_KNOB_PIN), 0, 1023, 0, 8);
  interface_state.speed_knob_state = map(analogRead(SPEED_KNOB_PIN), 0, 1023, 0, MAX_SPEED);
  interface_state.brightness_knob_state = map(analogRead(BRIGHTNESS_KNOB_PIN), 0, 1023, 0, 100);

  Serial.print(digitalRead(ADD_BUTTON)); Serial.print(", ");
  Serial.print(digitalRead(BACK_BUTTON)); Serial.print(", ");
  Serial.print(digitalRead(RESET_BUTTON)); Serial.print(", ");
   Serial.print(map(analogRead(COLOR_KNOB_PIN), 0, 4095, 0, 8)); Serial.print(", ");
  Serial.print(map(analogRead(SPEED_KNOB_PIN), 0, 4095, 0, MAX_SPEED)); Serial.print(", ");
  Serial.println( map(analogRead(BRIGHTNESS_KNOB_PIN), 0, 4095, 0, 100) );
  /*
  
Serial.print(analogRead(COLOR_KNOB_PIN)); Serial.print(", ");
  Serial.print(analogRead(SPEED_KNOB_PIN)); Serial.print(", ");
  Serial.println(analogRead(BRIGHTNESS_KNOB_PIN));
Serial.println(led_state);

  digitalWrite(5, HIGH);
  led_state = !led_state;
  delay(500);
  digitalWrite(5, LOW);
  delay(500);
  */

  return interface_state;
}
