#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  uint8_t mac[6];
  
  // This replaces WiFi.macAddress(mac) and never returns zeros
  Network.macAddress(mac); 
  
  Serial.printf("Base MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", 
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void loop() {}