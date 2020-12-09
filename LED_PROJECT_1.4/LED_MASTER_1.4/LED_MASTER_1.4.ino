#include <WiFi.h>
//https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
const char* garderobe_ip_address;
const char* password = SUCCOI!!;

WiFiServer server(80);

String header;

String output_led_1 = "off";
String output_led_2 = "off";

const int pin_led_1 = 26;
const int pin_led_2 = 27;

unsigned long current_time = millis();

unsigned long  previous_time = 0;

const long timeout_time = 2000;

void setup(){
  Serial.begin(115200);

  pinMode(pin_led_1, OUTPUT);
  pinMode(pin_led_2, OUTPUT);

  digitalWrite(pin_led_1, LOW);
  digitalWrite(pin_led_2, LOW);

  Serial.print("connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);


  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient garderobe_client = server.available();

  if(garderobe_client){
    current_time = millis();
    previous_time = current_time;
    Serial.println("New Client, ");
    String current_line = "";

    while(garderobe_client.connected() && current_time - previous_time <= timeoutTime){
      current_time = millis();
      if(client.available()){
        char command = garderobe_client.read();
        Serial.write(command);
        header += command;
        if(command = '\n'){

          if(current_line.length() == 0){
            garderobe_client.println("HTTP/1.1 200 OK");
            garderobe_client.println("Content-type:text/html");
            garderobe_client.println("Connection: close");
            garderobe_client.println();

            if(header.indexOf("GET /26/on") >= 0){
              Serial.println("GPIO 26 ON");
              output_led_1 = "on";
              digitalWrite(output_led_1, HIGH);
            }else if(header.indexOf("GET /26/off") >= 0){
              Serial.println("GPIO 26 OFF");
              output_led_1 = "off");
              digitalWrite(output_led_1, LOW);
            }else if(header.indexOf("GET /27/on") >= 0){
              Serial.println("GPIO 27 ON");
              output_led_2 = "on";
              digitalWrite(output_led_2, HIGH);
            }else if(header.indexOf("GET /27/off") >= 0){
              Serial.println("GPIO 27 OFF");
              output_led_2 = "off");
              digitalWrite(output_led_2, LOW);
            }

            garderobe_client.println("<!DOCTYPE html><html>");
            garderobe_client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            garderobe_client.println("<link rel=\"icon\" href=\"data:,\">");
            garderobe_client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            garderobe_client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            garderobe_client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            garderobe_client.println(".button2 {background-color: #555555;}</style></head>");

            garderobe_garderobe_client.println("<body><h1>ESP32 Web Server</h1>");

            garderobe_client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              garderobe_client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              garderobe_client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 


            garderobe_client.client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              garderobe_client.client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              garderobe_client.client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            garderobe_client.client.println("</body></html>");

            garderobe_client.println();
            break;
          }else {
            current_line = "";
          }
        }else if(command != '\r'){
          current_line += command;
        }
      }
    }
    header = "";
    // Close the connection
    garderobe_client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    
  }
}
