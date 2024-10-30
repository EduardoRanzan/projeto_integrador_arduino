# Explicação do código

Este código configura um servidor web em um ESP8266, permitindo o controle de um LED conectado ao pino D1 através de uma interface web. Abaixo estão os principais componentes e funções.

## 1. Inclusão de Bibliotecas

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
```

## 2. Configurações Wi-Fi

```cpp
ESP8266WebServer server(80);
const int output5 = 5; //D1
IPAddress local_IP(192,168,110,172);
IPAddress gateway(192,168,110,1);
IPAddress subnet(255,255,255,240);
```

## 3. Configurações do Servidor e Rede

```cpp
ESP8266WebServer server(80);
const int output5 = 5; //D1
IPAddress local_IP(192,168,110,172);
IPAddress gateway(192,168,110,1);
IPAddress subnet(255,255,255,240);
```
### server(80): configura o servidor web para a porta 80 (padrão HTTP).
### local_IP, gateway, subnet: configuração manual do endereço IP e da máscara de sub-rede.

## 4. Funções de Controle
#### handleRoot controla a interface principal da página
```cpp
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  // Código HTML da interface do servidor
  server.send(200, "text/html", html);
}
```

#### handleOn: Será utilizado para ativar o LED

```cpp
void handleOn() {
  digitalWrite(output5, HIGH);
  server.send(200, "text/plain", "LED Ligado!");
}
```
#### handleOff: Será utilizado para desativar o LED

```cpp
void handleOff() {
  digitalWrite(output5, LOW);
  server.send(200, "text/plain", "LED Desligado!");
}
```

## 5. Setup do ESP8266 (setup())

```cpp
void setup() {
  Serial.begin(9600);
  pinMode(output5, OUTPUT);
  digitalWrite(output5, LOW);
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  // Conexão Wi-Fi e inicialização do servidor
}
```

- Configura a comunicação serial e o modo de operação do pino do LED.
- Configura e conecta o Wi-Fi.
- Associa as URLs "/", "/5/on", e "/5/off" com suas respectivas funções (handleRoot, handleOn, e handleOff).
- Inicia o servidor.

## 6. Loop Principal (loop())
```cpp
void loop() {
  server.handleClient();
}
```
Mantém o servidor ativo, aguardando requisições.


