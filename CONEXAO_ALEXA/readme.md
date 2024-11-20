### Video do funcionamento em aula
![Funcionamento](/CONEXAO_ALEXA/gravacaoConexaoAlexa.gif)

### Arquitetura para entendimento
![Arquitetura](/CONEXAO_ALEXA/Arquitetura.png)

# Explicação do Código
## 1. Bibliotecas

```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
```

## 2. Função onPowerState

```cpp
bool onPowerState(const String &deviceId, bool &state)
{
  Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state ? "on" : "off");
  myPowerState = state;
  digitalWrite(RELE_PIN, myPowerState ? LOW : HIGH);
  return true;
}
```
- Esta função é chamada sempre que o dispositivo é controlado remotamente pelo SinricPro. Basicamente é quem faz ligar e desligar.

## 3. Função handleButtonPress

```cpp
void handleButtonPress()
{
  unsigned long actualMillis = millis();
  if (digitalRead(BUTTON_PIN) == LOW && actualMillis - lastBtnPress > 1000)
  {
    myPowerState = !myPowerState;
    digitalWrite(RELE_PIN, myPowerState ? LOW : HIGH);
    SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
    mySwitch.sendPowerStateEvent(myPowerState);
    Serial.printf("Device %s turned %s (manually via flashbutton)\r\n",
                  mySwitch.getDeviceId().c_str(), myPowerState ? "on" : "off");
    lastBtnPress = actualMillis;
  }
}
```
- Essa função verifica se o botão foi pressionado e se passou mais de um segundo desde a última pressão.
Se essas condições forem atendidas, o estado do dispositivo é invertido, o relé é atualizado e o novo estado é enviado ao SinricPro.

## 4. Função setupWiFi

```cpp
void setupWiFi()
{
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}
```
- Esta função conecta o ESP8266 à rede Wi-Fi usando o SSID e a senha fornecidos.
Enquanto a conexão não é estabelecida, imprime pontos no monitor serial. Quando a conexão é bem-sucedida, exibe o endereço IP.

## 7. Função setupSinricPro

```cpp
void setupSinricPro()
{
  SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.onConnected([]() { Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([]() { Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
```
- Configura a conexão com a plataforma SinricPro.

## 8. Função setup
```cpp
void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, HIGH);
  Serial.begin(BAUD_RATE);
  setupWiFi();
  setupSinricPro();
}
```
- Configura o botão como entrada com INPUT_PULLUP e o relé como saída, inicializando-o em estado desligado.Inicia a comunicação serial e chama as funções setupWiFi e setupSinricPro para estabelecer as conexões.

## 9. Loop Principal (loop)
```cpp
void loop()
{
  handleButtonPress();
  SinricPro.handle();
}
```
- A função handleButtonPress é chamada para monitorar o botão físico. SinricPro.handle() processa as solicitações vindas da plataforma SinricPro para atualizar o estado do dispositivo conforme necessário.
