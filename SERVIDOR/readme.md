# Explicação Resumida do Código

Este código implementa um servidor web simples usando o **ESP8266**, permitindo o controle remoto de um LED embutido por meio de uma página web. Ele também suporta atualizações OTA (Over-The-Air), possibilitando a atualização do firmware sem conexão física ao dispositivo.

## Componentes do Código

### 1. **Conexão Wi-Fi**
- Configura o ESP8266 para conectar-se a uma rede Wi-Fi utilizando o SSID (`"Eduardo"`) e a senha (`"12345678"`).
- Exibe no terminal o endereço IP atribuído ao ESP8266 após a conexão bem-sucedida.


```cpp
const char* ssid = "Eduardo";
const char* password = "12345678";

WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
Serial.println("WiFi conectado.");
Serial.print("Endereço IP: ");
Serial.println(WiFi.localIP());
```

### 2. **Servidor Web**
- Configura um servidor HTTP na porta 80.
- Define três rotas principais:
  - `/` - Exibe uma página HTML para controle do LED, com botões para ligá-lo e desligá-lo.
  - `/led/on` - Liga o LED (escreve `LOW` no pino do LED embutido).
  - `/led/off` - Desliga o LED (escreve `HIGH` no pino do LED embutido).
- Código de definição de rotas:

```cpp
server.on("/", HTTP_GET, handleRoot);  
server.on("/led/on", HTTP_GET, handleOn);  
server.on("/led/off", HTTP_GET, handleOff);
server.begin();
```

### 3. **Interface Web**
- A página HTML exibida na rota raiz (/) permite ligar ou desligar o LED com dois botões e contém o seguinte script para comunicação com o servidor:

```cpp
<script>
  function toggleLED(state) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/led/' + state, true);
    xhr.send();
  }
</script>
```
  - Os botões da página são configurados assim:
```cpp
<button onclick="toggleLED('on')" class="button">ON</button>
<button onclick="toggleLED('off')" class="button button2">OFF</button>
```

### 4. **Controle do LED**
  - O LED embutido do ESP8266 é controlado pelas rotas /led/on e /led/off. O código correspondente:
  ```cpp
  void handleOn() {
  digitalWrite(LED_BUILTIN, LOW); // Liga o LED
  server.send(200, "text/plain", "LED Ligado!");
  }
  void handleOff() {
    digitalWrite(LED_BUILTIN, HIGH); // Desliga o LED
    server.send(200, "text/plain", "LED Desligado!");
  }
  ```
  
### 5. **Atualizações OTA**
  - HaHabilita suporte a atualizações OTA e define callbacks para monitorar o processo:

  ```cpp
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
    else if (error == OTA_BEGIN_ERROR) Serial.println("Erro no início");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Erro de conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Erro de recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Erro ao finalizar");
  });
  ArduinoOTA.begin();
  ```
### 6. **Ciclo Principal**
  - No loop principal, o código processa requisições do servidor e gerencia atualizações OTA:
  ```cpp
  void loop() {
  server.handleClient();  
  ArduinoOTA.handle();    
  }
  ```