#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid = "TestSSID";
const char* password = "TestPassword";

// Sensor pin connections
const int lm35Pin = A0;  
const int pulsePin = A1;

// Variables for sensor readings
int temperatureRaw;
float temperatureC;
int pulseRaw;

ESP8266WebServer server(80);

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Custom Captive Portal</title>
  <style>
    /* CSS styles */
  </style>
</head>
<body>
  <h1>Sensor Data</h1>
  <p>Body Temperature: [TEMP] °C</p>
  <p>Pulse Rate: [PULSE] BPM</p>
</body>
</html>
  )";

  // Replace placeholders with actual sensor data
  html.replace("[TEMP]", String(temperatureC));
  html.replace("[PULSE]", String(pulseRaw));

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.softAP(ssid, password);
  Serial.print("Access Point started with IP: ");
  Serial.println(WiFi.softAPIP());

  // Initialize server routes
  server.on("/", handleRoot);

  server.begin();
}

void loop() {
  // Read LM35 temperature sensor
  temperatureRaw = analogRead(lm35Pin); 
  temperatureC = (temperatureRaw * 5.0 / 1023.0) * 100.0; 

  // Read pulse sensor
  pulseRaw = analogRead(pulsePin); 
  
  // Handle client requests
  server.handleClient();
  
  delay(1000);
}
