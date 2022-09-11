# Alarm Project

A contratação de um sistema de segurança para empresas ou domicílios atualmente se tornam caras e nem sempre vantajosas por n motivos, como por exemplo em uma compra de kit de alarme, a central já vem programada para trabalhar com determinados módulos, ou seja, acaba nos limitando quanto aos recursos que queremos utilizar. 

## Objetivo
Criar um alarme utilizando uma placa de prototipagem e alguns módulos. A ideia é incrementando os módulos conforme o passar do tempo, evoluindo a integração dos módulos e posteriormente a melhoria no código. 

O objetivo final é obter um alarme completo, integrando módulos GSM para emitir avisos via SMS.

### Incremento
Posteriormente a criação de um app Android para habilitar/desabilitar o alarme, testar os módulos - se estão funcionando corretamente - para não haver custos (com ligação ou SMS no GSM), utilizando módulo Ethernet ou Wi-fi.



## O Projeto

Abaixo está o esquema de ligação dos módulos, bem como a lista dos itens necessários para construção do alarme. **O esquema e os itens serão atualizados no decorrer do desenvolvimento**

 * NodeMCU ESP8266
 * 1x Mód. Sensor Magnético de sobrepor
 * 2x Mód. PIR (Sensor de Presença Infrevermelho)
 * 1x Relé 12v (**JQC-3FF**)
 * <s> 1x Mód. GSM (**SIM800L**) + Regulador de tensão (**LM2596**) </s>
 * <s> 1x Mód. RFID (**MFRC522**) + Buzzer </s>
 


![Esquema](https://raw.githubusercontent.com/willyamcts/projeto-alarme/master/Projeto%20Alarme%20-%20Esquema%20ESP8266_comming_bb.png)
 


## Estado atual:
[...]

Alarme iniciando com a conexão ao wi-fi, indicado pelo LED lilás intermitente. Na sequência é realizado a consulta no banco de dados sobre o estado do alarme (ativo/inativo), que é indicado pelo LED verde para ativo e vermelho para inativo. Cada consulta ao db sobre o estado do alarme o LED fica intermitente.

Os módulos dispostos no esquema "coming" são para implementação futura, se necessário.
