/*
  LED do servidor Modbus RTU

  Este esboço cria um servidor Modbus RTU com uma bobina simulada.
  O valor da bobina simulada é definido no LED

  O circuito:
   - Placa MKR
   - Escudo MKR 485
     - ISO GND conectado ao GND do servidor Modbus RTU
     - Y conectado a A/Y do cliente Modbus RTU
     - Z conectado a B/Z do cliente Modbus RTU
     - Posições de jumpers
       - FULL definido como OFF
       - Z \/\/ Y configurado para OFF

  criado em 16 de julho de 2018
  por Sandeep Mistry
*/
#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

const int ledPin = LED_BUILTIN;

void setup() {
  Serial.begin(9600);

  Serial.println("Modbus RTU Server LED");

  
// inicia o servidor Modbus RTU, com (escravo) id 1
  if (!ModbusRTUServer.begin(1, 9600)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  // configure the LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

 
// configura uma única bobina no endereço 0x00
  ModbusRTUServer.configureCoils(0x00, 1);
}

void loop() {
  // pesquisa para solicitações Modbus RTU
  int packetReceived = ModbusRTUServer.poll();

  if(packetReceived) {
   
// lê o valor atual da bobina
    int coilValue = ModbusRTUServer.coilRead(0x00);
  
    if (coilValue) {
      // valor da bobina definido, acende o LED
      digitalWrite(ledPin, HIGH);
    } else {
      // valor da bobina limpa, desliga o LED
      digitalWrite(ledPin, LOW);
    }
  }
}
