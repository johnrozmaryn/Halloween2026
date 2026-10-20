#include <esp_now.h>
#include <WiFi.h>

// Define output pins
const int sirenPin = 23;

// Define a structure to match the sender
typedef struct struct_message {
  bool triggerState;
} struct_message;

struct_message myData;

// Callback function executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Control both the onboard LED and the siren pin simultaneously
  if (myData.triggerState) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(sirenPin, HIGH);
    Serial.println("Trigger ON (LED & Siren active)");
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(sirenPin, LOW);
    Serial.println("Trigger OFF");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize output pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sirenPin, OUTPUT);
  
  // Ensure both start LOW (off)
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(sirenPin, LOW);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function for incoming data (ESP32 Core 3.x format)
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Nothing to do here; everything is handled instantly in the callback!
}