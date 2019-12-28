# Alarm Project

A contratação de um sistema de segurança para empresas ou domicílios eventualmente se tornam caras e nem sempre vantajosas por N motivos, como por exemplo, na aquisição de um kit alarme, a central já vem programada para trabalhar com determinados módulos, ou seja, limita os recursos que queremos utilizar.

Nesse projeto a intenção é criar um alarme utilizando uma placa Arduino e alguns módulos.

A ideia é incrementando os módulos conforme o passar do tempo, evoluindo a integração dos módulos e posteriormente a melhoria no código.

O objetivo final é obter um alarme completo, utilizando sensores de movimento, abertura de portas, ativação e desativação remota, fazendo uso do módulo GSM para emitir avisos via SMS e via app que será usado para gerenciamento.



## Componentes

Os seguinte módulos e componentes são necessários para a montagem completa do alarme:

 * NodeMCU ESP8266
 * <s>Buzzer</s>
 * Módulo magnético
 * PIR - Sensor de presença Infrevermelho
 * Relê
 * <s> GSM SIM800L + Regulador de tensão
 * RFID </s>


## Estado atual:
[...]

Alarme iniciando com a conexão ao wi-fi, indicado pelo LED lilás intermitente. Na sequência é realizado a consulta no banco de dados sobre o estado do alarme (ativo/inativo), que é indicado pelo LED verde para ativo e vermelho para inativo. Cada consulta ao db sobre o estado do alarme o LED fica intermitente.

Os módulos dispostos no esquema "coming" são para implementação futura, se necessário.
