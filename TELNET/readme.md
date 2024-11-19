# Explicação do Código

Este código configura um ESP8266 para controle de um LED através de uma conexão Telnet, além de permitir atualizações de firmware OTA (Over-the-Air). Utilizando a rede Wi-Fi, o ESP8266 pode ser controlado remotamente, enviando comandos para ligar ou desligar o LED.

---

## 1. Inclusão de Bibliotecas

```cpp
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
```
- ESP8266WiFi.h: Biblioteca para configurar a conexão Wi-Fi do ESP8266.
- ArduinoOTA.h: Biblioteca que permite a atualização OTA, eliminando a necessidade de conexão física para carregar novos firmwares.
---
## 2. Configurações de Wi-Fi e Telnet
Define as credenciais da rede Wi-Fi e as configurações do LED e do servidor Telnet:

```cpp 
const char* ssid = "nome da rede";
const char* password = "senhasenha";
const int LED_PIN = 5; // D1
WiFiServer server(23);
WiFiClient client;
```

- ssid e password: Contêm as credenciais da rede Wi-Fi à qual o ESP8266 se conectará.
- LED_PIN: Define o pino D1 (GPIO5) para o controle do LED.
- WiFiServer server(23): Configura o servidor Telnet para ouvir na porta 23.
- WiFiClient client: Representa o cliente Telnet conectado ao ESP8266.
## Configuração de IP Fixo (Comentada)
Caso queira definir um IP estático, essas linhas podem ser descomentadas. Por padrão, o ESP8266 recebe um IP automaticamente pelo DHCP.
```cpp
// IPAddress local_IP(192, 168, 1, 184); // Definindo um IP fixo
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
```
## 3. Setup Inicial
Esta função setup() configura o ESP8266 para conexão Wi-Fi, inicializa o OTA, configura o Telnet e informa o IP obtido.

### Configuração do LED e Serial

```cpp
pinMode(LED_PIN, OUTPUT);
Serial.begin(9600);
```
Define o pino do LED como saída e inicializa a comunicação serial para exibir mensagens de status.

## Conexão Wi-Fi
```cpp
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Conexão falhou, tentando novamente...");
    WiFi.begin(ssid, password);
    delay(5000);
}
```

- WIFI_STA: Define o ESP8266 no modo estação (STA), conectando-se a uma rede.
- WiFi.begin: Inicia a conexão Wi-Fi.
waitForConnectResult(): Tenta conectar até obter sucesso.
- Mostra uma mensagem caso a conexão falhe e tenta novamente após 5 segundos.
## Configuração da Atualização OTA

Define funções para gerenciar eventos OTA:

```cpp
ArduinoOTA.onStart([]() { Serial.println("Iniciando atualização OTA..."); });
ArduinoOTA.onEnd([]() { Serial.println("\nAtualização concluída."); });
ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
});
ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Falha na autenticação");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Falha no início");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Falha na conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Falha no recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Falha no finalização");
});
ArduinoOTA.begin();
```

- onStart, onEnd, onProgress, onError: Funções para informar o progresso da atualização OTA e capturar erros, se ocorrerem.
## Inicialização do Telnet
```cpp
server.begin();
server.setNoDelay(true);
Serial.println("Servidor Telnet iniciado...");
```

Inicia o servidor Telnet na porta 23, permitindo que um cliente se conecte para controlar o LED.

## 4. Loop Principal
A função loop() aguarda as atualizações OTA e processa comandos Telnet para controlar o LED.

Tratamento de Atualizações OTA
```cpp
ArduinoOTA.handle();
```
Mantém o ESP8266 pronto para receber atualizações OTA a qualquer momento.

### Aceitando Conexão de Cliente Telnet

```cpp
if (server.hasClient()) {
    if (!client || !client.connected()) {
        if (client) client.stop();
        client = server.available();
        Serial.println("Cliente conectado via Telnet.");
        client.println("Digite '1' para ligar o LED e '0' para desligar o LED.");
    } else {
        server.available().stop();
    }
}
```
- server.hasClient(): Verifica se um novo cliente está tentando se conectar.
- client.stop(): Desconecta qualquer cliente anterior.
- client.println: Envia uma mensagem de instrução ao cliente conectado.
## Processamento de Comandos do Cliente
- client.read(): Lê o comando enviado pelo cliente.
- '1': Liga o LED e informa o cliente.
- '0': Desliga o LED e informa o cliente.
Comando inválido: Informa o cliente que o comando não é reconhecido.
