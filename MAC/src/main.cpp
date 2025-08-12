
#include "WiFi.h"
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_AP);
  Serial.println(WiFi.macAddress());
  WiFi.softAP(WiFi.macAddress());
}
 
void loop(){

}
