#include <WiFi.h>
#include <WebServer.h>

// Set your Access Point network credentials
const char* ssid = "SirenAP";
const char* password = "password123";

// Define the GPIO pin connected to your output
const int outputPin = 23; 

// Create an instance of the server on port 80
WebServer server(80);

// Variable to track the state of the output
bool outputState = false;

// HTML template with embedded JavaScript for AJAX
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 AJAX Controller</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f4f4f9; }
    h1 { color: #333; }
    .btn { background-color: #4CAF50; color: white; padding: 15px 32px; font-size: 20px; border: none; border-radius: 5px; cursor: pointer; display: inline-block; }
    .btn.off { background-color: #f44336; }
    .status { font-size: 24px; margin-bottom: 20px; font-weight: bold; }
  </style>
</head>
<body>
  <h1>LoudSiren Button!!!</h1>
  <h2>HaHaHaHaHaHaHa!!!</h2>
  <p class="status">Output State: <span id="state">%STATE%</span></p>
  <button id="toggleBtn" class="btn %BTN_CLASS%" onclick="toggleOutput()">%BTN_TEXT%</button>

  <script>
    function toggleOutput() {
      fetch('/toggle')
        .then(response => response.text())
        .then(state => {
          const btn = document.getElementById('toggleBtn');
          const stateSpan = document.getElementById('state');
          
          // Update status text
          stateSpan.innerText = state;
          
          // Update button appearance and text based on new state
          if (state === "ON") {
            btn.innerText = "Turn OFF";
            btn.classList.add('off');
          } else {
            btn.innerText = "Turn ON";
            btn.classList.remove('off');
          }
        })
        .catch(error => console.error('Error:', error));
    }
  </script>
</body>
</html>
)rawliteral";

// Handle root web page request with initial state substitution
void handleRoot() {
  String s = MAIN_page;
  s.replace("%STATE%", outputState ? "ON" : "OFF");
  s.replace("%BTN_CLASS%", outputState ? "off" : "");
  s.replace("%BTN_TEXT%", outputState ? "Turn OFF" : "Turn ON");
  server.send(200, "text/html", s);
}

// Handle asynchronous toggle request from AJAX
void handleToggle() {
  outputState = !outputState;
  digitalWrite(outputPin, outputState ? HIGH : LOW);
  
  // Return just the new state as plain text
  server.send(200, "text/plain", outputState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);

  // Configure the output pin
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);

  // Start the Wi-Fi Access Point
  Serial.println("Setting up Access Point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Define routing for web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_GET, handleToggle);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}