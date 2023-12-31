# Sistema de monitoração de temperatura e umidade

## Equipe

|           NOME           |  RM   |
| :----------------------: | :---: |
| Kaiky Alvaro de Miranda  | 98118 |
| Lucas Rodrigues da Silva | 98344 |

---

A gota-a-gota(GG), nada mais é do que um sistema de monitoramento de umidade e temperatura do ambiente, que tem por objetivo a qualidade de vida e saúde das pessoas. Dessa forma, toda vez que a temperatura e a umidade estiverem em condições desfavoráveis e prejudiciais a sáude de uma pessoa, um buzzer é acionado e no LCD aparece a mensagem "Beba Água".

Para acessar o modelo no Wowki [Clique aqui](https://wokwi.com/projects/381852241971423233)

---

# MONTAGEM DO DISPOSITIVO

#### Materias Necessários Para a Montagem do Sistema

|      Componente      | quantidade |                                   Descrição                                   |
| :------------------: | :--------: | :---------------------------------------------------------------------------: |
|      **ESP32**       |     1      |                   Vai se conectar à rede wifi e ao servidor                   |
|     **DHT11/22**     |     1      |         Utilizado para obter as informações de temperatura e umidade          |
|      **Buzzer**      |     1      | Responsável por disparar um alarme quando a as condições não forem favoráveis |
|     **LCD I2C**      |     1      |      Será utilizado para exibir as informações de temperatura e umidade       |
| **Resistor 1K Ohms** |     1      |                             Conexão para o buzzer                             |
| **Placa de Ensaio**  |     1      |                Utilizado como base de conexão dos componentes                 |

### Modelo de Montagem

![Modelo de Montagem no Wokwi](./imagens//modelo_montagem_gota_gs.png)

---

## Funcionamento

O projeto Gota-a-Gota funciona da seguinte forma:

Quando iniciar a compilação do código, ele irá se conectar ao wifi e ao servidor MQTT do Tago.Io
![Imagem ilustrativa da conexão com o wifi](./imagens/conectando_gota_gs.png)
![Imagem ilustrativa da conexão com o MQTT](./imagens/conectando_mqtt_gota_gs.png)

Dessa forma, o sistema está pronto para rodar e testar as condições de ambiente.
Nesse caso, se a temperatura estiver maior qur 35 graus celsius ou umidade menor que 20%
![Imagem ilustrativa das condições não favoráveis](./imagens/condicoes_nao_favoraveis_gs.png)

O buzzer será acionado e aparecerá uma mensagem no LCD para que o usuário beba água
![Imagem ilustrativa da mensagem beba água](./imagens/mensagem_beber_agua_gs.png)

Caso contrário, as condições de ambiente estiverem favoráveis.
![Imagem ilustrativa das condições favoráveis](./imagens/condicoes_favoraveis_gs.png)

Aparecerá uma mensagem no LCD informando que as condições estão favoráveis.
![Imagem ilustrativa das condições favoráveis](./imagens/mensagem_condicoes_favoraveis_gs.png)

---

## Conectando com o Servidor MQTT

Antes de tudo, será preciso que você entre no TagoIo e faça uma conta. Após isso, clica em "Devices" e adicione um device em MQTT.
![Imagem ilustrativa criando um device](./imagens/criando_device_tago.png)

No device criado, é possível ver o token de acesso em "General information". Além do "Live Inspector" que é utilizado para saber se as informações estão chegando no servidor.
![Imagem ilustrativa do device gota-a-gota](./imagens/device_gota_tago.png)

Com o device criado, agora basta configurar no código as informações do servidor, como o nome do device e o token do servidor MQTT

```C++
EspMQTTClient client {
    "Wokwi-GUEST",   // SSID do WiFi
    "",              // Senha do WiFi
    "mqtt.tago.io",  // Endereço do servidor MQTT
    "Default",       // Nome de usuário MQTT
    "1da4f76a-8533-446c-969b-e25ad3d84fd0", // Token MQTT
    "Gota-a-gota",   // Nome do device
    1883             // Porta MQTT
};
```

---

## Enviando para o Tago em Json

Para enviar o código, de maneira que ele chegue com as variáveis no servidor, é preciso enviar em formato JSON.
No código fonte [Código no Wokwi](https://wokwi.com/projects/381852241971423233) é possível visualizar.

Este é código necessário para que os dados sejam enviados ao Tagoio

```C++
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
```

Após configurar no código e criar o envio em JSON, será preciso que você vai em Actions e adicione um nome, o tipo, que será MQTT e a action sendo "Insert to Device Bucket"
![Imagem ilustrativa criando um action insertBucket](./imagens/insertBucket_tago.png)

Agora, basta configurar selecionando o device e colocando o tópico MQTT
![Imagem ilustrativa de configuração da Action](./imagens/config_action_tago.png)

---

Depois de tudo isso, você pode voltar para a home e ir em "dashboards" e criar a sua com base nos dados que vem chegando.
Como por exemplo:
Essas dashboards que exibem informações de umidade e temperatura captados pelo sistema
![Imagem ilustrativa das dashboards de temperatura e umidade](./imagens/criando_dashboards_tago.png)
