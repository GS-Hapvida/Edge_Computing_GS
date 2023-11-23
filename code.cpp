#include <WiFi.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <EspMQTTClient.h>

// Define o pino e o tipo para o sensor DHT
#define DHTPIN 13
#define DHTTYPE DHT22

// Endereço I2C para o LCD
#define I2C_ADDR 0x27

// Cria objetos DHT e LCD
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

EspMQTTClient client {
    "Wokwi-GUEST",   // SSID do WiFi
    "",              // Senha do WiFi
    "mqtt.tago.io",  // Endereço do servidor MQTT
    "Default",       // Nome de usuário MQTT
    "1da4f76a-8533-446c-969b-e25ad3d84fd0", // Token MQTT
    "Gota-a-gota",   // Nome do device 
    1883             // Porta MQTT
};

void setup() {
  Serial.begin(9600);
  dht.begin(); 
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);

  LCD.print("Conectando ao ");
  LCD.setCursor(0, 1);
  LCD.print("WiFi ");
  delay(1000);

  // Loop até que a conexão WiFi seja estabelecida
  while (!client.isWifiConnected()){
    LCD.print('.'); 
    client.loop();
    delay(1000);
  }

  LCD.clear();
  LCD.print("Conectado ao");
  LCD.setCursor(0, 1);
  LCD.print("MQTT ");
  delay(2000);
  LCD.clear();
}

//Callback da EspMQTTClient
void onConnectionEstablished() 
{}
char bufferJson[100];

void loop() {

  // Lê temperatura e umidade do sensor DHT
  int temperature = dht.readTemperature();
  int humidity = dht.readHumidity();

  // Verifica se as condições não são favoráveis 
  if (temperature > 35 || humidity < 20) {
      LCD.clear();
      tone(13, 500, 1000);
      LCD.print("Temperatura: " + String(temperature) + "C");
      LCD.setCursor(0, 1);
      LCD.print("Umidade: " + String(humidity) + "%");
      delay(4000);
      LCD.clear();
      LCD.print("BEBA AGUA!");
      delay(4000);
      LCD.clear();
  } else {
      LCD.print("Temperatura: " + String(temperature) + "C");
      LCD.setCursor(0, 1);
      LCD.print("Umidade: " + String(humidity) + "%");
      delay(4000);
      LCD.clear();
      LCD.print("Condicoes");
      LCD.setCursor(0, 1);
      LCD.print("Favoraveis");
      noTone(13);
      delay(4000);
      LCD.clear();
  }

  // Criação e envio de mensagens JSON para o servidor MQTT
  StaticJsonDocument<300> documentJsonTemp;
    documentJsonTemp["variable"] = "temperatura";
    documentJsonTemp["value"] = temperature;
    documentJsonTemp["unit"] = "°C";

    serializeJson(documentJsonTemp, bufferJson);
    Serial.println(bufferJson);
    client.publish("GlobalSolution-Gota", bufferJson);

  StaticJsonDocument<300> documentJsonUmid;
    documentJsonUmid["variable"] = "umidade";
    documentJsonUmid["value"] = humidity;
    documentJsonUmid["unit"] = "%";

    serializeJson(documentJsonUmid, bufferJson);
    Serial.println(bufferJson);
    client.publish("GlobalSolution-Gota", bufferJson);

    delay(500);
    client.loop();

  delay(1000);
}
