#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR SIREN BOARD'S MAC ADDRESS
uint8_t broadcastAddress[] = {0x8C, 0x94, 0xDF, 0xAA, 0x46, 0x34};

// Pin definitions
const int buttonPin = 4;       // Existing wireless trigger button
const int canonSwitchPin = 13; // New local Canon switch
const int canonOutputPin = 23; // New local Canon output pin

// Debounce variables for buttonPin
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 
int lastButtonState = HIGH;
int buttonState = HIGH;

// Define a structure to send data to the SirenBoard
typedef struct struct_message {
  bool triggerState;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(canonSwitchPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(canonOutputPin, OUTPUT);
  
  // Ensure local outputs start LOW
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(canonOutputPin, LOW);

  // Set Device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // --- 1. Handle Existing Wireless Button (Pin 4) ---
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        myData.triggerState = true;
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
        
        if (result == ESP_OK) {
          Serial.println("Sent wireless trigger ON");
        } else {
          Serial.println("Error sending wireless data");
        }
      } else {
        myData.triggerState = false;
        esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
        Serial.println("Sent wireless trigger OFF");
      }
    }
  }
  lastButtonState = reading;

  // --- 2. Handle New Local Canon Switch (Pin 13) ---
  // Using a direct digital read (assuming active LOW like the push button, 
  // tied to GND when closed. Change to HIGH if it's a latching switch supplying 3.3V)
  int canonState = digitalRead(canonSwitchPin);
  
  if (canonState == LOW) { // Switch is active/ON
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(canonOutputPin, HIGH);
  } else {                 // Switch is inactive/OFF
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(canonOutputPin, LOW);
  }
}