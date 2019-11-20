# Alarm Project

A contratação de um sistema de segurança para empresas ou domicílios eventualmente se tornam caras e nem sempre vantajosas por N motivos, como por exemplo, na aquisição de um kit alarme, a central já vem programada para trabalhar com determinados módulos, ou seja, limita os recursos que queremos utilizar.

Nesse projeto a intenção é criar um alarme utilizando uma placa Arduino e alguns módulos.

A ideia é incrementando os módulos conforme o passar do tempo, evoluindo a integração dos módulos e posteriormente a melhoria no código.

O objetivo final é obter um alarme completo, utilizando sensores de movimento, abertura de portas, ativação e desativação remota, fazendo uso do módulo GSM para emitir avisos via SMS.

Posteriormente, talvez seja desenvolvido um app para Android para habilitar/desabilitar o alarme, testar os módulos - se estão funcionando corretamente - para não haver custos (com ligação ou SMS no GSM), utilizando módulo Ethernet.


## Componentes

Os seguinte módulos e componentes são necessários para a montagem completa do alarme:

 * Arduino Uno 
 * Buzzer 
 * RFID
 * Switch Magnetic module
 * PIR - Presence Infra Red module
 * <s>GSM SIM800L + Regulador de tensão </s>
 * <s>Relay 12v </s>
 
 **Extra:**
 * <s> Ethernet </s>


## Estado atual:
Funcionando normalmente utilizando Ethernet como meio de controle. Utiliza página web como meio de ativação e consome api pushingbox.com para fazer o registro de detecção de movimento para uma planilha do Google. Os módulos GSM, ethernet e RFID disposto no esquema "coming" para implementação futura se necessário.
