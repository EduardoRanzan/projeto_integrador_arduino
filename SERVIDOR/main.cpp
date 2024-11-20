#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

const char* ssid = "Eduardo";
const char* password = "12345678";

ESP8266WebServer server(80);
const int ledPin = LED_BUILTIN;

void handleRoot();
void handleOn();
void handleOff();

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 {background-color: #77878A;}</style></head>";
  html += "<body><h1>ESP8266 Web Server</h1>";
  html += "<h2>Controle do LED Embutido</h2>";
  html += "<button onclick=\"toggleLED('on')\" class=\"button\">ON</button>";
  html += "<button onclick=\"toggleLED('off')\" class=\"button button2\">OFF</button>";
  html += "<script>";
  html += "function toggleLED(state) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/led/' + state, true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED Ligado!");
}

void handleOff() {
  digitalWrite(ledPin, HIGH); 
  server.send(200, "text/plain", "LED Desligado!");
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  
  server.on("/", HTTP_GET, handleRoot);  
  server.on("/led/on", HTTP_GET, handleOn);  
  server.on("/led/off", HTTP_GET, handleOff);  
  server.begin();
  Serial.println("Servidor web iniciado...");

  
  ArduinoOTA.onStart([]() {
    Serial.println("Iniciando atualização OTA...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nAtualização concluída.");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Erro de autenticação");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Erro no início da atualização");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Erro de conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Erro de recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Erro ao finalizar");
  });

  ArduinoOTA.begin();
  Serial.println("Pronto para atualizações OTA.");
}

void loop() {
  server.handleClient();  
  ArduinoOTA.handle();    
}