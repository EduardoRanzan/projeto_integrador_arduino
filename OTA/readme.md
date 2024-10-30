# Explicação do Código:

Este código configura um ESP8266. Ele conecta o ESP8266 a uma rede Wi-Fi e aguarda atualizações enviadas pelo ambiente de desenvolvimento (IDE Arduino).

---

## 1. Inclusão de Bibliotecas

```cpp
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
```

#### ESP8266WiFi.h: Biblioteca para gerenciar a conexão Wi-Fi no ESP8266.
#### ArduinoOTA.h: Biblioteca que permite a atualização OTA, sem a necessidade de conectar o ESP8266 via USB para carregar novo firmware.

---

## 2. Configurações de Wi-Fi

Define as credenciais da rede Wi-Fi:

```cpp
const char* ssid = "nome da rede";
const char* password = "senha";
```
### Configuração de IP Fixo (Comentada)
Essas linhas comentadas permitem configurar um IP estático, se desejado. Neste caso, o código usa o DHCP para obter um IP automaticamente
```cpp
// IPAddress local_IP(192, 168, 1, 184); // Definindo um IP fixo
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
```
---
## 3. Configuração da Atualização OTA

```cpp
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);

while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Conexão falhou, tentando novamente...");
    WiFi.begin(ssid, password);
    delay(5000);
}
```

- WIFI_STA: Define o modo de Wi-Fi do ESP8266 como estação (conectando-se a uma rede Wi-Fi).
- WiFi.begin: Inicia a conexão Wi-Fi com as credenciais fornecidas.
- waitForConnectResult(): Verifica o status da conexão, repetindo a tentativa a cada 5 segundos até conseguir.

## Eventos OTA
Define uma série de eventos para feedback durante o processo OTA:
```cpp
ArduinoOTA.onStart([]() {
    Serial.println("Iniciando atualização OTA...");
});
```
onStart: Executado quando a atualização começa.
```cpp
ArduinoOTA.onEnd([]() {
    Serial.println("\nAtualização concluída.");
});
```
onEnd: Executado ao finalizar a atualização.
```cpp
ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
});
```
onProgress: Mostra o progresso da atualização OTA, exibindo uma porcentagem no console Serial.
```cpp
ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Falha na autenticação");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Falha no início");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Falha na conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Falha no recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Falha na finalização");
});
```
onError: Executado se ocorrer algum erro durante o processo de atualização OTA. Mostra mensagens de erro específicas conforme o tipo.


---

## Inicialização da OTA e Exibição do IP
```cpp
ArduinoOTA.begin();
Serial.println("Pronto para atualizações OTA.");
Serial.print("Endereço IP: ");
Serial.println(WiFi.localIP());
```

- ArduinoOTA.begin(): Inicia o serviço OTA.
- WiFi.localIP(): Exibe o endereço IP atribuído ao ESP8266.

## Inicialização do Servidor Telnet (Comentado)
```cpp
server.begin();
server.setNoDelay(true);
Serial.println("Servidor Telnet iniciado...");
```
- server.begin() e server.setNoDelay(true): Inicializa o servidor Telnet, que está comentado aqui, mas pode ser útil para controle remoto via Telnet.

## 4. Loop Principal
No loop(), o ESP8266 aguarda e processa qualquer solicitação OTA.

```cpp
void loop() {
    ArduinoOTA.handle();
}
```
ArduinoOTA.handle(): Deve ser chamado repetidamente para que o ESP8266 fique ouvindo atualizações OTA.
