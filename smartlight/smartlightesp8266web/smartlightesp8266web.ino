#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "";// your wifi ssid 
const char* password = "";//your pass

const int relayPin = D5;

ESP8266WebServer server(80);

void handleOn() {
  digitalWrite(relayPin, HIGH);
  server.send(200, "application/json", "{\"status\":\"ON\"}");
}

void handleOff() {
  digitalWrite(relayPin, LOW);
  server.send(200, "application/json", "{\"status\":\"OFF\"}");
}

void handleStatus() {
  String state = digitalRead(relayPin) ? "ON" : "OFF";
  server.send(200, "application/json", "{\"status\":\"" + state + "\"}");
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Relay Dashboard</title><style>";
  html += "body{font-family:sans-serif;background:#f2f2f2;text-align:center;}";
  html += ".card{background:white;padding:30px;margin:50px auto;max-width:400px;border-radius:10px;box-shadow:0 0 10px rgba(0,0,0,0.1);}";
  html += ".btn{display:inline-block;padding:15px 30px;font-size:18px;margin:10px;border:none;border-radius:5px;cursor:pointer;text-decoration:none;}";
  html += ".btn-on{background-color:green;color:white;}";
  html += ".btn-off{background-color:red;color:white;}";
  html += "</style></head><body><div class='card'><h1>Relay Dashboard</h1>";

  if (digitalRead(relayPin)) {
    html += "<p>Status: <strong style='color:green;'>ON</strong></p>";
  } else {
    html += "<p>Status: <strong style='color:red;'>OFF</strong></p>";
  }

  html += "<a href='/api/on'><button class='btn btn-on'>Turn ON</button></a>";
  html += "<a href='/api/off'><button class='btn btn-off'>Turn OFF</button></a>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Open in browser: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/api/on", handleOn);
  server.on("/api/off", handleOff);
  server.on("/api/status", handleStatus);

  server.begin();
}

void loop() {
  server.handleClient();
}
