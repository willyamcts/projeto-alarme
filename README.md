# Alarm Project

## Objetivo
Criar um alarme utilizando uma placa de prototipagem e alguns módulos. A ideia é incrementando os módulos conforme o passar do tempo, evoluindo a integração dos módulos e posteriormente a melhoria no código. 

O objetivo final é obter um alarme completo, integrando módulos GSM para emitir avisos via SMS.

### Incremento
Posteriormente a criação de um app Android para habilitar/desabilitar o alarme, testar os módulos - se estão funcionando corretamente - para não haver custos (com ligação ou SMS no GSM), utilizando módulo Ethernet ou Wi-fi.



## O Projeto

Abaixo está o esquema de ligação dos módulos, bem como a lista dos itens necessários para construção do alarme. **O esquema e os itens serão atualizados no decorrer do desenvolvimento**

 * NodeMCU ESP8266
 * 1x Mód. Sensor Magnético de sobrepor
 * 2x Mód. PIR (Sensor de Presença Infravermelho)
 * 1x Relé 12v (**JQC-3FF**)
 * <s> 1x Mód. RFID RC-522 </s>
 * <s> 1x Mód. RFID (**MFRC522**) + Buzzer </s>
 * <s> 1x Mód. GSM (**SIM800L**) + Regulador de tensão (**LM2596**) </s>
 


![Esquema](https://raw.githubusercontent.com/willyamcts/projeto-alarme/master/Projeto%20Alarme%20-%20Esquema%20ESP8266_comming_bb.png)


* Os módulos dispostos no esquema "coming" são para implementação futura, se necessário.


## Estado atual:
[...]




Bibliotecas:
 - MFRC522 v
 - [ESP8266WiFi](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html) (release april/2021)
 - [Firebase_ESP_Client v4.4.x](https://github.com/mobizt/Firebase-ESP-Client)